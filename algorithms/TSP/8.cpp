#include<apue.h>
#include<stdio.h>
#include<time.h>
#include<iostream>
#include<map>
#include<vector>
#include<set>
using namespace std;

#define Line 5533214 
#define Node 1965206

int Dis[23][8388607];
int Map[23][23];
int Pathss[23][8388607];

map<int,short> Road_net[2065206];

typedef struct{
	int curcity;
	vector<int> unvisited;
	set<int> type;
	int distance;
}status;

typedef struct{
	vector<int> short_path;
	int distance;
}path;

path Paths[Node];

map<int,path> Road_path[100];
const int s_n=99;

vector<status> cur_vector;
int S=0;
int T=0;
int k[1000];

bool Vis[Node];

struct cmp{
	bool operator()(const int &a,const int &b)
	{
		return Paths[a].distance < Paths[b].distance || ( Paths[a].distance == Paths[b].distance && a < b);
	}
};

set<int,cmp> h;

long dijkstra2(int s,int end,int k_n,int node_n)
{
	int i=0;
	memset(Vis,0,sizeof(Vis));
	map<int,short>::iterator miter;
	set<int,cmp>::iterator siter;
	Paths[s].short_path.clear();
	Paths[s].distance=0;
	Paths[s].short_path.push_back(s);
	for(i=0;i<node_n;i++){
		if(i==s)
			continue;
		Paths[i].short_path.clear();
		Paths[i].distance = 100000;
	}
	h.clear();
	h.insert(s);
	while(!h.empty()){
		i = *h.begin();
		if(i == end)
			break;
		h.erase(i);
		Vis[i]=true;

		//relax
		int j=0,w=0;
		for(miter = Road_net[i].begin();miter!=Road_net[i].end();miter++){
			j = miter->first;
			w = miter->second;
			if(!Vis[j] && Paths[j].distance > Paths[i].distance + w){

				int f=0;
				for(f=0;f<k_n;f++){
					if(f == k[f] && k[f] != end)
						break;
				}

				if(f != k_n || j == S || (j == T && end != T))
					continue;

				h.erase(j);
				path temp = Paths[i];
				temp.distance += w;
				temp.short_path.push_back(j);
				Paths[j]=temp;
				h.insert(j);
			}
		}
	}
	if(h.empty()){
		return 100000;
	}else{
		if(s == S){
			Road_path[s_n].insert(pair<int,path>(end,Paths[end]));
		}else{
			int j=0;
			for(j=0;j<k_n;j++)
				if(s == k[j])
					break;
			Road_path[j].insert(pair<int,path>(end,Paths[end]));
		}
	}
	return Paths[end].distance;
}


long dijkstra(int s,int end,int k_n,int node_n)
{
	//cout<<s<<' '<<end<<endl;
	map<int,short>::iterator miter;
	set<int> Set;
	Set.insert(s);
	int i=0;
	//init
	Paths[s].distance=0;
	for(i=0;i<node_n;i++){
		Paths[i].short_path.clear();
		miter=Road_net[s].find(i);
		if(i == s)
			continue;
		Paths[i].distance=100000;
		if(miter == Road_net[s].end())
			continue;
		Paths[i].distance = miter->second;
		Paths[i].short_path.push_back(s);
		Paths[i].short_path.push_back(i);
	}

	// find new S_node
	set<int>::iterator siter;
	while(1){
		int min=100000,j=0,min_j=0;
		for(siter=Set.begin();siter!=Set.end();siter++){
			j=*siter;
			for(miter = Road_net[j].begin();miter!=Road_net[j].end();miter++){
					i=miter->first;
					if(Set.count(i))
						continue;
					int temp = Paths[i].distance;
					if(temp<min){
						min = temp;
						min_j=i;
					}
			}
		}
	
		
		if(Set.count(min_j)){
			cout<<"error\n";
			continue;
		}
		//cout<<"New S_node: "<<min_j<<endl;
		Set.insert(min_j); // add new S-node
		if(min==100000)// can not find the path from s to end;
			return min;
		if(min_j == end)
			break;
		if(Set.size()>7000)
			break;
		
		//relax
		for(miter = Road_net[min_j].begin();miter!=Road_net[min_j].end();miter++){
			i=miter->first;

			for(j=0;j<k_n;j++){
				if(i == k[j] && k[j] != end)
					break;
			}
			if(j != k_n || i == S || (i==T && end != T))
				continue;

			int w=miter->second;
			path temp = Paths[min_j];
			temp.distance = temp.distance + w;
			temp.short_path.push_back(i);
			Paths[i] = temp;
		}
	}

	int j=0;
	if(s == S){
		Road_path[s_n].insert(pair<int,path>(end,Paths[end]));
	}else{
		for(j=0;j<k_n;j++)
			if(s == k[j])
				break;
		Road_path[j].insert(pair<int,path>(end,Paths[end]));
	}

	return Paths[end].distance;
}

