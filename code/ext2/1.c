#include<apue.h>
#include<stdio.h>
#include<fcntl.h>
#include<fcntl.h>

int main(void)
{
	int fd=0;
	struct stat sbuf;
	fd=open("./1.txt",O_RDWR);
	stat("./1.txt",&sbuf);
	printf("%ld,%ld,%ld\n",sbuf.st_blocks,sbuf.st_blksize,sbuf.st_size);
	exit(0);
}
