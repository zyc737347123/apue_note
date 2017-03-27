#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>


int tpf(const char*,...);

int main(void)
{
	pid_t pid;
	pid=fork();

	if(pid==0){
		sleep(5);
		tpf("Hellp from Child Pocess!\n");
		tpf("I am calling exce\n");
		execl("/bin/ps","-a",NULL);
		tpf("you never can see me\n");
	}else if(pid !=-1){
		tpf("hello from parent,pid %d\n",getpid());
		sleep(1);
		tpf("paretn is wait for child to exit\n");
		waitpid(pid,NULL,0);
		tpf("parent had exit\n");
	}

	exit(0);
}

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
