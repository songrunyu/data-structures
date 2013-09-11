/*
 * =====================================================================================
 *
 *       Filename:  polynomial.c
 *
 *    Description:  多项式的运算
 *
 *        Version:  1.0
 *        Created:  2013年09月08日 17时46分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void choice(); 
void go_choice();

typedef struct polynomial
{
	int 			coef;           //系数
	int 			expn;           //指数
	struct polynomial 	*next;          //指针域
}LNode;

void choice_derivation(LNode *head1);
LNode *creat_poly()
{
	LNode *head,*p,*s;
	int   c,e;
	head = (LNode *)malloc(sizeof(LNode));
	p = head;
	printf("请输入多项式的系数与指数(输入“0 0”时结束):\n");
	scanf("%d %d",&c,&e);
	while(c != 0)
	{
		s = (LNode *)malloc(sizeof(LNode));
		s->coef = c;
		s->expn = e;
		p->next = s;
		p = s;
		scanf("%d %d",&c,&e);
	}
	p->next = NULL;

	return(head);
}

void output(LNode *head)
{
	LNode *p = head->next;
	int flag = 1;
	if(p == 0)
	{
		printf("0\n");
		return;
	}
	while(p != NULL)
	{
		if(p->coef == 0 )
		{
			goto loop;
		}
		if(p->coef > 0 && flag != 1)
			putchar('+');
		if(p->coef != 1 && p->coef != -1)
		{
			printf("%d",p->coef);
			if(p->expn == 1)
				putchar('X');
			else if(p->expn == 0)
				;
			else
				printf("X^%d",p->expn);
			
		}
		else
		{
			if(p->coef == 1)
			{
				if(p->expn == 1)
					putchar('X');
				else if(p->expn == 0)
				      printf("%d",p->coef);
				else
					printf("X^%d",p->expn);
			}
			if(p->coef == -1)
			{
				if(p->expn == 1)
					printf("-X");
				else if(p->expn == 0)
				      printf("%d",p->coef);
				else
					printf("-X^%d",p->expn);
			}
		}
		flag++;
loop:		p = p->next;
	}
	printf("\n");
}

void choice_add(LNode *head1,LNode *head2)
{
	LNode *head3;
	char judge;
	int  m;

	head3 = (LNode *)malloc(sizeof(LNode));
	LNode *p = head1->next;
	LNode *q = head2->next;
	LNode *s,*r = head3;

	while(p != NULL && q != NULL)          
	{
	
		s = (LNode *)malloc(sizeof(LNode));
	 	r->next = s;
	
	 	if(p->expn < q->expn) 	
		{
			s->coef = p->coef;
			s->expn = p->expn;
			p = p->next;
		}
		else if(p->expn > q->expn)
		{
			s->coef = q->coef;
			s->expn = q->expn;
			q = q->next;
		}
		else if(p->expn == q->expn)
		{
			s->coef = p->coef + q->coef;
			s->expn = p->expn;
			p = p->next;
			q = q->next;
		}

		r = r->next;
		
	}
	
	if(p == NULL)
	{
		while(q != NULL)
		{
			r->next = q;
			q = q->next;
			r = r->next;
		}
	}

	else if(q == NULL)
	{
	
		while(p != NULL)
		{
			r->next = p;
			p = p->next;
			r = r->next;
		}
	}

	printf("pa+pb=");
	output(head3);

	go_choice(head1,head2);

}

void choice_subtraction(LNode *head1,LNode *head2)
{
	LNode *head3;

	LNode *p = head2;
	LNode *q = head2->next;

	while(q)
	{
		q->coef*=-1;
		q = q->next;
	}
	
	choice_add(head1,p);
	
	printf("pa-pb=");
	output(head3);

	go_choice(head1,head2);

}

void choice_work(LNode *head1)
{
	int x,y=0;
	char judge;
	LNode *p = head1->next;

	printf("请输入X：");
	scanf("%d",&x);

	while(p)
	{
		y += p->coef*pow(x,p->expn);
		p = p->next;
	}

	printf("p=%d\n",y);

	printf("\n\n是否继续求导(y or n)：\n");
	getchar();
	scanf("%c",&judge);
	if(judge == 'y'||judge == 'Y')
	{
		printf("pa=");
	        output(head1);
    		choice_derivation(head1);
	}
	else
	{
		choice();
	}
}

void choice_derivation(LNode *head1)
{
	LNode *p = head1->next;
	char judge;

	while(p)
	{
		p->coef = p->coef*p->expn;
		p->expn -= 1;
		p = p->next;

	}

	printf("p'=");
	output(head1);

	printf("\n\n是否继续求值(y or n)\n");
	getchar();
	scanf("%c",&judge);
	if(judge == 'y'|| judge == 'Y')
	{
		printf("pa=");
	       	output(head1);
	       	choice_work(head1);
	}
	else 
	{
		choice();
	}

}

void choice_multiply(LNode *head1,LNode *head2)
{
	LNode *p = head1->next;
	LNode *q = head2->next;
	LNode *head3,*s,*r;

	head3 = (LNode *)malloc(sizeof(LNode));
	r = head3;

	while(p)
	{
		while(q)
		{
			s = (LNode *)malloc(sizeof(LNode));
			s->coef = p->coef * q->coef;
			s->expn = p->expn + q->expn;
			r->next= s;
			r = s;
			q = q->next;
		}
		p = p->next;
		q = head2->next;

	}
	r->next = NULL;

	printf("pa*pb=");
	output(head3);

	go_choice(head1,head2);

}

int login()
{
	int choice;

	printf("*******************************\n");
	printf("\n");
	printf("1.加法\n");
	printf("2.减法\n");
	printf("3.相乘\n");
	printf("4.求值\n");
	printf("5.求导\n");
	printf("6.退出\n");
	printf("\n\n请输入你的选项:");
	scanf("%d",&choice);

	return choice;
}

void choice()
{
	int choice;
	LNode *head1,*head2;

	choice = login();

	switch(choice)
	{
		case 1:
		       head1 = creat_poly();
		       head2 = creat_poly();
		       printf("pa=");
		       output(head1);
		       printf("pb=");
		       output(head2);
		       choice_add(head1,head2);
		       break;
		case 2:
		       head1 = creat_poly();
		       head2 = creat_poly();
		       printf("pa=");
		       output(head1);
		       printf("pb=");
		       output(head2);
		       choice_subtraction(head1,head2);
		       break;
		case 3:
		       head1 = creat_poly();
		       head2 = creat_poly();
		       printf("pa=");
		       output(head1);
		       printf("pb=");
		       output(head2);
		       choice_multiply(head1,head2);
		       break;
		case 4:
		       head1 = creat_poly();
		       printf("pa=");
		       output(head1);
		       choice_work(head1);
		       break;
		case 5:
		       head1 = creat_poly();
		       printf("pa=");
		       output(head1);
		       choice_derivation(head1);
		       break;
		case 6:exit(0);
		       
		       
	}
}

void go_choice(LNode *head1,LNode *head2)
{
	char judge;
	int m;

	printf("是否继续使用pa，pb(y or n)：");
	getchar();
	scanf("%c",&judge);
	if(judge == 'y' || judge == 'Y')
	{
		printf("*******************\n");
		printf("\n1.相加\n");
		printf("2.相减\n");
		printf("3.相乘\n");
		printf("\n\n请输入你的选择：");
		scanf("%d",&m);
		switch(m)
		{
			case 1:
				printf("pa=");
		       		output(head1);
		       		printf("pb=");
		       		output(head2);
		       		choice_add(head1,head2);
		      	 	break;
			case 2:
				printf("pa=");
		       		output(head1);
		       		printf("pb=");
		       		output(head2);
		       		choice_subtraction(head1,head2);
		       		break;
			case 3:
				printf("pa=");
		       		output(head1);
		       		printf("pb=");
		       		output(head2);
		       		choice_multiply(head1,head2);
		      	 	break;

		}
	}
	else
	{
		choice();
	}
}


int main(int argc, char *argv[])
{

	choice();

	return EXIT_SUCCESS;
}

