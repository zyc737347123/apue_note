#include<apue.h>
#include<stdio.h>
#include<time.h>

int main(int argc,char* argv[])
{
	srand(time(NULL));

	int i=0,j=0,n=0;
	n=atoi(argv[1]);
	printf("%d\n",n);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i==j)
				printf("%d ",0);
			else
				printf("%d ",(rand()%100)+1);
		}
		printf("\n");
	}

	exit(0);
}
