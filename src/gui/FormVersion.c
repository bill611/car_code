/*
 * =============================================================================
 *
 *       Filename:  FormVersion.c
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
#include "predefine.h"

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/
extern int createFormPassword(HWND hMainWnd);

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static int formVersionProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
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
    STATIC_LB_L(178,333,200,30,IDC_LABER_VERSION,"",0,NULL),
    STATIC_LB_L(178,376,200,30,IDC_LABER_DATE,"",0,NULL),
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
	.name = "Fver",
	.dlgProc = formVersionProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

static MgCtrlButton otp_controls[] = {
	{IDC_BUTTON_MANAGE,	0,"BUTTON_MANAGE",132,422,212,49,btManagePress}, // 后台管理系统
	{IDC_BUTTON_EXIT,	0,"BUTTON_EXIT",392,205,51,48,btExitPress}, // 退出
};

static FormBase* form_base = NULL;

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
    createFormPassword(GetParent(hwnd));
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
    ShowWindow(GetParent(hwnd),SW_HIDE);
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
	char image_path[128] = {0};
    printf("create %s\n",__FILE__);
    bmpsLoad(bmp_load,NELEMENTS(bmp_load));
	SetWindowText(GetDlgItem(hDlg,IDC_LABER_VERSION),CODE_VERSION);
	SetWindowText(GetDlgItem(hDlg,IDC_LABER_DATE),__DATE__);
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
 * @brief formVersionProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formVersionProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == 0) {
		return 0;
	}

    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

/* ----------------------------------------------------------------*/
/**
 * @brief createFormVersion 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormVersion(HWND hMainWnd)
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

