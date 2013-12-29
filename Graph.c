/**
 * =====================================================================================
 *
 *    @file Graph.c
 *
 *    @brief 一个校园导游系统，包括：创建，增加、删除结点路径，查询简单路径、最短路径等功能。
 *    
 *    @authour 宋润雨
 *
 *    @data 2013年12月16日
 *    
 *    
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXVEX 25
#define INFINITY 65535

int visited[MAXVEX] = {0};    //访问标志数组visited数组
int pre[MAXVEX] = {-1};       //记录前驱结点下标的数组
int Path[MAXVEX][MAXVEX];     //记录最短查找路径的数组
int D[MAXVEX][MAXVEX];        //记录最短路径权值的数组
int All_path[MAXVEX];         //记录当前路径的下标

//typedef char VertexType;    //顶点类型
//typedef int  EdgeType;      //边上权值的类型

/**
 * @brief 边表结点 
 */
typedef struct EdgeNode       
{
	int adjvex;            ///<下标值
	int weight;            ///<权值，即两点间的距离
	struct EdgeNode *next; ///<指针域
}EdgeNode;             

/**
 * @brief 顶点表结点
 */
typedef struct VertexNode     
{
	int num;             ///<地点序号
	char *name;          ///<地点名称
	char *introduce;     ///<边表首指针
	EdgeNode *firstedge;  
}VertexNode;

/**
 * @brief 邻接表
 */
typedef struct
{
	VertexNode AdjList[MAXVEX]; ///<顶点数组
	int numVertexes;            ///<顶点数
	int numEdges;               ///<边数
}GraphAdjList;

/**
 * @brief 链队列结点
 */
typedef struct queue
{
	int data;           ///<data域
	struct queue *next; ///<指针域
}Queue;

/**
 * @brief 将头尾指针封装在一起的链队
 */
typedef struct
{
	Queue *front;  ///<头指针
	Queue *rear;   ///<尾指针
}QueueNode;

/** 
 * @fn 初始化队列Q
 *
 * @return 返回一个带头结点的空队
 *
 */
QueueNode *InitQueue() 
{
	QueueNode *Q;
	Queue *p;

	Q = (QueueNode *)malloc(sizeof(QueueNode));
	///申请空间，并将头结点置空
	p = (Queue *)malloc(sizeof(Queue));
	p->next = NULL;
	Q->front = Q->rear = p;

	return Q;
}

/**
 * @fn 入队函数
 *
 * @param Q 队列的头指针
 * @param i 队列的结构体中的data值，即地点的下标值
 *
 */
void EnterQueue(QueueNode *Q,int i)
{
	Queue *p;
	p = (Queue *)malloc(sizeof(Queue));
	p->data = i;
	p->next = NULL;
	Q->rear->next = p;
	Q->rear = p;
}

/**
 * @fn 出队函数
 *
 * @param Q 队列的头指针
 * @param s 记录要删除的结点下标
 *
 */
void DeleteQueue(QueueNode *Q, int **s)
{
	*s = (int *)malloc(sizeof(int));
	Queue *p;
	/**< 判断队空 */
	if(Q->front == Q->rear)
	{
		printf("队空，不得出队!\n");
		exit(0);
	}
 	else
	{
		p = Q->front->next;
		Q->front->next = p->next;
		**s = p->data;
		free(p);
		if(Q->front->next == NULL)
		{
			Q->rear = Q->front;
		}
	}
}

/**
 * @fn 创建图的邻接表
 *
 * @return 返回该邻接表的头指针
 *
 */
GraphAdjList *CreatALGraph()                   
{
	int i,j,k,weight;
	EdgeNode *e;
	GraphAdjList *G;
	G = (GraphAdjList *)malloc(sizeof(GraphAdjList));

	///输入结点数和边数
	printf("Please input numVertexes and numEdges:\n");
	scanf("%d%d",&G->numVertexes,&G->numEdges);
	getchar();

	///输入每个结点的名称和介绍（下标从1开始）
	printf("Please input Vertexes(name,introduce):\n");
	for(i=1; i<=G->numVertexes; i++)                 
	{
		G->AdjList[i].name = (char *)malloc(sizeof(char));
		G->AdjList[i].num = i;
		printf("name:");
		scanf("%[^\n]",G->AdjList[i].name);
		getchar();

		G->AdjList[i].introduce = (char *)malloc(sizeof(char));
		printf("introduce:");
		scanf("%[^\n]",G->AdjList[i].introduce);
		getchar();
		G->AdjList[i].firstedge = NULL;
	}

	///输入连接每个边的两个结点的序号，以及边的权值
	for(k=1; k<=G->numEdges; k++)                      
	{
		printf("Please input the edge's num(vertexi,vertexj,weight):\n");
		scanf("%d%d%d",&i,&j,&weight);

		///每条边使用头插法插入
		e = (EdgeNode *)malloc(sizeof(EdgeNode));   
		e->adjvex = j;
		e->weight = weight;
		e->next = G->AdjList[i].firstedge;
		G->AdjList[i].firstedge = e; 

		///无向图，需要在两个结点的边表里同时插入
		e = (EdgeNode *)malloc(sizeof(EdgeNode));
		e->adjvex = i;
		e->weight = weight;
		e->next = G->AdjList[j].firstedge;		
		G->AdjList[j].firstedge = e; 

	}

	return G;
}

