#include<apue.h>
#include<stdio.h>
#include<time.h>

void TD(int V,int N,int *w,int *v,int r[][100]);

int main(void)
{
	srand(time(NULL));

	int V,N,i,j;
	scanf("%d %d",&V,&N);
	
	int *w=(int *)malloc(sizeof(int)*(N+1));
	memset(w,0,sizeof(int)*(N+1));

	for(i=1;i<=N;i++){
		w[i]=rand()%(V-1)+1;
		printf("%2d ",w[i]);
	}
	printf("\n");


	int *v=(int *)malloc(sizeof(int)*(N+1));
	memset(v,0,sizeof(int)*(N+1));

	for(i=1;i<=N;i++){
		v[i]=(rand()%19)+1;
		printf("%2d ",v[i]);
	}
	printf("\n");

	int max[100][100];
	for(i=0;i<100;i++)
		for(j=0;j<100;j++)
			max[i][j]=0;
	time_t s1,t1,sum;
	sum=0;

	for(i=0;i<10000;i++){
	
	int j=0;
	for(j=1;j<=N;j++){
		w[j]=rand()%(V-1)+1;
		//printf("%2d ",w[i]);
	}

	for(j=1;j<=N;j++){
		v[j]=(rand()%19)+1;
		//printf("%2d ",v[i]);
	}

	s1=clock();
	TD(V,N,v,w,max);
	t1=clock();
	sum+=t1-s1;
	}

	printf("time %lf ms\n",(double)sum/10000);

	i=N;
	j=V;
	while(i>=0){
		if(max[i][j]){
			printf("%d ",i);
			j=j-w[i];
		}
		i--;
	}
	printf("\n");


	exit(0);
}

void TD(const int V,const int N,int *w,int *v,int r[][100])
{
	//w is value,v is volume
	int *max=(int *)malloc(sizeof(int)*(V+1));
	memset(max,0,sizeof(int)*(V+1));

	int j=0,i=0;
	/*
	for(i=0;i<N+1;i++)
		printf("%d ",max[i]);
	printf("\n");
	*/

	for(i=1;i<=N;i++){
		for(j=V;j>0;j--){
			if(j>=v[i]){
				int tmp=max[j-v[i]]+w[i];// 当i>1时，max[j-v[i]]保留的是max[i-1][j-v[i]]的值
				max[j]=max[j] > tmp ? max[j] : tmp;
				if(max[j]==tmp)
					r[i][j]=1;
			}else{
				max[j]=max[j]; // max[i][j]=max[i-1][j];
			}
			//printf("%2d ",max[j]);
		}
		//printf("\n");
	}
	//printf("max value %d\n",max[V]);
}
