#ifndef EXTERNFUNC_H
#define EXTERNFUNC_H

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

// include for net
#include "remotefile.h"
#include "TCPClient.h"
#include "UDPClient.h"
#include "UDPServer.h"

// include for drivers
#include "detlink.h"
#include "iwlib.h"
#include "wireless.h"
#include "zorder.h"
#include "port.h"
#include "uart.h"
#include "LinkList.h"
#include "StateMachine.h"
                            
// include for gui
#include "commongdi.h"
#include "MyButton.h"
#include "MyDialog.h"
#include "FormMain.h"
#include "FormTopBox.h"

// include for app
#include "debug.h"
#include "communication.h"
#include "predefine.h"
#include "main.h"
#include "MyGpioCtr.h"
#include "public.h"
#include "UartDeal.h"
#include "UDPSocketRead.h"
#include "update.h"

#ifndef FALSE
    #define FALSE 0
    #define TRUE 1
    #define BOOL int
#endif

typedef struct
{
	char TypeNumber[4];
	char ItemNumber[8];
	char ItemName[30];
	double Price;
	int Amounts;
}SELECTITEM;

//联系人结构
typedef struct
{
	char ID[16];		//编号
	char Name[30];		//名称
	char IP[16];		//IP地址
	int State;			//情形，不同的方式下使用该变量用于传递其它信息
}TerminalInfo;

typedef struct
{
	char Code[20];		//编号 MAX 16Byte And \0
	char Name[24];		//名称 MAX 20+1
	char IP[16];		//IP 地址
}SERVICESSTRUCT;

typedef struct {
	unsigned short wYear;
	unsigned char wMonth;
	unsigned char wDay;
} DATE,*PDATE;

typedef enum {
	VEDIO_CALL_IN_NO_VEDIO,	//正在呼入，无视频
	VEDIO_CALL_IN_VEDIO,	//正在呼入，有视频
	VEDIO_CALL_OUT_NO_VEDIO,//正在呼出，无视频
	VEDIO_CALL_OUT_VEDIO,	//正在呼出，有视频
	VEDIO_CALL_OUT_FAIL,	//呼出失败
}ENUM_VEDIO_STATUS;

typedef struct _st_dir {
	char 	path[128];		// 文件名称 包括路径
	void *	data;			// 文件
}ST_DIR;

//---------------------------------------------------------------------------
//外部函数
//---------------------------------------------------------------------------
int fileexists(const char *FileName);
char *strupper(char *pdst,const char *pstr,int Size);
void DelayMs(int ms);
int ExcuteCmd(int bWait,char *Cmd,...);
void ErrorLog(int ecode,const char *fmt,...);
BOOL IsSameNetMask(const char *IP1,const char *IP2);
char * GetDate(char *cBuf,int Size);
const char * GetSendIP(const char *pSrcIP,const char *pDestIP,const char *pMask);
int jugdeRecIP(const char *pSrcIP,const char *pDestIP,const char *pMask);
unsigned int my_inet_addr(const char *IP);

void SetNetwork(int flag,const char *cIp,const char *cMask,const char *cGateWay,const char *cMac);
void SetNetMac(unsigned char *pImei,char *MAC);
void WatchDogOpen(void);
void WatchDogFeed(void);
int SetValueToEtcFile_Ext(const char* pEtcFile, const char* pSection, const char* pKey, char* pValue);
// unsigned long GetMs(void);
unsigned long long int GetMs(void);
void backData(char *file);
void recoverData(char *file,int reboot);
int etcFileCheck(char *file);
// int sqlCheck(TSqlite **sql,char *file,int reboot);
// void sqlLoad(TSqlite **sql,char *file);
int GetFileSize(char *file);
time_t MyGetTickCount(void);
char *wchar_to_char( char*dst,const char*src,int size);
void talkLedControl(int status);
unsigned long long GetFlashFreeSpace(void);
unsigned long long GetFlashFreeSpaceKB(void);
unsigned long long GetFlashFreeSpaceMB(void);

void SaveLog(const char *fmt,...);
int AdjustLCDLight(int Light,int force_off);
BOOL screensaverStart(BOOL On);
int check_database_sql(void);
int net_detect(char* net_name);
void print_data(char *data,int len);
int GetFilesNum(char *pPathDir,void (*func)(void *));
void clearFramebuffer(void);
BOOL playButtonSound(void);
void WatchDogClose(void);
int GetLocalIP(char *IP,char *NetMask,char *MAC);
#endif

