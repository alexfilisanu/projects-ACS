// SPDX-License-Identifier: BSD-3-Clause

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmd.h"
#include "utils.h"

#define READ		0
#define WRITE		1

/**
 * Internal change-directory command.
 */
static bool shell_cd(word_t *dir)
{
	/* Execute cd. */
	if (dir == NULL)
		return SHELL_EXIT;

	return chdir(dir->string);
}

/**
 * Internal exit/quit command.
 */
static int shell_exit(void)
{
	/* Execute exit/quit. */
	return SHELL_EXIT;
}

/**
 * Does redirection depend on type of fildes(STDIN_FILENO / STDOUT_FILENO / STDERR_FILENO)
 * and io_flags where it is stored the information if it should be redirected with
 * O_APPEDN or O_TRUNC. (starting from: content/chapters/io/lab/solution/mini-shell/mini_shell.c
 * and content/chapters/io/lab/solution/file-descriptors/open_read_write.c)
 */
static void do_redirect(int filedes, const char *filename, int io_flags)
{
	int rc;
	int fd;

	/* Redirect `filedes` into `fd` representing `filename`. */
	switch (filedes) {
	/* Redirect standard input to the specified file (`filename`). */
	case STDIN_FILENO:
		fd = open(filename, O_RDONLY | O_CREAT, 0644);
		DIE(fd < 0, "open");
		break;

	/* Redirect standard output to the specified file (`filename`),
	 * with O_APPEND or O_TRUNC depends on io_flags.
	 */
	case STDOUT_FILENO:
		fd = io_flags == IO_OUT_APPEND
			? open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644)
			: open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		DIE(fd < 0, "open");
		break;

	/* Redirect standard error to the specified file (`filename`),
	 * with O_APPEND or O_TRUNC depends on io_flags.
	 */
	case STDERR_FILENO:
		fd = io_flags == IO_ERR_APPEND
			? open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644)
			: open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		DIE(fd < 0, "open");
		break;

	default:
		break;
	}

	/* `dup2()` closes the old `filedes` and replaces it with `fd`. */
	rc = dup2(fd, filedes);
	DIE(rc < 0, "dup2");

	rc = close(fd);
	DIE(rc < 0, "close");
}

/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	pid_t pid;
	pid_t wait_ret;
	int status, ret = 0;
	int argv_size;
	char **argv;
	char *verb = get_word(s->verb);
	char *in = get_word(s->in);
	char *out = get_word(s->out);
	char *err = get_word(s->err);

	/* Sanity checks. */
	if (s == NULL)
		return SHELL_EXIT;

	/* If builtin command, execute the command. */
	if (strcmp(verb, "exit") == 0 || strcmp(verb, "quit") == 0)
		return shell_exit();

	if (strcmp(verb, "cd") == 0)
		return shell_cd(s->params);

	/* If variable assignment, execute the assignment and return the exit status. */
	if (strchr(verb, '=') != NULL) {
		char *saveptr;
		char *var = strtok_r(verb, "=", &saveptr);
		char *value = strtok_r(NULL, "=", &saveptr);

		return setenv(var, value, 1);
	}

	/*   If external command:
	 *   1. Fork new process
	 *     2c. Perform redirections in child
	 *     3c. Load executable in child
	 *   2. Wait for child
	 *   3. Return exit status
	 */

	pid = fork();
	switch (pid) {
	case -1:
		/* Error */
		DIE(1, "fork");
		break;

	case 0:
		/* Child process */
		/* redirect STDIN */
		if (in != NULL)
			do_redirect(STDIN_FILENO, in, s->io_flags);

		/* if STDOUT and STDERR exists and are equals => first STDERR, then STDOUT with append */
		if (out != NULL && err != NULL && strcmp(out, err) == 0) {
			do_redirect(STDERR_FILENO, err, s->io_flags);
			do_redirect(STDOUT_FILENO, out, IO_OUT_APPEND);
		} else {
			/* redirect stdout */
			if (out != NULL)
				do_redirect(STDOUT_FILENO, out, s->io_flags);

			/* redirect stderr */
			if (err != NULL)
				do_redirect(STDERR_FILENO, err, s->io_flags);
		}

		/* get command (already = verb) and argv  */
		argv = get_argv(s, &argv_size);

		/* execute the command */
		execvp(verb, argv);

		/* equivalent for DIE(1, "execvp") to return a message in case of unknown command */
		fprintf(stderr, "Execution failed for '%s'\n", verb);

		break;

	default:
		/* Parent process */
		wait_ret = waitpid(pid, &status, 0);
		DIE(wait_ret < 0, "waitpid");

		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);

		break;
	}

	free(verb);
	free(in);
	free(out);
	free(err);
	return ret;
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool run_in_parallel(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	/* Execute cmd1 and cmd2 simultaneously. */
	pid_t pid1, pid2;
	pid_t wait_ret;
	int status, ret1 = 0, ret2 = 0;

	/* Fork the first child process to run cmd1 */
	pid1 = fork();
	switch (pid1) {
	case -1:
		/* Error */
		DIE(1, "fork");
		break;

	case 0:
		/* Child process for cmd1 */
		ret1 = parse_command(cmd1, level + 1, father);
		exit(ret1);
		break;

	default:
		/* Parent process */
		/* Fork the second child process to run cmd2 */
		pid2 = fork();
		switch (pid2) {
		case -1:
			/* Error */
			DIE(1, "fork");
			break;

		case 0:
			/* Child process for cmd2 */
			ret2 = parse_command(cmd2, level + 1, father);
			exit(ret2);
			break;

		default:
			/* Parent process */
			wait_ret = waitpid(pid1, &status, 0);
			DIE(wait_ret < 0, "waitpid");

			if (WIFEXITED(status))
				ret1 = WEXITSTATUS(status);

			wait_ret = waitpid(pid2, &status, 0);
			DIE(wait_ret < 0, "waitpid");

			if (WIFEXITED(status))
				ret2 = WEXITSTATUS(status);

			break;
		}
	}

	return ret1 && ret2;
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2).
 * (starting from: content/chapters/io/lab/solution/pipes/anonymous_pipe.c)
 */
