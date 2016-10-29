#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<cstring>
using namespace std;

void charu(int* org,int lenth);

void maopao(int* org,int lenth);

void kuaisu(int *org,int left,int right);

void chioce(int *org,int lenth);

void MergeSort(int *org,int n);

void Merge(int *org,int a,int b,int length,int n);

static inline void swap(int &a,int &b)
{
	int t=0;
	t=a;a=b;b=t;
}

int main(void)
{
	srand(time(NULL));
	int n=0,c=0;
	time_t result[5][20],s,e;
	cout<<"please input a number"<<endl;
	cin>>n;

	int *org=new int[n];

	int j=20;
	while(j--){
	for(int i=0;i<n;i++)
		org[i]=rand()%100000;

	int* orga[4];
	for(int i=0;i<4;i++){
		orga[i]=new int[n];
		memcpy(orga[i],org,n*sizeof(int));
	}

		
	s=clock();
	for(int i=0;i<10000;i++){
		for(int k=0;k<n;k++)
			org[k]=rand()%1000;
		charu(org,n);
	}
	e=clock();
	result[0][19-j]=e-s;

	
	s=clock();
	for(int i=0;i<10000;i++){
		for(int k=0;k<n;k++)
			orga[0][k]=rand()%1000;
		maopao(orga[0],n);
	}
	e=clock();
	result[1][19-j]=e-s;

	s=clock();
	for(int i=0;i<10000;i++){
		for(int k=0;k<n;k++)
			orga[1][k]=rand()%1000;
		kuaisu(orga[1],0,n-1);
	}
	e=clock();
	result[2][19-j]=e-s;

	s=clock();
	for(int i=0;i<10000;i++){
		for(int k=0;k<n;k++)
			orga[2][k]=rand()%1000;
		chioce(orga[2],n);
	}
	e=clock();
	result[3][19-j]=e-s;
	
	s=clock();
	for(int i=0;i<10000;i++){
		for(int k=0;k<n;k++)
			orga[3][k]=rand()%1000;
		MergeSort(orga[3],n);
	}
	e=clock();
	result[4][19-j]=e-s;
	
	}

	time_t sum=0;
	
	for(int i=0;i<5;i++){
		switch(i){
			case 0:
				sum=0;
				cout<<"插入:(ms)"<<endl;
				for(int j=0;j<20;j++){
					cout<<(double)result[i][j]/1000<<' ';
					sum+=result[i][j];
				}
				cout<<(double)sum/20000;
				cout<<endl;
				break;
			case 1:
				sum=0;
				cout<<"冒泡:(ms)"<<endl;
				for(int j=0;j<20;j++){
					cout<<(double)result[i][j]/1000<<' ';
					sum+=result[i][j];
				}
				cout<<(double)sum/20000;
				cout<<endl;
				break;
			case 2:
				sum=0;
				cout<<"快速:(ms)"<<endl;
				for(int j=0;j<20;j++){
					cout<<(double)result[i][j]/1000<<' ';
					sum+=result[i][j];
				}
				cout<<(double)sum/20000;
				cout<<endl;
				break;
			case 3:
				sum=0;
				cout<<"选择:(ms)"<<endl;
				for(int j=0;j<20;j++){
					cout<<(double)result[i][j]/1000<<' ';
					sum+=result[i][j];
				}
				cout<<(double)sum/20000;
				cout<<endl;
				break;
			case 4:
				sum=0;
				cout<<"归并:(ms)"<<endl;
				for(int j=0;j<20;j++){
					cout<<(double)result[i][j]/1000<<' ';
					sum+=result[i][j];
				}
				cout<<(double)sum/20000;
				cout<<endl;
				break;
		}
	}
	
	return 0;
}

void charu(int *org,int lenth)
{
	int j=0,t=0,m=0;
	for(j=1;j<lenth;j++){
		t=org[j];
		m=j-1;

		while(m>=0&&t<org[m]){
			org[m+1]=org[m];
			m--;
		}

		org[m+1]=t;
	}
}

void maopao(int *org,int lenth)
{
	int i=0,j=0,t=0,n=0;
	for(j=1;j<lenth;j++){
		
		n=0;
		for(i=0;i<lenth-j;i++)
			if(org[i]>org[i+1]){
				t=org[i];
				org[i]=org[i+1];
				org[i+1]=t;
				n++;
			}
		if(n==0)
			break;
	}
}

void kuaisu(int *org,int left,int right)
{
	int idx=org[right],l=left,r=right;

	while(l<r){
		swap(org[l],org[r]);

		while(l<r&&org[r]>idx)
			r--;

		while(l<r&&org[l]<=idx)
			l++;
	}

	swap(org[left],org[r]);//r==l

	if(left<r-1)
		kuaisu(org,left,r-1);

	if(r+1<right)
		kuaisu(org,r+1,right);
}

void chioce(int *org,int lenth)
{
	int t=0,i=0,j=0,b=0;

	for(i=0;i<lenth-1;i++){
		t=i;
		for(j=i+1;j<lenth;j++)
			if(org[t]>org[j])
				t=j;
		if(i!=t){
			b=org[t];
			org[t]=org[i];
			org[i]=b;
		}
	}
}

void Merge(int *org,int a,int b,int length,int n)
{
	int right=0;
	if(b+length-1>=n-1)
		right=n-b;
	else
		right=length;
	if(right<=0)
		return;
	int i=0,j=0;

	int* temp=new int[length+right];

	while(i<length && j<right){
		if(org[a+i]<org[b+j]){
			temp[i+j]=org[a+i];
			i++;
		}else{
			temp[i+j]=org[b+j];
			j++;
		}
	}
	
	if(j==right)
		memcpy(temp+i+j,org+a+i,(length-i)*sizeof(int));
	else if(i==length)
		memcpy(temp+i+j,org+b+j,(right-j)*sizeof(int));

	memcpy(org+a,temp,(right+length)*sizeof(int));

	delete []temp;
}

void MergeSort(int *org,int n)
{
	int step=1;
	while(step<n){
		for(int i=0;i<n-step;i+=2*step)
			Merge(org,i,i+step,step,n);
		step*=2;
	}
}
