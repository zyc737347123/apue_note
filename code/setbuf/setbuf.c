#include<apue.h>
#include<stdio.h>


/*
 * restrict is the key word of C99
 * so,gcc -std=c99 setbuf.c
 */

void setbuf2(FILE * restrict fp ,char * restrict buf);
void pr_stdio(const char *,FILE *);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);

int main(void)
{
	FILE *fp;
	static char buf[BUFSIZ];
	if((fp=fopen("./test.c","r")) == NULL)
		err_sys("fopen error");
	pr_stdio("file",fp);
	pr_stdio("stdout",stdout);
	
	setbuf2(fp,NULL);
	pr_stdio("file",fp);
	setbuf2(stdout,NULL);
	pr_stdio("stdout",stdout);
	/*
	setbuf2(fp,buf);
	pr_stdio("file",fp);
	setbuf2(stdout,buf);
	pr_stdio("stdout",stdout);*/
	return 0;
}

void setbuf2(FILE * restrict fp,char * restrict buf)
{
	
	if(buf == NULL){
		if(setvbuf(fp,buf,_IONBF,BUFSIZ))
			err_sys("setvbuf error");
		return;
	}

	/*
	size_t bufsize=0;
	bufsize=strlen(buf);
	if(bufsize != BUFSIZ){
		printf("buf size %ld is error\n",bufsize);
		return;
	}*/

	if(fp == stdout || fp == stdin || fp == stderr){
		if(setvbuf(fp,buf,_IOLBF,BUFSIZ))
			err_sys("setvbuf error");
		printf("is stdio\n");
	}else{
		if(setvbuf(fp,buf,_IOFBF,BUFSIZ))
			err_sys("setvbuf error");
		printf("is regular file stream\n");
	}

}

void pr_stdio(const char *name,FILE *fp)
{
	printf("stream = %s,",name);
	if(is_unbuffered(fp))
		printf("unbuffered");
	else if(is_linebuffered(fp))
		printf("linebuffered");
	else
		printf("fully buffered");
	printf(",buffer size = %d\n",buffer_size(fp));
}

#if defined(_IO_UNBUFFERED)
int is_unbuffered(FILE *fp)
{
	return fp->_flags & _IO_UNBUFFERED;
}

int is_linebuffered(FILE *fp)
{
	return fp->_flags & _IO_LINE_BUF;
}

int buffer_size(FILE *fp)
{
	return fp->_IO_buf_end - fp->_IO_buf_base;
}
#endif
