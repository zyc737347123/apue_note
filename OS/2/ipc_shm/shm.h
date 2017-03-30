#ifndef _SHM_H_HEADER
#define _SHM_H_HEADER

#define BUFSIZE 1024

struct shared_use_st{
	int writeable;
	char text[BUFSIZE];
};

#endif
