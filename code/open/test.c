#include<apue.h>
#include<fcntl.h>
#include<sys/wait.h>
int main(void)
{
	char buf1[21];
	char buf3[21];
	char buf2[101];
	buf2[100]='\0';
	buf1[20]='\0';
	buf3[20]='\0';
	pid_t pid;

	const int fd=open("/home/gen/apue/error.c",O_RDONLY);

	if((pid=fork())<0){
		err_sys("frok error");
	}else if(pid==0){
		int n=0;
		printf("\nc fd %d\n",fd);
		if((n=pread(fd,buf1,20,20))!=20)
			err_sys("child read error");
		buf1[20]='\0';
		read(fd,buf3,20);
		printf("buf3\n%s\n",buf3);
		read(fd,buf3,20);
		printf("buf4\n%s\n",buf3);
		printf("buf1\n%s\n",buf1);
	}else{
		int status;
		const  int ffd=open("/home/gen/apue/error.c",O_RDONLY);/* 这里实现了文件表项的分离  */
		printf("p ffd %d\n",ffd);
		int n=0;
		waitpid(pid,&status,0);//这里是等待子进程结束，再调用共享 文件描述符
		if((n=read(fd,buf2,100))!=100)
		   	err_sys("child read error");
		printf("buf2 \n");
		fputs(buf2,stdout);
	}

	exit(0);
}
