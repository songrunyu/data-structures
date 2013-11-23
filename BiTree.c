/*
 * =====================================================================================
 *
 *       Filename:  HuffmanTree.c
 *
 *    Description:  二叉树的各种遍历方法
 *
 *        Version:  1.0
 *        Created:  2013年11月21日 18时08分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  Software Engineering 1203
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	char data;
	struct Node *Lchild;
	struct Node *Rchild;
}BiTree;

typedef struct stack
{
	BiTree *data;
	struct stack *next;
}StackNode;

typedef struct queue
{
	BiTree *data;
	struct queue *next;
}Queue;

typedef struct
{
	Queue *front;
	Queue *rear;
}QueueNode;


StackNode *InitStack()
{
	StackNode *top;
	if((top = (StackNode *)malloc(sizeof(StackNode))) == NULL)
	{
		printf("申请链栈空间失败！\n");
		exit(0);
	}
	top->next = NULL;
	return top;
}

void push(StackNode *top, BiTree *p)
{
	StackNode *s;
	if((s = (StackNode *)malloc(sizeof(StackNode))) == NULL)
	{
		printf("申请链栈空间失败！\n");
		exit(0);
	}
	s->data = p;
	s->next = top->next;
	top->next = s;
}

void pop(StackNode *top, BiTree **p)
{
	StackNode *s;
	if(top->next == NULL)
	{
		printf("栈空，不得出栈！\n");
		exit(0);
	}
	s = top->next;
	top->next = s->next;
	*p = s->data;
	free(s);
}

BiTree * GetTop(StackNode *top, BiTree *s)
{
	if(top->next == NULL)
	{
		printf("栈空，不得出栈！\n");
		exit(0);
	}
	s = top->next->data;
	return s;
}

QueueNode *Initqueue()
{
	QueueNode *q;
	Queue *p;

	q = (QueueNode *)malloc(sizeof(QueueNode));
	p = (Queue *)malloc(sizeof(Queue));
	p->next = NULL;
	q->front = q->rear = p;
	return q;
}

void Inqueue(QueueNode *q,BiTree *data)
{
	Queue *p;
	p = (Queue *)malloc(sizeof(Queue));
	p->data = data;
	p->next = NULL;
	q->rear->next = p;
	q->rear = p;
}

void Outqueue(QueueNode *q, BiTree **data)
{
	Queue *p;
	if(q->front == q->rear)
	{
		printf("队空，不得出队!\n");
		exit(0);
	}
	else
	{
		p = q->front->next;
		q->front->next = p->next;
		*data = p->data;
		free(p);
		if(q->front->next == NULL)
		{
			q->rear = q->front;
		}
	}
}

/*
void CreateBiTree(BiTree * root)
{
	char ch;
	ch = getchar();
	if(ch == '^')
	      root = NULL;
	else
	{
		root = (BiTree *)malloc(sizeof(BiTree));
		root->data = ch;
		CreateBiTree(root->Lchild);
		CreateBiTree(root->Rchild);	
	}
}
*/

BiTree  *CreateBiTree()          //创建静态二叉链表
{
	char ch;
	BiTree *root;
	ch = getchar();
	if(ch == '^')
	      root = NULL;
	else
	{
		root = (BiTree *)malloc(sizeof(BiTree));
		root->data = ch;
	       	root->Lchild = CreateBiTree();
		root->Rchild = CreateBiTree();	
	}
	return root;
}

void PreOrder(BiTree *root)             //递归先序遍历
{
	if(root)
	{
		printf("%c ",root->data);
		PreOrder(root->Lchild);
		PreOrder(root->Rchild);
	}
}

void InOrder(BiTree *root)              //递归中序遍历
{
	if(root)
	{
		InOrder(root->Lchild);
		printf("%c ",root->data);
		InOrder(root->Rchild);
	}
}

void PostOrder(BiTree *root)             //递归后序遍历
{
	if(root)
	{
		PostOrder(root->Lchild);
		PostOrder(root->Rchild);
		printf("%c ",root->data);
	}
}


void Un_PreOrder(BiTree *root)         //非递归先序遍历
{
	StackNode *top;
	BiTree *p;
	top = InitStack();
	p = root;
	while(p != NULL || top->next != NULL)
	{
		while(p != NULL)
		{
			printf("%c ",p->data);
			push(top,p);
			p = p->Lchild;
		}
		if(top->next != NULL)
		{
			pop(top,&p);
			p = p->Rchild;
		}
	}
}

void Un_InOrder(BiTree *root)            //非递归中序遍历
{
	StackNode *top;
	BiTree *p;
	top = InitStack();
	p = root;
	while(p != NULL || top->next != NULL)
	{
		while(p != NULL)
		{
			push(top,p);
			p = p->Lchild;
		}
		if(top->next != NULL)
		{
			pop(top,&p);
			printf("%c ",p->data);
			p = p->Rchild;
		}
	}
}

void Un_PostOrder(BiTree *root)         //非递归后序遍历
{
	StackNode *top;
	BiTree *p,*q;
	top = InitStack();
	p = root;
	q = NULL;
	while(p != NULL || top->next != NULL)
	{
		while(p != NULL)
		{
			push(top,p);
			p = p->Lchild;
		}
		if(top->next != NULL)
		{
			p = GetTop(top,p);
			if((p->Rchild == NULL) || (p->Rchild == q))
			{
				pop(top,&p);
				printf("%c ",p->data);
				q = p;
				p = NULL;
			}
			else
			{
				p = p->Rchild;
			}
		}
	}
}

void LevelOrder(BiTree *root)             //层次遍历
{
	QueueNode *p;
	BiTree *s;
	p = Initqueue();
	Inqueue(p , root);
	while(p->front != p->rear )
	{
		Outqueue(p, &s);
		printf("%c ",s->data);
		if(s->Lchild != NULL)
		{
			Inqueue(p, s->Lchild);
		}
		if(s->Rchild != NULL)
		{
			Inqueue(p,s->Rchild);
		}
	}

}

int main(int argc, char *argv[])
{
	BiTree *root;
	root = CreateBiTree();
	printf("递归先序遍历：");
	PreOrder(root);
	printf("\n");
	printf("递归中序遍历：");
	InOrder(root);
	printf("\n");
	printf("递归后序遍历：");
	PostOrder(root);
	printf("\n");
	printf("非递归先序遍历：");
	Un_PreOrder(root);
	printf("\n");
	printf("非递归中序遍历：");
	Un_InOrder(root);
	printf("\n");
	printf("非递归后序遍历：");
	Un_PostOrder(root);
	printf("\n");
	printf("层次遍历:");
	LevelOrder(root);
	printf("\n");


	return EXIT_SUCCESS;
}

