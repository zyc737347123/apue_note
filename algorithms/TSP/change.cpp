#include<apue.h>
#include<stdio.h>
#include<time.h>
#include<iostream>
using namespace std;

int main()
{
	srand(time(NULL));

	int n=5533214;
	int i=0,j=0;
	int src=0,dst=0,w=0;
	for(i=0;i<n;i++){
		w=(rand()%9)+1;
		cin>>src;
		cin>>dst;
		cout<<src<<'\t'<<dst<<'\t'<<w<<endl;
	}
	return 1;
}
