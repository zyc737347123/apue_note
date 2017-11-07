#ifndef C_HEAPSORT_H
#define C_HEAPSORT_H

void MinHeapFixup(int *a,int i,int(*)(const void *,const void*));
void MinHeapAdd(int *a,int n,int value,int(*comp_func)(const void *,const void*));
void MinHeapFixdown(int *a,int i,int n,int(*)(const void *,const void*));
void MinHeapPop(int a[],int n,int(*comp_func)(const void *,const void*));
void Swap(int *a,int *b);
void MakeMinHeap1(int *a,int n,int(*comp_func)(const void *,const void*));
void MakeMinHeap2(int *a,int n,int(*comp_func)(const void *,const void*));
void MinHeapSort(int *a,int n,int(*comp_func)(const void *,const void*));

#endif
