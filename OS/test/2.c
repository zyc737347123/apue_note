#include<stdio.h>
#include<stdlib.h>

#define MB 1024*1024

int main(void)
{
	char *a;
	int i=0,j;

	scanf("%d",&j);

	getchar();
	getchar();
	a = (char*)malloc(j*MB);
	if(a==NULL){
		printf("malloc error\n");
		exit(1);
	}

	getchar();
	getchar();

	for(i=0;i<256*MB;i++){
		a[i]=i%100;
	}

	getchar();
	for(i=0;i<256*MB;i++)
		j=a[i];

	getchar();
	getchar();

	return 0;
}
