/*
 * =====================================================================================
 *
 *       Filename:  horse_chess.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年09月29日 11时06分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


#include<stdio.h>
#include<stdlib.h>

#define STACK_SIZE 100
#define STACKINCREMENT 10
#define N 8

int weight[N][N];         //各点的权值
int Board[N][N][8];       //各点按权值升序存放的待走方向

typedef struct  //位置（点）
{        
	int x;
        int y;

}PosType;

typedef struct         //栈的元素
{        
	int ord;
        PosType seat;     //点
	int di;           //方向

}ElemType;

typedef struct    //栈
{        
	ElemType *base;
        ElemType *top;
	int stacksize;

}SqStack;

SqStack s;         //全局栈

int InitStack()        //初始化栈
{        
	s.base = (ElemType *)malloc(STACK_SIZE * sizeof(ElemType));
	if(!s.base)  
	      return 0;
	s.top = s.base;
	s.stacksize = STACK_SIZE;

        return 1;
}

ElemType GetTop()    //取栈顶
{        
	if(s.top == s.base)
        	exit(0);

        return *(s.top-1);

}

void Push(ElemType elem)     //入栈
{        
	*s.top++ = elem;
}

int Pop(ElemType *elem)      //出栈
{        
	if(s.top == s.base)       
	      return 0;
	*elem = *--s.top;

        return 1;
}

int StackEmpty()          //判空栈
{        
	if(s.top == s.base)       
	      return 1;
        else                    
	      return 0;         
}

void OutputPath()     //输出
{        
	int i,f,k;
	int path[N][N];	
	SqStack s1 = s;

	for(i=0; s1.top!=s1.base; i++)
        {        
		path[(*s1.base).seat.x][(*s1.base).seat.y] = i+1;
		++s1.base;
	}

        for(f=0; f<N; f++)
	{        
		printf("\n");
	        for(k=0; k<N; k++)        
		      printf("\t%d",(path[f][k]));
	}
	printf("\n");
}

int Pass(PosType curpos)   //判断当前位置是否合法
{        
	SqStack s1 = s;

        if(curpos.x<0 || curpos.x>(N-1) || curpos.y<0 || curpos.y>(N-1))     //坐标超出坐标系
	      return 0;

        for( ; s1.top!=s1.base; )        
        {         
		--s1.top;
	        if(curpos.x==(*s1.top).seat.x && curpos.y == (*s1.top).seat.y)      
		      return 0;
	}

        return 1;
}

PosType NextPos(PosType curpos,int direction)   //8个候选方向
{        
	switch(direction)
        { 
		case 1: curpos.x += 1;
			curpos.y -= 2; 
			break;
		case 2: curpos.x += 2;
			curpos.y -= 1; 
			break;
		case 3: curpos.x += 2;
			curpos.y += 1; 
			break;
		case 4: curpos.x += 1;
			curpos.y += 2; 
			break;
		case 5: curpos.x -= 1;
			curpos.y += 2; 
			break;
		case 6: curpos.x -= 2;
			curpos.y += 1; 
			break;
		case 7: curpos.x -= 2;
			curpos.y -= 1; 
			break;
		case 8: curpos.x -= 1;
			curpos.y -= 2; 
			break;
	 }

          return curpos;   //返回当前位置的下一个位置的坐标
}


void setweight()   //求各点权值
{
	int i,j,k;
	PosType m;  //点
	ElemType elem;//栈元素
 
	for(i=0; i<N; i++)       //遍历棋盘上每个点
	{
		for(j=0; j<N; j++)
          	{
			elem.seat.x = i;
			elem.seat.y = j;
			weight[i][j]= 0;        //每次初始化权
	  
			for(k=0; k<8; k++)      //从1到8判断这个点共有几个方向可走
	             	{
		 		m = NextPos(elem.seat, k+1);
		                if(m.x>=0 && m.x<N && m.y>=0 && m.y<N)		   
		                	weight[i][j]++;    //存入当前位置有几个方向
		    	}
	        }
	}   
}

void setmap()  //各点8个方向按权值递增排列
{ 
	int a[8];
	int i,j,k,m,min,s,h;
	PosType n1,n2;

   	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
         	{	 
	          	for(h=0; h<8; h++) //用数组a[8]记录当前位置的下一个位置的可行路径条数
	          	{		  
		        	n2.x = i;
		              	n2.y = j;		  
		                n1 = NextPos(n2,h+1);		  
		              	if(n1.x>=0 && n1.x<N && n1.y>=0 && n1.y<N)      
				      a[h] = weight[n1.x][n1.y];   //存入当前位置的下一个点（即NextPos返回的新点）有几个可选的方向
		              	else   
				      a[h]=0;		  
		        }
        		for(m=0; m<8; m++)//对方向索引权值升序排列存入Borad[N][N][8]，不能到达的方向排在最后
	               	{
				min = 9;		       
		                for(k=0; k<8; k++)//循环一次，找出一个权值最小的存入Borad数组
		                	if(min>a[k])		       
		                  	{         
						min = a[k];  //min存入当前位置的所有下一个点中可选方向最少的点的权值
				                Board[i][j][m] = k;//将权值最小点的下标值存入Borad中
				                s = k;
				        }
		                a[s] = 9;//每找出一个最小权值的点，将该点的权值改为9,以便下次循环不再找出它
		        }  	      
		}  	     
	}
}

int HorsePath(PosType start)  //马走过的路径
{
   	PosType curpos;
   	int horsestep = 0, off;
   	ElemType  elem;  // 一个栈的元素
   	curpos = start;
   	do
	{
   		if(Pass(curpos))       //如果当前位置合法
           	{
	        	horsestep++;
	               	elem.di = 0; 
	               	elem.ord = horsestep;
	               	elem.seat = curpos;
	               	Push(elem);         //入栈
	               	if(N*N == horsestep)  //马走过的步数等于棋盘大小时退出      
			      return 1;
	               	off = Board[elem.seat.x][elem.seat.y][elem.di]+1;
	               	curpos = NextPos(elem.seat,off);    //取得下一个坐标点
	        }
           	else                  //当前位置不合法
	   	{
	        	if(!StackEmpty())   //栈s非空
	                {
				while(!StackEmpty() && elem.di==8)
				{
					Pop(&elem);
               					if(!StackEmpty()) //判断弹出后是否为空
					{
						elem = GetTop();
						horsestep = elem.ord;
					}
				}   
				if(!StackEmpty() && elem.di<8)
				{
					Pop(&elem);
					off = Board[elem.seat.x][elem.seat.y][++elem.di];
					curpos = NextPos(elem.seat,off+1);
					Push(elem);
				}
			}
		}
	}while(!StackEmpty());
	printf("走不通！\n");
	return 0;
}

void  main()
{
	PosType start;    //一个起始位置点
     	InitStack();      //初始化栈
     	printf("输入起始位置（0-7）X:");
	scanf("%d",&start.x);
	printf("Y:"); 
	scanf("%d",&start.y);
	setweight();     //求出每个点的权值
	setmap();        //按权值大小排序
	HorsePath(start);
	OutputPath();
}

