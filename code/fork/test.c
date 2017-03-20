#include<apue.h>
#include<unistd.h>

int globval = 6;
char buf[] = "a write to stdout\n";

int main(void)
{
	int var;
	pid_t pid;

	var = 88;
	if(write(STDOUT_FILENO,buf,sizeof(buf)-1) != sizeof(buf)-1)
		err_sys("write error");
	printf("before fork\n");	// don't flush stdout
	// 体现的是行缓冲与全缓冲的区别
	// 当stdout连接到终端设备tty时，是行缓冲，在fork之前，字符就从缓冲区写入到内核了
	// 当stdout连接到文件时，stdout是全缓冲，在fork时，数据仍在缓冲区，被复制到子进程了

	if((pid=fork())<0){
		err_sys("fork error");
	}else if(pid==0){
		globval++;
		var++;
	}else{
		sleep(2);
	}

	printf("pid = %ld, glob = %d, var = %d\n",(long)getpid(),globval,var);
	exit(0);
}
