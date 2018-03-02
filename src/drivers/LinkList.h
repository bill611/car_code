/*
 * =====================================================================================
 *
 *       Filename:  LinkList.h
 *
 *    Description:  通用链表函数
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

		void (*ClearList)(struct _List*);						/* 清空链表 */
		int (*ListAppend)(struct _List*,...);					/* 追加元素 */
		int (*ListInsert)(struct _List*,int,...);				/* 加入元素 */
		int (*ListDelete)(struct _List *,int);					/* 删除第几个元素 */
		int (*GetElem)(struct _List*,int,ElemType );			/* 取得第几个元素的值用第三个参数返回 */
		ElemType * (*GetElemTail)(struct _List*);				/* 取得最后一个元素的内容 */
		ChainNode * (*GetAddr)(struct _List *, int);			/* 取得编号为N的元素所在地址 */
		int (*TraverseList)(struct _List*,int (*)(ElemType ));	/* 遍历访问，访问某个节点元素用函数处理 */
		void (*DestoryList)(struct _List*);						/* 销毁链表 */
	}List;

	List * CreateList(int);						/* 创建链表 */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
