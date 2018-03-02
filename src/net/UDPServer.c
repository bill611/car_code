#ifdef WIN32
		#include <stdio.h>
		#include <stdlib.h>
		#include <windows.h>
		#include <winsock.h>
		#define MSG_USER            0x0800
		int MyPostMessage(unsigned int hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
		#define IPDATA sin_addr.S_un.S_addr
		#define MSG_NOSIGNAL (0)
		#define VAILDTIME (5000)			//5秒
		#define TASKTIMEOUT (300)			//300ms
#else
		#include <string.h>
		#include <stdlib.h>
		#include <sys/types.h>
		#include <sys/socket.h>
		#include <netdb.h>
		#include <arpa/inet.h>
		#include <minigui/common.h>
		#include <minigui/minigui.h>
		#include <minigui/gdi.h>
		#include <minigui/window.h>
		#include <minigui/control.h>

		#define MyPostMessage SendNotifyMessage
		#define IPDATA sin_addr.s_addr
		#define closesocket close
		#define VAILDTIME (500)				//5秒
		#define TASKTIMEOUT (20)//(30)			//300ms
#endif

#include "UDPServer.h"
#include "predefine.h"
#include "communication.h"
void ErrorLog(int ecode,const char *fmt,...);
//---------------------------------------------------------------------------
typedef struct UDPTHREADOPS
{
	unsigned int Handle;         //接收信息的窗口包柄
	int m_socket;       //套接字
	int port;           //端口号
	int Terminated;     //是否中止线程
}*PUDPTHREADOPS;

typedef struct
{
	u_long IP;
	unsigned int ID;
	unsigned int dwTick;		//时间
}PACKETSID;
static PACKETSID PacketsID[10];
static int PacketPos;
void DelayMs(int ms);
//---------------------------------------------------------------------------
unsigned int GetIPBySockAddr(const void *Data)
{
	struct sockaddr_in * addr = (struct sockaddr_in*)Data;
	//	return addr->sin_addr.S_un.S_addr;
	return addr->sin_addr.s_addr;
}
//---------------------------------------------------------------------------
unsigned int my_inet_addr(const char *IP)
{
	return inet_addr(IP);
}
//---------------------------------------------------------------------------
int CheckIsIP(const char *IP)
{
	int Flag = 1;
	int i,len,cnt;
	cnt = 0;
	len = strlen(IP);
	for(i=0;i<len;i++) {
		if(IP[i]=='.') {
			cnt++;
		}
	}
	if(cnt<3) {
		Flag = 0;
	} else if(inet_addr(IP)==INADDR_NONE) {
		Flag = 0;
	}
	return Flag;
}
//---------------------------------------------------------------------------
unsigned int GetPortBySockAddr(const void *Data)
{
	struct sockaddr_in * addr = (struct sockaddr_in*)Data;
	return htons(addr->sin_port);
}
//---------------------------------------------------------------------------
static void ProcRetPacket(TUDPServer* This,unsigned int ID)
{
	int i;
	pthread_mutex_lock (&This->mutex);		//加锁
	for(i=0;i<MAXLIST;i++) {
		UDPSendLists *pList = &This->Lists[i];
		if(pList->pData) {
			COMMUNICATION * pHead = (COMMUNICATION*)pList->pData;
			if(pHead->ID==ID) {
				//对方成功接收
				if(pList->hWnd)
					MyPostMessage(pList->hWnd,MSG_SENDMSG,MSG_SENDSUCCESS,(LPARAM)pList->CallBackData);
				else if(pList->Func)
					pList->Func(MSG_SENDSUCCESS,pList->CallBackData);
				free(pList->pData);
				pList->pData = NULL;
				This->ListCnt--;
				break;
			}
		}
	}
	pthread_mutex_unlock (&This->mutex);		//解锁
}
//---------------------------------------------------------------------------
static void ProcTask(TUDPServer* This)
{
	int i;
	pthread_mutex_lock (&This->mutex);		//加锁
	for(i=0;i<MAXLIST;i++) {
		UDPSendLists *pList = &This->Lists[i];
		if(pList->pData) {
			if(pList->SendTimes<pList->Times) {
				//重发送次数未超时
				This->SendBuffer(This,pList->IP,pList->Port,pList->pData,pList->Size);
//				printf("[UDPSend] IP=%s,Port=%d,Size=%d\n", pList->IP,pList->Port,pList->Size);
				pList->SendTimes++;
			} else {
				//发送次数超时
				if(pList->hWnd)
					MyPostMessage(pList->hWnd,MSG_SENDMSG,MSG_SENDTIMEOUT,0);
				else if(pList->Func)
					pList->Func(MSG_SENDTIMEOUT,pList->CallBackData);
				free(pList->pData);
				pList->pData = NULL;
				This->ListCnt--;
			}
		}
	}
	pthread_mutex_unlock (&This->mutex);		//解锁
}
//---------------------------------------------------------------------------
//  UDP Server监听数据线程
//---------------------------------------------------------------------------
static void ProcUdpRecvData(TUDPServer* This,SocketPacket *AData,struct sockaddr_in *from,int fromlen)
{
	int i;
    char *pTmp;
	SocketHandle *ABinding;
	PUDPTHREADOPS control = This->control;
	unsigned int dwTick;

	//回复数据包
	if(AData->Size>12) {
		sendto(control->m_socket,AData->Data,4,0,(struct sockaddr*)from,fromlen);
	} else if(AData->Size==4) {
		//4个字节为对方返回的应答包
		ProcRetPacket(This,*(unsigned int *)AData->Data);
		free(AData);
		return;
	}
	
	dwTick = GetTickCount();
	//排除WAP转发通讯程序的协议,目前无法分辨第一个字节为0xA5并且长度为12或13的数据包
	if(AData->Size>=12) { // || ((unsigned char*)AData->Data)[0]!=0xA5) {
		//判断包是否重发
		for(i=0;i<10;i++) {
			if((from->IPDATA == PacketsID[i].IP && AData->Size>4) &&
				(*(unsigned int*)AData->Data == PacketsID[i].ID) && 
				(dwTick<(PacketsID[i].dwTick+VAILDTIME))) 
			{
				//printf("Packet ID %d is already receive!\n",PacketsID[i].ID);
				free(AData);
				return;
			}
		}
	}
	//保存ID
	PacketsID[PacketPos].IP = from->IPDATA;
	PacketsID[PacketPos].ID = *(int*)AData->Data;
	PacketsID[PacketPos].dwTick = dwTick;
	PacketPos = (++PacketPos) % 10;
    
	ABinding = (SocketHandle*)malloc(sizeof(SocketHandle));
    if(ABinding == NULL) {
        free(AData);
        ErrorLog(0x7FFFFFFF,"udp server No Memory alloc ABinding\n");
        return;
    }
    memset(ABinding,0,sizeof(SocketHandle));
    ABinding->Port = htons(from->sin_port);
    pTmp = inet_ntoa(from->sin_addr);
    if(pTmp) strcpy(ABinding->IP,pTmp);
//	printf("Send MSG_SOCKETREAD message\n");
    MyPostMessage(control->Handle,MSG_SOCKETREAD,(WPARAM)ABinding,(LPARAM)AData);
}
//---------------------------------------------------------------------------
#ifdef WIN32
static DWORD WINAPI UDPThreadExecute(void * ThreadData)
#else
static void * UDPThreadExecute(void *ThreadData)
#endif
{
//	socklen_t addrlen;
	TUDPServer *This = (TUDPServer *)ThreadData;
	PUDPTHREADOPS control = This->control;
	struct timeval timeout;
	fd_set fdR;
	DWORD dwLastTick = GetTickCount();
	while(!control->Terminated)
	{
		FD_ZERO(&fdR);
		FD_SET(control->m_socket, &fdR);
		timeout.tv_sec=0;
		timeout.tv_usec=50000;		//50ms
		switch (select(control->m_socket+1, &fdR,NULL, NULL, &timeout))
		{
		case -1: 
		/*error handled by u; */
			ErrorLog(0,"error handled by u\n");
			goto error;
		case 0:
			{
				DWORD dwTick = GetTickCount();
				if(dwTick - dwLastTick > TASKTIMEOUT)
				{
					ProcTask(This);
					dwLastTick = dwTick;
				}
			}
			break;
		default: 
			if(FD_ISSET(control->m_socket,&fdR)) 
			{ 
				//socket 可读
				struct sockaddr_in from;
				//pBuf 前4个字节用于保存UDP包长
				SocketPacket *AData = (SocketPacket *)malloc(UDP_PACKET_MAX);
				int fromlen = sizeof(struct sockaddr_in);
				memset(&from,0,sizeof(from));
				if(AData == NULL) {
					ErrorLog(0x7FFFFFFF,"UDP Server No Memory alloc\n");
					break;
				}
				if((AData->Size=recvfrom(control->m_socket,AData->Data,UDP_PACKET_MAX-4,MSG_NOSIGNAL,(struct sockaddr*)&from,&fromlen))>0)
				{
					ProcUdpRecvData(This,AData,&from,fromlen);
				}
                else {
                    free(AData);
                }
			} else {
				DelayMs(1);
			}
			do {
				DWORD dwTick = GetTickCount();
				if(dwTick-dwLastTick>TASKTIMEOUT)
				{
					ProcTask(This);
					dwLastTick = dwTick;
				}
			} while(0);
			break;
		}
	}
error:
    PRINTF("UDP Server Thread Exit\n");
	control->Terminated = 1;
	free(control);
	pthread_exit(NULL);
#ifdef WIN32
	return 0;
#else
	return NULL;
#endif
}
//---------------------------------------------------------------------------
static void TUDPServer_Destroy(TUDPServer *This)
{
	This->control->Terminated = 1;
	closesocket(This->control->m_socket);
	free(This);
}
//---------------------------------------------------------------------------
static int TUDPServer_GetSocket(TUDPServer *This)
{
	return This->control->m_socket;
}
//---------------------------------------------------------------------------
int TUDPServer_SendBuffer(TUDPServer *This,const char *IP,int port,const void *pBuf,int size)
{
    struct sockaddr_in *p;
	struct sockaddr addr;
	struct hostent *hostaddr;
	if(This->control->m_socket<0)
		return -1;
	if(IP[0]==0 || port==0)
		return -2;
    memset(&addr,0,sizeof(addr));
	p=(struct sockaddr_in *)&addr;
    p->sin_family=AF_INET;
    p->sin_port=htons(port);
	if(IP[0]<'0' || IP[0]>'9') {
		hostaddr = gethostbyname(IP);
		if(!hostaddr)
		{
			return -3;
		}
		memcpy(&p->sin_addr,hostaddr->h_addr,hostaddr->h_length);
	} else {
		p->sin_addr.s_addr = inet_addr(IP);
		if((p->sin_addr.s_addr == INADDR_NONE) && (strcmp(IP,"255.255.255.255")!=0))
		{
			return -4;
		}
	}
	return sendto(This->control->m_socket,(char*)pBuf,size,MSG_NOSIGNAL,&addr,sizeof(struct sockaddr_in));
}
//---------------------------------------------------------------------------
int TUDPServer_RecvBuffer(TUDPServer *This,void *pBuf,int size,int TimeOut,
						  void * from,int * fromlen)
{
	int iRet;
	struct timeval timeout;
	fd_set fdR;

	if(This->control->m_socket<0)
		return -1;
	if(TimeOut<0) {
		iRet = recvfrom(This->control->m_socket,(char*)pBuf,size,MSG_NOSIGNAL,(struct sockaddr *)from,fromlen);
    	return iRet;
	}
    else
    {
		FD_ZERO(&fdR);
		FD_SET(This->control->m_socket, &fdR);
        timeout.tv_sec=TimeOut / 1000;
        timeout.tv_usec=(TimeOut % 1000)*1000;
		if(select(This->control->m_socket+1, &fdR,NULL, NULL, &timeout)<=0) {
            return -1;
		}
		iRet = recvfrom(This->control->m_socket,(char*)pBuf,size,MSG_NOSIGNAL,(struct sockaddr *)from,fromlen);
    	return iRet;
    }
}
//---------------------------------------------------------------------------
int TUDPServer_Terminated(TUDPServer *This)
{
	return This->control->Terminated;
}
//---------------------------------------------------------------------------
//添加任务
static void TUDPServer_AddTask(TUDPServer* This,const char *IP,int Port,void *pData,int Size,int Times,
							   unsigned int hWnd,CallBackUDP Func,void *CallBackData)
{
	int idx;
	while(This->ListCnt>MAXLIST) {
		DelayMs(100);
	}
	pthread_mutex_lock (&This->mutex);		//加锁
	//搜索空闲任务位置
	for(idx=0;idx<MAXLIST;idx++) {
		if(This->Lists[idx].pData==NULL)
			break;
	}
	//添加任务
	if(idx<MAXLIST)
	{
	This->Lists[idx].pData = malloc(Size);
	memcpy(This->Lists[idx].pData,pData,Size);
	strcpy(This->Lists[idx].IP,IP);
	This->Lists[idx].Port = Port;
	This->Lists[idx].Size = Size;
	This->Lists[idx].SendTimes = 0;
	This->Lists[idx].Times = Times;
	This->Lists[idx].hWnd = hWnd;
	This->Lists[idx].Func = Func;
	This->Lists[idx].CallBackData = CallBackData;
	This->ListCnt++;
	}
	pthread_mutex_unlock (&This->mutex);		//解锁
}
//---------------------------------------------------------------------------
//删除任务
static void TUDPServer_KillTask(TUDPServer* This,const char *IP,int Port)
{
	int i;
	pthread_mutex_lock (&This->mutex);		//加锁
	for(i=0;i<MAXLIST;i++) {
		UDPSendLists *pList = &This->Lists[i];
		if(pList->pData && strcmp(pList->IP,IP)==0 && pList->Port==Port) {
			free(pList->pData);
			pList->pData = NULL;
			This->ListCnt--;
		}
	}
	pthread_mutex_unlock (&This->mutex);		//解锁
}
//---------------------------------------------------------------------------
//创建一个线程，监听指定的端口
//---------------------------------------------------------------------------
TUDPServer* TUDPServer_Create(unsigned int Handle,int Port)
{
	int ret;
	int opt = 1;
#ifdef WIN32
	DWORD id;
#else
	int result;
	pthread_attr_t threadAttr1;			//线程属性
	int trueval = 1;
#endif
	struct sockaddr_in local_addr;
	pthread_mutexattr_t mutexattr;
    TUDPServer* This;
#ifdef WIN32
    WSADATA  wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
        return NULL;
#endif
	This = (TUDPServer *)calloc(sizeof(TUDPServer),1);
    if(This==NULL) {PRINTF("alloc UDPServer memory failt!\n"); goto error;}
	This->control =(PUDPTHREADOPS)calloc(sizeof(struct UDPTHREADOPS),1);
    if(This->control==NULL) {PRINTF("alloc control memory failt!\n"); goto error;}
	This->control->Handle = Handle;
	This->control->port = Port;
	This->control->Terminated = 0;

	pthread_mutexattr_init(&mutexattr);
	/* Set the mutex as a recursive mutex */
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);

	/* create the mutex with the attributes set */
	pthread_mutex_init(&This->mutex, &mutexattr);
	/* destroy the attribute */
	pthread_mutexattr_destroy(&mutexattr);

	memset(&local_addr,0,sizeof(local_addr));
	//初始化套接字
	This->control->m_socket=socket(AF_INET,SOCK_DGRAM,0);
	if(This->control->m_socket < 0) {
		PRINTF("UDP Server init socket failed!\n");
		goto error;
	}
	ret = setsockopt(This->control->m_socket,SOL_SOCKET,SO_BROADCAST,(char*)&opt,sizeof(opt));
	if(ret!=0) {
		printf("setsockopt server error %d\n",ret);
	}
	//绑定套接字
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(Port);
	local_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(This->control->m_socket, (struct sockaddr *)&local_addr, sizeof(struct sockaddr))<0)
	{
		PRINTF("bind to port failed!\n");
        closesocket(This->control->m_socket);
		goto error;
	}

