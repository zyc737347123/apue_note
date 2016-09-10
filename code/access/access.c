#include<apue.h>
#include<fcntl.h>

#define w W_OK
#define r R_OK
#define x X_OK

int myaccess(const char* pathname,int mode);

int main(int argc,char *argv[])
{
	if(argc!=2)
		err_quit("usage: myaccess <pathname>");
	if(myaccess(argv[1],r)<0)
		err_ret("access error for %s",argv[1]);
	else
		printf("read access OK\n");
	exit(0);
}

int myaccess(const char *pathname,int mode)
{
	return access(pathname,mode);
}
