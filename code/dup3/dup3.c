#include<apue.h>
#include<fcntl.h>
#include<unistd.h>

int dup3(int fd);
int dup4(int fd,int fd2);
void get_FL(int val);

int main(int argc,char* argv[])
{
	int fd;
	if((fd=dup3(0))<0)
		err_sys("dup3 error");
	if(write(fd,"hello\n",strlen("hello\n"))<6)
		err_sys("write error");
	printf("fd %d\n",fd);
	printf("%ld\n",strlen("hello"));	
	exit(0);
}

int dup3(const int fd)
{
	long fl;
	int new_fd=-1;
	char dir[]="/dev/fd/";
	char buffer[64];

	if((fl=fcntl(fd,F_GETFL,0))<0)
		err_sys("get %d  FL error",fd);
	get_FL(fl);

	snprintf(buffer,sizeof(buffer),"%s%d",dir,fd);
	
	if((new_fd=open(buffer,fl))<0)
		err_sys("dup3 %d error",fd);

	
	return new_fd;
}

int dup4(int fd,int fd2)
{
	fd2=-1;
	return fd2;
}

void get_FL(int val)
{
	switch(val & O_ACCMODE)
	{
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read write");
			break;
		default:
			err_dump("unknow access mode");
	}

	if(val & O_APPEND)
		printf(", append");
	if(val & O_NONBLOCK)
		printf(", nonblocking");
	if(val & O_SYNC)
		printf(", synchronous");
	
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)// _POSIX_C_SOURCE 功能测试宏
	if(val & O_FSYNC)
		printf(", synchronous writes");
#endif
	putchar('\n');
}
