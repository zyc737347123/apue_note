#include<sys/sem.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "sem.h"


void sem_get(int *sem,int key)
{
	if((*sem=semget((key_t)key,1,0666 | IPC_CREAT | IPC_EXCL))==-1){
		// sem has inti
		printf("sem %d had init\n",key);
		if((*sem = semget((key_t)key,1,0666 | IPC_CREAT))==-1){
			printf("sem %d get error\n",key);
			exit(0);
		}
	}else{
		// we need init sem
		set_semvalue(*sem);
	}
}

int set_semvalue(int sem)
{
	union semun sem_union;

	// must before use sem
	sem_union.val = 1; // init sem value
	if(semctl(sem,0,SETVAL,sem_union) == -1)
		return 0;
	return 1;
}

void del_semvalue(int sem)
{
	union semun sem_union;

	if(semctl(sem,0,IPC_RMID,sem_union) == -1)
		;
}

int sem_p(int sem)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; // P()
	sem_b.sem_flg = SEM_UNDO; // can avoid deadlock
	if(semop(sem,&sem_b,1) == -1)
		return 0;
	return 1;
}

int sem_v(int sem)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; //V()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem,&sem_b,1) == -1)
		return 0;
	return 1;
}
