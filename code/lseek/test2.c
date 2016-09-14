#include<apue.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
	char buffer[10000];
	memset(buffer,'c',10000);

	if(argc<2)
		err_sys("usage fail");
	int fd=open(argv[1],O_RDWR | O_TRUNC);

	int r=5000;
	if(argc>2)
		r=atoi(argv[2]);
	if(write(fd,buffer,r)!=r)
		err_sys("write error");
	//if(read(fd,buffer,1000)!=1000)
		//err_sys("read error");

	/*测试证明，空洞文件到被读取到空洞部分是，read和cat都会以'\0'读取*/
	/*而且，在文件系统中，相同字节的空洞比正常文件小*/


	//printf("%s\n",buffer);



	exit(0);
}
	
