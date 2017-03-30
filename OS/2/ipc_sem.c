#include<apue.h>
#include<sys/sem.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};

static int sem_id = 0;
static int set_semvalue(int sem);
static int sem_p(int sem);
static int sem_v(int sem);
static void del_semvalue(int sem);

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

static int set_semvalue(int sem)
{
	union semun sem_union;

	// must before use sem
	sem_union.val = 1; // init sem value
	if(semctl(sem,0,SETVAL,sem_union) == -1)
		return 0;
	return 1;
}

static void del_semvalue(int sem)
{
	union semun sem_union;

	if(semctl(sem,0,IPC_RMID,sem_union) == -1)
		err_sys("delete sem error");
}

static int sem_p(int sem)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; // P()
	sem_b.sem_flg = SEM_UNDO; // can avoid deadlock
	if(semop(sem,&sem_b,1) == -1)
		return 0;
	return 1;
}

static int sem_v(int sem)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; //V()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem,&sem_b,1) == -1)
		return 0;
	return 1;
}
