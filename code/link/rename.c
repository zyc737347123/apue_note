#include<apue.h>

int main(void)
{
	if(rename("1","../4")<0)
		err_sys("rename error");
	exit(0);
}
