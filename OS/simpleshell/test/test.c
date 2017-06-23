#include <stdio.h>
#include <stdlib.h>

char defprompt[] = "[debug@oaid]>";

int main(int argc, char **argv)
{
#if 0
	fprintf(stderr, "This is a test message%c", '\b');
	fprintf(stderr, "\b\b");
	putchar('\a');
#else
	puts(defprompt);
	fprintf(stderr, "OUNT\n");
#endif
}
