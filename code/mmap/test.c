#include<apue.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>

int main(void)
{
	char meg[] = "hello world!";
	printf("pid: %d,meg addr: %llx\n",getpid(),meg);
	getchar();
	sleep(100);
	return 0;
}
