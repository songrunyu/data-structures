/*
 * =====================================================================================
 *
 *       Filename:  KMP.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月04日 20时02分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  Software Engineering 1203
 *
 * =====================================================================================
 */


#include <stdio.h>

void get_next(char *T,int *next,int m)
{
	int j=1,k=0;
	next[1]=0;
	while(j <= m )
	{
		if(k==0 || T[k]==T[j])
		{
			++j;
			++k;
			next[j]=k;
		}
		else
		{
			k=next[k];
		}
	}
}

int Index_KMP(char *S, int pos, char *T, int n, int m)
{
	int i=pos,j=1;        //主串从第pos开始，模式串从头开始
	int next[m];
	get_next(T, next, m);
	while(i <= n && j <= m)
	{
		if(j==0 || S[i]==T[j])
		{
			++i;
			++j;
		}
		else
		{
			j=next[j];
		}
	}
	if(j > m)
	      return i-m;
	else 
	      return 0;
}

int main(int argc, char *argv[])
{
	int i,n,m,t;

	printf("Please input the length of S:\n");
	scanf("%d",&n);
	getchar();
	char S[n];
	printf("Please input S:\n");
	for(i=1; i<=n; i++)
	      scanf("%c",&S[i]);
	getchar();

	printf("Please input the length of T:\n");
	scanf("%d",&m);
	getchar();
	char T[m];
	printf("Please input T:\n");
	for(i=1; i<=m; i++)
	      scanf("%c",&T[i]);

	t = Index_KMP(S,1,T,n,m);
	if(t)
	{
		printf("匹配到的第一个字符的位置是：%d\n",t);
	}
	else
	{
		printf("匹配失败！\n");
	}
	return 0;

}

