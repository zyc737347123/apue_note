#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<string.h>
using namespace std;

void createK(int &k);
void encode(char* m,const int k);
void decode(char* c,const int k);

int main(void)
{
	srand(time(NULL));

	int n=0;
	cout<<"please input a number"<<endl;
	cin>>n;

	char *org=new char[n+1];
	org[n]=0;

	for(int i=0;i<n;i++)
		cin>>org[i];
	
	int k=-1;
	createK(k);
	//cout<<k<<endl;
	if(k<0){
		cout<<"key is round"<<endl;
		exit(0);
	}

	encode(org,k);
	cout<<"c: "<<org<<endl;

	decode(org,k);
	cout<<"m: "<<org<<endl;

	return 1;
}


void createK(int &k)
{
	k=rand()%25+1;
}

void encode(char *m,const int k)
{
	const int len=strlen(m);
	for(int i=0;i<len;i++)
		m[i]=((m[i]-'a')+k)%26+'a';
}

void decode(char *c,const int k)
{
	const int len=strlen(c);
	for(int i=0;i<len;i++){
		c[i]=((c[i]-'a'-k+26)%26)+'a';
	}
}
