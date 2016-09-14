#include<apue.h>

int main(void)
{
	printf("father %ld\n",(long)getppid());
	printf("hello %ld\n",(long)getpid());
	exit(0);
}
