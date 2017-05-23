#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	pid_t pid;
	int fds[2];
	char buf[256];
	int nwr = 0;

	pipe(fds);

	pid=fork();
	if(pid == 0){
		printf("child close write %d\n",fds[1]);
		close(fds[1]);
		nwr = read(fds[0],buf,sizeof(buf));
		printf("child recv: %s",buf);
	}else{
		printf("parent close read %d\n",fds[0]);
		close(fds[0]);
		strcpy(buf,"something mine nothing\n");
		nwr = write(fds[1],buf,sizeof(buf));
	}

	return 0;
}
