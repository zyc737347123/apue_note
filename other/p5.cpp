#include <iostream>

using namespace std;

int func(int v)
{
	if(v == 0)
		return 0;
	if(v == 2)
		return 1;
	return 1 + func(v-2);
}

int count(int n)
{
	if(n == 0)
		return 0;
	if(n == 1)
		return 1;
	if(n == 2)
		return 2;
	if(n % 2 != 0) {
		//cout << "n : " << n << ", res :" << count(n-1) << endl;
		return count(n-1);
	}
	else if(){
		//cout << "n : " << n << ", res :" << count(n-1)+count(n-2) << endl;
		return count(n-1) + func(n);
	}
}

int main()
{
	int n;
	cin >> n;
	cout << count(n) << endl;
	return 0;
}