/**
 * @fn 存入文件
 *
 * @param G 邻接表的头指针
 */
void save_inf(GraphAdjList *G)
{
	FILE *fp;
	int i=1;
	EdgeNode *p;

	///判断是否能打开文件
	if((fp = fopen("map.txt","wt")) == NULL)
	{
		printf("打开文件map.txt出错！\n");
		exit(0);
	}

	///存入顶点数、边数
	fprintf(fp, "%d %d\n", G->numVertexes,G->numEdges);
	for(i=1; i<=G->numVertexes; i++)
	{
		///存入每个点的num、name、introduce
		fprintf(fp, "\n%d %s : %s\n", G->AdjList[i].num,G->AdjList[i].name,G->AdjList[i].introduce);
		///循环存入每个结点相邻的点的下标值与权值
		p = G->AdjList[i].firstedge;
		while(p != NULL)
		{
			fprintf(fp, "%d,%d ", p->adjvex,p->weight);
			p = p->next;
		}
		///每个结点存入完毕后，存入'#'作为结束标志
		fprintf(fp,"%c",'#');
	}
	///关闭文件
	fclose(fp);
}

/**
 * @fn 读取文件
 *
 * @return 返回邻接表的头指针
 *
 */
GraphAdjList *read_inf()
{
	FILE *fp;
	int i=1,j=0,weight=0;
	char ch;
	GraphAdjList *G;
	EdgeNode *e;
	G = (GraphAdjList *)malloc(sizeof(GraphAdjList));

	///判断能否打开文件
	if((fp = fopen("map.txt","rt")) == NULL)
	{
		printf("打开文件map.txt出错！\n");
		exit(0);
	}

	///读入顶点数、边数
	fscanf(fp, "%d %d", &G->numVertexes, &G->numEdges);
	for(i=1; i<=G->numVertexes; i++)
	{
		G->AdjList[i].name = (char *)malloc(sizeof(char));
		G->AdjList[i].introduce = (char *)malloc(sizeof(char));
		///读入每个点的num、name、introduce
		fscanf(fp, "\n%d %s : %s\n", &G->AdjList[i].num,G->AdjList[i].name,G->AdjList[i].introduce);
		G->AdjList[i].firstedge = NULL;
		fscanf(fp, "%c", &ch);
		///当读取的ch不等于#时，继续读入
		if(ch != '#')
		{
			fseek(fp, -1L, SEEK_CUR);
			while(ch != '#')
			{
		 		///循环读入每个结点相邻的点的下标值与权值
				e = (EdgeNode *)malloc(sizeof(EdgeNode));   
				fscanf(fp, "%d,%d ",&j,&weight);
				e->adjvex = j; 
				e->weight = weight;
				e->next = G->AdjList[i].firstedge;
				G->AdjList[i].firstedge = e; 
				///读入的不是'#'时，文件指针fp从当前位置往回移动一位
				fscanf(fp, "%c", &ch);
				if (ch != '#')
				fseek(fp,-1L,SEEK_CUR);
			}
		
		}
	}

	return G;

}

/**
 * @fn 查询地点信息
 *
 * @param G 邻接表的头指针
 *
 */
void find_spot(GraphAdjList *G)
{
	int i;
	char name[20];
	EdgeNode *p;
	///输入要查询的地点名称
	printf("请输入所要查询地点名称：");
	scanf("%s[^\n]",name);
	
	///遍历邻接表查询
	for(i=1; i<=G->numVertexes; i++)
	{
		if(strcmp(name, G->AdjList[i].name) == 0)
		{
			///找到就输出，跳出循环
			printf("[%d] %s:%s\n",G->AdjList[i].num, G->AdjList[i].name, G->AdjList[i].introduce);
			p = G->AdjList[i].firstedge;
			while(p != NULL)
			{
				printf("%s--%s: %d\n",G->AdjList[i].name,G->AdjList[p->adjvex].name,p->weight);
				p = p->next;
			}
			break;
		}
	}

	///没有找到就输出提示信息
	if(i > G->numVertexes)
	{
		printf("没有找到所查询地点信息.\n");
	}
	
}


