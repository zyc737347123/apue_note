#include<apue.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdio.h>
#include<string.h>
#include "sem.h"

int sem_shm;
int sem_full;
int sem_empty;
int shm_id;

int main(void)
{
	// get sem and init sem
	sem_get(&sem_shm,1001);

	sem_get(&sem_empty,1002);

	sem_get(&sem_full,1003);

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
		sem_p(sem_shm);

		i = sbuf->count;
		if(i<0){
			sbuf->count=0;
			i=0;
		}
		if(sbuf->count>=1023){
			sem_v(sem_shm);
			sem_v(sem_full);
			sem_p(sem_empty);
			sem_p(sem_shm);
		}
		
		memset(inbuf,0,1024);
		fgets(inbuf,1024,stdin);
		int len = strlen(inbuf);
		len++; // include \0
		len = 1024-sbuf->count >= 0 ? len : 1024-sbuf->count;
		strncpy(&(sbuf->buf[i]),inbuf,len);
		//printf("%d\n",i);
		sbuf->count += len;
		sbuf->buf[1023]=0;
		sem_v(sem_shm);
		if(strncmp(inbuf,"end",3)==0)
			run=0;
		sleep(1);
	}

	// free the shm 
	if(shmdt(shm)==-1)
		err_sys("shmdt error");

	exit(0);
}
