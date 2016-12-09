#include<apue.h>
#include<time.h>
#include<stdio.h>
#include<iostream>
#include<queue>
using namespace std;

void print_adj(int);
void print_path(int);

#define arraysize 6001
int inf = 0x7fffffff;
int capacity[arraysize][arraysize];
int flow[arraysize];
int pre[arraysize];
queue<int> myqueue;

bool judge_BFS(int src,int des)
{
	queue<int> judge;
	bool visa[arraysize];
	memset(visa,0,sizeof(visa));
	
	int i=0,count=0;
	judge.push(src);
	visa[src]=true;

	while(!judge.empty()){
		
		int v=judge.front();
		judge.pop();
		count++;

		for(i=1;i<=des;i++){
			if(capacity[v][i] && !visa[i]){
				judge.push(i);
				visa[i]=true;
			}
		}
	}
	
	if(count==des)
		return true;
	return false;

}

int BFS(int src,int des,int n)
{
	int i=0,j=0;
	while(!myqueue.empty())
		myqueue.pop();

	for(i=1;i<=n;i++)
		pre[i]=-1;

	pre[src]=0;
	flow[src]=inf;
	myqueue.push(src);

	while(!myqueue.empty()){

		int index=myqueue.front();
		myqueue.pop();
		if(index==des)
			break;
		for(i=1;i<=n;i++){
			if(i!=src && capacity[index][i]>0 && pre[i]==-1){
				pre[i]=index;
				flow[i] = capacity[index][i] < flow[index] ? capacity[index][i] : flow[index];
				myqueue.push(i);
			}
		}
	}

	if(pre[des]==-1)
		return -1;

	print_path(des);
	return flow[des];
}

int maxFlow(int src,int des,int n)
{
	int add = 0;
	int sumflow = 0;
	int count=0;

	while((add=BFS(src,des,n))!=-1){

		int k=des;
		while(k!=src){
			int last = pre[k];
			capacity[last][k] -= add;
			capacity[k][last] += add;
			k=last;
		}
		sumflow += add;
		count++;
	}
	printf("path: %d\n",count);
	return sumflow;
}

void create1(int v,int e)
{
	if(v>arraysize || e>arraysize*arraysize)
		return;

	int i=0,j=0,count=0,count2=0;

	memset(capacity,0,sizeof(capacity));
	memset(flow,0,sizeof(flow));

	for(i=1;i<v;i++){
		count2=0;
		for(j=2;j<=v;j++){
			if(i==j)
				continue;
			int temp=rand()%4;
			if(temp>0){
				count++;
				count2++;
			}
			capacity[i][j]=temp;
			if(count==e)
				return;
			if(count2>e/v)
				break;
		}
	}
}

void print_adj(int des)
{
	if(des>10)
		return;

	int i=0,j=0;

	printf("    ");
	for(i=1;i<=des;i++)
		printf("%2d ",i);

	printf("\n");

	for(i=1;i<=des;i++){
		printf("%2d  ",i);
		for(j=1;j<=des;j++)
			printf("%2d ",capacity[i][j]);
		printf("\n");
	}
	printf("\n");
}

void print_path(int des)
{
	int k=des;
	while(k!=1){
		printf("%d <-- ",k);
		k=pre[k];
	}
	printf("1  add flow: %d\n",flow[des]);
}

int main(void)
{
	srand(time(NULL));

	memset(flow,0,sizeof(flow));
	memset(pre,0,sizeof(pre));
	memset(capacity,0,sizeof(capacity));

	int n=0,e=0;
	//int i=50;
	time_t s1,e1,sum=0;
	cin>>n;
	//cin>>e;

	/*
	while(1){
		create1(n,e);
		if(judge_BFS(1,n))
			break;
	}*/

	/*
	while(i--){
		while(1){
			create1(n,e);
			if(judge_BFS(1,n))
				break;
		}
		printf("begin\n");
		s1=clock();
		maxFlow(1,n,n);
		e1=clock();
		printf("%ld ms\n",e1-s1);
		sum += e1-s1;
	}*/

	//printf("time %ld ms\n",(long)sum/1000);
	
	int i=0,j=0;
	for(i=1;i<7;i++)
		for(j=1;j<7;j++)
			cin>>capacity[i][j];


	print_adj(n);

	printf("Max: %d\n",maxFlow(1,n,n));

	exit(0);
}
