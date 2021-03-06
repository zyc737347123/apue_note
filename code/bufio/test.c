#include<apue.h>
#include<stdio.h>
#include<string.h>

#define BSZ 48

int main()
{
	FILE *fp;
	char buf[BSZ];
	int i=0;

	memset(buf,'a',BSZ-2);
	buf[BSZ-2]='\0';
	buf[BSZ-1]='X';
	printf("%s\n",buf);
	if((fp=fmemopen(buf,BSZ,"w+")) == NULL)
		err_sys("fmemopen failed");
	printf("initial buffer contents: %s\n",buf);
	fprintf(fp,"hello world");
	printf("before flush: %s\n",buf);
	fflush(fp);
	for(i=0;i<48;i++){
		putc(buf[i],stdout);
		if(buf[i]==0)
			printf(" ");
	}
	printf("\n");
	printf("after fflush: %s\n",buf);
	printf("len of string in buf = %ld\n",(long)strlen(buf));

	//memset(buf,'b',BSZ-2);   FILE *fp 维护着文件偏移量
	buf[BSZ-2] = '\0';
	buf[BSZ-1] = 'X';
	printf("%s\n",buf);
	fprintf(fp,"hello world");
	fseek(fp,0,SEEK_SET);
	printf("after fseek: %s\n",buf);
	printf("len of string in buf = %ld\n",(long)strlen(buf));

	memset(buf,'c',BSZ-2);
	buf[BSZ-2] = '\0';
	buf[BSZ-1] = 'X';
	printf("%s\n",buf);
	fprintf(fp,"hello world");
	fclose(fp);
	printf("after fclose: %s\n",buf);
	printf("len of string in buf = %ld\n",(long)strlen(buf));

	return 0;

}


