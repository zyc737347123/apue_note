#include<limits.h>
#include<apue.h>
//#include<heapsort.h>
#include<time.h>
#include<pthread.h>
#include<iostream>
using namespace std;

#define LEN 8000000


extern "C"{
#include<heapsort.h>
//void MakeMinHeap1(int *a,int n,int(*comp_func)(const void *,const void*));
//void MinHeapSort(int *a,int n,int(*comp_func)(const void *,const void*));
}



int nums[LEN];

int comp(const void *arg1,const void *arg2)
{
	int a=*(int*)arg1;
	int b=*(int*)arg2;

	if(a==b)
		return 0;
	if(a<b)
		return -1;
	return 1;
}

void *thr(void *arg)
{
	return (void*)0;
}

int main(void)
{
	srand(time(NULL));
	int i=0;
	time_t start,end;

	for(i=0;i<LEN;i++){
		nums[i]=rand()%INT_MAX;
	}

	//pthread_t tid;
	//pthread_create(&tid,NULL,thr,NULL);

	start=clock();
	MakeMinHeap1(nums,LEN,comp);
	MinHeapSort(nums,LEN,comp);
	end=clock();

	printf("%ld\n",end-start);
	exit(0);

}
