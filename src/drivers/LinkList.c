/*
 * =====================================================================================
 *
 *       Filename:  LinkList.c
 *
 *    Description:  通用链表函数
 *
 *        Version:  1.0
 *        Created:  2015-11-04 17:28:56
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
/* ----------------------------------------------------------------*
 *                      include head files
 *-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "LinkList.h"


/* ----------------------------------------------------------------*
 *                  extern variables declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                  internal functions declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                        macro define
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                      variables define
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*/
/**
 * @brief NewChainNode 新建链表节点
 *
 * @param data 节点数据
 *
 * @returns 1成功 0失败
 */
/* ----------------------------------------------------------------*/
static ChainNode * NewChainNode(ElemType data)
{
	ChainNode * pChain=0;
	pChain = ( ChainNode * )malloc( sizeof(ChainNode) );

	if( ! pChain ) {
		printf("Err: [%s] malloc fail\n", __FUNCTION__);
		return 0;
	}

	pChain->data=data;
	pChain->next=0;

	return pChain;
}

/* ----------------------------------------------------------------*/
/**
 * @brief ListAppend 追加链表元素
 *
 * @param This 目标链表
 * @param ... 加入的数据
 *
 * @returns 1成功 0失败
 */
/* ----------------------------------------------------------------*/
static int ListAppend(List * This,...)
{
	ChainNode * newpt = 0;
	void * data;
	void * pos;
	pos = &This + 1;

	if( !(This && This->head) )
		return 0;

	data = (void *)malloc(This->Nodesize);
	if( !data )
		return 0;

	memcpy(data,pos,This->Nodesize);
	newpt =  NewChainNode(data);

	if( !newpt )
		return 0;

	This->tail->next = newpt;
	This->tail = newpt;
	return 1;
}

/* ----------------------------------------------------------------*/
/**
 * @brief ListInsert 加入元素
 *
 * @param This 目标链表
 * @param n 加入位置
 * @param ... 加入的数据
 *
 * @returns 1成功 0失败
 */
/* ----------------------------------------------------------------*/
static int ListInsert(List * This, int n ,...)
{
	ChainNode * pt = 0;
	ChainNode * newpt = 0;
	void * data;
	void *pos = &This + 2;
	pt = This->GetAddr( This, n-1 );
	if( !(pt) )
		return 0;
	data = (void*)malloc(This->Nodesize);

	if( !data )
		return 0;
	memcpy(data,pos,This->Nodesize);

	newpt = NewChainNode(data);
	if( !newpt )
		return 0;

	if ( pt->next == This->tail )
		This->tail = newpt;

	newpt->next = pt->next;

	pt->next = newpt;

	return 1;

}

/* ----------------------------------------------------------------*/
/**
 * @brief GetElem 取得第几个元素的值
 *
 * @param This 目标链表
 * @param n 取得元素的位置
 * @param data 取得元素的数据
 *
 * @returns 1成功 0失败
 */
/* ----------------------------------------------------------------*/
static int GetElem(List * This,int n,ElemType data)
{
	ChainNode * pt = 0;

	if( !data )
		return 0;

	pt = This->GetAddr(This,n);
	if( ! pt )
		return 0;

	memcpy(data, pt->data ,This->Nodesize);

	return 1;
}

/* ----------------------------------------------------------------*/
/**
 * @brief GetElemTail 取得最后一个元素的数据
 *
 * @param This 目标链表
 *
 * @returns 元素内容
 */
/* ----------------------------------------------------------------*/
static ElemType * GetElemTail(List *This)
{
	return This->tail->data;
}
/* ----------------------------------------------------------------*/
/**
 * @brief TraverseList 遍历访问，访问某个节点元素用函数处理
 *
 * @param This 目标链表
 * @param func 访问改节点元素的处理函数
 *
 * @returns 1成功 0失败
 */
/* ----------------------------------------------------------------*/
static int TraverseList(List* This,int (*func)(ElemType ))
{
	ChainNode * pt = 0;
	int a=0;

	if( !(This && This->head) )
		return 0;
	for( a = 0 ,pt = This->head->next; pt ; pt = pt->next ) {
		if( ! func( (pt->data)) )
			return a+1;
		a++;
	}
	return 0;
}

/* ----------------------------------------------------------------*/
/**
 * @brief ClearList 清空链表
 *
 * @param This 目标链表
 */
/* ----------------------------------------------------------------*/
static void ClearList(List * This)
{
	while ( This->ListDelete(This,1) );
}

/* ----------------------------------------------------------------*/
/**
 * @brief ListDelete 删除第几个元素
 *
 * @param This 目标链表
 * @param n 删除元素位置
 *
 * @returns 1成功 0失败
 */
/* ----------------------------------------------------------------*/
static int ListDelete( List * This,  int n )
{
	ChainNode * pt =0;
	ChainNode * pf=0;

 	if( !This->head->next )
		return 0;

	pt = This->GetAddr( This,n-1 );

	if ( pt->next == This->tail )
		This->tail = pt;

	if( !(pt && pt->next ))
		return 0;

	pf = pt->next;
	pt->next = pt->next->next;

	free(pf->data);
	free(pf);

	return 1;

}

/* ----------------------------------------------------------------*/
/**
 * @brief GetAddr 取得编号为n的元素所在地址
 *
 * @param This 目标链表
 * @param n 目标编号位置
 *
 * @returns 1成功 0失败
 */
/* ----------------------------------------------------------------*/
static ChainNode * GetAddr(List * This,int n)
{
	ChainNode * pt = 0;
	int a = 0;

	if( n < 0) {
		printf("Err: [%s] n < 0\n", __FUNCTION__);
		return 0;
	}

	pt = This->head;

	while( pt && a < n ) {
		pt = pt->next;
		a++;
	}
	return pt;
}

/* ----------------------------------------------------------------*/
/**
 * @brief DestoryList 销毁链表
 *
 * @param This 销毁对象
 */
/* ----------------------------------------------------------------*/
static void DestoryList(List * This)
{
	This->ClearList(This);

	free(This->head);
	This->head = 0;

	free(This);
	This = 0;

}

/* ----------------------------------------------------------------*/
/**
 * @brief CreateList 创建链表
 *
 * @param size 数据长度
 *
 * @returns 创建的链表
 */
/* ----------------------------------------------------------------*/
List *CreateList(int size )
{
	List * This = 0;
	ElemType data = 0;
	This=(List*)malloc( sizeof(List) );

	if( !This )
		return 0;
	This->head = NewChainNode(data );
	if( ! This->head ) {
		free(This);
		return 0;
	}
	This->Nodesize = size;
	This->tail = This->head;
	
	This->ClearList 	= ClearList;
	This->ListAppend 	= ListAppend;
	This->ListInsert 	= ListInsert;
	This->ListDelete 	= ListDelete;
	This->GetElem 		= GetElem;
	This->GetElemTail 	= GetElemTail;
	This->GetAddr 		= GetAddr;
	This->TraverseList 	= TraverseList;
	This->DestoryList 	= DestoryList;

	return This;
}
