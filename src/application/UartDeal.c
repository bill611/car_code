/*
 * =============================================================================
 *
 *       Filename:  UartDeal.c
 *
 *    Description:  解析串口协议(发送及接收)
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
 * @brief FromSingleChip 接收到Uart来的消息
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
 * @brief ToSingleChip 发送命令到单片机
 *
 * @param This
 * @param Cmd 命令
 * @param ... 设置报警时间,返回刷卡结果使用
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
 * @brief UartDeal_Destory 释放UART资源
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
 * @brief uartDealCreate 创建UART串口业务处理对象
 *
 * @param hWnd 对应UI主窗体
 *
 * @returns 对象指针
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