void map_init(const int line)
{
	int i=0,src=0,dst=0,w=0;
	for(i=0;i<line;i++){
		cin>>src;
		cin>>dst;
		cin>>w;
		Road_net[src].insert(pair<int,short>(dst,w));
	}
}

bool contain(int i,status &sta)
{
	vector<int>::iterator iter;
	if(i==sta.curcity)
		return true;
	for(iter=sta.unvisited.begin();iter!=sta.unvisited.end();iter++)
		if(i==*iter)
			return true;
	return false;
}



int TSP(int cur,int status2,int n)
{
	if(Dis[cur][status2]!=-1){
		return Dis[cur][status2];
	}


	int i=0,temp=0,new_status=0,next=0;
	int min=10000000;
	for(i=1;i<n-1;i++){
		if(cur==i)
			continue;
		
		int t1 = (1<<i) & status2;
		if(t1==0)
			continue;

		new_status = (status2 & (~(1<<i))) & ((1<<n)-1);
		temp=Map[cur][i]+TSP(i,new_status,n);
	
		if(temp < min)
			next=i;

		min = min < temp ? min : temp;
	}
	Dis[cur][status2]=min;
	Pathss[cur][status2]=next; //会有重复
	return min;
}

void print_path(int s,int end)
{
	map<int,path>::iterator miter;
	map<int,path> temp;
	int e=0;
	if(s==0){
		temp = Road_path[s_n];
	}else{
		temp = Road_path[s-1];
	}

	miter = temp.find(end);

	if(miter == temp.end())
		return;
	vector<int>::iterator iter;
	path short_path = miter->second;
	for(iter = short_path.short_path.begin();iter!=short_path.short_path.end();iter++)
			cout<<*iter<<' ';
	cout<<endl;
}

int main()
{
	int i=0,j=0;
	srand(time(NULL));

	//init map
	cout<<"init map\n";
	map_init(Line);

	//init path of S,T,Q{}
	cout<<"init path of S,T,Q{}\n";
	int n=7,k_n=5,temp=0;
	S=rand()%100;
	S=0;
	cout<<"S: "<<S<<endl;
	T=(rand()%200)+100;
	cout<<"T: "<<T<<endl;
	set<int> k_set;
	k[0]=3;
	cout<<"k[0]: 3\n";
	k_set.insert(3);
	for(i=1;i<k_n;){
		temp=rand()%300;
		if(temp != S && temp !=T && !k_set.count(temp)){
			k[i]=temp;
			k_set.insert(temp);
			cout<<"k["<<i<<"]: "<<temp<<endl;
			i++;
		}
	}

	memset(Map,0,sizeof(Map));

	for(i=0;i<n-1;i++){
		for(j=0;j<n;j++){
			if(i!=j && j!=0){
				if(i==0){
					if(j!=n-1){
						Map[i][j] = dijkstra2(S,k[j-1],k_n,Node);
					}
				}else{
					if(j!=n-1){
						Map[i][j] = dijkstra2(k[i-1],k[j-1],k_n,Node);
					}else{
						Map[i][j] = dijkstra2(k[i-1],T,k_n,Node);
					}
				}
			}
			cout<<Map[i][j]<<' '<<flush;
		}
		cout<<endl;
	}

	//init tsp
	cout<<"init tsp\n";
	memset(Dis,-1,sizeof(Dis));
	memset(Pathss,0,sizeof(Pathss));
	int t = 1<<(n-1);
	for(i=1;i<n-1;i++)
		Dis[i][t]=Map[i][n-1];

	int Status = (1<<n)-2;
	printf("status: %d\n",Status);
	int result = TSP(0,Status,n);
	cout<<"res: "<<result<<endl;

	int cur=0,next=0,count=n-2;
	cout<<S<<"->"<<endl;
	while(count--){
		next = Pathss[cur][Status];
		cout<<k[next-1]<<"->"<<endl;
		print_path(cur,k[next-1]);
		Status = Status - (1<<next);
		cur = next;
	}
	cout<<T<<endl;
	print_path(next,T);

	return 1;
}

