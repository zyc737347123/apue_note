#include<apue.h>
#include<fcntl.h>

int main(int argc,char* argv[])
{
	char* ptr;
	size_t size;

	if(argc<2)
		err_sys("usage: dirname");
	if(chdir(argv[1])<0)
		err_sys("chdir error");

	ptr=path_alloc(&size);
	if(getcwd(ptr,size)==NULL)
		err_sys("getcwd error");
	printf("cwd = %s\n",ptr);
	exit(0);
}

