#include<apue.h>

static void charatatime(char *);

int main(void)
{
	pid_t pid;

	TELL_WAIT();

	if((pid = fork())<0)
		err_sys("fork error");
	else if(pid==0){
		WAIT_PARENT();
		charatatime("abcdefg\n");
	}else{
		charatatime("123456789\n");
		TELL_CHILD(pid);
	}
	exit(0);
}

static void charatatime(char *str)
{
	char *ptr;
	int c=0;

	setbuf(stdout,NULL);
	for(ptr=str;(c = *ptr++) !=0;){
		putc(c,stdout);
		sleep(1);
	}
}
