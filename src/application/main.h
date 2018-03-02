/*
 * =====================================================================================
 *
 *       Filename:  main.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2015-10-30 10:39:50
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _MAIN_H
#define _MAIN_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


	extern struct _FormMain *form_main;
	extern struct _TSqlite *LocalQuery;
	extern struct _PreviewStruct *preview;
	extern struct _DownLoadData *DownData;
	extern struct _TUDPClient *udpclient;
	extern struct _TUDPServer *UdpServer;
	extern struct _UartDeal *uart;
	extern struct _RfCard *Card;
	extern struct _BlueTooth *pBlueTooth;
	extern struct _PlayMp3Struct *PlayMp3;
	extern struct _PlayMp3Struct *PlayUnlock;
	extern struct _DownLoadData *DownData;
	extern struct _PreviewStruct *preview;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

