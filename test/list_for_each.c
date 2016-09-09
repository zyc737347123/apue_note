#include<apue.h>


int main(void)
{
	struct test{
		char *s;
		char t;
		int i;
	};
	
	struct test t;
	t.s="fuck";
	t.t='t';
	t.i=0;
	struct test *ptr;

	size_t offset;
	
	//container_of(ptr,type,menber)
	const typeof(((struct test*)0)->t)* _mptr=&t.t;//未知原因，vim语法检查报，类型不定错误，下同。获得已知成员变量的类型的指针
	
	//offsetof(type,menber)
	offset=(size_t)(&((struct test *)0)->t);// get the offset 结构成员在结构中的偏移量，即结构成员地址与结构体头部地址偏移量

	//list_entry(ptr,type,menber)
	ptr=(struct test *)((char*)_mptr-offset);

	printf("offset %ld\n",offset);
	printf("t.s %s\nptr.s %s\n",t.s,ptr->s);
}
