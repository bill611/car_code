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

	enum{
		UERROR_IDLE,
		UERROR_RECVOVER
	};

	enum{
		UART_CCDOPEN,       //开CCD
		UART_CCDCLOSE,      //关CCD
		UART_ENWDT,         //使能看门狗
		UART_DISWDT,        //禁止看门狗
		UART_WDT,           //喂狗
		UART_ENALARM,       //使能防拆报警
		UART_DISALARM,      //取消防拆报警
		UART_CMDUNLOCK,     //命令开锁
		UART_IDUNLOCK,      //ID卡开锁
		UART_BUZZERLONG,    //蜂鸣器长响
		UART_BUZZERSHORT,   //蜂鸣器短响
		UART_AUDIOHINT,     //语音提示
		UART_MICOPEN,       //开Mic
		UART_MICCLOSE,      //关Mic
		UART_WRIMEI,        // 写IMEI   xiehb 20120613
		UART_RDIMEI,        // 读IMEI
		UART_RESET,         // 复位命令
		UART_VERSION,       // 版本读取
		UART_ALARMDOOR,     // 发送门锁报警时间协议
		UART_CARD,     		// 发送刷卡结果
	};

	enum{
		UART_REC_KEYBOARD,		//按键操作
		UART_REC_UNBROKEN, 		//门口机防拆报警
		UART_REC_UNLOCK,    	//回应室内、中心、密码开锁结果
		UART_REC_WDT,   		//看门狗操作
		UART_REC_ENUNLOCK,      //使能防拆
		UART_REC_CARD,  		//刷卡开锁
		UART_REC_IMEI, 			//写机身码IMEI 返回结果 成功或失败
		UART_REC_DUNLOCK, 		//ID卡开锁
		UART_REC_DIMEI,        	// 读IMEI
		UART_REC_SINGLE_VERSION,//单片机返回软件版本号
	};

	// typedef struct _ELEVATOR
	// {
		// unsigned char  head;
		// unsigned char  Address; //AddrH
		// unsigned char  Cell;    //AddrL
		// unsigned char  Function;
		// char  StartFloor;   //CellCode;
		// char  EndFloor;     //FloorCode
		// unsigned char  Lrc;
		// unsigned char  Cr;
		// unsigned char  Lf;
	// }Elevator;

	typedef struct _UartDeal {
		HWND hWnd; // 窗口句柄
		struct _UARTSERVER *uart;
		unsigned char RecData[BUFSIZE];             //接收数据缓存
		unsigned char SndData[BUFSIZE];             //接收数据缓存

		void (*ToSingleChip)(struct _UartDeal *This,int Cmd,...);
		void (*FromSingleChip)(struct _UartDeal *This);
		// void (*ElevatorSndData)(struct _UartDeal *This,Elevator *pElevator);
		void (*Destroy)(struct _UartDeal *This);
	}UartDeal;

	UartDeal *uartDealCreate(HWND hWnd);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

