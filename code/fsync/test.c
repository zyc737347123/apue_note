#include<apue.h>
#include<unistd.h>
#include<stdio.h>

int main(void)
{
	FILE *fp;
	if((fp=fopen("./test.c","rb")) == NULL)
		err_sys("fopen error");
	int fd=0;
	if((fd=fileno(fp))<0)
		err_sys("fileno error");

	//key
	fflush(fp);
	fsync(fd);

	return 0;
}
