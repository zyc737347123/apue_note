#ifndef _SEM_H_HEADER
#define _SEM_H_HEADER
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};

typedef struct buf{
	int count;
	int index;
	char buf[1024];
}shmbuf;

void sem_get(int *sem,int key,int init_value);
int set_semvalue(int sem,int value);
int sem_p(int sem);
int sem_v(int sem);
void del_semvalue(int sem);
#endif
