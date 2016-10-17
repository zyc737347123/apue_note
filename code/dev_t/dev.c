#include<apue.h>
#include<linux/kdev_t.h>
#ifdef SOLARIS
#include<sys/mkdev.h>
#endif

int main(int argc,char *argv[])
{
	int i;
	struct stat buf;
	for(i=1;i<argc;i++){
		printf("%s: ",argv[i]);
		if(stat(argv[i],&buf)<0){
			err_ret("stat error");
			continue;
		}
		printf("dev = %d/%d",(int)MAJOR(buf.st_dev),(int)MINOR(buf.st_dev));
		if(S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)){
			printf(" (%s) rdev = %d/%d",(S_ISBLK(buf.st_mode)) ? "block" : "character",(int)MAJOR(buf.st_rdev),(int)MINOR(buf.st_rdev));
		}
		printf("\n");
	}
	exit(0);
}