/**
 * @fn 根据名称查找序号
 *
 * @param G 邻接表的头指针
 * @param num1 起始点的序号
 * @param num2 目的地的序号
 *
 */
void name_num(GraphAdjList *G, int *num1, int *num2)
{
	int i;
	char name1[20],name2[20];
	EdgeNode *p;

	*num1 = 0;
	*num2 = 0;
	
	///输入起始点、目的地的名称
	printf("请输入地点名称(起始点 目的地)：");
	scanf("%s%s",name1,name2);

	///遍历邻接表查询
	for(i=1; i<=G->numVertexes; i++)
	{
		///找到了就将序号存入num1、num2
		if(strcmp(name2, G->AdjList[i].name) == 0)
			*num2 = i;
		else if(strcmp(name1, G->AdjList[i].name) == 0)
                      	*num1 = i;
	}
	
	///找不到就输出提示信息，重新调用该函数
	if(*num1 == 0 || *num2 == 0)
	{
		if(*num1 == 0)
			printf("没有找到起始点.\n");
		if(*num2 == 0)
			printf("没有找到目的地.\n");
		name_num(G, num1, num2);
	}

}

/**
 * @fn 简单路径
 *
 * @param G 邻接表的头指针
 * @param num1 起始点下标
 * @param num2 目的地下标
 *
 */
void Simple_Path(GraphAdjList *G, int num1, int num2)         //简单路径
{
	int i,path[20];
	
	i = 0;

	///目的地下标存入parh[0]
	path[i] = num2;

	///根据pre数组依次将path值填入，直到path[i]等于起始点下标
	while(path[i] != num1)
	{
		i += 1;
		path[i] = pre[path[i-1]];
	}

	///倒序输出
	for( ; i>0; i--)
	{
		printf("%s-->",G->AdjList[path[i]].name);
	}
	printf("%s",G->AdjList[path[0]].name);

}

/**
 * @fn 广搜
 *
 * @param G 邻接点头指针
 */
void BFS(GraphAdjList *G)              //广搜
{
	int i,*s,num1,num2;
	EdgeNode *p;
	QueueNode *Q;
	///初始化队列
	Q = InitQueue();

	///visited数组初始化为0，pre数组初始化为-1
	for(i=1; i<=G->numVertexes; i++)
	{
		visited[i] = 0;
		pre[i] = -1;
	}
	///求出起始点、目的地下标
	name_num(G, &num1, &num2);
	///将起始点的visited值置0
	visited[num1] = 1;
	pre[num1] = -1;
	///起始点入队
	EnterQueue(Q, num1);

        
	while(Q->front != Q->rear)
	{
		///队头结点出队
		DeleteQueue(Q, &s);
		p = G->AdjList[*s].firstedge;
		///将出队结点的邻接点依次入队
		while(p)
		{
			if(!visited[p->adjvex])
			{
				///入队结点visited值置1
				visited[p->adjvex] = 1;
				EnterQueue(Q, p->adjvex);
				///入队结点的pre值为刚出队的结点
				pre[p->adjvex] = *s;
			}
			p = p->next;
		}
	}
/*
	for(i=0; i<=G->numVertexes; i++)
	{
		printf("pre%d:%d\n ",i,pre[i]);
	}
*/
	///调用Simple_Path函数
	Simple_Path(G, num1, num2);
	
}

/**
 * @fn 深搜
 *
 * @param G 邻接表头指针
 * @param i 结点下标值
 *
 */
void DFS(GraphAdjList *G,int i)        //深搜
{
	EdgeNode *p;
	visited[i] = 1;   
	printf("%s ",G->AdjList[i].name);
	p = G->AdjList[i].firstedge;
	while(p)
	{
		///当visited值不等于0时，递归调用DFS函数
		if(!visited[p->adjvex])
		{
			DFS(G, p->adjvex);
		}
		p = p->next;
	}
}

/**
 * @fn 深度优先搜索
 *
 * @param G 邻接表头指针
 */
