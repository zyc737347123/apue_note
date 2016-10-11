#include<apue.h>
#include<dirent.h>
#include<limits.h>

/******************* path_alloc **************************/

#ifdef PATH_MAX
static size_t pathmax=PATH_MAX;
#else
static size_t pathmax=0;
#endif

static long posix_version=0;
static long xsi_version=0;

#define PATH_MAX_GUESS 1024

char* path_alloc(size_t *sizep)
{
	char *ptr;
	size_t size;

	if(posix_version == 0)
		posix_version = sysconf(_SC_VERSION);
	if(xsi_version == 0)
		xsi_version = sysconf(_SC_XOPEN_VERSION);

	if(pathmax == 0){ /* return PATH_MAX+1  for null in path end */
		errno=0;
		if((pathmax=pathconf("/",_PC_PATH_MAX))<0){ /* _PC_PATH_MAX 是相对路径名最大字节数，包括null字节（看标准的版本） */
			if(errno == 0)
				pathmax=PATH_MAX_GUESS;
			else
				err_sys("pathconf error for _PC_PATH_MAX");
		}else{
			pathmax++;
		}
	}

	if((posix_version < 200112L) && (xsi_version < 4))
		size=pathmax+1;
	else
		size=pathmax;
	if((ptr=(char *)malloc(size))==NULL)
		err_sys("malloc error for pathname");
	if(sizep !=NULL)
		*sizep=size;
	return ptr;
}

/****************** path_alloc ***************************/


typedef int Myfunc(const char*,const struct stat *,int);

static Myfunc myfunc;
static int myftw(char *,Myfunc *);//static 修饰符 函数,内部函数，不怕重名
static int dopath(Myfunc *);
static long nreg,ndir,nblk,nchr,nfifo,nslink,nsock,ntot;

int main(int argc,char* argv[])
{
	int ret;
	if(argc!=2)
		err_quit("usage: nftw <starting-pathname>");
	ret=myftw(argv[1],myfunc);	/*does it all ??? */

	ntot=nreg+ndir+nblk+nchr+nfifo+nsock+nslink;//统计
	if(ntot==0)
		ntot=1;	/* avoid divide by 0;print 0 for all counts */
	printf("regular files	= %7ld,%5.2f %%\n",nreg,nreg*100.0/ntot);

	printf("directories	= %7ld,%5.2f %%\n",ndir,ndir*100.0/ntot);	

	printf("block		= %7ld,%5.2f %%\n",nblk,nblk*100.0/ntot);

	printf("char special	= %7ld,%5.2f %%\n",nchr,nchr*100.0/ntot);

	printf("FIFOs 		= %7ld,%5.2f %%\n",nfifo,nfifo*100.0/ntot);

	printf("symbolic link	= %7ld,%5.2f %%\n",nslink,nslink*100.0/ntot);

	printf("sockets		= %7ld,%5.2f %%\n",nsock,nsock*100.0/ntot);

	exit(0);
}

/*
 * the caller's func() is called for every file
 */

#define FTW_F 1		/* file other than directory */
#define FTW_D 2		/* directory */
#define FTW_DNR 3	/* directory that can't be read */
#define FTW_N 4		/*file that we can't stat */

static char *fullpath;	/* contains full pathname for everyfile */
static size_t pathlen;

static int myftw(char *pathname,Myfunc *func)
{
	fullpath = path_alloc(&pathlen);	/* malloc PATH_MAX + 1 bytes */
	// 获得 限制（PATH_MAX)
	if(pathlen<=strlen(pathname)){
			pathlen=strlen(pathname)*2;
			if((fullpath = (char *)realloc(fullpath,pathlen))==NULL) /* 为什么要加大fullpath的大小，大小为 pathname*2 */
				err_sys("realloc error");
	}
	strcpy(fullpath,pathname);// 在这里更新fullpath
	return (dopath(func));
}

static int dopath(Myfunc *func)
{
	struct stat statbuf;
	struct dirent *drip;
	DIR *dp;
	int ret ,n;

	if(lstat(fullpath,&statbuf)<0)
		return func(fullpath,&statbuf,FTW_N);

	if(S_ISDIR(statbuf.st_mode)==0)	//兼容不完善
		return func(fullpath,&statbuf,FTW_F);
	/* if(S_IFDIR == (statbuf.st_mode & S_IFMT))
	 * 		return func(fullpath,&statbuf,FTW_F);
	 */

	/* it's a directory */
	if((ret = func(fullpath,&statbuf,FTW_D))!=0)
		return ret;
	n=strlen(fullpath);
	if((size_t)(n+NAME_MAX+2)>pathlen){	//NAME_MAX 文件名最大字节数，n（文件名之前的路径），NAME_MAX(文件名允许最大值),2('/','\0')
		pathlen *=2;// 如果fullpath不够大就翻倍
		if((fullpath = (char *)realloc(fullpath,pathlen))==NULL)
			err_sys("realloc error");
	}
	fullpath[n++]='/'; /* 以 '/' 结尾，n++后加式 */
	fullpath[n]=0;
	if((dp=opendir(fullpath)) == NULL)
		return func(fullpath,&statbuf,FTW_DNR);
	while((drip=readdir(dp))!=NULL){
		if(strcmp(drip->d_name,".")==0 || strcmp(drip->d_name,"..") ==0)
			continue;
		strcpy(&fullpath[n],drip->d_name); /* append file name after "/"  把d_name复制到fullpath后面 更新了fullpath */
		if((ret = dopath(func))!=0)
			break;
	}
	fullpath[n-1]=0;// 截断，把d_name去掉
	if(closedir(dp)<0)
		err_ret("can't close directory %s,fullpath");
	return ret;
}

static int myfunc(const char* pathname,const struct stat* statptr,int type)
{
	switch(type){
		case FTW_F:
			switch (statptr->st_mode & S_IFMT){ /* 为了兼容旧版本的UNIX 使用S_IFxxx宏 */
				case S_IFREG:	nreg++;		break;
				case S_IFBLK:	nblk++;		break;
				case S_IFCHR:	nchr++;		break;
				case S_IFIFO:	nfifo++;	break;
				case S_IFLNK:	nslink++;	break;
				case S_IFSOCK:	nsock++;	break;
				case S_IFDIR:	err_dump("for S_IFDIR for %s",pathname);
			}
			break;
		case FTW_D:
			ndir++;
			break;
		case FTW_DNR:
			err_ret("can't read directory %s",pathname);
			break;
		case FTW_N:
			err_ret("stat error for %s",pathname);
			break;
		default:
			err_dump("unknown type %d for pathname %s",type,pathname);
	}
	return 0;
}
