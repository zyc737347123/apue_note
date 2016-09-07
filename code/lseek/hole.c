#include<apue.h>
#include<fcntl.h>

char buf1[]="abcd";
char buf2[]="efg";

int main(void)
{
	int fd;

	if((fd=open("file.hole",O_TRUNC | O_RDWR))<0)
			err_sys("creat error");
	/*
	if((fd=creat("file",FILE_MODE)<0))
			err_sys("creat error");
	*/
	printf("fd %d\n",fd);

	
	if(write(fd,buf1,strlen(buf1))!=4)
			err_sys("wreite error");

	printf("buf1 %ld\n",strlen(buf1));
	
	
	if(lseek(fd,9997,SEEK_SET)==-1)
		err_sys("lseek error");
	
	if(write(fd,buf2,strlen(buf2))!=3)
		err_sys("write error");
	
	exit(0);
}
