#include<apue.h>

int main(void)
{
	int a[2][3]={0,1,2,3,4,5};
	int *p1=&a[0][0];
	int b[6]={0,1,2,3,4,5};

	int i=0,j=0;
	for(i=0;i<2;i++)
		for(j=0;j<3;j++)
			printf("%d ",*(p1+j+(i*3)));
	return 0;
}
