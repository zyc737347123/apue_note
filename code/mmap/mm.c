#include<apue.h>
#include<stdio.h>
#include<stdlib.h>

#define K 1024

typedef struct{
	int anog[(64*K)][1024]; // 1MB
}mb;

int main(void)
{
	char *ptr;
	printf("%ld\n",sizeof(mb)/1024);
	getchar();
	getchar();
	ptr = (char*)malloc(sizeof(mb));
	printf("%0x\n",ptr);
	int i=0;/*
	for(i=0;i<64*K*K;i++)
		ptr[i]='a';*/
	getchar();
	return 0;
}
