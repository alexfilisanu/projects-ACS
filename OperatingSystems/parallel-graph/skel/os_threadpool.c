#include "os_threadpool.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* === TASK === */

/* Creates a task that thread must execute */
os_task_t *task_create(void *arg, void (*f)(void *))
{
    os_task_t *task = (os_task_t *) malloc(sizeof(os_task_t));

	if (!task)
		exit(1);

	task->argument = arg;
	task->task = f;
	return task;
}

/* Add a new task to threadpool task queue */
void add_task_in_queue(os_threadpool_t *tp, os_task_t *t)
{
    os_task_queue_t *task_queue = (os_task_queue_t *) malloc(sizeof(os_task_queue_t));

	if (!task_queue)
		exit(1);

	task_queue->task = t;
	task_queue->next = NULL;

	pthread_mutex_lock(&tp->taskLock);

	os_task_queue_t *current_node = tp->tasks;

	if (current_node == NULL) {
		tp->tasks = task_queue;
	} else {
		while (current_node->next != NULL)
			current_node = current_node->next;

		current_node->next = task_queue;
	}

	pthread_mutex_unlock(&tp->taskLock);
}

/* Get the head of task queue from threadpool */
os_task_t *get_task(os_threadpool_t *tp)
{
    os_task_queue_t *task_queue = NULL;

	pthread_mutex_lock(&tp->taskLock);

	if (tp->tasks != NULL) {
		task_queue = tp->tasks;
		tp->tasks = tp->tasks->next;
	}

	pthread_mutex_unlock(&tp->taskLock);

	os_task_t *task = NULL;

	if (task_queue != NULL) {
		task = task_queue->task;
		free(task_queue);
	}

	return task;
}

/* === THREAD POOL === */

/* Initialize the new threadpool */
os_threadpool_t *threadpool_create(unsigned int nTasks, unsigned int nThreads)
{
    os_threadpool_t *tp = (os_threadpool_t *)malloc(sizeof(os_threadpool_t));

	if (!tp)
		exit(1);

	tp->num_threads = nThreads;
	pthread_mutex_init(&tp->taskLock, NULL);

	tp->threads = malloc(sizeof(pthread_t) * nThreads);
	if (!tp->threads) {
		free(tp);
		exit(1);
	}

	for (int i = 0; i < nThreads; i++) {
		if (pthread_create(&tp->threads[i], NULL, thread_loop_function, tp) != 0) {
			threadpool_stop(tp, NULL);
			exit(1);
		}
	}

	return tp;
}

/* Loop function for threads */
void *thread_loop_function(void *args)
{
    os_threadpool_t *tp = (os_threadpool_t *)args;

    // cat timp conditiile de oprire ale thradpoolului nu sunt implinite fiecare
    // thread poate sa ia un task din coada
	while (!tp->should_stop || tp->tasks != NULL) {
		os_task_t *task = get_task(tp);

		if (task == NULL)
			continue;

		task->task(task->argument);
		free(task);
	}

	return NULL;
}

/* Stop the thread pool once a condition is met */
void threadpool_stop(os_threadpool_t *tp, int (*processingIsDone)(os_threadpool_t *))
{
    if (processingIsDone != NULL) {
		while (1) {
			if (processingIsDone(tp) == 1) {
				tp->should_stop = 1;
				break;
			}
		}
	}

	for (int i = 0; i < tp->num_threads; i++)
		pthread_join(tp->threads[i], NULL);

	os_task_queue_t *task_queue = tp->tasks;

	while (task_queue != NULL) {
		os_task_queue_t *tmp = task_queue;

		task_queue = task_queue->next;
		free(tmp->task);
		free(tmp);
	}

	pthread_mutex_destroy(&tp->taskLock);
	free(tp->threads);
	free(tp);
}
