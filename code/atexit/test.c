#include<apue.h>
#include<stdlib.h>

// atexit是先来后出的，估计是栈,至少可以注册32个函数

static void my_exit1(void);
static void my_exit2(void);

int main(void)
{
	if(atexit(my_exit2)!=0)
		err_sys("can't register my_exit2");
	if(atexit(my_exit1)!=0)
		err_sys("can't register my_exit1");
	if(atexit(my_exit1)!=0)
		err_sys("can't register my_exit1");
	printf("main is done\n");
	return 0; 	// 特意不用exit
}

static void my_exit1(void)
{
	printf("my_exit1\n");
}

static void my_exit2(void)
{
	printf("my_exit2\n");
}
