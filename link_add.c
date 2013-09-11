/*
 * =====================================================================================
 *
 *       Filename:  link_add.c
 *
 *    Description:  合并两个链表
 *
 *        Version:  1.0
 *        Created:  2013年09月08日 15时09分06秒
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

typedef struct node
{
	int 		data;
	struct node 	*next;
}LNode;

LNode *creat_linklist(int *m)                                
{
	LNode *head = (LNode *)malloc(sizeof(LNode));
	head->next = NULL;
	LNode *p = head,*s = head;
      	int x;
	*m = 0;
	scanf("%d",&x);
	while(x != 0)
	{
		s = (LNode *)malloc(sizeof(LNode));
		s->data = x;
		p->next = s;
		p = s;
		(*m)++;
		scanf("%d",&x);
	}
	p->next = NULL;

	return head;
}

LNode *creat_linklistC(LNode *L1, LNode *L2)
{
	LNode *head = (LNode *)malloc(sizeof(LNode));	
	LNode *p = head;
	L1 = L1->next;
	L2 = L2->next;
	while(L1 != NULL)
	{
		p->next = L1;
		L1 = L1->next;
		p = p->next;
		p->next = L2;
		L2 = L2->next;
		p = p->next;		
	}
	while(L2 != NULL)
	{
		p->next = L2;
		p = p->next;
		L2 = L2->next;
	}

	return head;
}

void output_linklist(LNode *p)
{
	while(p->next != NULL)
	{
		p = p->next;
		printf("  %d",p->data);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{

	LNode *head1,*head2,*head3;
	int len1,len2;

	printf("请输入链表A：\n");
	head1 = creat_linklist(&len1);
	printf("请输入链表B：\n");
	head2 = creat_linklist(&len2);
	printf("链表A表长为%d,链表B表长为%d\n",len1,len2);

	if(len1 <= len2)
	{
		head3 = creat_linklistC(head1, head2);
	}
	else
	{
		head3 = creat_linklistC(head2, head1);
	}

	output_linklist(head3);

	return EXIT_SUCCESS;
}

