#include<heap.h>

inline void Swap(int *a,int *b)
{
	int temp=*a;
	*a=*b;
	*b=temp;
}

void MinHeapFixup(int *a,int i)
{
	int j=0,temp=0;

	temp = a[i];
	j = (i-1)/2;	// father
	while(j >= 0 && i!=0){
		if(a[j] <= temp)
			break;
		a[i] = a[j];
		i = j;
		j = (i-1)/2;
	}
	a[i] = temp;
}

void MinHeapAdd(int *a,int n,int value)
{
	// n is lenth
	a[n] = value;
	MinHeapFixup(a,n);
}

void MinHeapFixdown(int *a,int i,int n)
{
	// n is lenth
	int j=0,temp=0;

	temp = a[i];
	j = 2*i + 1;	//child
	while(j<n){
		if(j+1<n && a[j+1] < a[j]) // find the min child
			j++;
		if(a[j] >= temp)
			break;

		a[i] = a[j];
		i=j;
		j=2*i+1;
	}
	a[i]=temp;
}

void MinHeapPop(int a[],int n)
{
	// n is lenth
	Swap(&a[0],&a[n-1]);
	n--;
	MinHeapFixdown(a,0,n);
}



void MakeMinHeap1(int *a,int n)
{
	// n is lenth
	int i=0;
	for(i = n/2-1;i>=0;i--)
		MinHeapFixdown(a,i,n);
}

void MakeMinHeap2(int *a,int n)
{
	int i=0;
	for(i=1;i<n;i++)
		MinHeapFixup(a,i);
}

void MinHeapSort(int *a,int n)
{
	int i=0;
	for(i=n-1;i>=1;i--){
		Swap(&a[i],&a[0]);
		MinHeapFixdown(a,0,i);
	}
}
