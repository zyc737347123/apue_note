#include<stack>
#include<iostream>
#include<string>
#include<cstdio>
#include<string.h>
#include<iomanip>
#include<cstdlib>
using namespace std;
#define OK 0
#define ERROR -1
#define OVERFLOW -1
#define OPSETSIZE 7
typedef int Status;
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
char OPSET[OPSETSIZE]={'+','-','*','/','(',')','#'};
Status In(char Test,char *s)//是否为运算符
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
float Eva(string exp)
{
    stack <char> Optr;
    stack <double> Opnd;
    char TempData[20];
    double Data,a,b,r;
    char theta,Dr[2];
    char c;
    int i=0;
    Optr.push('#');
    c=exp[0];
    strcpy(TempData,"\0");
	cout<<exp<<endl;
    while(c!='#'||Optr.top()!='#')
    {
        cout<<"char:"<<c<<endl;
        if(In(c,OPSET))
        {
            Dr[0]=c;
            Dr[1]='\0';
            strcat(TempData,Dr);
            c=exp[++i];
            if(!In(c,OPSET))
            {
                Data=(float)atof(TempData);
                cout<<"data:"<<Data<<endl;
                Opnd.push(Data);
                strcpy(TempData,"\0");
            }
        }
        else
        {
            cout<<"join ("<<i<<")"<<endl;
            switch(precede(Optr.top(),c))
            {
            case'<':Optr.push(c);
                    c=exp[++i];
                    break;
            case'=':Optr.pop();
                    c=exp[++i];
                    break;
            case'>':theta=Optr.top();
                    Optr.pop();
                    b=Opnd.top();
                    Opnd.pop();
                    a=Opnd.top();
                    Opnd.pop();
                    r = Operate(a,theta,b);
					cout<<"res:"<<r<<endl;
                    Opnd.push(Operate(a,theta,b));
                    break;
            }
			cout<<"join done\n";
		}
    }
    return Opnd.top();
}
int main()
{
 
    string exp;
    int t;
    double r;
    cin>>t;
    while(t--)
    {
        cin>>exp;
        r=Eva(exp);
        cout<<fixed<<setprecision(4)<<r<<endl;
    }
    return 1;
}
 
