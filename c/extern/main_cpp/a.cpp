#include "cExample.h"
#include<iostream>
using namespace std;

double add(double x,double y);

int main(void)
{
	cout<<add(2,3)<<endl;
	cout<<add(2.3,3.3)<<endl;
	return 0;
}

double add(double x,double y)
{
	return x+y;
}
