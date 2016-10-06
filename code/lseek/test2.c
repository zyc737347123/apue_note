#include<apue.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
	char buffer[100];
	memset(buffer,'c',100);
	printf("%s\n",buffer);

	if(argc<2)
		err_sys("usage fail");
	int fd=open(argv[1],O_RDWR);

	int r=5000;
	if(argc>2){
		r=atoi(argv[2]);
	if(write(fd,buffer,r)!=r)
		err_sys("write error");
	}
	int c=-2;
	if((c=read(fd,buffer,100))!=100)
		printf("%d\n",c);
		//err_sys("read error");

	/*测试证明，空洞文件到被读取到空洞部分是，read和cat都会以'\0'读取*/
	/*而且，在文件系统中，相同字节的空洞比正常文件小*/
	/*read返回的是0，在读取空洞部分时*/


	printf("test %s\n",buffer);



	exit(0);
}
	
