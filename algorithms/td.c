#include<apue.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

void TD(int V,int N,int *w,int *v,int r[][1000]);

void TD2(int V,int N,int *w,int *v)
{
	//w is value,v is volume

	int max[310][600];
	int *p1=&max[0][0];
	int j=0,i=0;

	for(i=1;i<=N;i++){
		for(j=0;j<=V;j++){
			if(j>=v[i]){
				int tmp=max[i-1][j-v[i]]+w[i];
				max[i][j]=max[i-1][j] > tmp ? max[i-1][j] : tmp;
			}else{
				max[i][j]=max[i-1][j];
			}
		}
	}
	printf("TD2 max value %d\n",max[N][V]);
}

void TD3(int V,int N,int *w,int *v)
{
	//w is value,v is volume

	int max[310][600];
	int *p1=&max[0][0];
	int j=0,i=0;

	for(i=1;i<=N;i++){
		for(j=0;j<=V;j++){
			if(j>=v[i]){
				//int tmp=max[i-1][j-v[i]]+w[i];// 当i>1时，max[j-v[i]]保留的是max[i-1][j-v[i]]的值
				int tmp=*(p1+(i-1)*310+j-*(v+i))+*(w+i);
				//max[i][j]=max[i-1][j] > tmp ? max[i-1][j] : tmp;
				*(p1+i*310+j)=*(p1+(i-1)*310+j) > tmp ? *(p1+(i-1)*310+j) : tmp;
			}else{
				//max[i][j]=max[i-1][j]; // max[i][j]=max[i-1][j];
				*(p1+i*310+j)=*(p1+(i-1)*310+j);
			}
		}
	}
	printf("TD2 max value %d\n",max[N][V]);
}

void TD_brute_force(int V,int N,int *w,int *v,long J) // 蛮力法
{
	long i=0;
	long j=0,k=0;
	int max=0;
	int m=0,vv=0;
	for(j=0;j<J;j++){
		long t=j;
		m=0;
		vv=0;
		for(k=N;k>0;k--){
			if(t&1){
				m=m+w[k];
				vv=vv+ v[k];
			}
			t=t>>1;
		}
		if(vv<=V && m > max){
			i=j;
			max=m;
		}
	}

	printf("TD_bit max value %d. i is %ld\n",max,i);

	/*
	k=N;
	while(i){
		if(i&1)
			printf("%ld-%d  ",k,w[k]);
		k--;
		i=i>>1;
	}
	printf("\n");
	*/

}

int main(void)
{
	srand(time(NULL));

	long ttt;
	int V,N,i;
	scanf("%d %d",&V,&N);
	ttt=1<<N;

	int *w=(int *)malloc(sizeof(int)*(N+1));
	memset(w,0,sizeof(int)*(N+1));

	int *v=(int *)malloc(sizeof(int)*(N+1));
	memset(v,0,sizeof(int)*(N+1));

	int max[1000][1000];

	time_t s1,t1,sum;
	sum=0;

	for(i=0;i<1;i++){
	
	int j=0;
	for(j=1;j<=N;j++){
		w[j]=rand()%(V-1)+1;
		//printf("%2d ",w[j]);
	}

	printf("\n");

	for(j=1;j<=N;j++){
		v[j]=(rand()%19)+1;
		//printf("%2d ",v[j]);
	}

	if(N<=64){
	printf("\n");
	TD_brute_force(V,N,v,w,ttt);
	printf("\n");
	}

	s1=clock();
	TD2(V,N,v,w);
	t1=clock();
	printf("TD2 time %ld us\n",t1-s1);

	s1=clock();
	TD3(V,N,v,w);
	t1=clock();
	printf("TD3 time %ld us\n",t1-s1);


	s1=clock();
	TD(V,N,v,w,max);
	t1=clock();
	sum+=t1-s1;
	}

	printf("time %lf us\n",(double)sum);

	/* 
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
	*/


	exit(0);
}

void TD(const int V,const int N,int *w,int *v,int r[][1000])
{
	//w is value,v is volume
	int *max=(int *)malloc(sizeof(int)*(V+1));
	memset(max,0,sizeof(int)*(V+1));

	int j=0,i=0;

	for(i=1;i<=N;i++){
		for(j=V;j>0;j--){
			if(j>=v[i]){
				int tmp=max[j-v[i]]+w[i];// 当i>1时，max[j-v[i]]保留的是max[i-1][j-v[i]]的值
				max[j]=max[j] > tmp ? max[j] : tmp;
				//if(max[j]==tmp)
					//r[i][j]=1;
			}else{
				max[j]=max[j]; // max[i][j]=max[i-1][j];
			}
			//printf("%2d ",max[j]);
		}
		//printf("\n");
	}
	printf("max value %d\n",max[V]);
}
