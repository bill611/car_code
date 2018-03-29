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
#include "externfunc.h"
#include "protocol.h"
#include "commongdi.h"
#include "FormBase.h"
#include "FormMain.h"
#include "predefine.h"

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static int formPasswordProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);

static void btKyeboardNumPress(HWND hwnd, int id, int nc, DWORD add_data);
static void btConfirmPress(HWND hwnd, int id, int nc, DWORD add_data);
static void btExitPress(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define BMP_LOCAL_PATH "res/image/系统-2/"
enum {
	IDC_BUTTON_0,
	IDC_BUTTON_1,
	IDC_BUTTON_2,
	IDC_BUTTON_3,
	IDC_BUTTON_4,
	IDC_BUTTON_5,
	IDC_BUTTON_6,
	IDC_BUTTON_7,
	IDC_BUTTON_8,
	IDC_BUTTON_9,
	IDC_BUTTON_CLEAR,
	IDC_BUTTON_DELET,
	IDC_BUTTON_EXIT,
	IDC_BUTTON_CONFIRM,

	IDC_LABER_ERR,
	IDC_BUTTON_ERR_CONFIRM,

	IDC_EDIT_PASSWORD,
};


/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_bkg_system2; // 背景
static BITMAP bmp_err; // 错误框
static BITMAP bmp_err_confirm; // 错误框确定
static BITMAP bmp_err_confirm1; // 错误框确定按下

static int bmp_load_finished = 0;
static pthread_mutex_t mutex;		//队列控制互斥信号
static pthread_mutexattr_t mutexattr2;

static BmpLocation bmp_load[] = {
    {&bmp_bkg_system2, BMP_LOCAL_PATH"bkg_system2.JPG"},
    {&bmp_err, BMP_LOCAL_PATH"密码错误提示框（X95，Y254）.JPG"},
    {&bmp_err_confirm, BMP_LOCAL_PATH"确定（X184,Y365）.JPG"},
    {&bmp_err_confirm1, BMP_LOCAL_PATH"确定亮（X184,Y365）.JPG"},
};

static MY_CTRLDATA ChildCtrls [] = {
    EDIT_PSD(195,286,180,20,IDC_EDIT_PASSWORD,"",0,NULL),
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
	.name = "Fpwd",
	.dlgProc = formPasswordProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

static MgCtrlButton opt_controls[] = {
	{IDC_BUTTON_0,	0,"0",294,385,50,52,btKyeboardNumPress},
	{IDC_BUTTON_1,	0,"1",79,328,50,52,btKyeboardNumPress},
	{IDC_BUTTON_2,	0,"2",134,328,50,52,btKyeboardNumPress},
	{IDC_BUTTON_3,	0,"3",187,328,50,52,btKyeboardNumPress},
	{IDC_BUTTON_4,	0,"4",241,328,50,52,btKyeboardNumPress},
	{IDC_BUTTON_5,	0,"5",294,328,50,52,btKyeboardNumPress},
	{IDC_BUTTON_6,	0,"6",79,385,50,52,btKyeboardNumPress},
	{IDC_BUTTON_7,	0,"7",134,385,50,52,btKyeboardNumPress},
	{IDC_BUTTON_8,	0,"8",187,385,50,52,btKyeboardNumPress},
	{IDC_BUTTON_9,	0,"9",241,385,50,52,btKyeboardNumPress},
	{IDC_BUTTON_CLEAR,	0,"全部消除",349,328,50,52,btKyeboardNumPress},
	{IDC_BUTTON_DELET,	0,"消除",349,385,50,52,btKyeboardNumPress},
	{IDC_BUTTON_EXIT,	0,"取消",243,450,114,48,btExitPress},
	{IDC_BUTTON_CONFIRM,0,"确定",119,450,114,48,btConfirmPress},
};

static FormBase* form_base = NULL;

/* ----------------------------------------------------------------*/
/**
 * @brief btKyeboardNumPress 获取设备ID按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void btKyeboardNumPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    if (id >= IDC_BUTTON_0 && id <= IDC_BUTTON_9) {
        char buf[4];
        sprintf(buf,"%d",id);
        SendMessage (GetDlgItem (GetParent (hwnd), IDC_EDIT_PASSWORD),
                MSG_CHAR, buf[0], 0L);
    } else if (id == IDC_BUTTON_CLEAR) 
        SetWindowText(GetDlgItem(GetParent (hwnd),IDC_EDIT_PASSWORD),"");
    else if (id == IDC_BUTTON_DELET)
        SendMessage (GetDlgItem (GetParent (hwnd), IDC_EDIT_PASSWORD),
                MSG_CHAR, '\b', 0L);
}

static void showErrInfo(HWND hwnd,BOOL type)
{
    if (type) {
        ShowWindow(GetDlgItem (hwnd, IDC_LABER_ERR),
                SW_SHOWNORMAL);
        ShowWindow(GetDlgItem (hwnd, IDC_BUTTON_ERR_CONFIRM),
                SW_SHOWNORMAL);
    } else {
        ShowWindow(GetDlgItem (hwnd, IDC_LABER_ERR),
                SW_HIDE);
        ShowWindow(GetDlgItem (hwnd, IDC_BUTTON_ERR_CONFIRM),
                SW_HIDE);
    } 
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
static void btErrConfirmPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    showErrInfo(GetParent (hwnd),FALSE);
	SetWindowText(GetDlgItem(GetParent (hwnd),IDC_EDIT_PASSWORD),"");
}

static void btConfirmPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    char buf[32] = {0};
    GetWindowText(GetDlgItem (GetParent (hwnd), IDC_EDIT_PASSWORD),
            buf,sizeof(buf));
    if (strcmp(buf,g_config.password) == 0) {
        createFormPreSet(GetParent (hwnd));
    } else {
        showErrInfo(GetParent (hwnd),TRUE);
    }
}

void formPasswordLoadLock(void)
{
    INIT_MUTEX_LOCK(mutexattr2,mutex);
}
void formPasswordLoadBmp(void)
{
	int i;
	char image_path[128] = {0};
	pthread_mutex_lock(&mutex);
    if (bmp_load_finished == 1) {
        pthread_mutex_unlock(&mutex);
        return;
    }
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
	for (i=0; i<NELEMENTS(opt_controls); i++) {
		sprintf(image_path,BMP_LOCAL_PATH"%s（X%d,Y%d）.JPG",opt_controls[i].img_name,
				opt_controls[i].x,
				opt_controls[i].y);
		bmpLoad(&opt_controls[i].image_normal, image_path);
		sprintf(image_path,BMP_LOCAL_PATH"%s亮（X%d,Y%d）.JPG",opt_controls[i].img_name,
				opt_controls[i].x,
				opt_controls[i].y);
		bmpLoad(&opt_controls[i].image_press, image_path);
	}
	bmp_load_finished = 1;
    pthread_mutex_unlock(&mutex);
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
	HWND hCtrl;
    formPasswordLoadBmp();
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_PASSWORD),"");
	for (i=0; i<NELEMENTS(opt_controls); i++) {
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
            WS_CHILD|SS_BITMAP,
            WS_EX_TRANSPARENT,
            IDC_LABER_ERR,
            95,254,296,177,
            hDlg, NULL, NULL,
            (DWORD)&bmp_err);
    createSkinButton(hDlg,
            IDC_BUTTON_ERR_CONFIRM,
            184,365,118,49,
            &bmp_err_confirm,
            &bmp_err_confirm1,
            1, 0,
            btErrConfirmPress);
    ShowWindow(GetDlgItem (hDlg, IDC_BUTTON_ERR_CONFIRM),
            SW_HIDE);
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
        SetWindowText(GetDlgItem(Form,IDC_EDIT_PASSWORD),"");
        showErrInfo(Form,FALSE);
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
		form_base_priv.hwnd = hMainWnd;
		form_base_priv.bmp_bkg = &bmp_bkg_system2;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

