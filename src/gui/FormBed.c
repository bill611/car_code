/*
 * =============================================================================
 *
 *       Filename:  FormBed.c
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
static int formBedProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void optNoticLieNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optNoticUnLieNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optMultiControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define IDC_FOMR_TIMER 0xff

#define BMP_LOCAL_PATH BMP_RES_PATH"电动床/"

typedef struct _BedBox{
    HWND idc;
    BITMAP bmp;
}BedBox;
typedef struct _BedBoxButton{
    HWND idc;
    BITMAP bmp[2];
}BedBoxButton;
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_title,bmp_icon; 

static int bmp_load_finished = 0;
static pthread_mutex_t mutex;		//队列控制互斥信号
static pthread_mutexattr_t mutexattr2;

static BmpLocation bmp_load[] = {
    {&bmp_title, BMP_LOCAL_PATH"电动床(x69，y89).JPG"},
    {&bmp_icon, BMP_LOCAL_PATH"电动床图标(x98，y184).JPG"},
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
	.name = "FBed",
	.dlgProc = formBedProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

// device 0x0b
static MgCtrlButton opt_controls[] = {
	{0,	0x81,"靠背后倾",266,469,114,55,optMultiControlsNotify},
	{0,	0x80,"靠背前倾",101,469,115,55,optMultiControlsNotify},
	{0,	0x83,"全收",101,564,115,56,optNoticUnLieNotify},
	{0,	0x82,"全躺",266,564,114,56,optNoticLieNotify},
};

static FormBase* form_base = NULL;
static MgCtrlButton * multi_ctrl = NULL; // 多次连续触发按键

/* ---------------------------------------------------------------------------*/
/**
 * @brief btConfirmPress 确认键回调
 */
/* ---------------------------------------------------------------------------*/
static void btConfirmPress(void)
{
    pro_com->sendOpt(multi_ctrl->device_id, multi_ctrl->op_code);
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief sendOptCmd 根据id查找当前触发控件
 *
 * @param id 触发控件id
 *
 * @returns 控件
 */
/* ---------------------------------------------------------------------------*/
static MgCtrlButton * sendOptCmd(HWND id)
{
	int i;
	MgCtrlButton *ctrl = NULL;
    for (i=0; i<NELEMENTS(opt_controls); i++) {
		if (opt_controls[i].idc == id) {
			ctrl = &opt_controls[i];
			break;
		}
    }
	// pro_com->sendOpt(ctrl->device_id, ctrl->op_code);
	return ctrl;
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief optNoticLieNotify 单次触发按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optNoticLieNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	saveLog("[%s]id:%d\n",__FUNCTION__, id);
    multi_ctrl = sendOptCmd(id);
    topMessage(GetParent (hwnd),TOPBOX_BED_LIE,btConfirmPress );
}
static void optNoticUnLieNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	saveLog("[%s]id:%d\n",__FUNCTION__, id);
    multi_ctrl = sendOptCmd(id);
    topMessage(GetParent (hwnd),TOPBOX_BED_UNLIE,btConfirmPress );
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief optMultiControlsNotify 多次触发按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optMultiControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc == BN_PUSHED) {
		SetTimer(GetParent (hwnd),IDC_FOMR_TIMER,TIME_500MS);
		multi_ctrl = sendOptCmd(id);
		return;
	}
	if (nc == BN_UNPUSHED) {
		KillTimer (GetParent (hwnd),IDC_FOMR_TIMER);
		multi_ctrl = sendOptCmd(id);
        pro_com->sendOpt(multi_ctrl->device_id, multi_ctrl->op_code);
	}
}

void formBedLoadLock(void)
{
    INIT_MUTEX_LOCK(mutexattr2,mutex);
}
void formBedLoadBmp(void)
{
	int i;
	char image_path[128] = {0};
    if (g_config.device_main_controls[IDC_BED_ELE] == 0)
        return;
	// pthread_mutex_lock(&mutex);
    if (bmp_load_finished == 1) {
        // pthread_mutex_unlock(&mutex);
        return;
    }
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
	for (i=0; i<NELEMENTS(opt_controls); i++) {
		sprintf(image_path,BMP_LOCAL_PATH"%s(x%d，y%d).JPG",opt_controls[i].img_name,
				opt_controls[i].x,
				opt_controls[i].y);
		bmpLoad(&opt_controls[i].image_normal, image_path);
		sprintf(image_path,BMP_LOCAL_PATH"%s-2(x%d，y%d).JPG",opt_controls[i].img_name,
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
    // formBedLoadBmp();
	for (i=0; i<NELEMENTS(opt_controls); i++) {
		opt_controls[i].idc = i;
		opt_controls[i].device_id = 0x06;
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
            i++,
            69,89,107,28,
            hDlg, NULL, NULL,
            (DWORD)&bmp_title);
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|WS_VISIBLE|SS_BITMAP,
            WS_EX_TRANSPARENT,
            i++,
            98,184,285,231,
            hDlg, NULL, NULL,
            (DWORD)&bmp_icon);
	formManiCreateToolBar(hDlg);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formBedProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formBedProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_UPDATESTATUS:
			{
				formMainUpdateMute(hDlg);
			} break;
		case MSG_TIMER:
			{
				if (wParam != IDC_FOMR_TIMER)
					break;
				
				if (multi_ctrl == NULL)
					break;

				pro_com->sendOpt(multi_ctrl->device_id, multi_ctrl->op_code);
			} break;
		default:
			break;
	}
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == 0) {
		return 0;
	}

    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

/* ----------------------------------------------------------------*/
/**
 * @brief createFormBed 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormBed(HWND hMainWnd)
{
	HWND Form = Screen.Find(form_base_priv.name);
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

