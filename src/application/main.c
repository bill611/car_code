/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main Window in MiniGUI
 *
 *        Version:  1.0
 *        Created:  2015-10-30 10:34:18
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
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
static void createGpioHandleThread(void);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_MAIN > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif
#define MAIN_TIMERVAL	90000	//精确定时器: 微秒

/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
FormMain *form_main = NULL;
TUDPServer *UdpServer = NULL;
UartDeal *uart = NULL;

/* ---------------------------------------------------------------------------*/
/**
 * @brief irqTimerInfo 100ms精确定时器，采用中断方式，不可在其中
 * 用delay或者其他阻塞函数,否则会造成程序阻塞死机
 *
 * @param signo
 */
/* ---------------------------------------------------------------------------*/
static void irqTimerInfo(int signo)
{
	static unsigned int TimerCnt=0;
	static struct itimerval timerval;	// 定时器定时值
	// ReSet timer
	timerval.it_value.tv_sec = 0;
	timerval.it_value.tv_usec = MAIN_TIMERVAL;
	timerval.it_interval = timerval.it_value;
	setitimer(ITIMER_REAL, &timerval, NULL);

	if((TimerCnt % 30) == 0) {// 3秒
		WatchDogFeed();
	}
	TimerCnt++;
}

static void irqTimerSigaction(void)
{
	sigset_t set;
    struct sigaction action;

    action.sa_handler = irqTimerInfo;
    action.sa_flags   = 0;
    sigemptyset(&set); 	// Get a clean set
	sigaddset(&set, SIGALRM);	// Add SIGALARM
    sigaction(SIGALRM, &action, NULL);
}

static void irqTimerInit(int sec)
{
	static struct itimerval timerval;	// 定时器定时值

	timerval.it_value.tv_sec = 0;
	timerval.it_value.tv_usec = MAIN_TIMERVAL;
	timerval.it_interval = timerval.it_value;
	setitimer(ITIMER_REAL, &timerval, NULL);
}


/* ---------------------------------------------------------------------------*/
/**
 * @brief timer1s 定时器1s
 */
/* ---------------------------------------------------------------------------*/
static void timer1s(void)
{
	// 检查网络连接状态
	if (net_detect("wlan0") < 0)
		form_main->setNetWorkState(0);
	else
		form_main->setNetWorkState(1);
	if (form_main->timerProc1s()) {
        printf("form timer 1s\n");
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief 定时器执行函数，顺序需要和枚举一致
 */
/* ---------------------------------------------------------------------------*/
static FormMainTimers timers_table[] = {
	{timer1s,				10},
};
/* ---------------------------------------------------------------------------*/
/**
 * @brief mainAppProc 主窗口回调函数
 *
 * @param hWnd
 * @param message
 * @param wParam
 * @param lParam
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
static int mainAppProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_CREATE:
			{
				// 初始化公共参数
				publicInit();
				initProtocol();
				// 创建UDP监控
                udpServerInit(pro_app->udpSocketRead,
                        pro_app->getRecivePort());
				// 串口对象
				uart = uartDealCreate(hWnd);

				// GPIO操作对象
				Mygpio = myGpioPrivCreate(gpiotbl);
				Mygpio->Init(Mygpio);
				createGpioHandleThread();
				// 看门狗定时器
				irqTimerSigaction();
				irqTimerInit(MAIN_TIMERVAL);
				WatchDogOpen();
				Public.initTouchIrq();
			} return 1;

		case MSG_SERIALPORT:
			{
				uart->FromSingleChip(uart);
			} return 0;

		default:
			return 1;
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief MiniGUIMain 主函数入口
 *
 * @param argc
 * @param argv[]
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
int MiniGUIMain(int argc, const char* argv[])
{
	form_main = formMainCreate(mainAppProc,
			timers_table);

	// 检查数据库完整性
	etcFileCheck("config.ini");
	form_main->loop();

	UdpServer->Destroy(UdpServer);
	Mygpio->Destroy(Mygpio);
	uart->Destroy(uart);
    publicDestroy();
	WatchDogClose();
    return 0;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief gpioHandle Gpio处理函数，50ms处理一次
 *
 * @param arg
 */
/* ---------------------------------------------------------------------------*/
static void * gpioHandle(void *arg)
{
#ifdef PC
    return NULL;
#endif
	while (Mygpio != NULL) {
		Mygpio->Handle(Mygpio);
		usleep(50000);// 50ms
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief createGpioHandleThread 创建Gpio处理线程
 */
/* ---------------------------------------------------------------------------*/
static void createGpioHandleThread(void)
{
	int result;
	pthread_t m_pthread;					//线程号
	pthread_attr_t threadAttr1;				//线程属性
	pthread_attr_init(&threadAttr1);		//附加参数
	//设置线程为自动销毁
	pthread_attr_setdetachstate(&threadAttr1,PTHREAD_CREATE_DETACHED);
	//创建线程，无传递参数
	result = pthread_create(&m_pthread,&threadAttr1,gpioHandle,NULL);
	if(result) {
		printf("[%s] pthread failt,Error code:%d\n",__FUNCTION__,result);
	}
	pthread_attr_destroy(&threadAttr1);		//释放附加参数
}

