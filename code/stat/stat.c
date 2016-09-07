#include<apue.h>

char * mystat_mode(struct stat buf);

int main(int argc,char *argv[])
{
	int i=0;
	struct stat buf;
	char *ptr;

	for(i=1;i<argc;i++){
		printf("%s: ",argv[i]);
		if(lstat(argv[i],&buf)<0){
			 /*
			 * #include<sys/stat.h>
			 * int lstat(const char * restrict pathname,struct stat *restrict buf);
			 * sucuess return 0,fail return -1
			 */

			err_sys("lstat error");
			continue;
		}

		ptr=mystat_mode(buf);
		printf("%s\n",ptr);
	}
	exit(0);
}

char* mystat_mode(struct stat buf)
{
	char *ptr;
	if(S_ISREG(buf.st_mode))
		return ptr="regular";
	if(S_ISBLK(buf.st_mode))
		return ptr="block special";
	if(S_ISCHR(buf.st_mode))
		return ptr="character special";
	if(S_ISDIR(buf.st_mode))
		return ptr="directory";
	if(S_ISLNK(buf.st_mode))
		return ptr="sysmbolic link";
	if(S_ISFIFO(buf.st_mode))
		return ptr="fifo";
	if(S_ISSOCK(buf.st_mode))
		return ptr="socket";
	ptr="** unknown mode **";
}
