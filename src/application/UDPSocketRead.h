/*
 * =====================================================================================
 *
 *       Filename:  UDPSocketRead.h
 *
 *    Description:  协议接收
 *
 *        Version:  1.0
 *        Created:  2015-11-20 14:11:16 
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _UDPSOCKETREAD_H
#define _UDPSOCKETREAD_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	typedef struct _UdpCmdRead {
		unsigned int cmd;
		void (*udpCmdProc)( struct _SocketHandle *ABinding,
				struct _SocketPacket *AData);	//呼叫对讲协议处理
	}UdpCmdRead;

	void udpSocketRead(struct _SocketHandle *ABinding,struct _SocketPacket *AData);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

