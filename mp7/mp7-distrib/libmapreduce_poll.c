/* 
 * CS 241
 * The University of Illinois
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>

#include "libmapreduce.h"
#include "libdictionary.h"

static const int BUFFER_SIZE = 1024 * 1024;


static void process_key_value(const char *key, const char *value, mapreduce_t *mr)
{
#ifdef DEBUG	
	printf("process_key_value\n");
#endif
	if(value == NULL) {
		printf("process_key_value error, value error\n");
		return;
	}

	const char *old_value = dictionary_get(&mr->result, key);
	if(old_value == NULL) {
		dictionary_add(&mr->result, key, value);
		// if multi thread , need check dictionary_add return value;
	}else {
		const char *new_value = mr->mr_reduce(old_value, value);
		if(dictionary_remove(&mr->result, key)){
			printf("process_key_value error, remove error\n");
			return;
		}
		dictionary_add(&mr->result, key, new_value);
	}
#ifdef DEBUG
	printf("leave process_key_value\n");
#endif
}


static int read_from_fd(int fd, char *buffer, mapreduce_t *mr)
{
	/* Find the end of the string. */
	int offset = strlen(buffer);

	/* Read bytes from the underlying stream. */
	int bytes_read = read(fd, buffer + offset, BUFFER_SIZE - offset);
	if (bytes_read == 0){
		return 0;
	}
	if(bytes_read < 0){
		fprintf(stderr, "error in read.\n");
		return -1;
	}
	buffer[offset + bytes_read] = '\0';

	/* Loop through each "key: value\n" line from the fd. */
	char *line;
	while ((line = strstr(buffer, "\n")) != NULL)
	{
		*line = '\0';

		/* Find the key/value split. */
		char *split = strstr(buffer, ": ");
		if (split == NULL)
			continue;

		//assert(split != NULL);

		/* Allocate and assign memory */
		char *key = (char*)malloc((split - buffer + 1) * sizeof(char));
		char *value = (char*)malloc((strlen(split) - 2 + 1) * sizeof(char));

		strncpy(key, buffer, split - buffer);
		key[split - buffer] = '\0';

		strcpy(value, split + 2);

		/* Process the key/value. */
		process_key_value(key, value, mr);

		/* Shift the contents of the buffer to remove the space used by the processed line. */
		memmove(buffer, line + 1, BUFFER_SIZE - ((line + 1) - buffer));
		buffer[BUFFER_SIZE - ((line + 1) - buffer)] = '\0';
	}

	return 1;
}

void mapreduce_init(mapreduce_t *mr, 
			void (*mymap)(int, const char *), 
				const char *(*myreduce)(const char *, const char *))
{
	if(mr == NULL)
		return;

	mr->mr_map = mymap;
	mr->mr_reduce = myreduce;
	
	dictionary_init(&mr->result);

	pthread_mutex_init(&mr->mr_lock, NULL);

	pthread_barrier_init(&mr->mr_barrier, NULL, 2);
}

void cleanup(void *arg)
{
	buffers_t *clean = (buffers_t*)arg;
	int i = 0;
	for(i = 0 ; i < clean->num ; i++)
		free(clean->buffers[i]);
	free(clean->buffers);
	free(clean);
}

void *reduce_worker(void *worker)
{
#ifdef DEBUG
	printf("reduce_worker\n");
#endif

	worker_t *w = (worker_t*)worker;
	int map_num = w->map_num, i = 0, poll_res = 0, read_res = 0;
	int all_close = 0;
	char **buffers = NULL;
	struct pollfd *pollfds = NULL;
	buffers_t *clean = NULL;
	
	pipefd_t *fds = w->fds;
	mapreduce_t *mr = w->mr;

	buffers = (char**)malloc(map_num * sizeof(char*)); // for reduce
	for(i = 0 ; i < map_num ; i++){
		buffers[i] = (char*)malloc(BUFFER_SIZE + 1);
	}

	// set pthread cleanup 
	clean = (buffers_t*)malloc(sizeof(buffers_t));
	clean->num = map_num;
	clean->buffers = buffers;

	pthread_cleanup_push(cleanup, (void*)clean);
	
	// init poll fd set
	pollfds = (struct pollfd*)malloc(map_num * sizeof(struct pollfd));
	for(i = 0 ; i < map_num ; i++){
		pollfds[i].fd = fds[i].fd[0]; // read fd
		pollfds[i].events = POLLIN;
	}

	// process map output 
	while((poll_res = poll(pollfds, map_num, -1)) > 0){
		for(i = 0 ; i < map_num ; i++){
			if(poll_res == 0)
				break;
			if(pollfds[i].revents & POLLIN){
				for(;;){
					read_res = read_from_fd(pollfds[i].fd, buffers[i], mr);
					if(read_res <= 0)
						break;
				}
				poll_res--;
			}
			if(pollfds[i].revents & POLLHUP){
				all_close++;
			}
		}

		if(all_close == map_num)
			break;
		else
			all_close = 0;
	}

	for(i = 0 ; i < map_num ; i++)
		close(fds[i].fd[0]);

	pthread_barrier_wait(&mr->mr_barrier);
	
	// clean
	free(w->fds);
	free(w);
	
#ifdef DEBUG
	printf("leave reduce_worker\n");
#endif 

	pthread_exit((void*)0);

	pthread_cleanup_pop(1);
}


void mapreduce_map_all(mapreduce_t *mr, const char **values)
{
#ifdef DEBUG
	printf("mapreduce_map_all\n");
#endif
	int map_num = 0, i = 0, res = 0;
	pid_t pid;
	pthread_t tid;

	while(values[i] != NULL){
		map_num++;
		i++;
	}

	pipefd_t *fds = (pipefd_t*)malloc(map_num * sizeof(pipefd_t));
	worker_t *worker = (worker_t*)malloc(sizeof(worker_t));

	// open pipe
	for(i = 0 ; i < map_num ; i++){
		res = pipe(fds[i].fd);
		if(res == -1){
			perror("pipe");
				return;
		}
	}
	
	for(i = 0 ; i < map_num ; i++){
		pid = fork();
		// if pid = --1, error
		if(pid == 0){
			close(fds[i].fd[1]); // father close write fd
			continue;
		}else if(pid > 0){
			close(fds[i].fd[0]); // child close read fd
			mr->mr_map(fds[i].fd[1], values[i]);
			mapreduce_destroy(mr);
#ifdef DEBUG
			printf("map(%d) pid: %d\n", i, pid);
#endif
			exit(0);
		}
	}

	// start reduce work
	worker->map_num = map_num;
	worker->fds = fds;
	worker->mr = mr;
	
	res = pthread_create(&tid, NULL, reduce_worker, (void*)worker);
#ifdef DEBUG
	printf("leave mapreduce_map_all\n");
#endif
}

void mapreduce_reduce_all(mapreduce_t *mr)
{
#ifdef DEBUG
	printf("mapreduce_reduce_all\n");
#endif
	pthread_barrier_wait(&mr->mr_barrier);
#ifdef DEBUG
	printf("leave mapreduce_reduce_all\n");
#endif
}

const char *mapreduce_get_value(mapreduce_t *mr, const char *result_key)
{
#ifdef DEBUG
	printf("mapreduce_get_value\n");
#endif
	const char *result = dictionary_get(&mr->result, result_key);
#ifdef DEBUG
	printf("leave mapreduce_get_value\n");
#endif
	return result;
}

void mapreduce_destroy(mapreduce_t *mr)
{
	dictionary_destroy_free(&mr->result);
	pthread_mutex_destroy(&mr->mr_lock);
	pthread_barrier_destroy(&mr->mr_barrier);
}

