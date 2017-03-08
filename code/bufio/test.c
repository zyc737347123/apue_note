#include<apue.h>
#include<stdio.h>

#define BSZ 48

int main()
{
	FILE *fp;
	char buf[BSZ];

	memset(buf,'a',BSZ-2);
	buf[BSZ-2]='\0';
	buf[BSZ-1]='X';
	if((fp=fmemopen(buf,BSZ,"w+")) == NULL)
		err_sys("fmemopen failed");
}
