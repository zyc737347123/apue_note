#include<apue.h>
#include<pthread.h>
#include<limits.h>
#include<sys/time.h>
#include<time.h>
#include<heapsort.h>

#define NTHR 8
#define NUMNUM 8000000
#define TNUM (NUMNUM/NTHR)

int nums[NUMNUM];
int snums[NUMNUM];

pthread_barrier_t b;


int complong(const void *arg1,const void *arg2)
{
	int a=*(long*)arg1;
	int b=*(long*)arg2;

	if(a==b)
		return 0;
	if(a<b)
		return -1;
	return 1;
}

void *thr_fn(void *arg)
{
	unsigned long idx = (unsigned long)arg;
	//heapsort(&nums[idx],TNUM,sizeof(long),complong);
	MakeMinHeap1(&nums[idx],TNUM,complong);
	MinHeapSort(&nums[idx],TNUM,complong);
	pthread_barrier_wait(&b);
	return((void*)0);
}

void merge()
{
	int idx[NTHR];
	int i,minidx,sidx,num;

	for(i=0;i<NTHR;i++)
		idx[i]=i*TNUM;
	for(sidx = 0;sidx<NUMNUM;sidx++){
		num = INT_MAX;
		for(i=0;i<NTHR;i++){
			if(idx[i]<(i+1)*TNUM && nums[idx[i]] < num){
				num = nums[idx[i]];
				minidx = i;
			}
		}
		snums[sidx] = nums[idx[minidx]];
		idx[minidx]++;
	}
}

int main(void)
{
	unsigned long i;
	time_t start,end;
	int err;
	pthread_t tid;

	srand(time(NULL));
	for(i=0;i<NUMNUM;i++)
		nums[i] = rand()%INT_MAX;

	start = clock();

	pthread_barrier_init(&b,NULL,NTHR+1);
	for(i=0;i<NTHR;i++){
		err = pthread_create(&tid,NULL,thr_fn,(void*)(i*TNUM));
		if(err !=0)
			err_exit(err,"can't crete thread");
	}
	pthread_barrier_wait(&b);
	merge();
	end = clock();

	fprintf(stderr,"%ld\n",end-start);
	for(i=0;i<NUMNUM;i++)
		printf("%d\n",snums[i]);
	exit(0);
}
