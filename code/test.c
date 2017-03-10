#include<apue.h>
#include<stdio.h>

int main(void)
{
	FILE *fp;
	char buf[64];

	memset(buf,0,64*sizeof(char));

	if((fp=fmemopen(buf,64,"w+")) == NULL)
		err_sys("fmemopne failed");
	char buf2[66];
	memset(buf2,'x',65);
	buf2[64]='f';
	buf2[65]=0;
	fprintf(fp,"%s",buf2);
	fscanf(fp,"%s",buf2);
	printf("%s\n",buf);
	fclose(fp);
	printf("%s\n",buf);
	printf("%s\n",buf2);
	return 0;
}
