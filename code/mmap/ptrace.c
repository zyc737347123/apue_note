#include<apue.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/ptrace.h>
#include<string.h>

int main(int argc,char *argv[])
{
	int fd=0;
	char filename[200];
	char buf[200];
	memset(filename,0,200);
	memset(buf,0,200);
	sprintf(filename,"/proc/%s/mem",argv[1]);

	if((fd=open(filename,O_RDONLY))<0)
		err_sys("open %s error",filename);

	int pid;
	sscanf(argv[1],"%d",&pid);
	printf("%d\n",pid);
	ptrace(PTRACE_ATTACH,pid,0,0);

	long offset;
	sscanf(argv[2],"%llx",&offset);
	printf("%lx\n",offset);
	off_t r=0;
	if((r=lseek64(fd,offset,SEEK_SET))<0)
		err_sys("lseek error");

	printf("addr: %lx\n",r);

	ssize_t size=0;
	if((size=read(fd,buf,200))<0)
		err_sys("read error");

	printf("%s\n",buf);
	close(fd);
	return 0;
}
