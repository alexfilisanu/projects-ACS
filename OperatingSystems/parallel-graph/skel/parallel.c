#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "os_graph.h"
#include "os_threadpool.h"
#include "os_list.h"

#define MAX_TASK 100
#define MAX_THREAD 4

int sum;
os_graph_t *graph;
pthread_mutex_t sum_mutex;
pthread_mutex_t visited_mutex;
os_threadpool_t *threadpool;

// functie apelata de workeri in care se realizeaza suma si adaaugarea vecinilor
// care nu au fost deja adaugati
void executeWork(void *arg)
{
	os_node_t *node = (os_node_t *)arg;

    // adaug valoarea din nod la suma totala, operatie izolata cu ajutorul
    // sum_mutex, pentru a evita race condition
	pthread_mutex_lock(&sum_mutex);
	sum += node->nodeInfo;
	pthread_mutex_unlock(&sum_mutex);

    // adaug in coada toti vecinii nodului care nu au fost vizitati
	for (int i = 0; i < node->cNeighbours; i++) {
		os_node_t *neighbour = graph->nodes[node->neighbours[i]];

		// verificarea daca un vecin a fost adaugat sau nu in coada (informatie salvata
		// in graph->visited) se face tot cu ajutorul unui mutex pentru a evita race condition
		// daca nu a fost adaugat in coada, creez taskul, il adaug in coada si marchez nodul
		// ca vizitat
		pthread_mutex_lock(&visited_mutex);
		if (graph->visited[neighbour->nodeID] == 0) {
			graph->visited[neighbour->nodeID] = 1;
			pthread_mutex_unlock(&visited_mutex);

			os_task_t *task = task_create(neighbour, executeWork);

			add_task_in_queue(threadpool, task);
		} else {
			pthread_mutex_unlock(&visited_mutex);
		}
	}
}

// functie ce returneaza 1 daca toate nodurile au fost vizitate(conditie de oprire)
int processingIsDone(os_threadpool_t *tp)
{
	for (int i = 0; i < graph->nCount; i++) {
		os_node_t *node = graph->nodes[i];

		if (graph->visited[node->nodeID] == 0)
			return 0;
	}

	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: ./main input_file\n");
		exit(1);
	}

	FILE *input_file = fopen(argv[1], "r");

	if (input_file == NULL) {
		printf("[Error] Can't open file\n");
		return -1;
	}

	graph = create_graph_from_file(input_file);
	if (graph == NULL) {
		printf("[Error] Can't read the graph from file\n");
		return -1;
	}

    // TODO: create thread pool and traverse the graf
    // initializarea celor 2 mutecsi, a threadpool ului si a vectorului de
    // visited al structurii graph
	pthread_mutex_init(&sum_mutex, NULL);
	pthread_mutex_init(&visited_mutex, NULL);
	threadpool = threadpool_create(MAX_TASK, MAX_THREAD);
	graph->visited = (unsigned int *)calloc(graph->nCount, sizeof(unsigned int));
	if (!graph->visited)
		exit(1);

    // adaug in coada taskul pentru nodul 0 si pentru toate nodurile care nu au vecini
    // (fiind radacini pentru cate o componenta conexa) + le marchez ca vizitate
	os_node_t *node = graph->nodes[0];

	graph->visited[node->nodeID] = 1;
	os_task_t *task = task_create(node, executeWork);

	add_task_in_queue(threadpool, task);

	for (int i = 1; i < graph->nCount; i++) {
		node = graph->nodes[i];

		if (node->cNeighbours == 0) {
			graph->visited[node->nodeID] = 1;
			os_task_t *task = task_create(node, executeWork);

			add_task_in_queue(threadpool, task);
		}
	}

    // oprirea threadpoolului cand processingIsDone
	threadpool_stop(threadpool, processingIsDone);

	pthread_mutex_destroy(&sum_mutex);
	pthread_mutex_destroy(&visited_mutex);
	free(graph->visited);

	printf("%d", sum);
	return 0;
}
