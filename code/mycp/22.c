#include<apue.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
	if(argc < 3)
		err_sys("usage: desc result");

	int fd=0,fd1;
	int uumask=00777;// 八进制
	struct stat statbuf;

	umask(00000);// key

	if((fd=open(argv[1],O_RDONLY))<0)
		err_sys("open %s error",argv[1]);

	if(stat(argv[1],&statbuf)<0)
		err_sys("stat %s error",argv[1]);

	if(S_ISDIR(statbuf.st_mode))
		err_sys("%s is a directory,please input a file",argv[1]);

	mode_t mode=statbuf.st_mode&uumask;
	//printf("%o\n",mode);
	if((fd1=open(argv[2],O_RDWR | O_EXCL | O_CREAT | O_TRUNC,mode))<0){
		printf("%s is exits,do you want to rewrite(y/n)\n",argv[2]);
		char n;
		scanf("%c",&n);
		if(n =='y' || n == 'Y')
			fd1=open(argv[2],O_RDWR | O_TRUNC);
		else return 0;
	}
	
	off_t size = statbuf.st_size;
	char buf[size];
	char b[1];
	int i=0;
	off_t lek=lseek(fd,0,SEEK_CUR);

	while(lek<=size){
		if(read(fd,b,1)==0){
			lek=lseek(fd,1,SEEK_CUR);
		}else{
			if(b[0]!=0){
			buf[i]=b[0];
			i++;
			}
		}
		lek=lseek(fd,0,SEEK_CUR);
	}
	size_t s=0;
	s=write(fd1,buf,i);
	printf("has copy %ld byte to %s\n",s,argv[2]);
	exit(0);
}
