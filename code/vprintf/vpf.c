#include<apue.h>
#include<stdio.h>
#include<stdarg.h>

int vpf(char* fmt,...);

int main(void)
{
	int i=30;
	float j=2.2;
	char* str="hello\n";

	vpf("%d,%f,%s",i,j,str);

	exit(0);
}

int vpf(char* fmt,...)
{
	va_list args;
	int cnt;
	va_start(args,fmt);
	cnt=vprintf(fmt,args);
	va_end(args);
	return cnt;
}