void TraversG(GraphAdjList *G)
{
	int i;
	
	///visited数组初始化为0
	for(i=1; i<=G->numVertexes; i++)       
	{
		visited[i] = 0;
	}

	///从第1个结点开始搜索
	for(i=1; i<=G->numVertexes; i++)      
	{
		if(!visited[i])
		      ///调用DFS函数
		      DFS(G, i);
	}
	printf("\n");
}

/**
 * @fn 佛罗伊达
 *
 * @param 邻接表的头指针
 */
void Floyd(GraphAdjList *G)
{
	int i,j,k;
	EdgeNode *p;

	///初始化Path和D数组
	for(i=0; i<G->numVertexes; i++)       
	{
		for(j=0; j<G->numVertexes; j++)
		{
			if(i == j)		
			      D[i][j] = 0;
			else
			      D[i][j] = INFINITY;
			Path[i][j] = j;
		}
	}

	///将权值赋给D数组
	for(i=1; i<=G->numVertexes; i++)    
	{
		p = G->AdjList[i].firstedge;
		while(p)
		{
			D[i-1][p->adjvex-1] = p->weight;
			p = p->next;
		}
	}

	for(k=0; k<G->numVertexes; k++)
	{
		for(i=0; i<G->numVertexes; i++)
		{
			for(j=0; j<G->numVertexes; j++)
			{
				if(D[i][j] > D[i][k]+D[k][j])		
				{
					///若经过下标为k顶点的路径比原来两点间路径更短，就更新D数组，存入权值更小的
					D[i][j] = D[i][k]+D[k][j];
					///路径设置经过下标为k的顶点
					Path[i][j] = Path[i][k];
				}
			}
		}
	}
/*
	for(i=0; i<G->numVertexes; i++)
	{
		for(j=0; j<G->numVertexes; j++)
		{
			printf("%-8d",D[i][j]);
		}
		printf("\n");
	}
	for(i=0; i<G->numVertexes; i++)
	{
		for(j=0; j<G->numVertexes; j++)
		{
			printf("%-8d",Path[i][j]);
		}
		printf("\n");
	}
*/
}

/**
 * @fn 最短路径
 *
 * @param G 邻接表的头指针
 */
void Short_path(GraphAdjList *G)       //最短路径
{
	int i,num1,num2,path;
	///求出起始点、目的地下标
	name_num(G, &num1, &num2);

	num1 = num1-1;
	num2 = num2-1;

	printf("最短需要走%dm.\n",D[num1][num2]);

	///根据Path数组找到最短路径
	path = Path[num1][num2];
	printf("路径为：%s-->",G->AdjList[num1+1].name);
	while(path != num2)
	{
		printf("%s-->",G->AdjList[path+1].name);
		path = Path[path][num2];
	}
	printf("%s\n",G->AdjList[num2+1].name);

}

/**
 * @fn 添加路径
 *
 * @param G 邻接点的头指针
 * @param num1 起始点的下标
 * @param num2 目的地的下标
 *
 */
void Add_path(GraphAdjList *G, int num1, int num2)
{
	int len;
	EdgeNode *p,*e;
	
	///找到num1、num2时，添加路径
	if(num1 != 0 && num2 != 0)	
	{
		///输入添加的距离
		printf("请输入所要添加路线之间的距离:");
		scanf("%d",&len);

		///头插法将新的边插入邻接表
		e = (EdgeNode *)malloc(sizeof(EdgeNode));
		e->adjvex = num2;
		e->weight = len;
		e->next = NULL;

		e->next = G->AdjList[num1].firstedge;
		G->AdjList[num1].firstedge = e;

		e = (EdgeNode *)malloc(sizeof(EdgeNode));
		e->adjvex = num1;
		e->weight = len;
		e->next = NULL;

		e->next = G->AdjList[num2].firstedge;
		G->AdjList[num2].firstedge = e;

		///边值+1
		G->numEdges ++;
		///存入文件
		save_inf(G);
	}

}

/**
 * @fn 添加结点
 *
 * @param G 邻接表的头指针
 */
