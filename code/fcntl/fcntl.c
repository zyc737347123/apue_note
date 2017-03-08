#include<apue.h>
#include<fcntl.h>

void get_FL(int val);

void set_FL(int fd,int flags);

int main(int argc,char *argv[])
{
	int val;

	if(argc!=2)
		err_quit("usage: a.out fd");

	if((val=fcntl(atoi(argv[1]),F_GETFL,0))<0)
		err_sys("fcntl errir for fd %d",atoi(argv[1]));
	
	get_FL(val);
	
	exit(0);
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

void set_FL(int fd,int flags)
{
	int val;

	if((val=fcntl(fd,F_GETFL,0))<0)
		err_sys("fcntl F_GETFL error");

	val |= flags; /* turn on flags , use | for save old flags  */

	if(fcntl(fd,F_SETFL,val)<0)
		err_sys("fcntl F_SETFL error");
}
