#include<apue.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	if(argc<2)
		err_sys("usage: dirname");
	if(chdir(argv[1])<0)
		err_sys("chdir error");

	int fd=-1;
	if((fd=open("1",O_RDWR))<0)
		err_sys("open error");

	exit(0);
}
