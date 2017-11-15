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
#include <fcntl.h>
#include <sys/epoll.h>

#include "libmapreduce.h"
#include "libdictionary.h"

static const int BUFFER_SIZE = 1024 * 1024;


static void process_key_value(const char *key, const char *value, mapreduce_t *mr)
{
#ifdef DEBUG	
	//printf("process_key_value\n");
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
		
		if(dictionary_remove_free(&mr->result, key)){
			printf("process_key_value error, remove error\n");
			return;
		}
		dictionary_add(&mr->result, key, new_value);
		free((char*)value);	
	}

#ifdef DEBUG
	//printf("leave process_key_value\n");
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
	mr->map_num = 0;
	mr->buffers = NULL;

	dictionary_init(&mr->result);

	pthread_mutex_init(&mr->mr_lock, NULL);

	pthread_barrier_init(&mr->mr_barrier, NULL, 2);
}

void *reduce_worker(void *worker)
{
#ifdef DEBUG
	printf("reduce_worker\n");
#endif

	worker_t *w = (worker_t*)worker;
	int map_num = w->map_num, i = 0, epoll_res = 0, read_res = 0, epollfd;
	struct epoll_event ev, *events;
	int all_close = 0, check = 0;
	char **buffers = NULL;;

	pipefd_t *fds = w->fds;
	mapreduce_t *mr = w->mr;
	buffers = w->mr->buffers;
	
	events = (struct epoll_event*)malloc(map_num * sizeof(struct epoll_event));

	epollfd = epoll_create(1);
	if(epollfd < 0){
		printf("epoll_create error\n");
		return (void*)0;
	}

	for(i = 0 ; i < map_num ; i++){
		ev.data.fd = fds[i].fd[0]; // read fd
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(epollfd, EPOLL_CTL_ADD, fds[i].fd[0], &ev);
	}

	while(1){
		epoll_res = epoll_wait(epollfd, events, map_num, -1);
		for(i = 0 ; i < epoll_res ; i++){
			if(events[i].events & EPOLLIN){
				for(;;){
					// sure read full data for ET model
					read_res = read_from_fd(events[i].data.fd, buffers[i], mr);
					if(read_res <= 0)
						break;
				}
			}
			if(events[i].events & EPOLLHUP){
				all_close++;
				epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
			}
		}
		if(all_close == map_num)
			break;
	}
	
	for(i = 0 ; i < map_num ; i++)
		close(fds[i].fd[0]);
	close(epollfd);
	
	// clean
	free(events);
	free(w->fds);
	free(w);

	check = pthread_barrier_wait(&mr->mr_barrier);
#ifdef DEBUG
	if(check == 0 || check == PTHREAD_BARRIER_SERIAL_THREAD)
		printf("barrier sucess\n");
	else
		printf("barrier error\n");
#endif
	
#ifdef DEBUG
	printf("leave reduce_worker\n");
#endif

	return (void*)0;
}


void mapreduce_map_all(mapreduce_t *mr, const char **values)
{
#ifdef DEBUG
	printf("mapreduce_map_all\n");
#endif

	int map_num = 0, i = 0, res = 0;
	pid_t pid;
	pthread_t tid;
	char ***buffers = &mr->buffers;
	
	// count map num
	while(values[i] != NULL){
		map_num++;
		i++;
	}
	mr->map_num = map_num;

	// alloc memory
	pipefd_t *fds = (pipefd_t*)malloc(map_num * sizeof(pipefd_t));
	worker_t *worker = (worker_t*)malloc(sizeof(worker_t));
	*buffers = (char**)malloc(map_num * sizeof(char*)); // for reduce
	for(i = 0 ; i < map_num ; i++){
		(*buffers)[i] = (char*)malloc(BUFFER_SIZE + 1);
	}

	// create pipe with non_block for epoll ET model 
	for(i = 0 ; i < map_num ; i++){
		res = pipe2(fds[i].fd, O_NONBLOCK);
		if(res == -1){
			perror("pipe");
			return;
		}
	}
	
	// create new process for map
	for(i = 0 ; i < map_num ; i++){
		pid = fork();
		// if pid =i= --1, error
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

	// start reduce work, init worker_t
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
	int i = 0;

	for(i = 0; i < mr->map_num ; i++){
		free(mr->buffers[i]);
	}
	free(mr->buffers);
	mr->buffers = NULL;

	dictionary_destroy_free(&mr->result);
	pthread_mutex_destroy(&mr->mr_lock);
	pthread_barrier_destroy(&mr->mr_barrier);
}

