#include<apue.h>
#include<fcntl.h>
#include<sys/wait.h>
int main(void)
{
	char buf1[6]="aaaaa";
	char buf2[6]="bbbbb";
	pid_t pid;

	const int fd=open("/home/gen/apue/1.c",O_RDWR | O_APPEND);//使用 O_APPEND 可以实现分别写，字符串写入顺序不定，但字符串本身不改变

	if((pid=fork())<0){
		err_sys("frok error");
	}else if(pid==0){
		write(fd,buf1,5);	
	}else{
		int status;
		const  int ffd=open("/home/gen/apue/1.c",O_RDWR | O_APPEND);/* 这里实现了文件表项的分离  */
		//waitpid(pid,&status,0);
		write(fd,buf2,5);
	}

	exit(0);
}
