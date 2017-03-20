#include<apue.h>
#include<sys/wait.h>

int main(void)
{
	pid_t pid;
	//printf("%d\n",getpid());
	if((pid = fork())<0){
		err_sys("fork error");
	}else if(pid==0){
		// child 1
		if((pid=fork())<0)
			err_sys("fork error");
		else if(pid>0){
			// child 1
			exit(0);
		}

		//grand 1
		sleep(2);
		printf("second child,parent pid = %ld\n",(long)getppid());
		exit(0);
	}

	// parten , pid is child1
	if(waitpid(pid,NULL,0)!=pid)
		err_sys("waitpid error");
	//printf("hello\n");
	exit(0);
}
