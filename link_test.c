/*
 * =====================================================================================
 *       Filename:  link_bubble_sort.c
 *    Description:  
 *        Version:  
 *        Created:  2012年08月29日 17时43分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  szm , xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 * ==================================================================================
 */
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define num 6

typedef struct node{
  int data;
  struct node *next;
}Node,*Linklist;

Linklist createlist( void ){
  Node *p=NULL,*q=NULL,*head=NULL;
  int a[num]={1,4,3,6,5,2},i;       /*插入数据初始化*/
  q=head=( Node *)malloc( sizeof( Node));
  q->next=NULL;
  q->data=a[0];
   for( i=1;i<num;i++ ){
	  p=( Node* )malloc( sizeof( Node ) );
	  p->data=a[i];
	  p->next=NULL;
	  q->next=p;
	  q=p;
  }
   q->next=NULL;
   return head;
}
void bubblesort( Linklist head){   /*对链表自身节点进行冒泡排序*/
	Node *p=NULL,*q=NULL,*s=NULL;
	s=( Node* )malloc( sizeof( Node ) );
	p=( Node* )malloc( sizeof( Node ) );
	if( head->next==NULL ) return ;
	for( head=head->next;head->next!=NULL;head=head->next ){
		for( q=head->next;q!=NULL;q=q->next ){
			if( head->data > q->data ){
				*s=*q;
				*p=*head;
				*head=*q;
				*q=*p;
				head->next=p->next;
				q->next=s->next;
			}
		}
	}
	free( s );
	free( p );
}
Linklist reverse( Linklist head ){
	 Linklist r=NULL,t=NULL,y=NULL;  /*单链表的就地逆置*/
	if( NULL==head ){
		printf( "Linklist is NULL" );
		return NULL;
	}
	y=head;
	while( NULL!=y ){
		t=y->next;
		y->next=r;
		r=y;
		y=t;
	}
	return r;
}
Linklist delete( Linklist head,int pos ){
	Node *p,*q;
	p=head;    /*链表删除操作*/
	while( p->next->data!=pos ){
		p=p->next;
	}
	q=p->next;
	p->next=q->next;
	free( q );
	return head;

}
Linklist insert( Linklist head,int pos ){
	Node *p,*q;        /*链表插入操作*/
	q=( Node * )malloc( sizeof( Node ) );
	q->next=NULL;
	q->data=pos;
	p=head;
	while(( (p->next->data)>pos) && (( p->next->next->data )<pos) ){
		p=p->next;
	}
	q->next=p->next;
	p->next=q;
	return head;
}
int Depth_test( Linklist head ){
	int k=0;           /*寻找链表的深度`*/
	while( head!=NULL ){
		k++;
		head=head->next;
	}
	return k;
}
int linkdestroy( Linklist head ){
	Node *p,*q,*k;                   /*销毁链表*/
	p=head;
	q=p->next;
	if( q==NULL ){
		free( head );
		return 1;
	}
	else{
		while( q!=NULL ){
			k=q;
			p->next=q->next;
			q=q->next;
			free( k );
		}
		free( head );
		return 1;
	}
	return 0;
}
void output( Linklist head){ /*打印数据*/
	 while(NULL!=head){
        printf( "%d\t",head->data);
	    head=head->next;
     }
	 printf( "\n" );
}
int main(void )
{
	Linklist head;
	int k;
	head=createlist( );
	k=Depth_test( head );
	printf( "开始链表深度为：%d",k );
	printf( "\n排序前链表节点元素顺序:\n");
        output( head);
        bubblesort( head);
        printf( "冒泡排序后链表节点元素顺序:\n");
        output( head);
	head=reverse( head );
	printf( "逆置后:\n" );
	output( head );
	printf( "删除4后：\n" );
	head=delete( head,4 );
	output( head );
	printf( "增加4后:\n" );
	head=insert( head,4 );
	output( head );
	if( linkdestroy( head ) )
		printf( "Link destroyed successfully !\n" );
	else
		printf( "link destroyed failed !\n" );
	return 0;
}


