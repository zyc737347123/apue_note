#include<apue.h>
#include<signal.h>
#include<sys/wait.h>

int main(void)
{
	pid_t p1,p2,p3,p4;

	if((p2=fork())<0)
		err_sys("frok error");
	if(p2>0){
		// partent pa
		printf("this is p1:%d\n",getpid());
		sleep(1);
		//kill(p2,SIGTERM);
		//kill(p2,SIGKILL);
		waitpid(p2,NULL,0);
		printf("\np2:%d is killed\n",p2);
	}else if(p2==0){
		// child p2
		if((p3=fork())==0){
			// grand p3
			if((p4=fork())==0){
				// p4
				sleep(60);
				printf("this is p4:%d(ppid:%d)\n",getpid(),getppid());
			}else if(p4>0){
				// grand p3
				sleep(60);
				printf("this is p3:%d(ppdi:%d)\n",getpid(),getppid());
			}
		}else if(p3>0){
			// child p2
			while(1){
				fprintf(stderr,"p2 ");
			}
		}
	}
	exit(0);
}
