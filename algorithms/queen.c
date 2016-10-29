#include<apue.h>
#include<time.h>
#include<math.h>
#define MAX_LENTH 1024

int judge(int *a,int n)// 判断第n行放置的q是否合适
{
	int flag=0;
	int i=0;
	for(i=0;i<n;i++){ // 第0行不判断
		if( a[i] == a[n] || a[i]-a[n] == n-i || a[n]-a[i] == n-i){
			flag=1;
			break;
		}
	}
	return flag;
}

void print_q(int *a,int n){
	int i=0,j=0;
	for(i=0;i<n;i++){
		for(j=0;j<a[i];j++){
			printf("-");
		}
		printf("Q");
		for(j=j+1;j<n;j++){
			printf("-");
		}
		printf("\n");
	}
	printf("************************\n");
}

void init_board(int *a,int n){// 初始化棋盘，所有q都在0列,所以a[i]的位置是从左到右
	int i=0;
	for(i=0;i<n;i++)
		a[i]=0;
}

int queen(int n,int *pp){
	int count=0;
	//printf("%d\n",*pp);
	int p=0;
	int a[MAX_LENTH];
	init_board(a,n);
	int i=0;
	while(1){
		if(a[i]<n){
			// q没有超出棋盘
			if(judge(a,i)){
				a[i]++; // 移动当前行（i)棋子的位置
				p++;
				continue;
			}
		if(i>=n-1){
			count++;
			print_q(a,n);
			a[i]++;
			p++;
			continue;
		}

		i++;
		continue;
		}else{
			// q超出棋盘，q的位置回到初始值 0 
			a[i]=0;
			// 退回上一行
			i--;
			if( i<0){
				// 不能再退，结束函数
				(*pp)=p;
				return count;
			}
			a[i]++;
			p++;
			continue;
		}
	}
}

long queen3(int n,long *pp){
	long count=0;
	//printf("%d\n",*pp);
	long p=0;
	int a[MAX_LENTH];
	init_board(a,n);
	int i=0;
	while(1){
		if(a[i]<n){
			// q没有超出棋盘
		/*	if(judge(a,i)){
				a[i]++;
				p++;
				continue;
			}*/
			if(i>=n-1){
				count++;
				//print_q(a,n);
				a[i]++;
				p++;
				continue;
			}

		i++;
		continue;
		}else{
			// q超出棋盘，q的位置回到初始值 0 
			a[i]=0;
			// 退回上一行
			i--;
			if( i<0){
				// 不能再退，结束函数
				(*pp)=p;
				return count;
			}
			a[i]++;
			p++;
			continue;
		}
	}
}

void get_symmetrical(int *a,int *b,int n)
{
	int i=0;
	for(i=0;i<n;i++)
		b[i]=n-1-a[i];
}

int queen2(int n,int *pp){
	int count=0;
	//printf("%d\n",*pp);
	int p=0;
	int a[MAX_LENTH];
	int b[MAX_LENTH];
	init_board(a,n);
	init_board(b,n);

	int i=0;
	while(1){
		if(a[0]==n/2+1 && n%2!=0){
			(*pp)=p;
			return count;
		}
		if(n%2==0 && a[0]==n/2){
			(*pp)=p;
			return count;
		}
		if(a[i]<n){ // 利用对称
			// q没有超出棋盘
			p++;
			if(judge(a,i)){
				a[i]++;
				continue;
			}
		if(i>=n-1){
			if(n%2!=0 && a[0] == n/2){
				count++;
				print_q(a,n);
			}else{
			count++;
			//print_q(a,n);
					
			get_symmetrical(a,b,n);

			count++;
			//print_q(b,n);

			}
			a[i]++;
			continue;
		}

		i++;
		continue;
		}else{
			// q超出棋盘，q的位置回到初始值 0 
			a[i]=0;
			// 退回上一行
			i--;
			if( i<0){
				// 不能再退，结束函数
				(*pp)=p;
				return count;
			}
			a[i]++;
			continue;
		}
	}
}

long sum=0,upperlim=1;

void test(int row,int ld,int rd,int *pp,int line,int *result,const int n)
{
	int pos,p;
	if(row != upperlim){
		pos = upperlim & (~(row | ld | rd)); //保证pos在有效位范围
		while(pos){
			(*pp)++;
			p=pos & (~pos + 1); // 获得pos最低位的1
			pos = pos - p;// 减去已经放置了Q的位置

			int i=0,j=p;;
			while(j >> 1){
				i++;
				j=j >> 1;
			}
			result[line]=i;

			test(row | p,(ld | p) << 1,(rd | p) >> 1,pp,line+1,result,n);
		}
	}else{
		print_q(result,n);
		sum++;
	}
}

int queen4(int n,int *pp)
{
	int node=0;
	int result[MAX_LENTH];
	init_board(result,n);
	upperlim = (upperlim << n)-1;
	test(0,0,0,&node,0,result,n);
	(*pp)=node;
	return sum;

}

int main(void){
	time_t q2,q3;
	time_t ss,end;
	int n=0;
	scanf("%d",&n);
	long node=-1;
	int node4=-1;
	int node2=-1;
	ss=clock();
 	long count = queen4(n,&node4);
	end=clock();
	q3=end-ss;

	ss=clock();
	int count2 = queen2(n,&node2);
	end=clock();
	q2=end-ss;
	printf("q3: %ld result in %d queens problem\n",count,n);
	printf("access node is %d\n time: %ld us\n",node4,q3);
	printf("q2: %d result in %d queens problem\n",count2,n);
	printf("access node is %d\n time: %ld us\n",node2,q2);
	return 0;
}

