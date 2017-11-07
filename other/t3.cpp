#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

bool isempty(vector<int> &v)
{
	long i = 0;
	for(i = 0 ; i < v.size() ; i++) {
		if(v[i])
			return false;
	}

	return true;
}

bool islegal(vector<int> &orgv, vector<int> &tmpv)
{
	long i = 0;
	for(i = 0 ; i < orgv.size() ; i++) {
		if(orgv[i] == 0 && tmpv[i] == 1)
			return false;
	}
	return true;
}

void cut(vector<int> &tmpv) {
	long i = tmpv.size() - 1;
	for(; i > -1 ; i--) {
		if(tmpv[i] == 1) {
			tmpv[i] = 0;
			break;
		}
	}
	long j = tmpv.size() - 1;

	for(; j > i ; j--)
		tmpv[j] = 1;

}

int main()
{
	string str;
	vector<int> binstr, tmpv;
	char oldc, newc, tmpc;
	long i;

	cin >> str >> oldc >> newc;

	for(i = 0 ; i < str.size() ; i++) {
		tmpc = str[i];
		if(tmpc == oldc)
			binstr.push_back(1);
		else
			binstr.push_back(0);
	}

	tmpv = binstr;
	
	while(!isempty(tmpv)) {
		if(!islegal(binstr, tmpv)) {
			cut(tmpv);
			continue;
		}
		for(i = 0 ; i < tmpv.size() ; i++) {
			if(tmpv[i] == 0 && binstr[i] == 1)
				cout << newc;
			else
				cout << str[i];
		}
		cout << ' ';
		cut(tmpv);
	}

	return 0;
}
