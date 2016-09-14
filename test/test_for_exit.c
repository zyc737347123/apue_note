#include<apue.h>

int main(void)
{
	pid_t cpid,fpid;
	if((cpid=fork())==0){
		//this is child
		printf("child id is %ld\n",(long)getpid());
		printf("father id is %ld\n",(long)getppid());
		sleep(10);
		printf("now father id is %ld\n",(long)getppid());
	}else{
		//this is father
		printf("pid is %ld\n",(long)getpid());
		sleep(1);
		exit(0);
	}
	return 1;
}