#ifndef WIN32
    //在程序关闭后可以立即使用该端口
    setsockopt(This->control->m_socket,SOL_SOCKET,SO_REUSEADDR,&trueval,sizeof(trueval));
#endif

    This->Destroy = TUDPServer_Destroy;
    This->GetSocket = TUDPServer_GetSocket;
    This->Terminated = TUDPServer_Terminated;
    This->RecvBuffer = TUDPServer_RecvBuffer;
    This->SendBuffer = TUDPServer_SendBuffer;
	This->AddTask = TUDPServer_AddTask;
	This->KillTask = TUDPServer_KillTask;

#ifdef WIN32
	This->m_pthread = CreateThread(NULL, 0, UDPThreadExecute, This, 0, &id); //兼容98 故使用&ID
#else
	pthread_attr_init(&threadAttr1);		//附加参数

	pthread_attr_setdetachstate(&threadAttr1,PTHREAD_CREATE_DETACHED);	//设置线程为自动销毁
	result = pthread_create(&(This->m_pthread),&threadAttr1,UDPThreadExecute,This);	//创建线程
	if(result) {
		//printf(__FUNCTION__" pthread failt,Error code:%d\n",result);
		printf("TUDPServer_Create() pthread failt,Error code:%d\n",result);
	}
	pthread_attr_destroy(&threadAttr1);		//释放附加参数
#endif
	return This;
error:
#ifdef WIN32
    WSACleanup();
#endif
    if(This && This->control)
        free(This->control);
    if(This)
        free(This);
    return NULL;
}
//---------------------------------------------------------------------------
