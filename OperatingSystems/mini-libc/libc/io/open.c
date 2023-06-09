// SPDX-License-Identifier: BSD-3-Clause

#include <fcntl.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>

int open(const char *filename, int flags, ...)
{
	/* TODO: Implement open system call. */
	mode_t mode = 0;
    if ((flags & O_CREAT) != 0) {
        va_list arg;
        va_start(arg, flags);
        mode = va_arg(arg, mode_t);
        va_end(arg);
    }

    int ret = syscall(__NR_open, filename, flags, mode);
	if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}
