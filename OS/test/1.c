#include<apue.h>
#include<stdio.h>
#include<stdlib.h>

#define M 1024*1024

int main(void)
{
	printf("%ld\n",getpid());
	char* ptr[6];
	int i=0;
	for(i=0;i<6;i++){
		ptr[i] = (char*)malloc(128*M);
	}

	scanf("%d",&i);

	free(ptr[1]);
	free(ptr[2]);
	free(ptr[4]);

	scanf("%d",&i);

	char *big = (char*)malloc(1024*M);
	printf("big : %x\n",big);

	scanf("%d",&i);

	char *small = (char*)malloc(64*M);
	printf("small : %x\n",small);

	scanf("%d",&i);

	return 0;


}
