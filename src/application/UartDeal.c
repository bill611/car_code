/*
 * =============================================================================
 *
 *       Filename:  UartDeal.c
 *
 *    Description:  ��������Э��(���ͼ�����)
 *
 *        Version:  1.0
 *        Created:  2015-11-13 10:56:26
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include "externfunc.h"
#include "protocol.h"


/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_UART_DEAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*/
/**
 * @brief FromSingleChip ���յ�Uart������Ϣ
 *
 * @param This
 */
/* ---------------------------------------------------------------------------*/
static void FromSingleChip(UartDeal *This)
{
	unsigned char buff[1024]={0};

	int	len = This->uart->RecvBuffer(This->uart,buff,sizeof(buff));
	if (len <= 0) {
		return;
	}
    if (pro_app)
        pro_app->dbgRecSerial(buff,len);
	pro_com->checkOnlineCmd(buff,len);	
	pro_com->checkStateCmd(buff,len);	

}

/* ---------------------------------------------------------------------------*/
/**
 * @brief ToSingleChip ���������Ƭ��
 *
 * @param This
 * @param Cmd ����
 * @param ... ���ñ���ʱ��,����ˢ�����ʹ��
 */
/* ---------------------------------------------------------------------------*/
static void ToSingleChip(UartDeal *This,int cnt)
{
#ifdef PC
    return;
#endif
	This->uart->Send(This->uart,This->SndData,cnt);
    if (pro_app)
        pro_app->dbgSendSerial(This->SndData,cnt);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief UartDeal_Destory �ͷ�UART��Դ
 *
 * @param This
 */
/* ---------------------------------------------------------------------------*/
static void UartDeal_Destory(UartDeal *This)
{
	if (This) {
		This->uart->Destroy(This->uart);
		free(This);
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief uartDealCreate ����UART����ҵ�������
 *
 * @param hWnd ��ӦUI������
 *
 * @returns ����ָ��
 */
/* ---------------------------------------------------------------------------*/
UartDeal *uartDealCreate(HWND hWnd)
{
	UartDeal *This = (UartDeal*)calloc(sizeof(UartDeal),1);
	if(This == NULL)
		return NULL;
	This->hWnd = hWnd;
	This->uart = Create_UartServer(hWnd);
#ifndef PC
    if(This->uart->Open(This->uart,0,9600)==0) {
        printf("Err:com0 open fail!\n");
    }
#endif

	This->ToSingleChip = ToSingleChip;
	This->FromSingleChip =  FromSingleChip;
	This->Destroy = UartDeal_Destory;

	return This;
}
