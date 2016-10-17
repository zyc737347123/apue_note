#include<apue.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
	chdir(argv[1]);
	if(open("apue.h",O_RDWR)<0)
		err_sys("open error");
	exit(0);
}

