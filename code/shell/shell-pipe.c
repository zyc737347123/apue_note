#include <apue.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
	int i=0,idx=0,j=0;
	char *after_argv[100];
	char *pre_argv[100];
	int fds[2];
	pid_t pid;


	for(i=0;i<argc;i++){
		idx=i+1;
		if(strcmp(argv[i],"pp")==0){
			idx--;
			break;	
		}
	}


	j=0;
	for(i=1;i<idx;i++){
		pre_argv[j]=argv[i];
		j++;
	}
	pre_argv[j]=NULL;

	j=0;
	for(i=idx+1;i<argc;i++){
		after_argv[j]=argv[i];
		j++;
	}
	after_argv[j]=NULL;

	if((pid=fork())==0){

		pipe(fds);

		if((pid=fork())==0){
			// write
			dup2(fds[1],1);
			close(fds[1]);
			close(fds[0]);
			execvp(argv[1],pre_argv);
		}else{
			// read
			//if((pid=fork())==0){
			dup2(fds[0],0);
			close(fds[0]);
			close(fds[1]);
			execvp(argv[idx+1],after_argv);
			//}
		}
	}

	waitpid(pid,NULL);
	return 0;
}
