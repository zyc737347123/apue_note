#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void thread_func(void)
{
	printf("this is a thread.\n");
	sleep(10);
}

int main(void)
{
	pthread_t id[100];
	int i,ret;
	for(i=0;i<100;i++){
	ret = pthread_create(&id[i],NULL,(void*)thread_func,NULL);
	if(ret!=0){
		printf("Create pthread error!\n");
		exit(1);
	}
	}

	printf("this is main thread.\n");
	for(i=0;i<100;i++){
		pthread_join(id[i],NULL);
	}
	return 0;
}
