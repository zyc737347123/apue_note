/* 
 * CS 241
 * The University of Illinois
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "libmapreduce.h"

void map(int fd, const char *data)
{
	unsigned int i, letters = 0;
	for (i = 0; i < strlen(data); i++)
		if (isalpha(data[i]))
			letters++;

	char s[100];
	sprintf(s, "letters: %d\n", letters);


	write(fd, s, strlen(s));
	close(fd);
}

const char *reduce(const char *value1, const char *value2)
{
	int i1 = atoi(value1);
	int i2 = atoi(value2);

	char *result;
	asprintf(&result, "%d", (i1 + i2));
	return result;
}


int main()
{
	mapreduce_t mr;
	char *values[2];
	int i = 10;
	values[0] = "Some text";
	values[1] = NULL;
	
	while(i--){
		//sleep(1);
		int j = 1000, k;
		for(k = 0 ; k < j ; k++)
			k++;

		mapreduce_init(&mr, map, reduce);
		mapreduce_map_all(&mr, (const char **)values);
		mapreduce_reduce_all(&mr);

		const char *s = "letters";
		const char *s1 = mapreduce_get_value(&mr, s);
		if (s1 == NULL)
			printf("%s: (null)\n", s);
		else
			printf("%s: %s\n", s, s1);
		mapreduce_destroy(&mr);
		//sleep(1);
	}



	return 0;
}
