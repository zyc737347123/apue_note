#include<apue.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void change(char *s , int l, int r)
{
	char tmp[124];
	int x, num;
	memcpy(tmp, &s[l+1], 3);
	memcpy(&s[l], tmp, 3);
	s[l+3] = ' ';
	s[l+4] = '(';
	
	if(s[l+5] == '(') {
		x = l+6;
		num = 1;
		for(;x <= r;x++) {
			if(s[x] == '(')
				num++;
			if(s[x] == ')')
				num--;
			if(num == 0)
				break;
		}
		change(s, l+5, x); 
	}else {
		x = l + 5;
		for(;x <= r;x++)
			if(s[x] <= '0' || s[x] >= '9')
				break;
		x--;
	}

	s[++x] = ',';

	int t = x+1;
	if(s[++x] == '(') {
		num = 1;
		for(;x <= r;x++) {
			if(s[x] == '(')
				num++;
			if(s[x] == ')')
				num--;
			if(num == 0)
				break;
		}
		change(s, t, x);
	}
}

int main()
{
	char line[124], tmp[124];
	int len, i, j;
	int num = 0;

	memset(tmp,0,124);
	memset(line,0,124);
	fgets(line,124,stdin);
	len = strlen(line);
	change(line,0,len-1);

	/*
	for(i = 0, j = 0;i < len;) {
		if(line[i] == '('){
			memcpy(tmp, &line[i+1], 3); // copy op
			j += 3;
			i += 4;
			tmp[j++] = '(';
			if(line[i+1] == '(') {
				int x = i + 2;
				num++;
				for(;x<len;x++){
					if(line[x] == '(')
						num++;
					if(line[x] == ')')
						num--;
					if(num == 0)
						break;
				}
				memcpy(&tmp[j], &line[i+1], x - (i + 1) + 1);
				j += x - i;
				tmp[j++] = ',';
				memcpy(&tmp[j], &line[x+2], len - (x + 2));

				//printf("%s\n",tmp);
			}else {
				int x = i + 1;
				for(;x<len;x++)
					if(line[x] <='0' || line[x] >= '9')
						break;
				x--;
				memcpy(&tmp[j], &line[i+1], x - i);
				j += x - i;
				tmp[j++] = ',';
				memcpy(&tmp[j], &line[x+2], len - (x + 2));
			}
			memcpy(line,tmp,strlen(tmp));
			continue;
		}
		i++;
	}*/
	printf("%s\n",line);
	return 0;
}
