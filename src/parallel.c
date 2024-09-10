#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "log/log.h"
#include "os_graph.h"
#include "os_threadpool.h"
#include "utils.h"

#define NUM_THREADS 4

static int sum;
static os_graph_t *graph;
static os_threadpool_t *tp;
static pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t graph_mutex = PTHREAD_MUTEX_INITIALIZER;

static void process_node(void *arg);

typedef struct {
	unsigned int idx;
} graph_task_arg_t;

static void process_node(void *arg)
{
	unsigned int idx = ((graph_task_arg_t *)arg)->idx;
	int local_sum = 0;

	pthread_mutex_lock(&graph_mutex);

	if (graph->visited[idx] != NOT_VISITED) {
		pthread_mutex_unlock(&graph_mutex);
		return;
	}

	graph->visited[idx] = PROCESSING;
	pthread_mutex_unlock(&graph_mutex);


	local_sum += graph->nodes[idx]->info;

	for (unsigned int i = 0; i < graph->nodes[idx]->num_neighbours; ++i) {
		unsigned int neighbour_idx = graph->nodes[idx]->neighbours[i];
		os_task_t *task;

		graph_task_arg_t *task_arg = malloc(sizeof(graph_task_arg_t));
		task_arg->idx = neighbour_idx;

		task = create_task(process_node, task_arg, free);

		pthread_mutex_lock(&graph_mutex);
		enqueue_task(tp, task);
		pthread_mutex_unlock(&graph_mutex);
	}

	pthread_mutex_lock(&sum_mutex);
	sum += local_sum;
	pthread_mutex_unlock(&sum_mutex);

	pthread_mutex_lock(&graph_mutex);
	graph->visited[idx] = DONE;
	pthread_mutex_unlock(&graph_mutex);
}

int main(int argc, char *argv[])
{
	FILE *input_file;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s input_file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	input_file = fopen(argv[1], "r");
	DIE(input_file == NULL, "fopen");

	graph = create_graph_from_file(input_file);

	tp = create_threadpool(NUM_THREADS);

	graph_task_arg_t *task_arg = malloc(sizeof(graph_task_arg_t));
	task_arg->idx = 0;

	os_task_t *task = create_task(process_node, task_arg, NULL);
	enqueue_task(tp, task);

	wait_for_completion(tp);
	destroy_threadpool(tp);

	printf("%d", sum);

	return 0;
}
