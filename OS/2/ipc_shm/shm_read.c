#include<apue.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<sys/shm.h>
#include<string.h>
#include "shm.h"

int main(void)
{
	srand(time(NULL));

	int run = 1;
	void *shm = NULL;
	struct shared_use_st *shared;
	int shmid=0;

	shmid = shmget((key_t)1234,sizeof(struct shared_use_st),0666 | IPC_CREAT);
	if(shmid == -1)
		err_sys("shmget error");

	if((shm=shmat(shmid,0,0))==(void*)-1)
		err_sys("shmat error");

	printf("SHM attached at %x\n",shm);

	shared = (struct shared_use_st*)shm;
	shared->writeable = 0;
	while(run){
		if(shared->writeable !=0){
			printf("you read %s\n",shared->text);
			sleep(rand()%3);
			shared->writeable = 0;

			if(strncmp(shared->text,"end",3)==0)
				run = 0; 
		}else{
			sleep(1);
		}
	}

	if(shmdt(shm) == -1){
		err_sys("shmdt error");
	}

	if(shmctl(shmid,IPC_RMID,0) == -1)
		err_sys("shmctl error");

	exit(0);
}
