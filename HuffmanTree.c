/*
 * =====================================================================================
 *
 *       Filename:  HuffmanTree.c
 *
 *    Description:  哈夫曼编码
 *
 *        Version:  1.0
 *        Created:  2013年11月10日 21时00分29秒
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

//#define N 10
//#define M 2*N-1
#define Max 256

typedef struct                    //哈夫曼树中的每个结点
{
	int weight;
	int parent,Lchild,Rchild;
}HTNode,HuffmanTree[100];

typedef struct                    //存储每个字符及其相对应的编码
{
	char ch;
	char number[10];
}CodeNode;

typedef struct Stacknode         
{
	char num;
	struct Stacknode *next;
}Stack;

int push(Stack *top, char x)         //压栈
{
	Stack *p;
	if((p = (Stack *)malloc(sizeof(Stack))) == NULL)
	{
		printf("链栈申请空间失败！\n");
		return 1;
	}
	p->num = x;
	p->next = top->next;
	top->next = p;
	return 0;
}

char pop(Stack *top)             //出栈
{
	Stack *p;
	char x;
	if(top->next == NULL)
	{
		printf("栈空不得出栈！\n");
		return 0;
	}
	p = top->next;
	top->next = p->next;
	x = p->num;
	free(p);
	return x;
}

int get_w( char data[], int w[], CodeNode htcode[])  //求每个结点的权值weight ， 并记录叶子结点的字符
{
	int i,j=1;
	int *s = NULL;
	s = (int *)malloc(sizeof(int)*Max);

	for(i=0; i<Max; i++)
	{
		s[i]='\0';
	}
	for(i=0; data[i]!='\0'; i++)
	{
		s[(unsigned char)data[i]]++;        //将字符中每个字符的ASCII值作为数组的下标
	}
	for(i=0; i<Max; i++)
	{
		if(s[i] != 0)
		{
			w[j++] = s[i];              
			htcode[j-1].ch = (char)i;
		}
	}
	free(s);
	return (j-1);
}

void select_min(HuffmanTree ht, int m, int *s1, int *s2)  //求出权值最小的两个结点的下标
{
	int i,j=1,temp[m];
	for(i=1; i<=m; i++)              //将父母结点为0，即还未被选用过的结点的下标存入temp数组
	{
		if(ht[i].parent == 0)
		{
			temp[j++] = i;
		}
	}
	if(ht[ temp[1] ].weight < ht[ temp[2] ].weight)
	{
		*s1 = temp[1];
		*s2 = temp[2];
	}
	else
	{
		*s1 = temp[2];
		*s2 = temp[1];
	}
	for(i=3; i<=j-1; i++)
	{
		if(ht[temp[i]].weight <= ht[*s2].weight)
		{
			if(ht[temp[i]].weight <= ht[*s1].weight)
			{
				*s2 = *s1;
				*s1 = temp[i];
			}
			else
			{
				*s2 = temp[i];
			}
		}
	}
}

void CrtHuffmanTree(HuffmanTree ht, int w[],int n)  //创建huffmantree（三叉静态链表）
{
	int m = 2*n-1;
	int i,s1,s2;
	for(i=1; i<=n; i++)
	{
		ht[i].weight = w[i];
		ht[i].parent = 0;
		ht[i].Lchild = 0;
		ht[i].Rchild = 0;
	}
	for(; i<=m; i++)
	{
		ht[i].weight = 0;
		ht[i].parent = 0;
		ht[i].Lchild = 0;
		ht[i].Rchild = 0;
	}
	for(i=n+1; i<=m; i++)
	{
		select_min(ht,i-1,&s1,&s2);
		ht[i].weight = ht[s1].weight + ht[s2].weight;
		ht[i].Lchild = s1;
		ht[i].Rchild = s2;
		ht[s1].parent= i;
		ht[s2].parent= i;
	}
}

void save_code(char data[], CodeNode htcode[], int n)
{
	FILE *fp;
	int i,j;
	if((fp = fopen("2.txt","wt")) == NULL)
	{
		printf("打开文件2.txt出错！");
		exit(1);
	}
	for(i=0; i<strlen(data); i++)
	{
		j=1;
		while(j <= n)
		{
			if(data[i] == htcode[j++].ch)
			{
				fprintf(fp,"%s",htcode[j-1].number);
			}
		}
	}
        printf("哈夫曼编码已保存到2.txt\n");
	fclose(fp);
}

void read_code(char code[])
{
	FILE *fp;
	int i=0;
	if((fp = fopen("2.txt","rt")) == NULL)
	{
		printf("读取文件2.txt出错！\n");
		exit(1);
	}
	while(feof(fp) != 1)
	{
		fscanf(fp, "%c", &code[i++]);
	}
	fclose(fp);
}

void CrtHuffmancode(HuffmanTree ht,int n, CodeNode htcode[])    //求哈夫曼编码
{
	Stack *top;
	char temp[n];
	top = (Stack *)malloc(sizeof(Stack));
	top->next = NULL;
	int i,j,c,p;
	for(i=1; i<=n; i++)
	{
		j = 0;
		c = i;
		p = ht[i].parent;
		while(p != 0)
		{
			if(ht[p].Lchild == c)
			      push(top, '0');
			else
			      push(top, '1');
			c = p;
			p = ht[p].parent;
		}
		while(top->next != NULL)
		{
			temp[j++] = pop(top);
		}	
		temp[j] = '\0';
		strcpy(htcode[i].number, temp);
		printf("%c : ",htcode[i].ch);
		puts(htcode[i].number);
//		save_code(htcode[i].number);
	}
}

void save_data()
{
	FILE *fp;
	int i;
	char data[100];
	if((fp = fopen("1.txt","wt")) == NULL)
	{
		printf("打开文件1.txt出错！");
		exit(1);
	}
	printf("请输入内容：\n");
	scanf("%[^\n]",data);
	for(i=0; i<strlen(data); i++)
	{
		fprintf(fp,"%c",data[i]);
	}
	printf("字符已保存到1.txt\n\n");
	fclose(fp);
}

void read_data(char data[])
{
	FILE *fp;
	int i=0;
	if((fp = fopen("1.txt","rt")) == NULL)
	{
		printf("读取文件1.txt出错！\n");
		exit(1);
	}
	while(feof(fp) != 1)
	{
		fscanf(fp, "%c", &data[i++]);
	}
	fclose(fp);
}

void save_char(char ch)
{
	FILE *fp;
	int i;
	if((fp = fopen("3.txt","a+")) == NULL)
	{
		printf("打开文件3.txt出错！");
		exit(1);
	}
	fprintf(fp,"%c",ch);
	fclose(fp);
}


void translate_code(HuffmanTree ht, char code[],int n, CodeNode htcode[])    //将编码翻译成字符
{
	int i,j,m,k,flag;
	printf("\n");
	for(i=0; i<strlen(code); i++)
	{
		m = i;
		for(j=1; j<=n; j++)
		{
			k = 0;
			flag = 1;
			while(k < strlen(htcode[j].number))
			{
				if(code[i++] == htcode[j].number[k++] )
				{
					flag = 0;
					continue;
				}
				else
				{
					flag = 1;
					i = m;
				      	break;

				}
			}
			if(k == strlen(htcode[j].number) && flag == 0)
			{
				save_char(htcode[j].ch);
				i--;
				break;
			}
		}
	}
	printf("\n");

}

int main(int argc, char *argv[])
{
	int n,i,j;
	char data[100];
	int  w[100];
	char code[100];
	memset(code, '\0', 100);
	CodeNode htcode[100];
	HuffmanTree ht;

	save_data();    
	memset(data, '\0', 100);
	read_data(data);
	n = get_w(data, w, htcode);
	CrtHuffmanTree(ht, w, n);
	CrtHuffmancode(ht, n, htcode);
	save_code(data, htcode, n);
	read_code(code);
	translate_code(ht, code, n, htcode);
	printf("编码已翻译，保存到3.txt\n");

        return 0;	
}

