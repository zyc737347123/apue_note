#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<memory.h>
#include<math.h>
#include<time.h>
#include<algorithm>
using namespace std;

const double INF=1e20;

struct point{
	double x;
	double y;
};



double Close_Pair(point *org,int len,int &p1,int &p2);


bool cmpxy(const point &a,const point &b)
{
	if(a.x!=b.x)
		return a.x<b.x;
	return a.y < b.y;
	/*
	 * 升序排列
	 * 如果x值相同，则比较y值
	 */
}

bool cmpy(const point &a,const point &b)
{
	return a.y < b.y;
}

double dis(point a,point b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

double close_pair(point *org,int left,int right,point &p1,point &p2)
{
	point l1,l2,r1,r2;
	double distraion=INF;
	if(left==right){
		//cout<<distraion<<endl;
		return distraion;
		//只有一个点，距离为无穷,结束递归
	}
	if(left + 1==right){
		distraion= dis(org[left],org[right]);//有两个点，结束递归，返回距离
		p1=org[left];
		p2=org[right];
		return distraion;
	}
	int mid = (left+right)/2;
	
	double dl = close_pair(org,left,mid,l1,l2);
	//cout<<"dl "<<dl<<endl;
	
	double dr = close_pair(org,mid+1,right,r1,r2);
	//cout<<"dr "<<dr<<endl;

	distraion = dl < dr ? dl : dr; 
	if(dl<dr){
		p1=l1;
		p2=l2;
	}else{
		p1=r1;
		p2=r2;
	}

	int i=0,j=0,k=0;
	
	point *tmp=new point[right-left+1];

	for(i=left;i<=right;i++){
		if(fabs(org[mid].x-org[i].x)<distraion)
			tmp[k++]=org[i];
	}

	sort(tmp,tmp+k,cmpy);// 时间复杂度为O(nlogn)

	int mi=0,mj=0;
	for(i=0;i<k;i++)
		for(j=i+1;j<k&&(tmp[j].y-tmp[i].y)<distraion;j++){// 根据鸽巢原理，最多是6*（n/2）次赋值操作
			double d3=dis(tmp[i],tmp[j]);
		if(distraion>d3)
			distraion=d3;
			p1=tmp[i];
			p2=tmp[j];
		}
	// 所以合并的开销是O（nlogn)
	delete []tmp;


	return distraion;
}

void crete_data(point *org,int len)
{
	int *judge=new int[len*10];
	memset(judge,0,sizeof(int)*2*len);
	int i=0,x=0;
	while(1){
		if(i>=len)
			break;
		x=rand()%(len*10);
		if(judge[x])
			continue;
		judge[x]=1;
		org[i].x=x;
		org[i].y=rand()%len;
		i++;
	}
}

int main()
{

	srand(time(NULL));

	int n=0,l=0,r=0;
	time_t s,e,t,sum1=0,sum2=0;
	point ln,rn;

	cout<<"input a number"<<endl;
	cin>>n;

	point *org=new point[n];
	point *org1=new point[n];	

	
	for(int i=0;i<10;i++){
	crete_data(org,n);

	memcpy(org1,org,sizeof(point)*n);

	sort(org,org+n,cmpxy);

	double dis=0;

	/*
	s=clock();
	dis=close_pair(org,0,n-1,ln,rn);
	e=clock();
	sum1+=e-s;

	cout<<i<<": r="<<dis<<endl<<"r ";
	cout<<ln.x<<','<<ln.y<<"  "<<rn.x<<','<<rn.y
		<<"  time(us): "<<e-s<<endl;*/
	
	s=clock();
	dis=Close_Pair(org1,n,l,r);
	e=clock();
	sum2+=e-s;

	cout<<i<<": R="<<dis<<endl<<"R ";
	cout<<org1[l].x<<','<<org1[l].y<<"  "<<org1[r].x<<','<<org1[r].y
		<<"  time(us): "<<e-s<<endl;
	}

	//cout<<"r 平均时间: "<<sum1/10<<" us"<<endl
	cout<<"R 平均时间: "<<sum2/10<<" us"<<endl;

	delete []org;
	delete []org1;

	return 1;
}

double Close_Pair(point *org,int len,int &p1,int &p2)
{
	double min_dis=INF;
	
	for(int i=0;i<len;i++)
		for(int j=i+1;j<len;j++){
			double dis=sqrt((org[i].x-org[j].x)*(org[i].x-org[j].x)+(org[i].y-org[j].y)*(org[i].y-org[j].y));
			if(min_dis>dis){
				min_dis=dis;
				p1=i;
				p2=j;
			}
		}

	return min_dis;

}
