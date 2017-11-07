#include <stdio.h>

void exchange(int n)
{
	char s;
	if(n<0) {
		n *= -1;
		printf("-");
	}
	if(n) {
		s = '0' + n%10;
		exchange(n/10);
		printf("%d ", s);
		return;
	}

	return;
}

int main(void)
{
	exchange(-100);
	printf("\n");
	return 0;
}
