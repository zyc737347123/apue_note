#include<apue.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
	int fd=0;
	pid_t pid;
	fd=open("./1.c",O_RDWR);
	dup2(fd,STDOUT_FILENO);
	if((pid=fork())==0){
		printf("ch\n");
	}else{
		printf("pr\n");
	}
	return 0;
}
