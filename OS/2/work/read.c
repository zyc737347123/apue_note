#include<apue.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include "sem.h"

int sem_shm;
int sem_full;
int sem_empty;
int shm_id;

int main(void)
{
	srand(time(NULL));

	fork();

	// get sem and init sem
	sem_get(&sem_shm,1001,1);

	sem_get(&sem_empty,1002,1024);

	sem_get(&sem_full,1003,0);

	void *shm =NULL;
	shmbuf *sbuf = NULL;

	// get shm and init shm
	if((shm_id=shmget((key_t)1004,sizeof(shmbuf),0666|IPC_CREAT))==-1)
		err_sys("shm get error");
	if((shm=shmat(shm_id,0,0))==(void*)-1)
		err_sys("shmat error");
	sbuf = (shmbuf*)shm;

	int run=1,j=0;
	while(run){
		sleep(rand()%2);

		sem_p(sem_full);
		if(!sem_p(sem_shm))
			err_sys("sem p error"); // if sem is not exist , proc will exit
		j = sbuf->index; // read ptr
		char c = sbuf->buf[j];
		if(c!=0)
			printf("%c",c);
		fflush(stdout);
		if(c=='#'){
			run = 0;
			printf("\n");
		}
		sbuf->index = (sbuf->index + 1)%1024;
		sem_v(sem_shm);
		sem_v(sem_empty);
		
	}

	// free and delete the shm 
	if(shmdt(shm)==-1)
		err_sys("shmdt error");
	if(shmctl(shm_id,IPC_RMID,0)==-1)
		err_sys("shmctl error");

	// delete sem
	del_semvalue(sem_shm);
	del_semvalue(sem_empty);
	del_semvalue(sem_full);

	exit(0);
}
