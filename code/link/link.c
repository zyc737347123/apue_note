#include<apue.h>
#include<unistd.h>

void mylink(const char *exit,const char *newf);
void myunlink(const char *path);


int main(int argc,char *argv[])
{
	char buf[100];
	memset(buf,0,100);

	struct stat file;

	if(argc!=2)
		printf("usage exit new");

	if(lstat(argv[1],&file)<0)
		err_sys("lstat error");
	//if(!S_ISLNK(file.st_mode))
		//err_sys("not symbolic link");

	if(readlink(argv[1],buf,100)<0)
		err_sys("readlink eroro");

	printf("%s\n",buf);

	//mylink(argv[1],argv[2]);
	exit(0);
}

void mylink(const char *exit,const char *newf)
{
	if(link(exit,newf)<0)
		err_sys("link error");
}

