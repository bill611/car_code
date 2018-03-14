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
static void showNotice(HWND hWnd,int type,int mode);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define IDC_FOMR_TIMER 0xff

#define BMP_LOCAL_PATH "res/image/电动床/"

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
static BITMAP bmp_bkg; 
static BedBox notice_lie;// 躺下
static BedBox notice_unlie;// 收起
static BedBoxButton notice_confirm,notice_cancel;

static BmpLocation bmp_load[] = {
    {&bmp_bkg, BMP_LOCAL_PATH"电动床.JPG"},
    {&notice_lie.bmp, BMP_LOCAL_PATH"全躺提示(X70-Y304).JPG"},
    {&notice_unlie.bmp, BMP_LOCAL_PATH"全收提示(X70-Y304).JPG"},
    {&notice_confirm.bmp[0], BMP_LOCAL_PATH"确定(X240-Y404).JPG"},
    {&notice_confirm.bmp[1], BMP_LOCAL_PATH"确定-2(X240-Y404).JPG"},
    {&notice_cancel.bmp[0], BMP_LOCAL_PATH"取消(X70-Y404).JPG"},
    {&notice_cancel.bmp[1], BMP_LOCAL_PATH"取消-2(X70-Y404).JPG"},
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

static void btConfirmPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    pro_com->sendOpt(multi_ctrl->device_id, multi_ctrl->op_code);
    showNotice(GetParent (hwnd),0,0);
}
static void btCancelPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    showNotice(GetParent (hwnd),0,0);
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief showNotice 显示提示框
 *
 * @param hWnd
 * @param type 0全收 1全躺
 * @param mode 0 隐藏 1显示
 */
/* ---------------------------------------------------------------------------*/
static void showNotice(HWND hWnd,int type,int mode)
{
    ShowWindow(GetDlgItem (hWnd, notice_lie.idc), SW_HIDE);
    ShowWindow(GetDlgItem (hWnd, notice_unlie.idc), SW_HIDE);
    if (mode == 0) {
        ShowWindow(GetDlgItem (hWnd, notice_confirm.idc), SW_HIDE);
        ShowWindow(GetDlgItem (hWnd, notice_cancel.idc), SW_HIDE);
        return;
    } 
    ShowWindow(GetDlgItem (hWnd, notice_confirm.idc), SW_SHOWNORMAL);
    ShowWindow(GetDlgItem (hWnd, notice_cancel.idc), SW_SHOWNORMAL);
    if (type == 0)
        ShowWindow(GetDlgItem (hWnd, notice_unlie.idc), SW_SHOWNORMAL);
    else
        ShowWindow(GetDlgItem (hWnd, notice_lie.idc), SW_SHOWNORMAL);
    
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
    showNotice(GetParent (hwnd),1,1);
}
static void optNoticUnLieNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	saveLog("[%s]id:%d\n",__FUNCTION__, id);
    multi_ctrl = sendOptCmd(id);
    showNotice(GetParent (hwnd),0,1);
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
	if (nc == BN_CLICKED) {
		KillTimer (GetParent (hwnd),IDC_FOMR_TIMER);
		multi_ctrl = sendOptCmd(id);
        pro_com->sendOpt(multi_ctrl->device_id, multi_ctrl->op_code);
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
	int i;
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
    notice_lie.idc = i++;
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|SS_BITMAP,
            WS_EX_TRANSPARENT,
            notice_lie.idc,
            70,304,340,155,
            hDlg, NULL, NULL,
            (DWORD)&notice_lie.bmp);
    notice_unlie.idc = i++;
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|SS_BITMAP,
            WS_EX_TRANSPARENT,
            notice_unlie.idc,
            70,304,340,155,
            hDlg, NULL, NULL,
            (DWORD)&notice_unlie.bmp);
    notice_confirm.idc = i++;
    createSkinButton(hDlg,
            notice_confirm.idc,
            240,404,170,55,
            &notice_confirm.bmp[0],
            &notice_confirm.bmp[1],
            1, 0,
            btConfirmPress);
    notice_cancel.idc = i;
    createSkinButton(hDlg,
            notice_cancel.idc,
            70,404,170,55,
            &notice_cancel.bmp[0],
            &notice_cancel.bmp[1],
            1, 0,
            btCancelPress);
    ShowWindow(GetDlgItem (hDlg, notice_confirm.idc), SW_HIDE);
    ShowWindow(GetDlgItem (hDlg, notice_cancel.idc), SW_HIDE);
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

void formBedLoadBmp(void)
{
	int i;
	char image_path[128] = {0};
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
		form_base_priv.hwnd = hMainWnd;
		form_base_priv.bmp_bkg = &bmp_bkg;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

