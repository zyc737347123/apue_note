#include<stdio.h>
#include<unistd.h>

int main(void)
{
	pid_t pid = fork();
	if(pid ==-1){
	}else if (pid==0){
		printf("this is the child proc\n");
		fork();
		getchar();
	}else{
		printf("this is the parent proc. child pid is %d\n",pid);
		fork();
		getchar();
	}
	return 0;
}
