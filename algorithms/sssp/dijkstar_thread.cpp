#include <set>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

using namespace std;

#define NTHR 4	// thread num , you can change
#define Node 1890816 // graph node num + 1
#define Line 281620

pthread_barrier_t barrier;
map<int, int> Road_net[Node];
bool Have_save[Node];
int thread_res[NTHR];

void map_init(const int line_num)
{
	int i = 0, src, dst, w;
	for(i = 0 ; i < line_num ; i++) {
		cin >> src
			>> dst
			>> w;
		Road_net[src].insert(pair<int, int>(dst, w));
		Have_save[src] = true;
	}
}

typedef struct node {
	int dst;
	int weight;
} node;

struct cmp{
	bool operator()(const node &a, const node &b)
	{
		return a.weight < b.weight ||
			(a.weight == b.weight && a.dst < b.dst);
	}
};

long dijkstra2(int source_node)
{
	int i, dst, weight;
	node select_node, tmp_node;
	long res = 0;
	static bool vis[Node];
	map<int, int>::iterator mitr;
	set<node, cmp> min_heap;
	int path_wight[Node];

	memset(vis, 0, sizeof(vis));

	for(i = 0 ; i < Node ; i++) {
		path_wight[i] = 100000000;
	}

	path_wight[source_node] = 0;
	select_node.dst = source_node;
	select_node.weight = 0;
	min_heap.insert(select_node);

	while(!min_heap.empty()) {	
		// select node
		select_node = *min_heap.begin();
		min_heap.erase(select_node);
		vis[select_node.dst] = true;
		res = res > path_wight[select_node.dst] ? res : path_wight[select_node.dst];

		// relax
		for(mitr = Road_net[select_node.dst].begin() ; 
				mitr != Road_net[select_node.dst].end(); mitr++) {
			
			dst = mitr->first;
			weight = mitr->second;

			if(!vis[dst] && 
					path_wight[dst] > path_wight[select_node.dst] + weight) {

				tmp_node.dst = dst;
				tmp_node.weight = path_wight[dst];
				min_heap.erase(tmp_node);

				path_wight[dst] = path_wight[select_node.dst] + weight;
				tmp_node.weight = path_wight[dst];
				min_heap.insert(tmp_node);
			}
		}
	}

	return res;
}

void *thread_func(void *arg)
{
	int idx = (long)arg;

	int start_node = (Node / NTHR) * idx;
	int end_node = (Node / NTHR) * (idx + 1);

	if(end_node > Node)
		end_node = Node;
	int i, max = 0;

	for(i = start_node ; i < end_node ; i++) {
		if(Have_save[i]) {
			int tmp;
			tmp = dijkstra2(i);
			max = max > tmp ? max : tmp;
		}
	}

	thread_res[idx] = max;
	pthread_barrier_wait(&barrier);

	return (void*)0;
}

int main()
{
	struct timeval tv1, tv2;
	struct timezone tz;
	int i = 0 , max = 0;
	int err;
	pthread_t tid;

	memset(Have_save, 0 , sizeof(Have_save));
	
	gettimeofday(&tv1, &tz);
	map_init(Line);
	gettimeofday(&tv2, &tz);

	gettimeofday(&tv1, &tz);
	pthread_barrier_init(&barrier, NULL, NTHR + 1);
	for(i = 0 ; i < NTHR ; i++) {
		err = pthread_create(&tid, NULL, thread_func, (void*)(i));
		if(err != 0)
			cout << "can't create thread" << endl;
	}
	pthread_barrier_wait(&barrier); //wait others thread finish
	for(i = 0 ; i < NTHR ; i++) {
		max = max > thread_res[i] ? max : thread_res[i];
	}
	gettimeofday(&tv2, &tz);

	cout << tv2.tv_sec - tv1.tv_sec << "sec" << endl;

	cout << "max: " << max << endl;

	return 0;
}
