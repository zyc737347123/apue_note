#include<apue.h>
#include<heapsort.h>
#include<stdio.h>

void pf(int *a,int n)
{
	int i=0;
	for(i=0;i<n;i++)
		printf("%d ",a[i]);
	printf("\n");
}

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


int main(void)
{
	int a[10]={9,12,17,30,50,20,60,65,4,19};
	MakeMinHeap1(a,10,comp);
	pf(a,10);
	MinHeapSort(a,10,comp);
	pf(a,10);
	getchar();
	exit(0);
}
