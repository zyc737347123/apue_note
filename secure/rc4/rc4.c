#include<apue.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<fcntl.h>

void create_key(unsigned char *k,const int keylen);
void encode(char* m,unsigned char *k,const int len,const int keylen);
void decode(char* c,unsigned char *k,const int len,const int keylen);

static void swap(unsigned char *a,unsigned char *b)
{
	unsigned char t=*a;
	*a=*b;
	*b=t;
}

int main(int argc,char* argv[])
{
	char nn;
	if(argc<4)
		err_sys("usage: ./rc4 -e/-d descfile keyfile outfile");

	int fd1=-1,fd2=-1,fd3=-1;
	struct stat statbuf;

	if((fd1=open(argv[2],O_RDONLY))<0)
		err_sys("open %s fail",argv[1]);
	if(lstat(argv[2],&statbuf)<0)
		err_sys("stat %s fail",argv[1]);

	mode_t mode=statbuf.st_mode&00777;

	if(argc>4){
		if((fd3=open(argv[4],O_RDWR | O_EXCL | O_CREAT | O_TRUNC,mode))<0){
		printf("%s is exits,do you want to rewrite(y/n)\n",argv[4]);
		scanf("%c",&nn);
		if(nn =='y' || nn == 'Y')
			fd3=open(argv[4],O_RDWR | O_TRUNC);
		else{ 
			return 0;
		}
		}
	}else{
		if((fd3=open("out.txt",O_RDWR | O_CREAT,mode))<0)
			err_sys("crete out.txt fail");
	}
	

	int r=0;

	if(strcmp(argv[1],"-e")==0){
		if((fd2=open(argv[3],O_RDWR | O_EXCL | O_CREAT | O_TRUNC,mode))<0){
			printf("%s is exits,do you want to rewrite(y/n)\n",argv[3]);
			scanf("%c",&nn);
			scanf("%c",&nn);
			if(nn =='y' || nn == 'Y')
				fd2=open(argv[3],O_RDWR | O_TRUNC);
			else return 0;
		}
	}else{
		if((fd2=open(argv[3],O_RDONLY))<0)
			err_sys("open %s fail",argv[3]);
	}

	srand(time(NULL));

	int n=10;
	unsigned char *k=(unsigned char*)malloc(sizeof(char)*n);
	create_key(k,n);
	
	if(strcmp(argv[1],"-e")==0){
			int w=0;
			if((w=write(fd2,k,10))!=10)
				err_sys("write fail,write %d byte",w);
			
	}else{
		if((r=read(fd2,k,10))!=10)
			err_sys("read fail,read,read %d byte",r);
			
	}
	close(fd2);

	char m[1024];
	off_t lek,size=statbuf.st_size;
	lek=lseek(fd1,0,SEEK_CUR);

	while(lek<size){
		memset(m,0,sizeof(char)*1024);
		r=read(fd1,m,1024);
		encode(m,k,r,n);
		write(fd3,m,r);
		lek=lseek(fd1,0,SEEK_CUR);
	}
	close(fd1);
	close(fd3);

	exit(0);
}

void create_key(unsigned char *k,const int keylen)
{
	int judge[256],i=0;
	memset(judge,0,sizeof(int)*256);

	for(i=0;i<keylen;){
		int t=i;
		t=rand()%256;
		if(judge[t])
			continue;
		judge[t]=1;
		k[i]=t;
		i++;
	}
}

void encode(char *m,unsigned char *k,const int len,const int keylen)
{
	unsigned char S[256],T[256];
	int i=0;

	for(i=0;i<256;i++){
		S[i]=(unsigned char)i;
		T[i]=k[i%keylen];
	}

	int j=0;
	for(i=0;i<256;i++){
		j=(j+S[i]+T[i])%256;
		swap(&S[i],&S[j]);
	}

	i=0;j=0;
	int l=0;
	for(l=0;l<len;l++){
		i=(i+1)%256;
		j=(j+S[i])%256;
		swap(&S[i],&S[j]);
		int t=(S[i]+S[j])%256;
		unsigned char K=S[t];
		m[l]=m[l]^K;
	}
	
}

