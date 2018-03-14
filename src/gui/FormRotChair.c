/*
 * =============================================================================
 *
 *       Filename:  FormRotChair.c
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
static int formRotChairProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void optMultiControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optLeftChairControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRightChairControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define IDC_FOMR_TIMER 0xff

#define BMP_LOCAL_PATH "res/image/旋转座椅/"
enum {
    CHAIR_DIR_LEFT,  // 左座椅
    CHAIR_DIR_RIGHT, // 右座椅
};

enum {
    CHAIR_LEFT,  // 左座椅界面
    CHAIR_RIGHT, // 右座椅界面
};

enum {
    CHAIR_BACKREST, // 靠背
    CHAIR_CUSHIN,  	// 座垫
    CHAIR_LEG,  	// 腿托
};

typedef struct _ButtonArray {
    MgCtrlButton *ctrl;
    int num;
}ButtonArray;

typedef struct _ElecChair {
    ButtonArray *array;
	int num;
}ElecChair;
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static FormBase* form_base = NULL;
static BITMAP bmp_bkg_l;
static BITMAP bmp_bkg_r;
static int chair_disp_type = CHAIR_LEFT;
static MgCtrlButton * multi_ctrl = NULL; // 多次连续触发按键

static BmpLocation bmp_load[] = {
    {&bmp_bkg_r, BMP_LOCAL_PATH"右椅/旋转右座椅.JPG"},
    {&bmp_bkg_l, BMP_LOCAL_PATH"左椅/旋转左座椅.JPG"},
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
	.name = "FRotChair",
	.dlgProc = formRotChairProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

// device 0x0b
static MgCtrlButton opt_controls[] = {
	{0,	0x84,"后转",276,543,117,56},
	{0,	0x95,"前转",86,543,115,56},
	{0,	0x0,"右座椅",276,624,117,56,optRightChairControlsNotify},
	{0,	0x0,"左座椅",86,624,115,56,optLeftChairControlsNotify},
};

// 左靠背
static MgCtrlButton opt_l_controls[] = {
	{0,	0x80,"左椅/左椅背向上",205,207,53,54},
	{0,	0x81,"左椅/左椅背向下",99,247,56,55},
	{0,	0x83,"左椅/左椅垫向后",228,333,53,57},
	{0,	0x82,"左椅/左椅垫向前",298,332,54,58},
};


// 右靠背
static MgCtrlButton opt_r_controls[] = {
	{0,	0x80,"右椅/右椅背向上",215,206,54,54},
	{0,	0x81,"右椅/右椅背向下",318,242,57,58},
	{0,	0x83,"右椅/右椅垫向后",189,333,57,58},
	{0,	0x82,"右椅/右椅垫向前",119,329,56,58},
};


static ButtonArray chair_l[] = {
	{BMP_LOAD_PARA(opt_l_controls)},
};
static ButtonArray chair_r[] = {
	{BMP_LOAD_PARA(opt_r_controls)},
};
static ElecChair chair[] = {
	{BMP_LOAD_PARA(chair_l) },
	{BMP_LOAD_PARA(chair_r)},
};

/* ---------------------------------------------------------------------------*/
/**
 * @brief updateChairType 更新显示界面模块
 *
 * @param hwnd
 */