void Add_vertex(GraphAdjList *G)
{
	int i,j,num1,num2=0,n,weight;
	char name1[20],name2[20],introduce[20];
	EdgeNode *e;

	i = G->numVertexes;
	///遍历邻接表，若发现有空位，则将新结点添加到空位中
	while(i)
	{
		if(strcmp("#", G->AdjList[i].name) == 0)
		{
			num1 = i;
			break;
		}
		else
		      i--;
	}

	///若没有空位，则开辟新空间，将新结点添加到邻接表的最后，结点值+1
	if( i == 0)
	{
		G->numVertexes++;
		num1 = G->numVertexes;
		G->AdjList[num1].name = (char *)malloc(sizeof(char));
		G->AdjList[num1].introduce = (char *)malloc(sizeof(char));
		G->AdjList[num1].num = num1;
	}
	
	printf("请输入需要添加的地点名称：");
	scanf("%s",name1);
	strcpy(G->AdjList[num1].name, name1);
	printf("请输入需要添加的地点介绍：");
	scanf("%s",introduce);
	strcpy(G->AdjList[num1].introduce,introduce);
	printf("请输入您要添加几条边：");
	scanf("%d",&n);

	for(i=0; i<n; i++)
	{
loop:		printf("请输入与添加的边相连的地点名称：");
		scanf("%s", name2);
		for(j=1; j<=G->numVertexes; j++)
		{
			if(strcmp(name2,G->AdjList[j].name) == 0)
			{
				num2 = j;
				break;
			}
		}
		if(j == G->numVertexes+1)
		{
			printf("没有找到所要添加的地点.\n");
			goto loop;
		}		
		else
		{
			Add_path(G,num1,num2);
			///每添加一条边，则将边值+1
			G->numEdges ++;	
		}
	}

	///存入文件
	save_inf(G);
	
}

/**
 * @fn 添加菜单函数
 *
 * @param G 邻接表的头指针
 */
void Add_choice(GraphAdjList *G)
{
	int m,num1,num2;

	printf("********************\n");
	printf("1.添加路线          \n");
	printf("2.添加地点          \n");
	printf("                    \n");
	printf("Please input your choice:");
	scanf("%d",&m);

	switch(m)
	{
		///添加路线
		case 1: name_num(G, &num1,&num2);
			Add_path(G, num1, num2);
			break;
          	///添加地点
		case 2: Add_vertex(G);
			break;
		default:break;
	}

}

/**
 * @fn 删除路线
 *
 * @param cur 后继结点
 * @param num 删除路线目的地的下标
 */
void Delete_path(EdgeNode **cur, int num)
{
	EdgeNode *en;
	while(*cur)
	{
		en = *cur;
		///若找到目的地所在结点，删除该结点
		if(en->adjvex == num)
		{
			*cur = en->next;
			free(en);
		}
		else
		      cur = &en->next;
	}	
}	

/**
 * @fn 删除结点
 *
 * @param 邻接表的头指针
 */
void Delete_vertex(GraphAdjList *G)
{
	int i,num1,num2,j=0;
	EdgeNode *p;
	char name[20];
	///输入需要删除地点的名字
	printf("请输入需要删除地点的名字：");
	scanf("%s",name);

	///遍历邻接表，将下标赋给num1
	i = G->numVertexes;
	while(i)
	{
		if(strcmp(name, G->AdjList[i].name) == 0)
		{
			num1 = i;
			break;
		}
		else
		      i--;
	}

	if(i != 0)
	{
		p = G->AdjList[num1].firstedge;

		///依次释放num1所连接的所有结点的空间
		while(p)
		{
			num2 = p->adjvex;
			Delete_path(&(G->AdjList[num1].firstedge), num2);
			Delete_path(&(G->AdjList[num2].firstedge), num1);
			j++;
			p = p->next;
		}
		G->AdjList[num1].firstedge = NULL;
		///将该结点的name、introduce赋为'#'
		strcpy(G->AdjList[i].name, "#");
		strcpy(G->AdjList[i].introduce,"#");
		///边值减去相应的值
		G->numEdges = G->numEdges-j;
		///存入文件
		save_inf(G);
	}
	///若没有找到，输出提示语句
	else
		printf("没有找到所要删除的地点.\n");

}

/**
 * @fn 删除菜单函数
 *
 * @param G 邻接表的头指针
 */
void Delete_choice(GraphAdjList *G)
{

	int m,num1,num2;

	printf("******************\n");
	printf("1.删除路线        \n");
	printf("2.删除地点        \n");
	printf("                  \n");
	printf("Please input your choice:");
	scanf("%d",&m);

	switch(m)
	{
		///删除路线
		case 1:	name_num(G, &num1, &num2);
			Delete_path(&(G->AdjList[num1].firstedge), num2);
			Delete_path(&(G->AdjList[num2].firstedge), num1);
			G->numEdges--;
			save_inf(G);
			break;
		///删除结点
		case 2: Delete_vertex(G);
			break;
		default:break;
	}
}

