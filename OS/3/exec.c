#include<stdio.h>
#include<unistd.h>

int main(void)
{
	pid_t pid = fork();
	if(pid ==-1){
	}else if (pid==0){
		printf("this is the child proc\n");
		char *argv[]={"ls","-al","/etc/passwd",NULL};
		char *envp[]={"PATH=/bin",NULL};
		execve("/bin/ls",argv,envp);

		printf("test\n");
	}else{
		printf("this is the parent proc. child pid is %d\n",pid);
		getchar();
	}
	return 0;
}
