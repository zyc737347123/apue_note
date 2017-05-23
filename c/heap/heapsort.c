#include"heapsort.h"

inline void Swap(int *a,int *b)
{
	int temp=*a;
	*a=*b;
	*b=temp;
}

void MinHeapFixup(int *a,int i,int(*comp_func)(const void *,const void*))
{
	int j=0,temp=0;
	temp = a[i];
	j = (i-1)/2;	// father
	while(j >= 0 && i!=0){
		/*
		if(a[j] <= temp)
			break;*/
		if(comp_func(&a[j],&temp)!=-1)
			break;
		a[i] = a[j];
		i = j;
		j = (i-1)/2;
	}
	a[i] = temp;
}

void MinHeapAdd(int *a,int n,int value,int(*comp_func)(const void *,const void*))
{
	// n is lenth
	a[n] = value;
	MinHeapFixup(a,n,comp_func);
}

void MinHeapFixdown(int *a,int i,int n,int(*comp_func)(const void *,const void*))
{
	// n is lenth
	int j=0,temp=0;

	temp = a[i];
	j = 2*i + 1;	//child
	while(j<n){
		if(j+1<n && comp_func(&a[j+1],&a[j])==1) // find the min child
			j++;
		if(comp_func(&a[j],&temp)!=1)
			break;

		a[i] = a[j];
		i=j;
		j=2*i+1;
	}
	a[i]=temp;
}

void MinHeapPop(int a[],int n,int(*comp_func)(const void *,const void*))
{
	// n is lenth
	Swap(&a[0],&a[n-1]);
	n--;
	MinHeapFixdown(a,0,n,comp_func);
}

void MakeMinHeap1(int *a,int n,int(*comp_func)(const void *,const void*))
{
	// n is lenth
	int i=0;
	for(i = n/2-1;i>=0;i--)
		MinHeapFixdown(a,i,n,comp_func);
}

void MakeMinHeap2(int *a,int n,int(*comp_func)(const void *,const void*))
{
	int i=0;
	for(i=1;i<n;i++)
		MinHeapFixup(a,i,comp_func);
}

void MinHeapSort(int *a,int n,int(*comp_func)(const void *,const void*))
{
	int i=0;
	for(i=n-1;i>=1;i--){
		Swap(&a[i],&a[0]);
		MinHeapFixdown(a,0,i,comp_func);
	}
}
