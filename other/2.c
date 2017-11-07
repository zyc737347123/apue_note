#include<apue.h>
#include<math.h>
#include<stdio.h>

#define P(i) printf("%s%d\n",#i,(i));

void test(int(*)(int,int));
int fuck(int,int);

int main(void)
{
	/*
	int a=(int)6;
	int i=2;
	for(i=2;i<=sqrt(6);i++){
		if(a%i==0){
			printf("%d error\n",i);
			break;
		}
	}*/
	//test(fuck);
	P(3);
	exit(0);
}

void test(int(*add)(int,int))
{
	int a=2,b=3;
	printf("%d\n",add(a,b));
}

int fuck(int a,int b)
{
	return a+b;
}
