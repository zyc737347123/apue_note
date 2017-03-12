#include<apue.h>
#include<errno.h>
#include<fcntl.h>
#include<limits.h>
#include<unistd.h>

int main(void)
{
	char buff[1024];
	memset(buff,0,1024);
 	const int fd=open("/home/gen/apue",O_RDONLY | O_DIRECTORY);/* 目录必须是绝对路径  */	
	printf("dir %d\n",fd);
	/*
	 * 如果在这里，程序本身不改变fd，应该是没问题
	 * 但是fd所代表的目录发生变化会怎样？？
	 * 实际测试以后，在sleep（30)时，移除了apue的位置,仍然可以正确读取文件
	 * 因为已经有了对应的文件表项，直接访问到了文件的v节点信息
	 */
	sleep(30);
	int ffd=openat(fd,"error.c",O_RDONLY);
	printf("openat %d\n",ffd);
	int n=0;
	if((n=read(ffd,buff,1024))!=1024)
			err_sys("read error");
	write(STDOUT_FILENO,buff,1024);
	return 1;
}
