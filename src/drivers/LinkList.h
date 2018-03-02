/*
 * =====================================================================================
 *
 *       Filename:  LinkList.h
 *
 *    Description:  ͨ��������
 *
 *        Version:  1.0
 *        Created:  2015-11-04 16:11:54
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _LINK_LIST_H
#define _LINK_LIST_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	typedef  void * ElemType;

	typedef struct node {
		ElemType data;
		struct node * next;
	}ChainNode;

	typedef struct _List{
		ChainNode *head;
		int Nodesize;
		ChainNode *tail;

		void (*ClearList)(struct _List*);						/* ������� */
		int (*ListAppend)(struct _List*,...);					/* ׷��Ԫ�� */
		int (*ListInsert)(struct _List*,int,...);				/* ����Ԫ�� */
		int (*ListDelete)(struct _List *,int);					/* ɾ���ڼ���Ԫ�� */
		int (*GetElem)(struct _List*,int,ElemType );			/* ȡ�õڼ���Ԫ�ص�ֵ�õ������������� */
		ElemType * (*GetElemTail)(struct _List*);				/* ȡ�����һ��Ԫ�ص����� */
		ChainNode * (*GetAddr)(struct _List *, int);			/* ȡ�ñ��ΪN��Ԫ�����ڵ�ַ */
		int (*TraverseList)(struct _List*,int (*)(ElemType ));	/* �������ʣ�����ĳ���ڵ�Ԫ���ú������� */
		void (*DestoryList)(struct _List*);						/* �������� */
	}List;

	List * CreateList(int);						/* �������� */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
