#include<apue.h>
#include<fcntl.h>
#include<stdio.h>
int main(void)
{
	int fd1,fd2;
	if((fd1=open("test.c",O_RDWR))<0)
		err_sys("open error");

	char buf[100];
	sprintf(buf,"/dev/fd/%d",fd1);//不安全的函数

	if((fd2=open(buf,O_RDWR))<0)
		err_sys("open error");

	int r=read(fd1,buf,10);
	printf("read %d\n",r);

	size_t f1=lseek(fd1,0,SEEK_CUR);
	size_t f2=lseek(fd2,0,SEEK_CUR);
	printf("f1 %ld,f2 %ld\n",f1,f2);
	char b[100];
	memset(b,0,100);
	read(fd2,b,10);
	printf("f1 %s\nf2 %s\n",buf,b);
	exit(0);
}
