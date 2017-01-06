#include<apue.h>
#include<time.h>

int map[24][24];
int dis[24][16777215];
int path[24][16777215];

int num_of_Q(int status)
{
	int sum=0;
	while(status){
		sum++;
		status = status >> 1;
	}
	return sum;
}

int TSP(int cur,int status,int n)
{
	if(dis[cur][status]!=-1){
		return dis[cur][status];
	}


	int i=0,temp=0,new_status=0,next=0;
	int min=10000;
	for(i=1;i<n-1;i++){
		if(cur==i)
			continue;
		
		int t1 = (1<<i) & status;
		if(t1==0)
			continue;

		new_status = (status & (~(1<<i))) & ((1<<n)-1);
		temp=map[cur][i]+TSP(i,new_status,n);
	
		if(temp < min)
			next=i;

		min = min < temp ? min : temp;
	}
	dis[cur][status]=min;
	path[cur][status]=next; //会有重复
	return min;
}

int TSP2(int cur,int status)
{
	if(dis[cur][status]!=-1)
		return dis[cur][status];

	int temp=0,new_status=0;
	int min=100000,i=0,p=0,pos=status;
	while(pos){
		i=1;// 建立在i恒不为0
		p = pos & (~pos + 1); // 获得最低位的1
		while((1<<i)==p)
			i++;
		new_status = status - p; // new_status = status & (~(1<<i));
		pos = pos - p;
		temp=map[cur][i]+TSP2(i,new_status);
		min = min < temp ? min : temp;
	}
	dis[cur][status]=min;
	return min;
}



int main(void)
{
	int i=0,j=0,n=0;
	scanf("%d",&n);

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			scanf("%d",&map[i][j]);
			printf("%d ",map[i][j]);
		}
		printf("\n");
	}

	//init
	//printf("1\n");
	memset(dis,-1,sizeof(dis));
	memset(path,0,sizeof(path));
	int t = 1<<(n-1);
	for(i=1;i<n-1;i++)
		dis[i][t]=map[i][n-1];

	//printf("3\n");
	int status = (1<<n)-2;
	//printf("2\n");
	printf("status: %d\n",status);
	int result = TSP(0,status,n);

	int cur=0,next=0,count=n-2;
	printf("1 ");
	while(count--){
		next=path[cur][status];
		printf("%d ",next+1);
		status = status - (1<<next);
		cur=next;
	}
	printf("%d\n",n);
		

	printf("res: %d\n",result);

	exit(0);
}
