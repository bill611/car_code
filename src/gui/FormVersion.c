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

#define BMP_LOCAL_PATH BMP_RES_PATH"系统-1/"
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

static int bmp_load_finished = 0;
static pthread_mutex_t mutex;		//队列控制互斥信号
static pthread_mutexattr_t mutexattr2;

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
	.name = "Fver",
	.dlgProc = formVersionProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

static MgCtrlButton opt_controls[] = {
	{IDC_BUTTON_MANAGE,	0,"后台管理系统",132,422,212,49,btManagePress}, // 后台管理系统
	{IDC_BUTTON_EXIT,	0,"退出",392,205,51,48,btExitPress}, // 退出
};

static FormBase* form_base = NULL;

/* ----------------------------------------------------------------*/
/**
 * @brief btManagePress 打开密码输入窗口
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
 * @brief btExitPress 退出按钮
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
 * @brief createFormVersion 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
void formVersionLoadBmp(void)
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
    // formVersionLoadBmp();
	SetWindowText(GetDlgItem(hDlg,IDC_LABER_VERSION),CODE_VERSION);
	SetWindowText(GetDlgItem(hDlg,IDC_LABER_DATE),__DATE__);
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

void formVersionLoadLock(void)
{
    INIT_MUTEX_LOCK(mutexattr2,mutex);
}

int createFormVersion(HWND hMainWnd)
{
	HWND Form = Screen.Find(form_base_priv.name);
	if(Form) {
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
        if (bmp_load_finished == 0) {
            topMessage(hMainWnd,TOPBOX_ICON_LOADING,NULL );
            return 0;
        }
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

