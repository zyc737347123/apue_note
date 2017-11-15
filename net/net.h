#ifndef _NET_H
#define _NET_H

#include <apue.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>

void daemonize(const char* cmd);

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen);

void set_fl(int fd, int flags);

#endif
