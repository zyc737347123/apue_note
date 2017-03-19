/*
 * 为了验证pipe()写入口的堵塞性
 */

#include<apue.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#define K 1024
#define len 128*K

int main(void)
{
	int result = -1;
	int fd[2],nb=0;
	pid_t pid;
	char buf[len]="fuck baby\n";
	char readbuf[10*K];

	if(pipe(fd)!=0)
		err_sys("pipe error");

	if((pid=fork())<0){
		err_sys("fork error");
	}else if(pid==0){
		int write_size=len;
		result=0;
		close(fd[0]);
		while(write_size>0){
			result=write(fd[1],buf,write_size); //这里会堵塞
			printf("hello\n");
			fflush(stdout);
			if(result>0){
				write_size -= result;
				printf("wrire %d bytes data,the rest is %d bytes\n",write_size,result);
			}else{
				printf("go to sleep\n");
				sleep(10);
			}
		}
	}else{
		close(fd[1]);
		while(1){
			nb=read(fd[0],readbuf,sizeof(readbuf));
			if(nb<=0){
				printf("no data to receive\n");
				break;
			}
			printf("receive %d bytes data:%s\n",nb,readbuf);
			fflush(stdout);
		}
	}

	waitpid(pid,NULL,0);
	return 0;
}
