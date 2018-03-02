/*
 * =====================================================================================
 *
 *       Filename:  UDPSocketRead.c
 *
 *    Description:  ����Э�����
 *
 *        Version:  1.0
 *        Created:  2015-11-20 14:08:55
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

#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <signal.h>

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
#if DBG_SOCKETREAD > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif


/* ----------------------------------------------------------------*
 *                      variables define
 *-----------------------------------------------------------------*/
/**
 * @brief UDPUpdateProc Զ������
 *
 * @param ABinding
 * @param AData
 *
 * @return
 */
static void udpUpdateProc(SocketHandle *ABinding,SocketPacket *AData)
{
	DBG_P("TP_RETUPDATEMSG:UpdateProc()\n");
	TUpdateRevertFile *cBuf;
	static BOOL bUpdateProc = FALSE;			//�����յ��ظ�������֪ͨ��ʱ
	char *Packet = (char *)&AData->Data[sizeof(COMMUNICATION)];

	if(bUpdateProc == FALSE) {
		bUpdateProc = TRUE;
		cBuf = (TUpdateRevertFile *)malloc(sizeof(TUpdateRevertFile));

		sprintf(cBuf->ip,"%s",ABinding->IP);
		sprintf(cBuf->file,"%s",Packet);
		bUpdateProc = FALSE;
	}
	free(cBuf);  /*ע:������̻߳����ܣ�������ǰ�ͷŻ������(��UIʱ�Ѿ��¿����ڴ�)*/
}


/* ----------------------------------------------------------------*/
/**
 * @brief Э�鴦���
 */
/* ----------------------------------------------------------------*/
static UdpCmdRead udp_cmd_read_handle[] = {
	{TP_RETUPDATEMSG,	udpUpdateProc},
};

/**
 * @brief udpSocketRead UDP�˿ڽ���Э��
 *
 * @param ABinding
 * @param AData
 */
void udpSocketRead(SocketHandle *ABinding,SocketPacket *AData)
{
	COMMUNICATION * head = (COMMUNICATION *)AData->Data;

	if(AData->Size < sizeof(COMMUNICATION))
		return;

	if(AData->Size != head->Size) {
		return;
	}

	// if(strcmp(Public.pCenterMsg->IP, ABinding->IP) == 0) {
		// DBG_P("TP_ONLINE:%s\n",ABinding->IP);
		// Public.CenterOnlineTime = 30;
	// }
	unsigned int i;
	for(i=0; i<NELEMENTS(udp_cmd_read_handle); i++) {
		if (udp_cmd_read_handle[i].cmd == head->Type) {
			udp_cmd_read_handle[i].udpCmdProc(ABinding,AData);
			return;
		}
	} 
	DBG_P("[%s]:IP:%s,Cmd=0x%04x\n",__FUNCTION__,ABinding->IP,head->Type);
}
