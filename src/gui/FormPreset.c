/*
 * =============================================================================
 *
 *       Filename:  FormPreSet.c
 *
 *    Description:  输入密码界面
 *
 *        Preset:  1.0
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
extern int createFormPassword(HWND hMainWnd);

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static int formPresetProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initCtrlButtons(HWND hDlg);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);

static void btConfirmPress(HWND hwnd, int id, int nc, DWORD add_data);
static void btCancelPress(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define BMP_LOCAL_PATH BMP_RES_PATH"选择功能/"
enum {
    IDC_CONFIRM = IDC_WALN, // 功能选择确认
    IDC_CANCEL, // 功能选择取消
};


/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_bkg_select; // 选择功能界面背景

static int bmp_load_finished = 0;
static pthread_mutex_t mutex;		//队列控制互斥信号
static pthread_mutexattr_t mutexattr2;

static BmpLocation bmp_load[] = {
    {&bmp_bkg_select, BMP_LOCAL_PATH"选择功能背景.JPG"},
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
	.name = "Fselect",
	.dlgProc = formPresetProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

static MgCtrlButton opt_controls[] = {
	{IDC_CONFIRM,		0,"确认",103,720,115,58,btConfirmPress},
	{IDC_CANCEL,		0,"取消",261,720,115,58,btCancelPress},
};

static FormBase* form_base = NULL;

static void btConfirmPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    int i;
    for (i=0; i<IDC_WALN; i++) {
        g_config.device_main_controls[i] =
            SendMessage(GetDlgItem (GetParent (hwnd), i),
                    MSG_MYBUTTON_GET_SELECT_STATE, 0, 0);
    }
	Public.saveConfig();
    SendMessage(Screen.hMainWnd, MSG_MAIN_SHOW_NORMAL, 0, 0);
}
static void btCancelPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    SendMessage(Screen.hMainWnd, MSG_MAIN_SHOW_NORMAL, 0, 0);
}

void formPresetLoadLock(void)
{
    INIT_MUTEX_LOCK(mutexattr2,mutex);
}

/* ----------------------------------------------------------------*/
/**
 * @brief createFormPreSet 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
void formPresetLoadBmp(void)
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
    formPresetLoadBmp();
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
    for (i=0; i<IDC_WALN; i++) {
		HWND hCtrl = createSkinButton(hDlg,
				i,
				25 + (i % 4)*114,
				96 + (i / 4)*123,
				118,
				122,
				NULL,NULL, g_config.device_main_controls[i],1,NULL);
        ShowWindow(hCtrl,SW_SHOWNORMAL);
    }
}

/* ----------------------------------------------------------------*/
/**
 * @brief formPresetProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formPresetProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == 0) {
		return 0;
	}

    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

int createFormPreSet(HWND hMainWnd)
{
	HWND Form = Screen.Find(form_base_priv.name);
	if(Form) {
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
		form_base_priv.hwnd = hMainWnd;
		form_base_priv.bmp_bkg = &bmp_bkg_select;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

