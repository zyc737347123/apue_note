#include<apue.h>
#include<stdio.h>
#include<stdlib.h>

#define K 1024

typedef struct{
	int anog[(64*K)][1024]; // 1MB
}mb;

int main(void)
{
	mb *ptr;
	printf("%ld\n",sizeof(mb)/1024);
	ptr = (mb*)malloc(sizeof(mb));
	getchar();
	return 0;
}
