#include<apue.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *thread(void *arg)
{
	int thread_var = 0xff;
	pthread_t tid;
	tid = pthread_self();
	printf("This pthread tid: 0x%lx\n",tid);
	printf("This is a pthread. thread_var addr:%llx\n",&thread_var);
	sleep(100);
	return (void*)0;
}

int main(void)
{
	pthread_t tid1,tid2;
	int i=0,err=0;
	getchar();
	err = pthread_create(&tid1,NULL,thread,NULL);
	err = pthread_create(&tid2,NULL,thread,NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	exit(0);
}
