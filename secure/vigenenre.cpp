#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<string.h>
using namespace std;

void createK(int *k,int len);
void encode(char* m,int *k,int len);
void decode(char* c,int *k,int len);

int main(void)
{
	srand(time(NULL));
	int len=rand()%10+3;
	int *k=new int[len];
	createK(k,len);
	for(int i=0;i<len;i++)
		cout<<k[i]<<' ';
	cout<<endl;

	int n=0;
	cin>>n;

	char *org=new char[n+1];
	org[n]=0;

	for(int i=0;i<n;i++)
		cin>>org[i];

	encode(org,k,len);
	cout<<"c: "<<org<<endl;

	decode(org,k,len);
	cout<<"m: "<<org<<endl;

	return 1;
}

void createK(int *k,int len)
{	
	for(int i=0;i<len;i++)
		k[i]=rand()%25+1;
}

void encode(char *m,int *k,int len)
{
	int j=0;
	const int n=strlen(m);
	for(int i=0;i<n;i++){
		m[i]=(m[i]-'a'+k[j])%26+'a';
		j=(++j)%len;
	}
}

void decode(char *c,int *k,int len)
{
	int j=0;
	const int n=strlen(c);
	for(int i=0;i<n;i++){
		c[i]=(c[i]-'a'-k[j]+26)%26+'a';
		j=(++j)%len;
	}
}
