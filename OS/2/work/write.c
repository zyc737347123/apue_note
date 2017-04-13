#include<apue.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include "sem.h"

int sem_shm;
int sem_full;
int sem_empty;
int shm_id;

int main(void)
{
	srand(time(NULL));
	// get sem and init sem
	sem_get(&sem_shm,1001,1);

	sem_get(&sem_empty,1002,1024);

	sem_get(&sem_full,1003,0);

	void *shm =NULL;
	shmbuf *sbuf = NULL;
	char inbuf[1024];

	// get shm and init shm
	if((shm_id=shmget((key_t)1004,sizeof(shmbuf),0666|IPC_CREAT))==-1)
		err_sys("shm get error");
	if((shm=shmat(shm_id,0,0))==(void*)-1)
		err_sys("shmat error");
	sbuf = (shmbuf*)shm;

	int run=1;
	int i=0;
	while(run){
		
		printf("type some\n");
		memset(inbuf,0,1024);
		fgets(inbuf,1024,stdin);
		int len = strlen(inbuf);
		int count=0;
		len++; // include \0

		while(count<len){
			sem_p(sem_empty);

			sem_p(sem_shm);
			i = sbuf->count; // write ptr
			sbuf->buf[i] = inbuf[count];
			i++;
			count++;
			sbuf->count = (sbuf->count + 1)%1024;
			sem_v(sem_shm);
			sem_v(sem_full);
		}

		if(strncmp(inbuf,"#",1)==0)
			run=0;
		sleep(rand()%10);
	}

	// free the shm 
	if(shmdt(shm)==-1)
		err_sys("shmdt error");

	exit(0);
}
