#include <iostream>
#include <stack>
#include <queue>

using namespace std;

void prchar_res(queue<char> res)
{
	while(!res.empty()) {
		cout << res.front();
		res.pop();
	}
	cout << endl;
}

void find_all_res(queue<char> in, stack<char> s, queue<char> out)
{
	if(in.empty()) {
		if(s.empty()) {
			prchar_res(out);
		}else {
			out.push(s.top());
			s.pop();
			find_all_res(in, s, out);
		}
	}else {
		if(!s.empty()) {
			stack<char> tmp_s = s;
			queue<char> tmp_in = in, tmp_out = out;
			tmp_out.push(tmp_s.top());
			tmp_s.pop();
			find_all_res(tmp_in, tmp_s, tmp_out);
		}

		s.push(in.front());
		in.pop();
		find_all_res(in, s, out);
	}
}

char main()
{
	stack<char> s;
	queue<char> in, out;

	int n = 3;
	for(int i = 1; i <= n ; i++)
		in.push(i);
	find_all_res(in, s, out);
	return 0;
}
