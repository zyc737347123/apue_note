#include<apue.h>
#include<fcntl.h>

int main(int argc,char* argv[])
{
	struct stat statbuf;
	struct timespec times[2];
	int fd;

	for(int i=1;i<argc;i++){
		if(stat(argv[i],&statbuf)<0){
			err_ret("%s: stat error",argv[i]);
			continue;
		}

		if((fd=open(argv[i],O_RDWR | O_TRUNC))<0){
			err_ret("%s: open error",argv[i]);
			continue;
		}
		
		times[0]=statbuf.st_atim;//access time
		times[1]=statbuf.st_mtim;//file change time

		if(futimens(fd,times)<0){
			err_ret("%s: futimens error",argv[i]);
			continue;
		}
		close(fd);
	}
	exit(0);
}