static bool run_on_pipe(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	/* Redirect the output of cmd1 to the input of cmd2. */
	int pipedes[2];
	pid_t pid1, pid2;
	pid_t wait_ret;
	int status, ret1 = 0, ret2 = 0;
	int rc;

    /* Create the pipe */
	rc = pipe(pipedes);
	DIE(rc < 0, "pipe");

    /* Fork the first child process to run cmd1 */
	pid1 = fork();
	switch (pid1) {
	case -1:
		/* Error */
		DIE(1, "fork");
		break;

	case 0:
		/* Child process for cmd1 */
		/* Redirect stdout to the write end of the pipe */
		close(pipedes[0]);
		dup2(pipedes[1], STDOUT_FILENO);
		close(pipedes[1]);

		ret1 = parse_command(cmd1, level + 1, father);
		exit(ret1);
		break;

	default:
		/* Parent process */
		/* Fork the second child process to run cmd2 */
		pid2 = fork();
		switch (pid2) {
		case -1:
			/* Error */
			DIE(1, "fork");
			break;

		case 0:
			/* Child process for cmd2 */
			/* Redirect stdin to the read end of the pipe */
			close(pipedes[1]);
			dup2(pipedes[0], STDIN_FILENO);
			close(pipedes[0]);

			ret2 = parse_command(cmd2, level + 1, father);
			exit(ret2);
			break;

		default:
			/* Parent process */
			/* Close the pipe file descriptors */
			close(pipedes[0]);
			close(pipedes[1]);

			wait_ret = waitpid(pid1, &status, 0);
			DIE(wait_ret < 0, "waitpid");

			if (WIFEXITED(status))
				ret1 = WEXITSTATUS(status);

			wait_ret = waitpid(pid2, &status, 0);
			DIE(wait_ret < 0, "waitpid");

			if (WIFEXITED(status))
				ret2 = WEXITSTATUS(status);

			break;
		}
	}

	return !ret1 && ret2;
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	int ret = 0;

	/* Sanity checks */
	if (c == NULL)
		return SHELL_EXIT;

	if (c->op == OP_NONE) {
		/* Execute a simple command. */
		ret = parse_simple(c->scmd, level + 1, father);

		return ret;
	}

	switch (c->op) {
	case OP_SEQUENTIAL:
		/* Execute the commands one after the other. */
		ret = parse_command(c->cmd1, level + 1, c);
		ret = parse_command(c->cmd2, level + 1, c);
		break;

	case OP_PARALLEL:
		/* Execute the commands simultaneously. */
		ret = run_in_parallel(c->cmd1, c->cmd2, level + 1, c);
		break;

	case OP_CONDITIONAL_NZERO:
		/* Execute the second command only if the first one
		 * returns non zero.
		 */
		ret = parse_command(c->cmd1, level + 1, c);
		if (ret != 0)
			ret = parse_command(c->cmd2, level + 1, c);
		break;

	case OP_CONDITIONAL_ZERO:
		/* Execute the second command only if the first one
		 * returns zero.
		 */
		ret = parse_command(c->cmd1, level + 1, c);
		if (ret == 0)
			ret = parse_command(c->cmd2, level + 1, c);
		break;

	case OP_PIPE:
		/* Redirect the output of the first command to the
		 * input of the second.
		 */
		ret = run_on_pipe(c->cmd1, c->cmd2, level + 1, c);
		break;

	default:
		return SHELL_EXIT;
	}

	return ret;
}
