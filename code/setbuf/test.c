#include<apue.h>

char buf[12];
char buf2[BUFSIZ+1];

int main(void)
{
	FILE *fp;
	memset(buf2,'x',BUFSIZ+1);
	buf2[BUFSIZ]='f';
	buf2[BUFSIZ-1]='u';
	/*
	fp=stdout;
	fprintf(fp,"fuck\n");
	printf("%d\n",BUFSIZ);*/
	setbuf(stdout,buf);	// buf的大小必须是BUFSIZ，因为函数默认就是BUFSIZ，不然会造成缓冲溢出
	//puts("123456"); // 不应该用puts,因为stdout连接到终端设备，是行缓冲
	//puts("7890\n");
	//fputs("123456",stdout);
	//fputs("7890123\n",stdout);
	fprintf(stdout,"%s",buf2);
	//fflush(stdout);
	//printf("%s",buf);
	//putc(buf[12],stdout);	 buf溢出
	sleep(3);
	exit(0);
}
