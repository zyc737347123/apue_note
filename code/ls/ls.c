#include<apue.h>
#include<dirent.h>

void ls(char * dir);

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		err_quit("usage: ls dir");
	}
	ls(argv[1]);
	exit(0);
}

void ls(char * dir)
{
	DIR *dp;
	struct dirent *dirp;

	
	
	if((dp=opendir(dir))==NULL)
		err_sys("can't open %s",dir);
	while((dirp=readdir(dp))!=NULL)
		printf("%s\n",dirp->d_name);
	closedir(dp);
}
