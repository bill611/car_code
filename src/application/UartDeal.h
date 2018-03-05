/*
 * =====================================================================================
 *
 *       Filename:  UartDeal.h
 *
 *    Description:  解析串口协议(发送及接收)
 *
 *        Version:  1.0
 *        Created:  2015-11-16 17:21:49
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _UARTDEAL_H
#define _UARTDEAL_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define BUFSIZE          1024

	typedef struct _UartDeal {
		HWND hWnd; // 窗口句柄
		struct _UARTSERVER *uart;
		unsigned char RecData[BUFSIZE];             //接收数据缓存
		unsigned char SndData[BUFSIZE];             //接收数据缓存

		void (*ToSingleChip)(struct _UartDeal *This,int cnt);
		void (*FromSingleChip)(struct _UartDeal *This);
		// void (*ElevatorSndData)(struct _UartDeal *This,Elevator *pElevator);
		void (*Destroy)(struct _UartDeal *This);
	}UartDeal;

	UartDeal *uartDealCreate(HWND hWnd);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

