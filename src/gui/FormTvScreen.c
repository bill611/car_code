/*
 * =============================================================================
 *
 *       Filename:  FormTvScreen.c
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
static int formTvScreenProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
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

#define IDC_FOMR_TIMER 0xff

#define BMP_LOCAL_PATH "TV/"

enum {
	IDC_TV_PWR,
	IDC_TV_UP,
	IDC_TV_DOWN,
};

enum {
    TV_STATE_UPING,
    TV_STATE_TOP,
    TV_STATE_DOWNING,
    TV_STATE_DOWN,
    TV_STATE_STOP = 5,
};

typedef struct _GlassIcon{
    BITMAP bmp;
    int idc;
}GlassIcon;
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static GlassIcon glass_icon[3],icon_title;

static int bmp_load_finished = 0;
static pthread_mutex_t mutex;		//队列控制互斥信号
static pthread_mutexattr_t mutexattr2;
static int flash_idc; // 闪烁的按键
static int flash_cnt; // 闪烁的次数

static BmpLocation bmp_load[] = {
    {&icon_title.bmp, BMP_LOCAL_PATH"电视(x69，y89).JPG"},
    {&glass_icon[IDC_TV_PWR].bmp, BMP_LOCAL_PATH"power(X219-Y536).JPG"},
    {&glass_icon[IDC_TV_UP].bmp, BMP_LOCAL_PATH"up(X335-Y521).JPG"},
    {&glass_icon[IDC_TV_DOWN].bmp, BMP_LOCAL_PATH"down(X92-Y538).JPG"},
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
	.name = "FTvScreen",
	.dlgProc = formTvScreenProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

// device 0x0b
static MgCtrlButton opt_controls[] = {
	{0, 0x80,"电视电源",182,576,116,55},
	{0, 0xa8,"电视上升",306,576,115,55},
	{0,	0xa9,"电视下降",59,576,115,55},
	{0,	0x8e,"确认",189,298,101,106},
	{0,	0xab,"向上",189,235,101,63},
	{0,	0xac,"向下",189,404,101,64},
	{0,	0x88,"向右",290,298,65,106},
	{0,	0x87,"向左",121,298,68,106},
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
	int op_code = opt_controls[id].op_code;
	if (nc == BN_PUSHED) {
		if(id == IDC_TV_UP) {
			op_code = 0xa6;
			pro_com->sendOpt(opt_controls[id].device_id, op_code);
		} else if (id == IDC_TV_DOWN) {
			op_code = 0xa7;
			pro_com->sendOpt(opt_controls[id].device_id, op_code);
		}
	} else if (nc == BN_UNPUSHED) {
		if(id == IDC_TV_PWR) {
			int power = SendMessage(GetDlgItem (GetParent (hwnd), id),
					MSG_MYBUTTON_GET_SELECT_STATE, 0, 0);
            if (power) {
                Public.tvPower = (Public.tvPower & 0x0f) | 0x80;
				op_code = 0xaa;
            } else {
                Public.tvPower = (Public.tvPower & 0x0f) | 0x90;
				op_code = 0x80;
            }
		}
		pro_com->sendOpt(opt_controls[id].device_id, op_code);
	}
}

static void updateTvScreenPower(HWND hDlg)
{
   int state = Public.tvPower & 0x0f; 
   if ((Public.tvPower & 0xf0) == 0x80)
        SendMessage(GetDlgItem(hDlg,opt_controls[IDC_TV_PWR].idc),
                MSG_MYBUTTON_SET_SELECT_STATE, 0, 0);
   else
        SendMessage(GetDlgItem(hDlg,opt_controls[IDC_TV_PWR].idc),
                MSG_MYBUTTON_SET_SELECT_STATE, 1, 0);
   switch (state) {
       case TV_STATE_UPING:
           {
               SetTimer(hDlg,IDC_FOMR_TIMER,TIME_500MS);
               flash_idc = IDC_TV_UP;
               flash_cnt = 0;
               ShowWindow(GetDlgItem(hDlg,glass_icon[IDC_TV_DOWN].idc),SW_SHOWNORMAL);
           } break;
       case TV_STATE_DOWNING:
           {
               SetTimer(hDlg,IDC_FOMR_TIMER,TIME_500MS);
               flash_idc = IDC_TV_DOWN;
               flash_cnt = 0;
               ShowWindow(GetDlgItem(hDlg,glass_icon[IDC_TV_UP].idc),SW_SHOWNORMAL);
           } break;
   
       case TV_STATE_TOP:
       case TV_STATE_DOWN:
       case TV_STATE_STOP:
           {
               if (IsTimerInstalled(hDlg,IDC_FOMR_TIMER) == TRUE)
                   KillTimer (hDlg,IDC_FOMR_TIMER);
               ShowWindow(GetDlgItem(hDlg,glass_icon[IDC_TV_UP].idc),SW_SHOWNORMAL);
               ShowWindow(GetDlgItem(hDlg,glass_icon[IDC_TV_DOWN].idc),SW_SHOWNORMAL);
           } break;
   
       default:
           break;
   }
}

void formTvScreenLoadLock(void)
{
    INIT_MUTEX_LOCK(mutexattr2,mutex);
}
void formTvScreenLoadBmp(void)
{
	int i;
	char image_path[128] = {0};
    if (g_config.device_main_controls[IDC_SCREEN_TV] == 0)
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
    // formTvScreenLoadBmp();
	for (i=0; i<NELEMENTS(opt_controls); i++) {
		opt_controls[i].idc = i;
		opt_controls[i].device_id = 0x0c;
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
	SendMessage(GetDlgItem(hDlg,opt_controls[IDC_TV_PWR].idc),
		   	MSG_MYBUTTON_SET_SELECT_MODE, 2, 0);
    updateTvScreenPower(hDlg);
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|WS_VISIBLE|SS_BITMAP,
            WS_EX_TRANSPARENT,
            i,
            69,89,101,29,
            hDlg, NULL, NULL,
            (DWORD)&icon_title.bmp);
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|WS_VISIBLE|SS_BITMAP,
            WS_EX_TRANSPARENT,
            i++,
            219,536,45,38,
            hDlg, NULL, NULL,
            (DWORD)&glass_icon[IDC_TV_PWR].bmp);
    glass_icon[IDC_TV_PWR].idc = i - 1;
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|WS_VISIBLE|SS_BITMAP,
            WS_EX_TRANSPARENT,
            i++,
            335,521,57,55,
            hDlg, NULL, NULL,
            (DWORD)&glass_icon[IDC_TV_UP].bmp);
    glass_icon[IDC_TV_UP].idc = i - 1;
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|WS_VISIBLE|SS_BITMAP,
            WS_EX_TRANSPARENT,
            i++,
            92,538,51,37,
            hDlg, NULL, NULL,
            (DWORD)&glass_icon[IDC_TV_DOWN].bmp);
    glass_icon[IDC_TV_DOWN].idc = i - 1;
	formManiCreateToolBar(hDlg);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formTvScreenProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formTvScreenProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_UPDATESTATUS:
			{
                updateTvScreenPower(hDlg);
				formMainUpdateMute(hDlg);
			} break;
		case MSG_TIMER:
			{
				if (wParam != IDC_FOMR_TIMER)
					break;
                if (flash_cnt++ % 2)
                    ShowWindow(GetDlgItem(hDlg,glass_icon[flash_idc].idc),SW_SHOWNORMAL);
                else
                    ShowWindow(GetDlgItem(hDlg,glass_icon[flash_idc].idc),SW_HIDE);
                
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
 * @brief createFormTvScreen 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormTvScreen(HWND hMainWnd)
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

