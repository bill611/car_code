/*
 * =====================================================================================
 *
 *       Filename:  LinkList.c
 *
 *    Description:  ͨ��������
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
 * @brief NewChainNode �½�����ڵ�
 *
 * @param data �ڵ�����
 *
 * @returns 1�ɹ� 0ʧ��
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
 * @brief ListAppend ׷������Ԫ��
 *
 * @param This Ŀ������
 * @param ... ���������
 *
 * @returns 1�ɹ� 0ʧ��
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
 * @brief ListInsert ����Ԫ��
 *
 * @param This Ŀ������
 * @param n ����λ��
 * @param ... ���������
 *
 * @returns 1�ɹ� 0ʧ��
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
 * @brief GetElem ȡ�õڼ���Ԫ�ص�ֵ
 *
 * @param This Ŀ������
 * @param n ȡ��Ԫ�ص�λ��
 * @param data ȡ��Ԫ�ص�����
 *
 * @returns 1�ɹ� 0ʧ��
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
 * @brief GetElemTail ȡ�����һ��Ԫ�ص�����
 *
 * @param This Ŀ������
 *
 * @returns Ԫ������
 */
/* ----------------------------------------------------------------*/
static ElemType * GetElemTail(List *This)
{
	return This->tail->data;
}
/* ----------------------------------------------------------------*/
/**
 * @brief TraverseList �������ʣ�����ĳ���ڵ�Ԫ���ú�������
 *
 * @param This Ŀ������
 * @param func ���ʸĽڵ�Ԫ�صĴ�����
 *
 * @returns 1�ɹ� 0ʧ��
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
 * @brief ClearList �������
 *
 * @param This Ŀ������
 */
/* ----------------------------------------------------------------*/
static void ClearList(List * This)
{
	while ( This->ListDelete(This,1) );
}

/* ----------------------------------------------------------------*/
/**
 * @brief ListDelete ɾ���ڼ���Ԫ��
 *
 * @param This Ŀ������
 * @param n ɾ��Ԫ��λ��
 *
 * @returns 1�ɹ� 0ʧ��
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
 * @brief GetAddr ȡ�ñ��Ϊn��Ԫ�����ڵ�ַ
 *
 * @param This Ŀ������
 * @param n Ŀ����λ��
 *
 * @returns 1�ɹ� 0ʧ��
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
 * @brief DestoryList ��������
 *
 * @param This ���ٶ���
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
 * @brief CreateList ��������
 *
 * @param size ���ݳ���
 *
 * @returns ����������
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
