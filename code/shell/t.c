#include<apue.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
	char *argv[]={"more","/home/gen/f",NULL};
	execvp("more",argv);
	return 0;
}
