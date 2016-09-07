#include<apue.h>
#include<errno.h>
#include<limits.h>
#include<sys/resource.h>

#define OPEN_MAX_GUESS 256 

static void pr_pathconf(char *msg,char *path,int name);

clock_t get_time();

long open_max(void);

int main(void)
{
	get_time();
	printf("opne file max %ld\n",open_max());
	return 1;
}

clock_t get_time()
{
	printf("clock_t ");
	long val;
	errno=0;
	if((val=sysconf(_SC_CLK_TCK))<0)
	{
		
		if(errno!=0){
			if(errno==EINVAL)
			fputs(" (no supported\n",stdout);
			else
			err_sys("sysconf error");
		}else{
			fputs(" (no limit)\n",stdout);
		}
	}else{
		printf(" %ld\n",val);
	}
	return val;

}

long open_max(void)
{
	long openmax;
	struct rlimit rl;
	
	if((openmax=sysconf(_SC_OPEN_MAX))<0||openmax==LONG_MAX){
		if(getrlimit(RLIMIT_NOFILE,&rl)<0)
			err_sys("can't get file limit");
		if(rl.rlim_max==RLIM_INFINITY)
			openmax=OPEN_MAX_GUESS;
		else
			openmax=rl.rlim_max;
	}

	return openmax;
}

static void pr_pathconf(char *msg,char *path,int name)
{
	long val;

	errno=0;
	if((val=pathconf(path,name))<0){
		if(errno!=0){
			if(errno==EINVAL)
				fputs(" (not support)\n",stdout);
			else
				err_sys("pathconf error,path= %s",path);
		}else
			fputs(" (no limit)\n",stdout);
	}else
		printf("%s %ld\n",msg,val);
}
