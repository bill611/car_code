/*
 * =============================================================================
 *
 *       Filename:  FormPassword.c
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
#include "FormBase.h"
#include "externfunc.h"

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static int formPasswordProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initCtrlButtons(HWND hDlg);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);

static void btManagePress(HWND hwnd, int id, int nc, DWORD add_data);
static void btExitPress(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define BMP_LOCAL_PATH "res/image/system1/"
enum {
	IDC_BUTTON_MANAGE,
	IDC_BUTTON_EXIT,
	IDC_LABER_VERSION,
	IDC_LABER_DATE,
};


/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_bkg_system1; // 背景

static BmpLocation bmp_load[] = {
    {&bmp_bkg_system1, BMP_LOCAL_PATH"bkg_system1.JPG"},
};

static MY_CTRLDATA ChildCtrls [] = {
};


static MY_DLGTEMPLATE DlgInitParam =
{
    WS_NONE,
//   WS_EX_AUTOSECONDARYDC,
    WS_EX_NONE,
    0,0,SCR_WIDTH,SCR_HEIGHT,
    "",
    0, 0,       //menu and icon is null
    sizeof(ChildCtrls)/sizeof(MY_CTRLDATA),
    ChildCtrls, //pointer to control array
    0           //additional data,must be zero
};

static FormBasePriv form_base_priv= {
	.name = "Fpwd",
	.dlgProc = formPasswordProc,
	.dlgInitParam = &DlgInitParam,

	.initCtrlLabers =  NULL,
	.initCtrlButtons =  initCtrlButtons,
	.initCtrlEdits =  NULL,
	.initPara =  initPara,
};

static MgCtrlButton otp_controls[] = {
	{IDC_BUTTON_MANAGE,			0,"BUTTON_MANAGE",132,422,212,49,btManagePress}, // 后台管理系统
	{IDC_BUTTON_EXIT,			0,"BUTTON_EXIT",392,205,51,48,btExitPress}, // 退出
};

static FormBase* form_base = NULL;
static int enable_alarm_door,enable_unbroken;
static int current_edit;

/* ----------------------------------------------------------------*/
/**
 * @brief btManagePress 获取设备ID按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void btManagePress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
}

/* ----------------------------------------------------------------*/
/**
 * @brief btExitPress 保存按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void btExitPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
}

/* ----------------------------------------------------------------*/
/**
 * @brief initCtrlButtons 初始化button控件
 *
 * @param hDlg
 */
/* ----------------------------------------------------------------*/
static void initCtrlButtons(HWND hDlg)
{
	int i;
	char image_path[128] = {0};
    printf("[%s]\n", __FUNCTION__);
	for (i=0; i<NELEMENTS(otp_controls); i++) {
        otp_controls[i].display = 1;
		sprintf(image_path,BMP_LOCAL_PATH"%s.JPG",otp_controls[i].img_name);
        bmpLoad(&otp_controls[i].image_normal, image_path);
		sprintf(image_path,BMP_LOCAL_PATH"%s2.JPG",otp_controls[i].img_name);
        bmpLoad(&otp_controls[i].image_press, image_path);
		createSkinButton(hDlg,
				otp_controls[i].idc,
				otp_controls[i].x,
				otp_controls[i].y,
				otp_controls[i].w,
				otp_controls[i].h,
				&otp_controls[i].image_normal,
				&otp_controls[i].image_press,
				otp_controls[i].display,
				otp_controls[i].notif_proc);
	}
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
	char buf[128];
    /*
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_IP),Public.cLocalIP);
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_MASK),Public.cNetMask);
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_GATEWAY),Public.cGateWay);
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_CENTER_IP),Public.pCenterMsg->IP);
	sprintf(buf,"%llX",Public.dwDevID);
	SetWindowText(GetDlgItem(hDlg,IDC_LB_IMEI_ADD),buf);
	SetWindowText(GetDlgItem(hDlg,IDC_LB_MAC_ADD),Public.MAC);

	sprintf(buf,"%s(1-255s)",Word[WORD_ALRM_DOOR].string);
	SetWindowText(GetDlgItem(hDlg,IDC_LB_ALARM_DOOR),buf);
	sprintf(buf,"%d",Public.AlarmDoorTime);
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_ALAMR_DOOR),buf);

	if (Public.EnableAlarmDoor) {
		SendMessage(GetDlgItem(hDlg, IDC_LB_ENABLE_ALARM_DOOR_IMG),
				STM_SETIMAGE, (WPARAM)&Bmp_icon_button_check, 0);
		ShowWindow(GetDlgItem(hDlg, IDC_LB_ALARM_DOOR),SW_SHOWNORMAL);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT_ALAMR_DOOR),SW_SHOWNORMAL);
	}
	if (Public.bUnbrokenUse) {
		SendMessage(GetDlgItem(hDlg, IDC_LB_ENABLE_UBROEKN_IMG),
				STM_SETIMAGE, (WPARAM)&Bmp_icon_button_check, 0);
	}
    */
}

/* ----------------------------------------------------------------*/
/**
 * @brief formPasswordProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formPasswordProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == 0) {
		return 0;
	}

    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

/* ----------------------------------------------------------------*/
/**
 * @brief createFormPassword 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormPassword(HWND hMainWnd)
{
	HWND Form = Screen.Find(form_base_priv.name);
	if(Form) {
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
		form_base_priv.hwnd = hMainWnd;
		form_base_priv.bmp_bkg = &bmp_bkg_system1;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

