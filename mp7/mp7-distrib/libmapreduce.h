/* 
 * CS 241
 * The University of Illinois
 */
 
/** @file libmapreduce.h*/

#ifndef _LIBMAPREDUCE_H_
#define _LIBMAPREDUCE_H_

#include <pthread.h>
#include "libdictionary.h"

typedef struct _mapreduce_t
{
	void (*mr_map)(int fd, const char *data);
	const char *(*mr_reduce)(const char *value1, const char *value2);
	dictionary_t result;
	pthread_mutex_t mr_lock;
	pthread_barrier_t mr_barrier;
	int map_num;
	char **buffers;
} mapreduce_t;

typedef struct _pipefd_t
{
	int fd[2];
} pipefd_t;

typedef struct _worker_t
{
	int map_num;
	mapreduce_t *mr;
	pipefd_t *fds;
} worker_t;

typedef struct _buffers_t
{
	int num;
	char **buffers;
} buffers_t;

/**
 * This function will be the first call made to the libmapreduce library. 
 * You should put any initialization logic here
 * @param mymap map function
 * @param myreduce reduce function
 * @return Void
 */
void mapreduce_init(mapreduce_t *mr,
	                void (*mymap)(int, const char *),
					const char *(*myreduce)(const char *, const char *));
					
/**
 * Main function of this MP.
 * @param values contains a NULL-terminated array of C-strings.
 * @return Void
 */
void mapreduce_map_all(mapreduce_t *mr, const char **values);

/**
 * This function should block until all map()'ing and reduce()'ing has completed.
 */
void mapreduce_reduce_all(mapreduce_t *mr);

/**
 * This function should return the current value of result_key. If the result_key does not exist, return NULL. 
 */
const char *mapreduce_get_value(mapreduce_t *mr, const char *result_key);

/**
 * Free all your memory. :)  Will always be called last.
 */
void mapreduce_destroy(mapreduce_t *mr);

#endif
