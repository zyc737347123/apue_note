#include<apue.h>
#include<limits.h>
#include<dirent.h>

typedef int Pfun(int,int);

/* int ff(int(*p)(int,int),int a,int b)
 * */
int ff(Pfun p,int a,int b)
{
	return p(a,b);
}

int add(int a,int b)
{
	printf("add\n");
	return a+b;
}

int sub(int a,int b)
{
	printf("sub\n");
	return a-b;
}

int f1()
{
	return 1;
}

int f2()
{
	return 2;
}

void f(int f())
{
	printf("%d\n",f());
}

int main(void)
{
	char p[]="test.c";
	size_t len=strlen(p)+1;
	char *t=path_alloc(&len);

	printf("%d\n",ff(add,1,2));
	printf("%d\n",ff(sub,1,2));
	f(f1);
	f(f2);
	exit(0);
}
