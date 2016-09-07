#include<apue.h>
#include<errno.h>
#include<fcntl.h>
#include<limits.h>

struct bucket_desc
{	/* 16 bytes */
	  void *page;	// 该桶描述符对应的内存页面指针。
	  struct bucket_desc *next;	// 下一个描述符指针。
      void *freeptr;	// 指向本桶中空闲内存位置的指针。
	  unsigned short refcnt;	// 引用计数。
	  unsigned short bucket_size;	// 本描述符对应存储桶的大小。
};

/*
struct _bucket_dir bucket_dir[] = {
	{16, (struct bucket_desc *) 0},	// 16 字节长度的内存块。
	{32, (struct bucket_desc *) 0},	// 32 字节长度的内存块。
    {64, (struct bucket_desc *) 0},	// 64 字节长度的内存块。
    {128, (struct bucket_desc *) 0},	// 128 字节长度的内存块。
    {256, (struct bucket_desc *) 0},	// 256 字节长度的内存块。
    {512, (struct bucket_desc *) 0},	// 512 字节长度的内存块。
    {1024, (struct bucket_desc *) 0},	// 1024 字节长度的内存块。
    {2048, (struct bucket_desc *) 0},	// 2048 字节长度的内存块。
	{4096, (struct bucket_desc *) 0},	// 4096 字节(1 页)内存。
	{0, (struct bucket_desc *) 0}
};	 End of list marker */

int main(void)
{
	struct bucket_desc a;
	//struct _bucket_dir *bdir;
	/*
	printf("void* %ld\n",sizeof(void*));
	printf("un short %ld\n",sizeof(unsigned short));
	printf("%ld\n",sizeof(struct bucket_desc*));
	printf("size %ld\n",sizeof(a));
	
	char b='a';
	char *ptr=&b;
	printf("long %ld\n",sizeof(long));
	printf("int %ld\n",sizeof(int));
	printf("long long %ld\n",sizeof(long long));
	printf("*ptr %s\n",ptr);
	printf("*(char **)ptr %ld\n",(long)*((char **)ptr));
	*/

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

 	int fd=open("/home/gen/apue",O_RDONLY | O_DIRECTORY);/* 目录必须是绝对路径  */	
	printf("dir %d\n",fd);
	int ffd=openat(fd,"error.c",O_RDONLY);
	printf("openat %d\n",ffd);
	return 1;
}
