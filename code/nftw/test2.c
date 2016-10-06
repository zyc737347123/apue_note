#include<apue.h>
#include<limits.h>

#ifdef PATH_MAX
static size_t pathmax = PATH_MAX;
#else
static size_t pathmax = 0;
#endif

#define SUSV# 200112L

