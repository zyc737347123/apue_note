#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

string n512 = "13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084096";

vector<int> up(vector<int>& value)
{
	vector<int> tmp;
	long i = value.size() - 1;
	int flag = 0, v = 0;

	for(; i >= 0 ; i--) {
		v = value.at(i);
		v *= 2;
		v += flag;
		flag = v/10;
		v %= 10;
		tmp.push_back(v);
	}

	if(flag)
		tmp.push_back(flag);

	reverse(tmp.begin(), tmp.end());	

	return tmp;
}

int main()
{
	int n = 0, i = 0;
	char c;
	unsigned size = n512.size();
	vector<int> res;
	cin >> n;

	for(i = 0 ; i < size ; i++) {
		c = n512[i];
		res.push_back( c - '0');
	}

	for( ; n > 512; n--) {
		res = up(res);
	}

	for (size_t i = 0; i < res.size(); i++) {
		cout << res.at(i);
	}
	cout << endl ;
	return 0;
}

