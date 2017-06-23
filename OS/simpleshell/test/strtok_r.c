#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//char cmdstr[] = "a bbb ccc		ff";
char *cmdstr = "a   bbb ccc		ff";

int main(int argc, char *argv[])
{
	char line[128];
	char *str, *token;
	char *saveptr;
	char delims[] = " \t";
	int c = 0;

	memcpy(line, cmdstr, strlen(cmdstr));
	printf("%s\n", line);

#if 1
	for (c = 1, str = line; ; c++, str = NULL) {
		token = strtok_r(str, delims, &saveptr);
		if (token == NULL)
			break;
		printf("%d : --> %s\n", c, token);
	}
#else
	token = strtok_r(str, delims, &saveptr);
	if (token == NULL)
		return 0;
	printf("%d : --> %s\n", c, token);
#endif
	printf("%s\n", line);
	return 0;
}
