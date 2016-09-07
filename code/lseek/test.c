#include<apue.h>
#include<fcntl.h>
#include<unistd.h>

int main(void)
{
	const int fd=open("/home/gen/apue/1.c",O_RDWR | O_APPEND);
	char buffer[10];
	buffer[9]='\0';

	if(fd<0)
		err_sys("open error");



	if(read(fd,buffer,9)<9)
		err_sys("read error");

	printf("%s\n",buffer);

	if(lseek(fd,20,SEEK_SET)==-1)
		err_sys("lseek error");
	
	read(fd,buffer,9);
	printf("%s\n",buffer);

	write(fd,"fuck you\n\0",strlen("fcuk you\n\0"));
	exit(0);
}
