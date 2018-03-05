/*
 * =============================================================================
 *
 *       Filename:  UDPServer.h
 *
 *    Description:  udpÇý¶¯
 *
 *        Version:  1.0
 *        Created:  2018-03-05 17:31:34 
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
#ifndef _UDPSERVER_H
#define _UDPSERVER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define MSG_SENDSUCCESS	 1		//wParam
#define MSG_SENDTIMEOUT	 2		//wParam

#define IPDATA sin_addr.s_addr

#define VAILDTIME 	(5000)				//5Ãë
#define TASKTIMEOUT (200)//(30)			//300ms
#define MAXLIST 	(50)

	typedef struct _SocketHandle {
		char IP[16];
		int Port;
	} SocketHandle;

	typedef struct _SocketPacket {
		int Size;
		char Data[1];
	} SocketPacket;

	typedef struct _UdpSocket {
		int handle;
		SocketHandle *ABinding;
		SocketPacket *AData;
	}UdpSocket;

	typedef void (*CallBackUDP)(int Ret,void *CallBackData);

	struct _udp_serverPriv;
	typedef struct _TUDPServer {
		struct _udp_serverPriv *priv;
		void (* Destroy)(struct _TUDPServer *This);
		int (* GetSocket)(struct _TUDPServer *This);
		int (* Terminated)(struct _TUDPServer *This);
		int (* RecvBuffer)(struct _TUDPServer *This,void *buf,int count,int TimeOut,
				void * from,int * fromlen);
		int (* SendBuffer)(struct _TUDPServer *This,const char *IP,int port,const void *buf,int count);
		void (*AddTask)(struct _TUDPServer *This,const char *IP,int Port,void *pData,int Size,int Times,
				unsigned int hWnd,CallBackUDP Func,void *CallBackData);
		void (*KillTask)(struct _TUDPServer *This,const char *IP,int Port);
	} TUDPServer;


	void udpServerInit(int port);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
