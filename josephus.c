/*
 * =====================================================================================
 *
 *       Filename:  josephus.c
 *
 *    Description:  约瑟夫环(双向链表(因为有顺逆时针之分))
 *
 *        Version:  1.0
 *        Created:  2013年06月11日 20时34分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10

typedef struct person
{
	char 	name[20];
	int 	password;
	int 	flag;
	struct person *prior;
	struct person *next;
}Node;

void output(Node *head);

Node *create_link(int n)
{
	int i = 1;
	Node *head, *s, *p;

	srand( (int) time (NULL) );
	head = (Node *) malloc (sizeof(Node));
	printf("请输入姓名：");
	getchar();
	scanf("%[^\n]", head->name);
	head->password = rand()%N+1;
	head->flag = rand()%2;
	p = head;
	while(i < n)
	{
		s = (Node *) malloc (sizeof(Node));
		printf("请输入姓名：");
		getchar();
		scanf("%[^\n]",s->name);
		s->password = rand()%N+1;
		s->flag = rand()%2;

		p->next = s;
		s->prior = p;
		p = s;
		i++;
	}
	p->next = head;
	head->prior = p;
       	
	return head;
}

void output(Node *head)
{
	Node *p = head;
	printf("flag=0表示顺时针，flag=1表示逆时针\n");
	while(p->next != head)
	{
		puts(p->name);
		printf("password:%d\n",p->password);
		printf("flag:%d\n\n",p->flag);
		p = p->next;
	}
	puts(p->name);
	printf("password:%d\nflag:%d\n\n",p->password,p->flag);

}

void run(Node *head,Node *q,int n)
{
	int i=0;
	int password = q->password;
	int flag = q->flag;
	Node *s,*r;
	s = r = q;

	while(i<(n-1))
	{
		s = r;
		if(flag == 0)
		{
			r = s->next;
			while(password > 0)
			{
				s = s->next;
				password--;
			}
		}
		else
		{
			r = s->prior;
			while(password > 0)
			{
				s = s->prior;
				password--;
			}
		}
		flag = s->flag;
		password = s->password;
		printf("此次出局的是：%s\n",s->name);
		s->prior->next = s->next;
		s->next->prior = s->prior;
		free(s);
		i++;		
	}
	printf("最后留下的是: ");
	puts(r->name);

}


int main(int argc, char *argv[])
{

	int n,i = 0,flag = 1;
	char name[20];
	Node *head, *p;
	printf("请输入总人数：");
	scanf("%d",&n);

	head = create_link(n);

	printf("请输入第一位开始者的姓名\n");
	getchar();
	scanf("%[^\n]",name);
	p = head;

	do
	{
		while(i<n)
		{
			if(strcmp(p->name,name)==0)
			{  	
				flag = 0;
				break;
			}
			else
			{
				p = p->next;
				i++;
			}
		}
		if ( i == n)
		{
			printf("未找到该人，请重新输入：");
			getchar();
			scanf("%[^\n]",name);
			p = head;
			i = 0;
		}
	}while(flag == 1);

	run(head,p,n);

	return EXIT_SUCCESS;
}

