#include<apue.h>
#include "heap.h"

void pf(int *a,int n)
{
	int i=0;
	for(i=0;i<n;i++)
		printf("%d ",a[i]);
	printf("\n");
}

int main(void)
{
	int a[10]={9,12,17,30,50,20,60,65,4,19};
	MakeMinHeap1(a,10);
	pf(a,10);
	MinHeapSort(a,10);
	pf(a,10);
	exit(0);
}
