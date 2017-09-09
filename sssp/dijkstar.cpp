#include <set>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

using namespace std;

#define Node 1890816
#define Line 281620

map<int, int> Road_net[Node];
set<pair<int, int> > Have_find_sp;
int path_wight[Node];

void map_init(const int line_num)
{
	int i = 0, src, dst, w;
	for(i = 0 ; i < line_num ; i++) {
		cin >> src
			>> dst
			>> w;
		Road_net[src].insert(pair<int, int>(dst, w));
	}
}

struct cmp{
	bool operator()(const int &a, const int &b)
	{
		return path_wight[a] < path_wight[b] ||
			(path_wight[a] == path_wight[b] && a < b);
	}
};

long dijkstra2(int source_node)
{
	int i, select_node, dst, weight;
	long res = 0;
	static bool vis[Node];
	map<int, int>::iterator mitr;
	set<int, cmp> min_heap;
	set<pair<int, int> >::iterator sitr;
	
	memset(vis, 0, sizeof(vis));

	for(i = 0 ; i < Node ; i++) {
		path_wight[i] = 10000000;
	}

	path_wight[source_node] = 0;
	min_heap.insert(source_node);

	while(!min_heap.empty()) {	
		// select node
		select_node = *min_heap.begin();
		min_heap.erase(select_node);
		vis[select_node] = true;
		res = res > path_wight[select_node] ? res : path_wight[select_node];

		// relax
		for(mitr = Road_net[select_node].begin() ; 
				mitr != Road_net[select_node].end(); mitr++) {
			
			dst = mitr->first;
			weight = mitr->second;

			if(!vis[dst] && 
					path_wight[dst] > path_wight[select_node] + weight) {
			
				min_heap.erase(dst);
				path_wight[dst] = path_wight[select_node] + weight;
				min_heap.insert(dst);
			}
		}
	}

	return res;
}

int main()
{
	struct timeval tv1, tv2;
	struct timezone tz;
	int i = 0 , max = 0;
	
	gettimeofday(&tv1, &tz);
	map_init(Line);
	gettimeofday(&tv2, &tz);

	gettimeofday(&tv1, &tz);
	for(i = 1 ; i < Node ; i++) {
		int tmp;
		tmp = dijkstra2(i);
		max = max > tmp ? max : tmp;
	}
	gettimeofday(&tv2, &tz);

	cout << tv2.tv_sec - tv1.tv_sec << "sec" << endl;

	cout << "max: " << max << endl;

	return 0;
}
