#include<iostream>
using namespace std;

/*
	״̬ת�Ʒ��̣� h[i] = Max{ h[j] | high[j] <= high[i] } + 1
	 	      (i<j<=n)
*/

void Hold_up(int Num)
{
	int *high = new int[Num + 1];
	int *h = new int[Num + 1];
	int *w = new int[Num + 1];
	for (int i = 0; i <= Num; i++)
	{
		high[i] = 0;//�����߶�
		h[i] = 0;//�ӵ�iö������ʼ���������ص������
		w[i] = 0;//����·��
	}
	for (int i = 1; i <= Num; i++)
	{
		cout << "�������" << i << "ö�����ĸ߶�" << endl;
		cin >> high[i];
	}
	int Max = 1,star=Num;
	h[Num] = 1; // �Ϸ��ǳ�ʼ��״̬
	for (int i = Num-1; i >= 1; i--)//����������е�h[i]
	{
		int max = 0;
		for (int j = i + 1; j <= Num; j++)
			if (high[j] <= high[i])
				if (h[j] > max)
				{
					max = h[j];
					w[i] = j;
				}
		h[i] = max + 1;
		if (h[i] > Max)
		{
			Max = h[i];
			star = i;
		}
	}

	//show
	cout << "Best:" << Max << endl;
	int i = star;
	cout << high[star] << "--";
	while (w[i] != 0)
	{
		cout << high[w[i]] << "--";
		i = w[i];
	}
}
int main()
{
	int n;
	cout << "�����뵼������" << endl;
	cin >> n;
	Hold_up(n);
	cout << "End" << endl;
	cin >> n;
	return 1;
}
