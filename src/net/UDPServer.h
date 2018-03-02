#ifndef UDPServerH
#define UDPServerH

#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------------------------------------------------
#include <pthread.h>

#define MSG_SOCKETREAD (MSG_USER+105)
#define MSG_SENDMSG	   (MSG_USER+106)
#define MSG_SENDSUCCESS	 1		//wParam
#define MSG_SENDTIMEOUT	 2		//wParam

typedef struct _SocketHandle
{
    char IP[16];
    int Port;
} SocketHandle;

typedef struct _SocketPacket
{
    int Size;
    char Data[1];
} SocketPacket;

typedef void (*CallBackUDP)(int Ret,void *CallBackData);

typedef struct _UDPSendLists
{
	char IP[16];
	int Port;
	void *pData;
	int Size;
	int Times;
	int SendTimes;
	unsigned int hWnd;
	CallBackUDP Func;
	void *CallBackData;
} UDPSendLists;

#ifndef BOOL
    #define BOOL int
#endif
//----------------------------------------------------------------------------
#define MAXLIST (50)
struct UDPTHREADOPS;

typedef struct _TUDPServer
{
#ifdef WIN32
	void * m_pthread;
#else
    pthread_t m_pthread;		//�̺߳�
#endif
	pthread_mutex_t mutex;		//���п��ƻ����ź�
	UDPSendLists Lists[MAXLIST];
	int ListCnt;
	struct UDPTHREADOPS * control;
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
//----------------------------------------------------------------------------

//  ����һ��UDP������,Handle�Ƿ�����Ϣ�ľ��������Port�˿�
TUDPServer* TUDPServer_Create(unsigned int Handle,int Port);
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
