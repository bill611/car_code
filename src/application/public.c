/*
 * =====================================================================================
 *
 *       Filename:  public.c
 *
 *    Description:  初始化全局变量
 *
 *        Version:  1.0
 *        Created:  2015-11-25 18:08:06
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
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>

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
#if DBG_PUBLIC > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define SIZE_CONFIG(x) x, sizeof(x) - 1


/* ----------------------------------------------------------------*
 *                      variables define
 *-----------------------------------------------------------------*/
PUBLIC_DATA Public;
SCREEN_FORM Screen;
TUDPClient *udpclient;

ConfigFile g_config;

static EtcValueInt etc_value_int[]={
{"MainPageDevice",	"电动座椅", &g_config.device_main_controls[0],	1},
{"MainPageDevice",	"秘书椅",   &g_config.device_main_controls[1],	1},
{"MainPageDevice",	"旋转座椅", &g_config.device_main_controls[2],	1},
{"MainPageDevice",	"窗帘",     &g_config.device_main_controls[3],	1},
{"MainPageDevice",	"玻璃屏",   &g_config.device_main_controls[4],	1},
{"MainPageDevice",	"电视屏幕", &g_config.device_main_controls[5],	1},
{"MainPageDevice",	"CD",       &g_config.device_main_controls[6],	1},
{"MainPageDevice",	"DVD",      &g_config.device_main_controls[7],	1},
{"MainPageDevice",	"监控",     &g_config.device_main_controls[8],	1},
{"MainPageDevice",	"车门",     &g_config.device_main_controls[9],	1},
{"MainPageDevice",	"电动床",   &g_config.device_main_controls[10],	1},
{"MainPageDevice",	"卫星电视", &g_config.device_main_controls[11],	1},
{"MainPageDevice",	"天窗",     &g_config.device_main_controls[12],	1},
{"MainPageDevice",	"灯光",     &g_config.device_main_controls[13],	1},
{"MainPageDevice",	"屏幕投影", &g_config.device_main_controls[14],	1},
{"MainPageDevice",	"桌板",     &g_config.device_main_controls[15],	1},
{"MainPageDevice",	"A1",       &g_config.device_main_controls[16],	0},
{"MainPageDevice",	"A2",       &g_config.device_main_controls[17],	0},
{"MainPageDevice",	"WALN",     &g_config.device_main_controls[18],	1},
{"MainPageDevice",	"系统",     &g_config.device_main_controls[19],	1},
};

static EtcValueChar etc_value_char[]={
{"Public",	"系统密码",         SIZE_CONFIG(g_config.password),	"0000"},
};

