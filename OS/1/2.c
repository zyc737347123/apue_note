#include<apue.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>

void sayhi(pid_t p,char *str)
{
	printf("%s,pid: %d(ppid:%d),say hi\n",str,p,getppid());
	sleep(1);
}

int main(void)
{
	int i=0;
	pid_t p1,p2,p3,p4;
	p1=getpid();
	if((p2=fork())==0){
		//child p2
		if((p3=fork())==0){
			//grand p3
			for(i=0;i<3;i++)
				sayhi(getpid(),"p3");
		}else{
			//child p2
			for(i=0;i<3;i++){
				sayhi(getpid(),"p2");
			}
			waitpid(p3,NULL,0);
			printf("p2 say: p3:%d is dead\n",p3);
		}
	}else{
		//father p1
		if((p4=fork())==0){
			//child p4
			for(i=0;i<3;i++)
				sayhi(getpid(),"p4");
		}else{
			//father p1
			for(i=0;i<3;i++)
				sayhi(getpid(),"p1");
			waitpid(p2,NULL,0);
			printf("p1 say: p2:%d is dead\n",p2);
			waitpid(p4,NULL,0);
			printf("p1 say: p4:%d is dead\n",p4);
			printf("father(p1:%d) is dead\n",p1);
		}
	}
	exit(0);
}
