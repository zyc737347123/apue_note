#include<apue.h>
#include<fcntl.h>

#define P 40009
#define Q 40031
#define E 65537

long extended_Euclidean_algorithm(long a,long b)
{
	int i=0;
	long Ri[1000],Qi[1000],Xi[1000],Yi[1000];

	long temp=a;
	if(a<b){
		a=b;
		b=temp;
	}

	// 因为数组无法从-1开始，所以全部i从0开始
	// 初始化
	Ri[0]=a;Ri[1]=b;
	Xi[0]=1;Xi[1]=0;
	Yi[0]=0;Yi[1]=1;
	i=1;

	while(Ri[i]){
		i++;

		Ri[i]=Ri[i-2]%Ri[i-1];

		Qi[i]=Ri[i-2]/Ri[i-1];

		Xi[i]=Xi[i-2]-Qi[i]*Xi[i-1];

		Yi[i]=Yi[i-2]-Qi[i]*Yi[i-1];
	}

	while(Yi[i-1]<0)
		Yi[i-1]=Yi[i-1]+a;

	return Yi[i-1];
}

long modexp(long a,long b,long n)
{
	long ret=1;
	long temp=a;
	while(b){
		if(b&0x1)
			ret=ret*temp%n;
		temp=temp*temp%n;
		b=b>>1;
	}
	while(ret<0)
		ret = ret + n;
	return ret;
}

void encode(unsigned char *str,long *m,const int len,const long n,const long e)
{
	int i=0;
	//char a='a';
	//printf("test %ld\n",modexp((long)a,e,n));
	for(i=0;i<len;i++){
		m[i]=modexp((long)str[i],e,n);
	}
}

void decode(long *l_str,unsigned char *c,const int len,const long n,const long d)
{
	int i=0;
	//printf("test %c\n",(char)modexp(l,d,n));
	for(i=0;i<len;i++){
		c[i]=(unsigned char)modexp(l_str[i],d,n);
	}
}

int main(int argc,char *argv[])
{
	long e=E,p=P,q=Q;
	long n=p*q,fn=(q-1)*(p-1);
	long d=extended_Euclidean_algorithm(fn,e);
	printf("n: %ld,e: %ld,d: %ld\n",n,e,d);

	int fd1=-1,fd3=-1;//fd1 is input,fd3 is output
	char nn;
	struct stat statbuf;

	if(argc<4)
		err_sys("usage: ./rc4 -e/-d descfile keyfile outfile");

	if((fd1=open(argv[2],O_RDONLY))<0)
		err_sys("open %s fail",argv[1]);
	if(lstat(argv[2],&statbuf)<0)
		err_sys("stat %s fail",argv[1]);
	
	mode_t mode=statbuf.st_mode&00777;

	if(argc>3){
		if((fd3=open(argv[3],O_RDWR | O_EXCL | O_CREAT | O_TRUNC,mode))<0){
		printf("%s is exits,do you want to rewrite(y/n)\n",argv[3]);
		scanf("%c",&nn);
		if(nn =='y' || nn == 'Y')
			fd3=open(argv[3],O_RDWR | O_TRUNC);
		else{ 
			return 0;
		}
		}
	}else{
		if((fd3=open("out.txt",O_RDWR | O_CREAT,mode))<0)
			err_sys("crete out.txt fail");
	}

	off_t lek,size=statbuf.st_size;
	unsigned char sbuf[1024];
	long lbuf[1024];
	int r=-1;

	if(strcmp(argv[1],"-e")==0){
		while(r){
			r=read(fd1,sbuf,1024*sizeof(char));
			encode(sbuf,lbuf,r,n,e);
			write(fd3,lbuf,r*sizeof(long));
		}
	}else{
		while(r){
			r=read(fd1,lbuf,1024*sizeof(long));
			//printf("%d\n",r);
			decode(lbuf,sbuf,r/sizeof(long),n,d);
			write(fd3,sbuf,(r/sizeof(long))*sizeof(char));
		}
	}
	
	close(fd1);
	close(fd3);
	

	exit(0);


}

