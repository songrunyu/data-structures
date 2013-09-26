/*
 * =====================================================================================
 *
 *       Filename:  stack_calculate.c
 *
 *    Description:  表达式求值(顺序栈存储)
 *
 *        Version:  1.0
 *        Created:  2013年09月23日 20时05分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 100

typedef struct           
{	
	double data[MAX];
	int top;
}node;

node *init()            
{
	node  *s;
	s = (node *)malloc(sizeof(node));
	s->top = -1;
	return s;
}

int push(node *s,double x)     
{
	if(s->top == MAX-1)
	{
		printf("栈满，不得入栈！");
		return 0;
	}
	else
	{
		s->top++;
		s->data[s->top] = x;

		return 1;
	}
}

int pop(node *s,double *x)       
{
	if(s->top == -1)
	{
		printf("栈空，不得出栈！");
		return 0;
	}
	else
	{
		*x = s->data[s->top];
		s->top--;
		return 1;
	}

}

int judge(char ch)          //判断运算符的优先级 
{
	switch(ch)
	{
		case '+':return 1;
			 break;
		case '-':return 1;
			 break;
		case '*':return 2;
			 break;
		case '/':return 2;
			 break;
		case '(':return 3;
			 break;
		case ')':return 3;
			 break;
		default: return 0;
			 break;
	}
}

double calculate(double a, double b, char c)  //计算 
{
	switch(c)
	{
		case '+':return (a+b);
			 break;
		case '-':return (b-a);
			 break;
		case '*':return (a*b);
			 break;
		case '/':if(a == 0)
			 {
				 printf("分母不得为零！\n");
				 exit(0);
			 }
			 else
			       return (b/a);
			 break;
	}

}

int main()  
{
	node *OPRD,*OPTR;
	char ch;
	double a,b,c,x;
	int m = -1;
	int p,q;

	OPRD = init();       //初始化
	OPTR = init();      //初始化
	push(OPTR,'#');
	
	printf("请输入表达式：");
	ch = getchar();

	while(ch != '#')
	{
		if(judge(ch) == 0)            //ch为操作数
		{
			x = ch-'0';
			ch = getchar();
			while((judge(ch) == 0) && (ch != '#')) //ch不止是个位数
			{
				if(ch == '.') //ch为小数
				{
					m = -1;
                     	 		ch = getchar();
loop: 					x = x+(ch-'0')*pow(10,m);
					m--;
					ch = getchar();
					if(judge(ch) == 0 && ch != '#')
					{
						goto loop;
					}
				}
				if(judge(ch) == 0 && ch != '#')         //ch不是小数
				{
					x = x*10 + (ch-'0');
					ch = getchar();
					continue;
				}
			}
			push(OPRD,x);           //压栈
		}
		if(judge(ch)!= 0 && ch != '#')       //ch为运算符
		{      
			p = judge(ch);
			q = judge((char)OPTR->data[OPTR->top]);
			
			if( p>q && ch !=')' )   //ch优先级大于栈顶字符
			{
				push(OPTR,(double)ch);
				ch = getchar();
			}
			if(ch == ')')            //ch为‘）’
			{
				while((char)(OPTR->data[OPTR->top]) != '(')
				{
					pop(OPRD, &a);
					pop(OPRD, &b);
					pop(OPTR, &c);
					x = calculate(a,b,c);
					push(OPRD,x);
				}
				pop(OPTR,&(OPTR->data[OPTR->top]));
				ch = getchar();
			}
			if( (p<=q) && (ch!=')')) //ch优先级小于等于栈顶字符
			{
				if( (char)(OPTR->data[OPTR->top]) == '(')
				{
					push(OPTR,(double)ch);
					ch = getchar();
				}
				else
				{
					pop(OPRD,&a);
					pop(OPRD,&b);
					pop(OPTR,&c);
					x = calculate(a,b,c);
					push(OPRD,x);
					push(OPTR,(double)ch);
					ch = getchar();
				}
			}				
		}
	}
	
	while((char)OPTR->data[OPTR->top]  != '#')  //最后的计算
	{
		pop(OPRD,&a);
		pop(OPRD,&b);
		pop(OPTR,&c);
		x = calculate(a,b,c);
		push(OPRD,x);
	}

	printf("计算结果为：%.2f\n",OPRD->data[OPRD->top]);

	return EXIT_SUCCESS;
}

