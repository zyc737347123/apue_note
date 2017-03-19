#include<apue.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

void vpf(unsigned char* buf,int nbyte);

int main(void)
{
	unsigned char buf[2048];
	FILE *fp;
	int fd=-1;
	if((fp=fopen("./log","w"))==NULL)
		err_sys("fopen error");
	if((fd=open("/dev/loop0",O_RDONLY))<0)	//need root
		err_sys("open error");

	memset(buf,0,sizeof(buf));
	
	int result=0;
	if((result=read(fd,buf,2048))!=2048)
		err_sys("read error");

	vpf(buf,2048);
	//printf("%02x %02x\n",0,99);
	exit(0);
}

void vpf(unsigned char* buf,int len)
{
	long line=0;
	long i=0,rest=0;
	int j=0;
	for(i=0;i<len;i+=16){
		printf("%06lx ",line);
		rest=len-i;
		if(rest>=16)
			rest=16;
		for(j=0;j<rest-1;j++){
			printf("%02x ",buf[i+j]);
		}
		printf("%02x\n",buf[i+rest-1]);
		line += 16;
	}
}
