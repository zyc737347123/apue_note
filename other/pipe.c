#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int pipefd[2];
	while(1){
		if(pipe(pipefd) == -1){
			perror("pipe");
			return 1;
		}
		int i = 0, j;
		for(i = 0 ; i < 100000 ; i++)
			j = i;
		//close(pipefd[0]);
		close(pipefd[1]);
	}
	return 0;
}