/* ---------------------------------------------------------------------------*/
static void updateChairType(HWND hwnd)
{
	int i,j,k;
	int display;
	MgCtrlButton *ctrl;
	ButtonArray *array;
	RECT rect;
    for (i=0; i<NELEMENTS(chair); i++) {
		for (j=0; j<chair[i].num; j++) {
			array = chair[i].array + j;
			for (k=0; k<array->num; k++) {
				if (i == chair_disp_type )
					display = SW_SHOWNORMAL;
				else
					display = SW_HIDE;
				ctrl = array->ctrl + k;
				ShowWindow(GetDlgItem(hwnd,ctrl->idc), display);
			}
		}
    }
	rect.top = 64;
	rect.left = 118;
	rect.right = 427;
	rect.bottom = 480;

    InvalidateRect (hwnd, &rect, FALSE);
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
static MgCtrlButton * sendOptCmd(int id)
{
	int i,j,k;
	MgCtrlButton *ctrl;
	ButtonArray *array;
    for (i=0; i<NELEMENTS(opt_controls); i++) {
		if (opt_controls[i].idc == id) {
			ctrl = &opt_controls[i];
			goto search_ok;
		}
    }
    for (i=0; i<NELEMENTS(chair); i++) {
		for (j=0; j<chair[i].num; j++) {
			array = chair[i].array + j;
			for (k=0; k<array->num; k++) {
				ctrl = array->ctrl + k;
				if (ctrl->idc == id) {
					goto search_ok;
				}
			}
		}
    }
	return NULL;
search_ok:
    if (chair_disp_type == CHAIR_LEFT)
        pro_com->sendOpt(0x08, ctrl->op_code);
    else
        pro_com->sendOpt(0x09, ctrl->op_code);
	return ctrl;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief optLeftChairControlsNotify 左椅按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optLeftChairControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	chair_disp_type = CHAIR_LEFT;
    form_base_priv.bmp_bkg = &bmp_bkg_l;
	SendMessage(GetParent (hwnd),MSG_ELECTRIC_CHAIR_TYPE,0,0);
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief optRightChairControlsNotify 右椅按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optRightChairControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	chair_disp_type = CHAIR_RIGHT;
    form_base_priv.bmp_bkg = &bmp_bkg_r;
	SendMessage(GetParent (hwnd),MSG_ELECTRIC_CHAIR_TYPE,0,0);
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
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief creatButtonControl 批量创建按键控件
 *
 * @param hDlg
 * @param ctrl
 * @param num
 * @param display
 * @param func
 */
/* ---------------------------------------------------------------------------*/
static void creatButtonControl(HWND hDlg,MgCtrlButton *ctrl,int num,int display,
        void (*func)(HWND hwnd, int id, int nc, DWORD add_data))
{
    static int k = 0;
    int i;
    MgCtrlButton *p;
	for (i=0; i<num; i++) {
        p = ctrl+i;
		p->idc = k++;
		p->device_id = 0x0b;
        if (p->notif_proc == NULL)
            p->notif_proc = func;
		createSkinButton(hDlg, p->idc,
				p->x, p->y, p->w, p->h,
				&p->image_normal,
				&p->image_press,
				display, 0, p->notif_proc);
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
    creatButtonControl(hDlg, BMP_LOAD_PARA(opt_controls),1,
            optMultiControlsNotify);
    int i,j;
	int display = 0;
	ButtonArray *array;
	MgCtrlButton *ctrl;
    for (i=0; i<NELEMENTS(chair); i++) {
		for (j=0; j<chair[i].num; j++) {
			array = chair[i].array + j;
			if (i == chair_disp_type)
				display = 1;
			else
				display = 0;
			creatButtonControl(hDlg, array->ctrl,array->num,display,
					optMultiControlsNotify);
		}
    }
    formManiCreateToolBar(hDlg);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formRotChairProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formRotChairProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_UPDATESTATUS:
			{
				formMainUpdateMute(hDlg);
			} break;
		case MSG_ELECTRIC_CHAIR_TYPE:
			{
				updateChairType(hDlg);
			} break;
		case MSG_TIMER:
			{
				if (wParam != IDC_FOMR_TIMER)
					break;
				
				if (multi_ctrl == NULL)
					break;
				if (chair_disp_type == CHAIR_LEFT)
					pro_com->sendOpt(0x08, multi_ctrl->op_code);
				else
					pro_com->sendOpt(0x09, multi_ctrl->op_code);

			} break;
		default:
			break;
	}
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == 0) {
		return 0;
	}

    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

static void bmpsRotChairButtonLoad(MgCtrlButton *controls,int num)
{
	int i;
	char image_path[128] = {0};
    MgCtrlButton *p;
	for (i=0; i<num; i++) {
        p = controls + i;
		sprintf(image_path,BMP_LOCAL_PATH"%s(x%d，y%d).JPG",p->img_name,
                p->x,
                p->y);
        bmpLoad(&p->image_normal, image_path);
		sprintf(image_path,BMP_LOCAL_PATH"%s-2(x%d，y%d).JPG",p->img_name,
                p->x,
                p->y);
        bmpLoad(&p->image_press, image_path);
	}
}
void formRotChairLoadBmp(void)
{
	int i;
	char image_path[128] = {0};
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
    bmpsRotChairButtonLoad(BMP_LOAD_PARA(opt_controls));
    bmpsRotChairButtonLoad(BMP_LOAD_PARA(opt_l_controls));
    bmpsRotChairButtonLoad(BMP_LOAD_PARA(opt_r_controls));
}
/* ----------------------------------------------------------------*/
/**
 * @brief createFormRotChair 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormRotChair(HWND hMainWnd)
{
	HWND Form = Screen.Find(form_base_priv.name);
	if(Form) {
		chair_disp_type = CHAIR_LEFT;
		form_base_priv.bmp_bkg = &bmp_bkg_l;
		SendMessage(Form,MSG_UPDATESTATUS,0,0);
		SendMessage(Form,MSG_ELECTRIC_CHAIR_TYPE,0,0);
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
		form_base_priv.hwnd = hMainWnd;
		form_base_priv.bmp_bkg = &bmp_bkg_l;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

