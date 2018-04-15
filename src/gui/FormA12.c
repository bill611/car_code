/*
 * =============================================================================
 *
 *       Filename:  FormA12.c
 *
 *    Description:  输入密码界面
 *
 *        Version:  1.0
 *        Created:  2018-03-01 23:32:41
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/
#include "externfunc.h"
#include "protocol.h"
#include "commongdi.h"
#include "FormBase.h"
#include "FormMain.h"
#include "predefine.h"

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/
extern void formManiCreateToolBar(HWND hWnd);
extern void formMainUpdateMute(HWND hWnd);

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static int formA12Proc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void optControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define BMP_LOCAL_PATH "预设/"

enum {
	TYPE_A1,
	TYPE_A2,
};
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_title; 
static int mode = TYPE_A1;

static int bmp_load_finished = 0;
static pthread_mutex_t mutex;		//队列控制互斥信号
static pthread_mutexattr_t mutexattr2;

static BmpLocation bmp_load[] = {
    {&bmp_title, BMP_LOCAL_PATH"预设(x69，y89).JPG"},
};

static MY_CTRLDATA ChildCtrls [] = {
};


static MY_DLGTEMPLATE DlgInitParam =
{
    WS_NONE,
    // WS_EX_AUTOSECONDARYDC,
	WS_EX_NONE,
    0,0,SCR_WIDTH,SCR_HEIGHT,
    "",
    0, 0,       //menu and icon is null
    sizeof(ChildCtrls)/sizeof(MY_CTRLDATA),
    ChildCtrls, //pointer to control array
    0           //additional data,must be zero
};

static FormBasePriv form_base_priv= {
	.name = "FA12",
	.dlgProc = formA12Proc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

// device 0x0b
static MgCtrlButton opt_controls[] = {
	{0,	0x93,"1",49,524,64,54},
	{0,	0x94,"2",113,524,65,54},
	{0,	0x95,"3",178,524,62,54},
	{0,	0x96,"4",240,524,64,54},
	{0,	0x97,"5",304,524,63,54},
	{0,	0x98,"6",367,524,65,54},
	{0,	0x99,"7",49,578,64,56},
	{0,	0x9a,"8",113,578,64,56},
	{0,	0x9b,"9",178,578,62,56},
	{0,	0x9c,"10",240,578,64,56},
	{0,	0x9d,"11",304,578,64,56},
	{0,	0x9e,"12",367,578,65,56},
	{0,	0x9f,"13",49,634,64,57},
	{0,	0xa0,"14",113,634,64,57},
	{0,	0xa1,"15",178,634,65,57},
	{0,	0xa2,"16",240,634,64,57},
	{0,	0xa3,"17",304,634,63,57},
	{0,	0xa4,"18",367,634,65,57},
	{0,	0x80,"a",52,117,114,55},
	{0,	0x81,"b",317,117,113,54},
	{0,	0x82,"c",52,279,113,54},
	{0,	0x83,"d",317,279,113,54},
	{0,	0x84,"e",52,340,113,55},
	{0,	0x85,"f",317,340,113,54},
	{0,	0x90,"g",61,465,113,53},
	{0,	0x91,"h",183,465,113,53},
	{0,	0x92,"i",306,465,115,53},
	{0,	0x8d,"播放",208,403,66,60},
	{0,	0x8f,"快进",338,403,71,60},
	{0,	0x8b,"快退",72,403,71,60},
	{0,	0xa5,"上一节",169,343,71,59},
	{0,	0x8c,"停止",143,403,65,60},
	{0,	0xa6,"下一节",240,343,71,59},
	{0,	0x8e,"暂停",274,403,64,60},
	{0,	0x8a,"确认",190,174,101,102},
	{0,	0x86,"向上",190,110,101,64},
	{0,	0x87,"向下",190,276,101,67},
	{0,	0x89,"向右",291,174,67,102},
	{0,	0x88,"向左",124,174,66,102},
};

static FormBase* form_base = NULL;

/* ----------------------------------------------------------------*/
/**
 * @brief optControlsNotify
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void optControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	int device_id;
	// saveLog("id:%d\n", id);
	if (mode == TYPE_A1)
		device_id = 0x17;
	else
		device_id = 0x19;
	pro_com->sendOpt(device_id, opt_controls[id].op_code);
}

void formA12LoadLock(void)
{
    INIT_MUTEX_LOCK(mutexattr2,mutex);
}
void formA12LoadBmp(void)
{
	int i;
	char image_path[128] = {0};
    if (g_config.device_main_controls[IDC_A1] == 0
            && g_config.device_main_controls[IDC_A2] == 0)
        return;
	// pthread_mutex_lock(&mutex);
    if (bmp_load_finished == 1) {
        // pthread_mutex_unlock(&mutex);
        return;
    }
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
	for (i=0; i<NELEMENTS(opt_controls); i++) {
		sprintf(image_path,BMP_LOCAL_PATH"%s(X%d-Y%d).JPG",opt_controls[i].img_name,
				opt_controls[i].x,
				opt_controls[i].y);
		bmpLoad(&opt_controls[i].image_normal, image_path);
		sprintf(image_path,BMP_LOCAL_PATH"%s-2(X%d-Y%d).JPG",opt_controls[i].img_name,
				opt_controls[i].x,
				opt_controls[i].y);
		bmpLoad(&opt_controls[i].image_press, image_path);
	}
	bmp_load_finished = 1;
    // pthread_mutex_unlock(&mutex);
}
/* ----------------------------------------------------------------*/
/**
 * @brief initPara 初始化参数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 */
/* ----------------------------------------------------------------*/
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	int i;
    // formA12LoadBmp();
	for (i=0; i<NELEMENTS(opt_controls); i++) {
		opt_controls[i].idc = i;
		opt_controls[i].device_id = 0x15;
		opt_controls[i].notif_proc = optControlsNotify;
		createSkinButton(hDlg,
				opt_controls[i].idc,
				opt_controls[i].x,
				opt_controls[i].y,
				opt_controls[i].w,
				opt_controls[i].h,
				&opt_controls[i].image_normal,
				&opt_controls[i].image_press,
				1, 0,
				opt_controls[i].notif_proc);
	}
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|WS_VISIBLE|SS_BITMAP,
            WS_EX_TRANSPARENT,
            i,
            69,89,108,31,
            hDlg, NULL, NULL,
            (DWORD)&bmp_title);
	formManiCreateToolBar(hDlg);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formA12Proc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formA12Proc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_UPDATESTATUS:
			{
				formMainUpdateMute(hDlg);
			} break;
		default:
			break;
	}
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == 0) {
		return 0;
	}

    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief createFormA12 
 *
 * @param hMainWnd
 * @param type 0为A1，1为A2
 *
 * @returns 
 */
/* ---------------------------------------------------------------------------*/
int createFormA12(HWND hMainWnd,int type)
{
	HWND Form = Screen.Find(form_base_priv.name);
	mode = type;
	if(Form) {
		SendMessage(Form,MSG_UPDATESTATUS,0,0);
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
        if (bmp_load_finished == 0) {
            topMessage(hMainWnd,TOPBOX_ICON_LOADING,NULL );
            return 0;
        }
		form_base_priv.hwnd = hMainWnd;
		form_base_priv.bmp_bkg = &bmp_bkg2;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

