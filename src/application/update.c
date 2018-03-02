/*
 * =====================================================================================
 *
 *       Filename:  update.c
 *
 *    Description:  升级函数
 *
 *        Version:  1.0
 *        Created:  2015-11-20 10:50:14
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
#include <string.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>

#include "externfunc.h"

/* ----------------------------------------------------------------*
 *                  extern variables declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                  internal functions declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                        macro define
 *-----------------------------------------------------------------*/
#define DBG_UPDATE 0
#if DBG_UPDATE > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

/* ----------------------------------------------------------------*
 *                      variables define
 *-----------------------------------------------------------------*/


/* ----------------------------------------------------------------*/
/**
 * @brief UpdateThreadProc 升级线程执行函数
 *
 * @param arg
 *
 * @returns 
 */
/* ----------------------------------------------------------------*/
static void * UpdateThreadProc(void *arg)
{
	TUpdateRevertFile *recdata = (TUpdateRevertFile *)arg;
	DBG_P("ip:%s,file:%s\n",recdata->ip,recdata->file);

	TRemoteFile * Remote = CreateRemoteFile(recdata->ip,"Update1.cab"); // 此处命名要与最终命名不一样，否则出错
	if(Remote) {
		BOOL bRet = Remote->Download(Remote,Screen.hMainWnd,recdata->file,"/mnt/nand1-2/Update.cab",FALSE);
		Remote->Destroy(Remote);
		if(bRet) {
			// SendMessage(Screen.hUpdate,MSG_UPDATE_SUCCESS,0,0);
			DBG_P("Reboot!! \n");
			// 无界面时需打开
			// WatchDogClose();
			// exit(0);
		} else {
			// SendMessage(Screen.hUpdate,MSG_UPDATE_FAIL,0,0);
			DBG_P("[Update] Download Error!\n");
		}
	} else {
		// SendMessage(Screen.hUpdate,MSG_UPDATE_FAIL,0,0);
		DBG_P("Download fail!! \n");
	}
	free(recdata);
	pthread_exit(NULL);
}



/* ----------------------------------------------------------------*/
/**
 * @brief CreateUpdateThread 创建升级线程
 *
 * @param arg
 */
/* ----------------------------------------------------------------*/
void CreateUpdateThread(void *arg)
{
	int result;
	pthread_t m_pthread;					//线程号
	pthread_attr_t threadAttr1;				//线程属性

	pthread_attr_init(&threadAttr1);		//附加参数

//	TUpdateRevertFile *recdata = (TUpdateRevertFile *)arg;
//	DBG_P("ip:%s,file:%s\n",recdata->ip,recdata->file);

	pthread_attr_setdetachstate(&threadAttr1,PTHREAD_CREATE_DETACHED);	//设置线程为自动销毁
	result = pthread_create(&m_pthread,&threadAttr1,UpdateThreadProc,arg);		//创建线程，无传递参数
	if(result) {
		printf("[%s] pthread failt,Error code:%d\n",__FUNCTION__,result);
	}
	pthread_attr_destroy(&threadAttr1);		//释放附加参数
}


//---------------------------------------------------------------------------

