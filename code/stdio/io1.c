#include<apue.h>

int main(void)
{
	int c=0;
	while((c=getc(stdin))!='#'){ // getc 是从流里读取，stdin流 会堵塞 直到遇到 （换行符就解除堵塞） 但从流能读到多少字符由调用的getc次数决定
		//ungetc('e',stdin);
		printf("1\n");
		if(putc(c,stdout) == EOF)
			err_sys("output error");
	}

	fflush(stdin);

	putchar('\n');
	ungetc('e',stdin);
	ungetc('f',stdin);
	c=getc(stdin);
	putc(c,stdout);
	c=getc(stdin);
	putc(c,stdout);
	putchar('\n');

	c=getc(stdin);

	ungetc('z',stdin);
	c=getc(stdin);
	putc(c,stdout);
	c=getc(stdin);
	putc(c,stdout);
	putchar('\n');


	if(ferror(stdin))
		err_sys("input error");

	exit(0);
}