/**
 * @fn 查询所有简单路径
 *
 * @param G 邻接表的头指针
 * @param num1 起始点下标
 * @param num2 目的地下标
 * @param k All_path数组下标
 *
 */
void Find_All_Path(GraphAdjList *G, int num1, int num2, int k)
{
	int i,t;
	EdgeNode *p;

	///把当前起始点下标值赋给All_path[k]
	All_path[k] = num1;
	///visisted值置1
	visited[num1] = 1;
	///如果当前起始点下标等于目的地下标，则找到一条路径
	if(num1 == num2)
	{
		printf("find one path\n");
		///输出该路径
		for(i=0; All_path[i]; i++)
		{
			printf("%s ",G->AdjList[All_path[i]].name);
		}
		printf("\n");
	}
	///如果当前起始点下标不等于目的地下标，则找到当前结点的邻接点继续搜索
	else
	{
		for(p = G->AdjList[num1].firstedge; p; p = p->next)
		{
			t = p->adjvex;
			///递归调用Find_All_Path函数
			if(!visited[t])
			      Find_All_Path(G, t, num2, k+1);
		}
	}
	///当前起始点visited值置0,以便递归返回时，下次递归继续遍历
	visited[num1] = 0;
	///当前All_path[k]值置0
	All_path[k] = 0;
}

/**
 * @fn 输出
 *
 * @param 邻接表的头指针
 */
void print(GraphAdjList *G)
{
	int i;
	EdgeNode *p = NULL;
	///从第一个结点开始依次遍历
	for(i=1; i<=G->numVertexes; i++)     
	{
		p = G->AdjList[i].firstedge;
		printf("\n[%d]",G->AdjList[i].num);
		printf("%s:",G->AdjList[i].name);
		printf("%s\n",G->AdjList[i].introduce);
		while(p!=NULL )
		{
			printf("%s--%s: %dm\n",G->AdjList[i].name,G->AdjList[p->adjvex].name,p->weight);
			p = p->next;
		}
		printf("\n");
	}
}

/**
 * @fn 选择函数
 *
 * @param choice 选项
 */
void choice(int choice)
{
	int i,m,num1,num2,k=0;
	GraphAdjList *G;
	///读入文件
	G = read_inf();
	switch(choice)
	{ 
		///创建邻接表
		case 1: G = CreatALGraph(); 
			save_inf(G);
			print(G);
			break;
		///输出邻接表
		case 2:	G = read_inf();
			print(G); 
			break;
		///查询地点简介及交通信息
		case 3:	find_spot(G);
			break;
		///查询简单路径（广搜）
   		case 4: BFS(G);
			break;
		///查询最短路径（floyd）
		case 5: Floyd(G);
			Short_path(G);
			break;
		///查询所有简单路径
		case 6: name_num(G, &num1, &num2);
			for(i=0; i<MAXVEX; i++)
				visited[i] = 0;
			Find_All_Path(G, num1, num2, k);
			break;
		///添加结点
		case 7: Add_choice(G);
			break;
		///删除结点
		case 8: Delete_choice(G);
			break;
		///退出
		case 0: exit(0);
		default:break;
	}
	printf("\n\nPress any key to continue.\n");
	getchar();
	getchar();
	menu();
}

/**
 * @fn 菜单函数
 */
int menu()
{
	int m;
	system("clear");
	printf("\t\t\t\t*******************************\n\n");
	printf("\t\t\t\t 1.创建图                        \n");
	printf("\t\t\t\t 2.打印图                   	 \n");
	printf("\t\t\t\t 3.查询地点简介及交通信息        \n");
	printf("\t\t\t\t 4.查询简单路径（中转次数最少）  \n");
	printf("\t\t\t\t 5.查询最短路径（权值最小）      \n");
	printf("\t\t\t\t 6.两点间所有简单路径            \n");
	printf("\t\t\t\t 7.增添路线                      \n");
	printf("\t\t\t\t 8.删除路线                      \n");
	printf("\t\t\t\t 0.退出                        \n\n");
	printf("\t\t\t\t*******************************\n\n");
     
	printf("Please input your choice:");
	scanf("%d",&m);

	///当输入非法时,按任意键重新选择
	if(m!=1 && m!=2 && m!=3 && m!=4 && m!=5 && m!=6 && m!=7 && m!=8 && m!=0)
	{
		printf("输入有误！\n");
		printf("\n\nPress any key to continue.\n");
		getchar();
		getchar();
		system("reset");
		menu();
		
	}
	choice(m);
}

/**
 * @fn main函数
 */
int main()
{
	menu();
}







