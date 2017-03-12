#include<apue.h>
#include<stdio.h>

int main(void)
{
	char buf[12];
	printf("%% ");
	fgets(buf,5,stdin);
	sleep(3);
	return 0;
}
