#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int i=0;
	for(i=0;i<100;i++){
		pid_t pid = fork();
		if(pid == 0){
			printf("this is child\n");
			sleep(10);
			return 0;
		}else{
			printf("this is old father,child pid %d\n",pid);
		}
	}

	sleep(100);
	return 0;
}
