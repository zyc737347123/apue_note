#include<apue.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>

int tpf(const char* fmt,...);

int main(void)
{
	pid_t p1,p2,p3,p4,p5;
	if((p2=fork())<0){
		err_sys("fork1 error");
	}
	if(p2==0){
		// first child proc
		sleep(2);
		printf("this is p2,pid: %d\n",getpid());
	}else{
		// partent proc
		printf("this is p1,pid: %d\n",getpid());
		if((p3=fork())==0){
			printf("this is p3,pid: %d\n",getpid());
		}else{
			//waitpid(p2,NULL,0);
			//printf("p2 is dead,pid: %d\n",p2);
			waitpid(p3,NULL,0);
			printf("p3 is dead,pid: %d\n",p3);
			waitpid(p2,NULL,0);
			printf("p2 is dead,pid: %d\n",p2);
			printf("father(p1) is dead,pid: %d\n",getpid());
		}
	}
	exit(0);
}

/*
int main(void)
{
	int i=0,j=0;
	pid_t pid;
	printf("hello from Parten Proc,PID is %d.\n",getpid());

	pid=fork();

	if(pid==0){
		sleep(1);
		for(i=0;i<3;i++){
			printf("hello from Child Proc %d. %d times\n",getpid(),i+1);
			sleep(1);
		}
	}else if(pid !=-1){
		tpf("Parent forked one child proc--%d.\n",pid);
		tpf("Parent is waiting for child to exit.\n");
		waitpid(pid,NULL,0);
		tpf("child proc has exited\n");
		tpf("parent had exited\n");
	}
	exit(0);
}*/

int tpf(const char* fmt,...)
{
	va_list argv;
	struct tm *tstruct;
	time_t sec;
	sec=time(NULL);
	tstruct = localtime(&sec);
	printf("%02d:%02d:%02d: %5d|",tstruct->tm_hour,tstruct->tm_min,tstruct->tm_sec,getpid());
	va_start(argv,fmt);
	return vprintf(fmt,argv);
}
