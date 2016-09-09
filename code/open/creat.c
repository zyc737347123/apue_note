#include<apue.h>
#include<fcntl.h>
#include<errno.h>

int main(void)
{
	int fd=-1;
	
	if((fd=creat("test",S_IRWXU))<0)
		err_sys("creat error");
	
	if((fd=open("test2",O_EXCL | O_CREAT | O_WRONLY | O_TRUNC,00700))<0) /* 测试证明，mode的00700有效，而且高位的00不可省略*/
		err_sys("open error");
	
	/*
	 * 而且，creat()重复创建相同文件不会报错，open()在加上O_EXCL标志位后会报错
	 */

	exit(0);
}
