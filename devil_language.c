/*
 * =====================================================================================
 *
 *       Filename:  devil_language.c
 *
 *    Description:  魔王语言(链队列)
 *
 *        Version:  1.0
 *        Created:  2013年09月27日 16时02分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 26

struct tra   //由大写转小写字符        
{
	char *CH;
	char *ch;
	struct tra *next;
	
}tra[MAX] = {{"A","abc"},{"B","bcd"},{"C","cde"},{"D","def"},{"E","efg"},{"F","fgh"},{"G","ghi"},{"H","hij"},{"I","ijk"},{"J","jkl"},{"K","klm"},{"L","lmn"},{"M","mno"},{"N","nop"},{"O","opq"},{"P","pqr"},{"Q","qrs"},{"R","rst"},{"S","stu"},{"T","tuv"},{"U","uvw"},{"V","vwx"},{"W","wxy"},{"X","xyz"},{"Y","yza"},{"Z","zab"}};


struct trans        //由小写转汉字     
{
	char ch;
	char *q;

}trans[MAX]= {{'a',"爱"},{'b',"吧"},{'c',"猜"},{'d',"的"},{'e',"恩"},{'f',"法"},{'g',"个"},{'h',"和"},{'i',"一"},{'j',"家"},{'k',"看"},{'l',"了"},{'m',"没"},{'n',"你"},{'o',"哦"},{'p',"跑"},{'q',"去"},{'r',"人"},{'s',"说"},{'t',"天"},{'u',"有"},{'v',"玩"},{'w',"我"},{'x',"小"},{'y',"一"},{'z',"在"}};


typedef struct translate //存储魔王语言的结点类型     
{
	char r[100];
	struct translate *next;

}TRANS;


typedef struct       //存储魔王语言的链队列的头尾指针    
{
	TRANS *front;
	TRANS *rear;

}Lnode;


Lnode *init_Lnode()        //初始化存储魔王语言的队列         
{
	TRANS *p;
	Lnode *q;

	p = (TRANS *)malloc(sizeof(TRANS));
	q = (Lnode *)malloc(sizeof(Lnode));

	p->next = NULL;
	q->front = q->rear = p;

	return q;

}


Lnode *push(Lnode *q, char *x)   //入队 (只更改rear的值)        
{
	TRANS *s;
	s = (TRANS *)malloc(sizeof(TRANS));
	strcpy(s->r, x);
	s->next = NULL;
	q->rear->next = s;
	q->rear = s;

	return q;

}


int pop(Lnode *q, char *x)    //出队(只更改front的值)            
{
	TRANS *p;
	if(q->front == q->rear)
	{
		printf("队空，不得出队\n");
		return 0;
	}
	else
	{
		p = q->front->next;
		q->front->next = p->next;
		strcpy(x,p->r);
		free(p);
		if(q->front->next == NULL) 
		{
			q->rear = q->front;
			return 0;
		}
	}
}


Lnode *change1(Lnode *head_q )        //change1:大写转小写字符      
{
	int i;
	TRANS *q = head_q->front->next;

	while(q != NULL)
	{
		i = 0;
		while(i < MAX)
		{
			if(strcmp(q->r, tra[i].CH) == 0)
			{
				strcpy(q->r,tra[i].ch);
				break;
			}
			else 
				i++;

		}
		if(i == MAX && (q->r >="a"||q->r <=  "z" ))           //如果输入的字符为小写字母，一样存入
		{
			strcpy(q->r,q->r);
		}
		else if(i == MAX)                                     //如果输入字符既非小写又非大写字符，则提示输入有误
		{
			printf("您的输入包含不合法字符！\n");
			exit(0);
		}
		q = q->next;
	}
	return head_q;
}


Lnode *change2(Lnode *head_q)                      //change2:小写转汉字
{
	int i,j=0,len;
	TRANS *q = head_q->front->next;

	printf("翻译后的语句为：");
	while(q != NULL)
	{
		len = strlen(q->r);
		for(j=0; j<len; j++)
		{
			for(i=0; i<MAX; i++)
			{
				if(trans[i].ch == q->r[j])
					printf("%s",trans[i].q);
			}			
		}
		q = q->next;
	}
	printf("\n");
	return head_q;
}


void print(Lnode *q)      //输出
{
	TRANS *p;
	p = q->front->next;
	while(p != NULL)
	{
		printf("%s",p->r);
		p = p->next;
	}
	printf("\n");
}


int main(int argc, char *argv[])
{
	char ch;
	Lnode *head_q;
	
	head_q = init_Lnode();

	printf("请输入魔王语言(输入#结束)：");
	ch = getchar();
	while(ch != '#')
	{
		head_q = push(head_q,&ch);
		ch = getchar();
	}
	head_q = change1(head_q);
	printf("由大写转小写后的语言：");
	print(head_q);
	head_q = change2(head_q);

	return EXIT_SUCCESS;
}

