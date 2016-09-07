#include<apue.h>
#include<sys/wait.h>//for waitpid()

static void sig_int(int);

int main(void)
{
	char buf[MAXLINE];
	pid_t pid;
	int status;

	if(signal(SIGINT,sig_int)==SIG_ERR)//捕捉中断信号
		err_sys("signal error");

	
	
	printf("%% ");/* %%  */
	while(fgets(buf,MAXLINE,stdin)!=NULL){
		if(buf[strlen(buf)-1]=='\n')
			buf[strlen(buf)-1]=0; /*diffent in fgets() and excelp() */
		if((pid=fork())<0){
			err_sys("fork error");
		}else if(pid==0){ /* child */
			execlp(buf,buf,(char *)0);
			err_ret("coukdn't execute: %s",buf);
			exit(127);
		}

		if((pid=waitpid(pid ,&status ,0))<0)//为了正确输出‘%’
			err_sys("waitpid error");
		printf("%% ");
	}
}

void sig_int(int signo)
{
	printf("interrupt\n%% ");
}
