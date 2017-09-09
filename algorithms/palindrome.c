#include <stdio.h>

int main()
{
	int x;
	scanf("%d", &x);
	
	long tmp = x;
	tmp = tmp > 0 ? tmp : (-1)*tmp; 
	long a = tmp;
	long max = 1;
	int l, r;
	
	while(a/10) {
		max *= 10;
		a /= 10;
	}

	printf("%ld %ld\n", tmp, max);
	fflush(stdout);

	while(tmp) {
		l = tmp / max;
		r = tmp % 10;
		tmp -= l*max;
		max /= 100;
		printf("l %d, r %d, tmp %ld\n",l, r, tmp);
		if(l != r)
			return 0;
		tmp = tmp / 10;
	}
	return 1;
}
