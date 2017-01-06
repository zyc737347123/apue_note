#include<apue.h>
#include<iostream>
using namespace std;

int main()
{
	int line=60;
	int i=0,j=0;

	int map[16][16];
	int src=0,dst=0,w=0;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++)
			map[i][j]=1000;
	for(i=0;i<16;i++)
		map[i][i]=0;
	for(i=0;i<line;i++){
		cin>>src>>dst>>w;
		map[src][dst]=w;
	}
	cout<<16<<endl;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			cout<<map[i][j]<<' ';
		}
		cout<<endl;
	}

	return 1;

}
