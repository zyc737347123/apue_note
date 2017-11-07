#include<iostream>
#include<vector>
#include<set>
using namespace std;

typedef struct idea{
	int time;
	int level;
	int need;
}idea;

struct cmp{
	bool operator()(const idea &a,const idea &b)const {
		if(a.level > b.level)
			return true;
		else if(a.level < b.level)
			return false;
		else {
			if(a.need < b.need)
				return true;
			else if(a.need > b.need)
				return false;
			else {
				if(a.time < b.time)
					return true;
				else 
					return false;
			}
		}
	}
};

int main()
{
	int pm_num, m_num, idea_num;
	int 
		cin >> pm_num >> m_num >> idea_num;

	set<idea,cmp> pm[10000];

	for(int i = 0 ; i < idea_num ; i++) {
		int pm;
		cin >> pm;
		
	}

}
