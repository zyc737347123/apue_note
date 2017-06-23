#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct c_stack{
	char s[100];
	int top;
	int base;
};

struct d_stack{
	double s[100];
	int top;
	int base;
};

char PPP[7][7]={
	'>','>','<','<','<','>','>',
	'>','>','<','<','<','>','>',
	'>','>','>','>','<','>','>',
	'>','>','>','>','<','>','>',
	'<','<','<','<','<','=',' ',
	'>','>','>','>',' ','>','>',
	'<','<','<','<','<',' ','=',
};

float Operate(float a,unsigned char s,float b)
{
	if(s=='+')
		return a+b;
	if(s=='-')
		return a-b;
	if(s=='*')
		return a*b;
	if(s=='/')
		return a/b;
}

char OPSET[7]={'+','-','*','/','(',')','#'};
int In(char Test,char *s)//是否为运算符
{
	int i;
	for(i=0;i<7;i++)
	{
		if(Test==s[i])
			return 0;
	}
	return 1;
}

char precede(char a,char b)//比较优先级，并返回
{
	int i,j;
	for(i=0;i<7;i++)
		if(OPSET[i]==a)
			break;
	for(j=0;j<7;j++)
		if(OPSET[j]==b)
			break;
	return PPP[i][j];
}

void cpush(struct c_stack *cs, char c)
{
	cs->top++;
	cs->s[cs->top] = c;
}

void cpop(struct c_stack *cs)
{
	cs->top--;
}

void dpush(struct d_stack *ds, double d)
{
	ds->top++;
	ds->s[ds->top] = d;
}

void dpop(struct d_stack *ds)
{
	ds->top--;
	if(ds->top <= ds->base)
		printf("d error\n");
}

double Eva(char *exp)
{
	struct c_stack Optr;
	memset(&Optr, 0 ,sizeof(Optr));
	struct d_stack Opnd;
	memset(&Opnd, 0 ,sizeof(Opnd));

	char TemData[20];
	char theta, c, Dr[2];

	double Data, a, b, res;

	int i = 0;
	int idx = 0;

	cpush(&Optr, '#');
	c = exp[0];

	strcpy(TemData, "\0");

	while(c != '#' || Optr.s[Optr.top] != '#') {
		
		if(c == 't' || c == 's' || c == 'c') {
			double tmp;
			char s[100];
			int num = 1;
			int count = 1;
			for(idx = i + 4; idx < (int)strlen(exp); idx++) {
				if(exp[idx] == '(') 
					num++;
				if(exp[idx] == ')')
					num--;
				count++;
				if(num == 0)
					break;
			}
			memcpy(s, &exp[i+3], count);
			s[count++] = '#';
			s[count] = '\0';
			tmp = Eva(s);
			switch(c) {
				case 't' :
					tmp = tan(tmp);
					break;
				case 's' :
					tmp = sin(tmp);
					break;
				case 'c' :
					tmp = cos(tmp);
					break;
			}
			dpush(&Opnd, tmp);
			i = idx+1;
			c = exp[i];
			continue;
		}

		if(In(c, OPSET)) {
			Dr[0] = c;
			Dr[1] = '\0';
			strcat(TemData, Dr);
			c = exp[++i];
			if(!In(c, OPSET)) {
				Data = (float)atof(TemData);
				dpush(&Opnd, Data);
				strcpy(TemData, "\0");
			}
		} else {
			switch(precede(Optr.s[Optr.top], c)) {
				case '<' :
					cpush(&Optr, c);
					c = exp[++i];
					break;
				case '=' :
					cpop(&Optr);
					c= exp[++i];
					break;
				case '>' :
					theta = Optr.s[Optr.top];
					cpop(&Optr);
					b = Opnd.s[Opnd.top];
					dpop(&Opnd);
					a = Opnd.s[Opnd.top];
					dpop(&Opnd);
					res= Operate(a, theta, b);
					dpush(&Opnd, res);
					break;
			}
		}
	}

	return Opnd.s[Opnd.top];

}

int main(int argc, char *argv[])
{
	double r;
	char s[100];
	scanf("%s",s);
	r = Eva(s);
	printf("%f\n", r);
	return 0;
}
