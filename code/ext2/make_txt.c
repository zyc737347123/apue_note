#include<apue.h>
#include<fcntl.h>
#include<unistd.h>

int main(void)
{
	char buf[1024];
	int fd=-1,i=0;
	char str='a';

	fd=open("./3",O_RDWR | O_TRUNC);

	for(i=0;i<10;i++){
		int i=0;
		memset(buf,str,1024);
		for(i=0;i<901;i += 100)
			buf[100+i]='\n';
		write(fd,buf,1024);
		str++;
	}
	close(fd);
	exit(0);
}
