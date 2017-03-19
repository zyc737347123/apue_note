#include<apue.h>
#include<unistd.h>
#include<stdio.h>

#define Write 1
#define Read 0

int main(void)
{
	int fifo[2]; // as for fd
	char buf[80];
	pid_t pid;

	if(pipe(fifo)!=0)
		err_sys("pipe error");

	if((pid=fork())<0){
		err_sys("fork error");
	}else if(pid > 0){
		printf("This is in the father\n");
		char s[]="form father\n";
		write(fifo[Write],s,sizeof(s));
		close(fifo[Write]);
		close(fifo[Read]);
	}else if(pid==0){
		printf("This is in the child\n");
		read(fifo[Read],buf,sizeof(buf));
		printf("%s\n",buf);
		close(fifo[Write]);
		close(fifo[Read]);
	}

	waitpid(pid,NULL,0);
	return 0;
}
