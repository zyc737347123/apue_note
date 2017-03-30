#include<apue.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<string.h>
#include<time.h>
#include "shm.h"

int main(void)
{
	srand(time(NULL));

	int run = 1;
	int shmid =0;
	void *shm = NULL;
	struct shared_use_st *shared = NULL;
	char buffer[BUFSIZ + 1];

	if((shmid = shmget((key_t)1234,sizeof(struct shared_use_st),0666 | IPC_CREAT))==-1)
			err_sys("shmget error");
	if((shm=shmat(shmid,0,0))==(void*)-1)
			err_sys("shmat error");

	shared = (struct shared_use_st*)shm;

	while(run){
		while(shared->writeable == 1){
			sleep(1);
			printf("wait..\n");
		}
		printf("type something\n");
		fgets(buffer,BUFSIZ,stdin);
		strncpy(shared->text,buffer,BUFSIZE);
		shared->writeable = 1;
		if(strncmp(buffer,"end",3)==0)
			run = 0;
	}

	if(shmdt(shm)==-1)
		err_sys("shmdt error");
	sleep(2);
	exit(0);
}
