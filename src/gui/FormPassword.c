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
static void updateReminder(HWND hwnd,char *string);
static void formInitCtrlLabers(HWND hDlg);
static void formInitCtrlButtons(HWND hDlg);
static void formInitCtrlEdits(HWND hDlg);
static void formInitPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);

static void btGetID(HWND hwnd, int id, int nc, DWORD add_data);
static void btSave(HWND hwnd, int id, int nc, DWORD add_data);
static void btBack(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define MSG_UPDATE_REMINDER		 	(MSG_USER + 1)

enum {
	IDC_TIMER_GETID = 10,

	IDC_LB_TITLE,
	IDC_LB_IP,
	IDC_LB_MASK,
	IDC_LB_GATEWAY,
	IDC_LB_CENTER_IP,
	IDC_LB_IMEI,
	IDC_LB_IMEI_ADD,
	IDC_LB_MAC,
	IDC_LB_MAC_ADD,
	IDC_LB_ENABLE_UBROEKN,
	IDC_LB_ENABLE_ALARM_DOOR,
	IDC_LB_ALARM_DOOR,
	IDC_LB_ENABLE_UBROEKN_IMG,
	IDC_LB_ENABLE_ALARM_DOOR_IMG,
	IDC_LB_REMINDER,

	IDC_EDIT_IP,
	IDC_EDIT_MASK,
	IDC_EDIT_GATEWAY,
	IDC_EDIT_CENTER_IP,
	IDC_EDIT_ALAMR_DOOR,

	IDC_BT_GET_IMEI,
	IDC_BT_SAVE,
	IDC_BT_BACK,
	IDC_MYKEYBOARD
};


/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/

static MY_CTRLDATA ChildCtrls [] = {
	STATIC_LB  (X_LB_TITLE, Y_LB_TITLE, W_LB_TITLE, H_LB_TITLE,
			IDC_LB_TITLE,
			Word[WORD_SETTING_LOCAL].string,
			0,
			&TEXT_FONT),
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

	.fontKeyboard = &BUTTON_FONT,
	.initCtrlLabers =  formInitCtrlLabers,
	.initCtrlButtons =  formInitCtrlButtons,
	.initCtrlEdits =  formInitCtrlEdits,
	.initPara =  formInitPara,
};

static FormBase* form_base = NULL;
static int enable_alarm_door,enable_unbroken;
static int current_edit;

/* ----------------------------------------------------------------*/
/**
 * @brief updateReminder 更新按键提示语言
 *
 * @param hwnd
 * @param string
 */
/* ----------------------------------------------------------------*/
static void updateReminder(HWND hwnd,char *string)
{
	SendMessage(hwnd, MSG_UPDATE_REMINDER,
			0,
			(LPARAM)string);
}

/* ----------------------------------------------------------------*/
/**
 * @brief editNotify 编辑框回调函数，用于获得焦点时设置光标位置
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void editNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	char text[50];
	if (nc == EN_SETFOCUS){
		int ip_len = SendMessage (GetDlgItem(GetParent(hwnd),id), MSG_GETTEXT, 50,  (LPARAM)text);
		SendMessage (GetDlgItem(GetParent(hwnd),id), EM_SETCARETPOS, 0, ip_len);
	} else if (nc == EN_KILLFOCUS) {
		current_edit = id;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief unBrokenNotify 单击报警图片静态控件回调函数
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void unBrokenNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	enable_unbroken ^= 1;
	if (enable_unbroken){
		SendMessage(GetDlgItem(GetParent(hwnd), IDC_LB_ENABLE_UBROEKN_IMG),
				STM_SETIMAGE, (WPARAM)&Bmp_icon_button_check, 0);
	} else {
		SendMessage(GetDlgItem(GetParent(hwnd), IDC_LB_ENABLE_UBROEKN_IMG),
				STM_SETIMAGE, (WPARAM)&Bmp_icon_button_uncheck, 0);
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief btGetID 获取设备ID按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void btGetID(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
}

/* ----------------------------------------------------------------*/
/**
 * @brief btSave 保存按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void btSave(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
}

/* ----------------------------------------------------------------*/
/**
 * @brief btBack 返回按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void btBack(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
}

/* ----------------------------------------------------------------*/
/**
 * @brief alarmNotify 单击报警图片静态控件回调函数
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void alarmNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	enable_alarm_door ^= 1;
	if (enable_alarm_door){
		SendMessage(GetDlgItem(GetParent(hwnd), IDC_LB_ENABLE_ALARM_DOOR_IMG),
				STM_SETIMAGE, (WPARAM)&Bmp_icon_button_check, 0);
		ShowWindow(GetDlgItem(GetParent(hwnd), IDC_LB_ALARM_DOOR),SW_SHOWNORMAL);
		ShowWindow(GetDlgItem(GetParent(hwnd), IDC_EDIT_ALAMR_DOOR),SW_SHOWNORMAL);
	} else {
		SendMessage(GetDlgItem(GetParent(hwnd), IDC_LB_ENABLE_ALARM_DOOR_IMG),
				STM_SETIMAGE, (WPARAM)&Bmp_icon_button_uncheck, 0);
		ShowWindow(GetDlgItem(GetParent(hwnd), IDC_LB_ALARM_DOOR),SW_HIDE);
		ShowWindow(GetDlgItem(GetParent(hwnd), IDC_EDIT_ALAMR_DOOR),SW_HIDE);
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief formInitCtrlLabers 初始化Laber控件
 *
 * @param hDlg
 */
/* ----------------------------------------------------------------*/
static void formInitCtrlLabers(HWND hDlg)
{
	int i;
	for (i=IDC_LB_TITLE;i<=IDC_LB_ALARM_DOOR; i++){
		SetWindowElementAttr(GetDlgItem(hDlg,i), WE_FGC_WINDOW,MY_WHIGHT);
	}
	SetWindowElementAttr(GetDlgItem(hDlg,IDC_LB_REMINDER), WE_FGC_WINDOW,MY_RED);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formInitCtrlEdits 初始化Edit控件
 *
 * @param hDlg
 */
/* ----------------------------------------------------------------*/
static void formInitCtrlEdits(HWND hDlg)
{
	int i;
	for (i=IDC_EDIT_IP;i<=IDC_EDIT_ALAMR_DOOR; i++){
		SetWindowElementAttr(GetDlgItem(hDlg,i), WE_FGC_WINDOW,MY_BLACK);
		SendDlgItemMessage(hDlg,i, EM_LIMITTEXT, 20, 0L);
		SetNotificationCallback (GetDlgItem(hDlg,i), editNotify);
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief formInitCtrlButtons 初始化button控件
 *
 * @param hDlg
 */
/* ----------------------------------------------------------------*/
static void formInitCtrlButtons(HWND hDlg)
{
	SetNotificationCallback (GetDlgItem(hDlg,IDC_LB_ENABLE_ALARM_DOOR_IMG), alarmNotify);
	SetNotificationCallback (GetDlgItem(hDlg,IDC_LB_ENABLE_ALARM_DOOR), alarmNotify);
	SetNotificationCallback (GetDlgItem(hDlg,IDC_LB_ENABLE_UBROEKN_IMG), unBrokenNotify);
	SetNotificationCallback (GetDlgItem(hDlg,IDC_LB_ENABLE_UBROEKN), unBrokenNotify);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formInitPara 初始化参数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 */
/* ----------------------------------------------------------------*/
static void formInitPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	char buf[128];
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
	switch (message)
	{
		case MSG_INITDIALOG:
			enable_alarm_door = Public.EnableAlarmDoor;
			enable_unbroken = Public.bUnbrokenUse;
			break;

		case MSG_TIMER:
			{
				if (wParam != IDC_TIMER_GETID) {
					break;
				}
				char buf[128];
				KillTimer(hDlg,IDC_TIMER_GETID);
				if(Public.dwDevIDTemp != Public.dwDevID) {
					updateReminder(hDlg,Word[WORD_GET_DEVICE_ID_SUCCESS].string);
					sprintf(buf,"%llx",Public.dwDevIDTemp);
					SetWindowText(GetDlgItem(hDlg,IDC_LB_IMEI_ADD),buf);
				} else {
					updateReminder(hDlg,Word[WORD_GET_DEVICE_ID_FAIL].string);
				}
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
		form_base_priv.bmp_bkg = Bmp_bkg;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

