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
	char outbuf[1024];

	// get shm and init shm
	if((shm_id=shmget((key_t)1004,sizeof(shmbuf),0666|IPC_CREAT))==-1)
		err_sys("shm get error");
	if((shm=shmat(shm_id,0,0))==(void*)-1)
		err_sys("shmat error");
	sbuf = (shmbuf*)shm;

	int run=1,i=0;
	while(run){
		sleep(2);
		sem_p(sem_shm);

		i = sbuf->count;
		if(sbuf->count == 0){
			sem_v(sem_shm);
			sem_v(sem_empty);
			sem_p(sem_full);
			sem_p(sem_shm);
		}
		int j=0,k=0;
		memset(outbuf,0,1024);
		for(j=0;j<sbuf->count;j++){
			if(sbuf->buf[j] == '\0'){
				printf("%s",outbuf);
				if(strncmp(outbuf,"end",3)==0)
					run=0;
				memset(outbuf,0,1024);
				k=0;
				continue;
			}
			//fputc(sbuf->buf[j],stdout);
			outbuf[k]=sbuf->buf[j];
			k++;
		}
		sbuf->count -= j+1;
		sem_v(sem_shm);
	}

	// free the shm 
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
