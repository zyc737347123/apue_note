#include<apue.h>
#include<sys/sem.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "sem.h"

static int sem_id;

int main(int argc,char *argv[])
{
	srand(time(NULL));

	char meg = 'X';
	int i=0;

	sem_id = semget((key_t)1234,1,0666 | IPC_CREAT);

	if(argc>1){
		if(!set_semvalue(sem_id))
			err_sys("failed to init sem");
		meg = argv[1][0];
		sleep(2);
	}

	for(i=0;i<10;i++){
		if(!sem_p(sem_id))
			err_sys("p error");
		printf("%c",meg);
		fflush(stdout);
		sleep(rand()%3);
		printf("%c",meg);
		fflush(stdout);
		if(!sem_v(sem_id))
			err_sys("v error");
		sleep(rand()%2);
	}

	sleep(10);
	printf("\npid: %d\n",getpid());

	if(argc > 1){
		sleep(3);
		del_semvalue(sem_id);
	}

	exit(0);
}