//--------------------------------------------------------------------------
static BOOL screenAddForm(HWND hWnd,const char *Class)
{
	PFormClass Form = (PFormClass)malloc(sizeof(FormClass));
	memset(Form,0,sizeof(FormClass));
	Form->hWnd = hWnd;
	strncpy(Form->Class,Class,15);
	if(Screen.head == NULL) {
		Screen.head = Screen.tail = Form;
	} else {
		Screen.tail->next = Form;
		Screen.tail = Form;
	}
	Screen.Count++;
//	DBG_P("Form(%s)Add\n",Class);
	return TRUE;
}
//--------------------------------------------------------------------------
static BOOL screenDelForm(HWND hWnd)
{
	PFormClass form,parentform;
	form = Screen.head;
	while(form)	{
		if(form->hWnd != hWnd) {
			parentform = form;
			form = form->next;
			continue;
		}
		// DBG_P("Form(%s)Del\n",form->Class);
		if(Screen.head == form) {
			Screen.head = form->next;
			if(Screen.tail == form) Screen.tail = NULL;
		} else {
			parentform->next = form->next;
			if(Screen.tail == form) Screen.tail = parentform;
		}
		free(form);
		Screen.Count--;
		return TRUE;
	}
	return FALSE;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief screenReturnMainForm 回到主界面
 */
/* ---------------------------------------------------------------------------*/
static void screenReturnMainForm(void)
{
	int FormCnt=0;
	HWND *Forms;
	PFormClass form;
	form = Screen.head;
	Forms = (HWND *)calloc(sizeof(HWND),Screen.Count);
	while(form && FormCnt<Screen.Count) {
		if(strcmp(form->Class,"TFrmMain"))
			Forms[FormCnt++] = form->hWnd;
		form = form->next;
	}
	while(FormCnt) {
        ShowWindow(Forms[FormCnt-1],SW_HIDE);
		// SendNotifyMessage(Forms[FormCnt-1],MSG_CLOSE,0,-1);
		FormCnt--;
	}
	free(Forms);
	//中断SQL SERVER的连接
//	Query->DisConnect(Query,TRUE);
//	TempQuery->DisConnect(TempQuery,TRUE);
}

//--------------------------------------------------------------------------
static HWND screenFindForm(const char *Class)
{
	PFormClass form = Screen.head;
	if(Class==NULL)
		return 0;

	while(form) {
		if(strncmp(Class,form->Class,15)==0)
			return form->hWnd;
		form = form->next;
	}
	return 0;
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief screenForeachForm 遍历所有窗口，发送消息
 *
 * @param Class
 *
 * @returns 
 */
/* ---------------------------------------------------------------------------*/
static void screenForeachForm(int iMsg, WPARAM wParam, LPARAM lParam)
{
	int FormCnt=0;
	PFormClass form;
	form = Screen.head;
	while(form && FormCnt<Screen.Count) {
		SendMessage(form->hWnd,iMsg,wParam,lParam);
		form = form->next;
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief publicInitTouchIrq 开启触摸屏中断
 */
/* ---------------------------------------------------------------------------*/
static void publicInitTouchIrq(void)
{
#ifndef PC
	ExcuteCmd(1,"cat","/sys/ft5x0x/touch_dev",NULL);	
#endif
}
static int saveConfig(void)
{
    int i;
    char buf[8] = {0};
    for (i=0; i<NELEMENTS(etc_value_int); i++) {
        sprintf(buf,"%d",*etc_value_int[i].value);
        SetValueToEtcFile("config.ini",
                    etc_value_int[i].section,
                    etc_value_int[i].key,
                    buf);
    }
	if (etcFileCheck("config.ini") == TRUE) {//检查配置文件并备份
	    backData("config.ini");
	}
    sync();
        
    return 1;
}

static void getConfigInt(char *section,char *key,int *value)
{

}
static void getConfigChar(char *section,char *key,char *value)
{
    
}
/* ----------------------------------------------------------------*/
/**
 * @brief publicInit 全局数据初始化
 */
/* ----------------------------------------------------------------*/
void publicInit(void)
{
    int hFile;
	
	memset(&Screen,0,sizeof(SCREEN_FORM));
	Screen.Add = screenAddForm;
	Screen.Del = screenDelForm;
	Screen.Find = screenFindForm;
	Screen.ReturnMain = screenReturnMainForm;
	Screen.foreachForm = screenForeachForm;

	Public.WatchDog_fd = -2;

    Public.saveConfig = saveConfig;
    Public.getConfigInt = getConfigInt;
    Public.getConfigChar = getConfigChar;
	Public.initTouchIrq = publicInitTouchIrq;


	hFile = LoadEtcFile("config.ini");

	if(hFile != ETC_FILENOTFOUND) {
		unsigned long i;

		for (i=0; i<NELEMENTS(etc_value_int); i++) {
				if(GetIntValueFromEtc(hFile,
							etc_value_int[i].section,
							etc_value_int[i].key,
							etc_value_int[i].value)!=ETC_OK) {
					printf("Err get int etc sec:%s,key:%s\n",
							etc_value_int[i].section,etc_value_int[i].key );
					*etc_value_int[i].value = etc_value_int[i].default_int;
				}
		}
		for (i=0; i<NELEMENTS(etc_value_char); i++) {
				if(GetValueFromEtc(hFile,
							etc_value_char[i].section,
							etc_value_char[i].key,
							etc_value_char[i].value,
							etc_value_char[i].leng)!=ETC_OK) {
					printf("Err get char etc sec:%s,key:%s\n",
							etc_value_char[i].section,etc_value_char[i].key );
					strcpy(etc_value_char[i].value,etc_value_char[i].default_char);
				}
		}

		UnloadEtcFile(hFile);
	}
	etcFileCheck("config.ini");

	// ExcuteCmd(1,"tar","xzf","image.tar.gz","-C","/image",NULL);	
	//取本机IP
    // GetLocalIP(Public.cLocalIP,Public.cNetMask,Public.MAC);
    // printf("LocalIP=%s, cNetMask=%s, MAC=%s\n",Public.cLocalIP, Public.cNetMask, Public.MAC);

	//创建 UDP客户端
	udpclient = TUDPClient_Create(0);

	Screen.Width = SCR_WIDTH;
	Screen.Height = SCR_HEIGHT;
	printf("Version :%s\n",CODE_VERSION); 
}
//--------------------------------------------------------------------------
void publicDestroy(void)
{
    if(udpclient)
		udpclient->Destroy(udpclient);

}


