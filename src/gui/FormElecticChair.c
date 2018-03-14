/*
 * =============================================================================
 *
 *       Filename:  FormElectricChair.c
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
static int formElectricChairProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void optNoticControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optSingleControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optMultiControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optAssistControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optAssistControlsReturnNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optLeftChairControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRightChairControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optSwichPartControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define IDC_FOMR_TIMER 0xff

#define BMP_LOCAL_PATH "res/image/电动座椅/"
enum {
    CHAIR_DIR_LEFT,  // 左座椅
    CHAIR_DIR_RIGHT, // 右座椅
};

enum {
    CHAIR_LEFT,  // 左座椅界面
    CHAIR_RIGHT, // 右座椅界面
    CHAIR_ASSIST, //辅助功能界面
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
static BITMAP bmp_bkg_assist;
static BITMAP bmp_bkg_l_backrest;
static BITMAP bmp_bkg_l_leg;
static BITMAP bmp_bkg_l_cushin;
static BITMAP bmp_bkg_r_backrest;
static BITMAP bmp_bkg_r_leg;
static BITMAP bmp_bkg_r_cushin;
static int chair_disp_type = CHAIR_LEFT;
static int chair_dir_type = CHAIR_DIR_LEFT;
static int chair_mode_type = CHAIR_BACKREST;
static MgCtrlButton * multi_ctrl = NULL; // 多次连续触发按键

static BmpLocation bmp_load[] = {
    {&bmp_bkg_assist, BMP_LOCAL_PATH"座椅功能.JPG"},
    {&bmp_bkg_l_backrest, BMP_LOCAL_PATH"左座椅/左靠背/左座椅靠背.JPG"},
    {&bmp_bkg_l_leg, BMP_LOCAL_PATH"左座椅/左腿托/左座椅腿托.JPG"},
    {&bmp_bkg_l_cushin, BMP_LOCAL_PATH"左座椅/左座垫/左座椅座垫.JPG"},
    {&bmp_bkg_r_backrest, BMP_LOCAL_PATH"右座椅/右靠背/右座椅靠背.JPG"},
    {&bmp_bkg_r_leg, BMP_LOCAL_PATH"右座椅/右腿托/右座椅腿托.JPG"},
    {&bmp_bkg_r_cushin, BMP_LOCAL_PATH"右座椅/右座垫/右座椅座垫.JPG"},
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
	.name = "FEleChair",
	.dlgProc = formElectricChairProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

// device 0x0b
static MgCtrlButton opt_controls[] = {
	{0,	0x0,"辅助功能",305,509,119,51,optAssistControlsNotify},
	{0,	0x82,"全收",58,509,60,51},
	{0,	0x83,"全躺",118,509,59,51},
	{0,	0x81,"腿托收",240,509,59,51},
	{0,	0x80,"腿托展开",181,509,59,51},
	{0,	0x0,"右椅",281,622,114,55,optRightChairControlsNotify},
	{0,	0x0,"左椅",83,622,113,54,optLeftChairControlsNotify},
};

// 辅助功能
static MgCtrlButton opt_assist_controls[] = {
	{0,	0x0,"返回",348,121,69,68,optAssistControlsReturnNotify},
	{0,	0x88,"按犘打开",116,418,59,46,optSingleControlsNotify},
	{0,	0x87,"按犘关闭",60,418,56,46,optSingleControlsNotify},
	{0,	0x85,"加热打开",116,325,58,46,optSingleControlsNotify},
	{0,	0x84,"加热关闭",59,325,58,46,optSingleControlsNotify},
	{0,	0x9a,"制冷打开",116,233,58,47,optSingleControlsNotify},
	{0,	0x99,"制冷关闭",59,233,57,47,optSingleControlsNotify},
};

// 左靠背
static MgCtrlButton opt_l_backrest_controls[] = {
	{0,	0x0,"左座椅/左靠背/左靠背部份",348,121,71,81,optSwichPartControlsNotify},
	{0,	0x8b,"左座椅/左靠背/左靠背向上",258,262,62,67},
	{0,	0x8c,"左座椅/左靠背/左靠背向下",139,309,63,66},
	{0,	0x9d,"左座椅/左靠背/左头枕向上",146,150,66,62},
	{0,	0x9e,"左座椅/左靠背/左头枕向下",126,218,66,65},
};

// 左腿托
static MgCtrlButton opt_l_leg_controls[] = {
	{0,	0x0,"左座椅/左腿托/左腿托部份",348,121,71,81,optSwichPartControlsNotify},
	{0,	0x93,"左座椅/左腿托/左腿托向上",262,246,66,67},
	{0,	0x94,"左座椅/左腿托/左腿托向下",154,319,64,68},
};

// 左座垫
static MgCtrlButton opt_l_cushin_controls[] = {
	{0,	0x0,"左座椅/左座垫/左座垫部份",348,121,69,81,optSwichPartControlsNotify},
	{0,	0x90,"左座椅/左座垫/左座垫向后",184,288,63,64},
	{0,	0x8f,"左座椅/左座垫/左座垫向前",266,266,62,64},
	{0,	0x8d,"左座椅/左座垫/左座垫向上",164,216,66,63},
	{0,	0x8e,"左座椅/左座垫/左座垫向下",191,359,64,65},
};

// 右靠背
static MgCtrlButton opt_r_backrest_controls[] = {
	{0,	0x0,"右座椅/右靠背/右靠背部份",348,121,71,81,optSwichPartControlsNotify},
	{0,	0x8b,"右座椅/右靠背/右靠背向上",160,261,63,69},
	{0,	0x8c,"右座椅/右靠背/右靠背向下",274,308,69,67},
	{0,	0x9d,"右座椅/右靠背/右头枕向上",266,148,70,67},
	{0,	0x9e,"右座椅/右靠背/右头枕向下",288,217,67,64},
};

// 右腿托
static MgCtrlButton opt_r_leg_controls[] = {
	{0,	0x0,"右座椅/右腿托/右腿托部份",348,121,69,84,optSwichPartControlsNotify},
	{0,	0x93,"右座椅/右腿托/右腿托向上",150,245,70,69},
	{0,	0x94,"右座椅/右腿托/右腿托向下",262,319,66,66},
};

// 右座垫
static MgCtrlButton opt_r_cushin_controls[] = {
	{0,	0x0,"右座椅/右座垫/右座垫部份",348,121,69,84,optSwichPartControlsNotify},
	{0,	0x90,"右座椅/右座垫/右座垫向后",233,288,64,65},
	{0,	0x8f,"右座椅/右座垫/右座垫向前",150,266,66,66},
	{0,	0x8d,"右座椅/右座垫/右座垫向上",250,215,68,65},
	{0,	0x8e,"右座椅/右座垫/右座垫向下",224,359,68,64},
};

static ButtonArray chair_l[] = {
	{BMP_LOAD_PARA(opt_l_backrest_controls)},
	{BMP_LOAD_PARA(opt_l_cushin_controls)},
	{BMP_LOAD_PARA(opt_l_leg_controls)},
};
static ButtonArray chair_r[] = {
	{BMP_LOAD_PARA(opt_r_backrest_controls)},
	{BMP_LOAD_PARA(opt_r_cushin_controls)},
	{BMP_LOAD_PARA(opt_r_leg_controls)},
};
static ButtonArray chair_s[] = {
	{BMP_LOAD_PARA(opt_assist_controls)},
};
static ElecChair chair[] = {
	{BMP_LOAD_PARA(chair_l) },
	{BMP_LOAD_PARA(chair_r)},
	{BMP_LOAD_PARA(chair_s)},
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
				if (i == chair_disp_type && j == chair_mode_type)
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
static MgCtrlButton * sendOptCmd(HWND id)
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
    if (chair_dir_type == CHAIR_DIR_LEFT)
        pro_com->sendOpt(0x01, ctrl->op_code);
    else
        pro_com->sendOpt(0x02, ctrl->op_code);
	return ctrl;
}
/* ----------------------------------------------------------------*/
/**
 * @brief optNoticControlsNotify 弹出提示按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void optNoticControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	saveLog("[%s]id:%d\n",__FUNCTION__, id);
	MgCtrlButton * ctrl = sendOptCmd(id);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief optAssistControlsNotify 辅助功能按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optAssistControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	int i,k;
	chair_disp_type = CHAIR_ASSIST;
    form_base_priv.bmp_bkg = &bmp_bkg_assist;
	chair_mode_type = CHAIR_BACKREST;
	SendMessage(GetParent (hwnd),MSG_ELECTRIC_CHAIR_TYPE,0,0);

}
/* ---------------------------------------------------------------------------*/
/**
 * @brief optAssistControlsReturnNotify 辅助功能返回按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optAssistControlsReturnNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	int i,k;
	if (chair_dir_type == CHAIR_DIR_LEFT) {
		chair_disp_type = CHAIR_LEFT;
		form_base_priv.bmp_bkg = &bmp_bkg_l_backrest;
	} else {
		chair_disp_type = CHAIR_RIGHT;
		form_base_priv.bmp_bkg = &bmp_bkg_r_backrest;
	}
	chair_mode_type = CHAIR_BACKREST;

	SendMessage(GetParent (hwnd),MSG_ELECTRIC_CHAIR_TYPE,0,0);

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
	chair_dir_type = CHAIR_DIR_LEFT;
	chair_disp_type = CHAIR_LEFT;
	chair_mode_type = CHAIR_BACKREST;
    form_base_priv.bmp_bkg = &bmp_bkg_l_backrest;
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
	chair_dir_type = CHAIR_DIR_RIGHT;
	chair_disp_type = CHAIR_RIGHT;
	chair_mode_type = CHAIR_BACKREST;
    form_base_priv.bmp_bkg = &bmp_bkg_r_backrest;
	SendMessage(GetParent (hwnd),MSG_ELECTRIC_CHAIR_TYPE,0,0);
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief optSwichPartControlsNotify 切换座椅部分按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optSwichPartControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	if (chair_mode_type < CHAIR_LEG)
		chair_mode_type++;
	else
		chair_mode_type = CHAIR_BACKREST;
    if (chair_mode_type == CHAIR_BACKREST) {
       if (chair_dir_type == CHAIR_DIR_LEFT) 
        form_base_priv.bmp_bkg = &bmp_bkg_l_backrest;
       else if (chair_dir_type == CHAIR_DIR_RIGHT) 
        form_base_priv.bmp_bkg = &bmp_bkg_r_backrest;
    } else if (chair_mode_type == CHAIR_CUSHIN) {
       if (chair_dir_type == CHAIR_DIR_LEFT) 
        form_base_priv.bmp_bkg = &bmp_bkg_l_cushin;
       else if (chair_dir_type == CHAIR_DIR_RIGHT) 
        form_base_priv.bmp_bkg = &bmp_bkg_r_cushin;
    } else if (chair_mode_type == CHAIR_LEG) {
       if (chair_dir_type == CHAIR_DIR_LEFT) 
        form_base_priv.bmp_bkg = &bmp_bkg_l_leg;
       else if (chair_dir_type == CHAIR_DIR_RIGHT) 
        form_base_priv.bmp_bkg = &bmp_bkg_r_leg;
    }
	SendMessage(GetParent (hwnd),MSG_ELECTRIC_CHAIR_TYPE,0,0);
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief optSingleControlsNotify 单次触发按键
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void optSingleControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	sendOptCmd(id);
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
            optNoticControlsNotify);
    int i,j;
	int display = 0;
	ButtonArray *array;
	MgCtrlButton *ctrl;
    for (i=0; i<NELEMENTS(chair); i++) {
		for (j=0; j<chair[i].num; j++) {
			array = chair[i].array + j;
			if (i == chair_disp_type && j == chair_mode_type)
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
 * @brief formElectricChairProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formElectricChairProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
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

				if (chair_dir_type == CHAIR_DIR_LEFT)
					pro_com->sendOpt(0x01, multi_ctrl->op_code);
				else
					pro_com->sendOpt(0x02, multi_ctrl->op_code);
			} break;

		default:
			break;
	}
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == 0) {
		return 0;
	}

    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

static void bmpsElectricChairButtonLoad(MgCtrlButton *controls,int num)
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
void formElectricChairLoadBmp(void)
{
	int i;
	char image_path[128] = {0};
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_controls));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_assist_controls));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_l_backrest_controls));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_l_leg_controls));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_l_cushin_controls));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_r_backrest_controls));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_r_leg_controls));
    bmpsElectricChairButtonLoad(BMP_LOAD_PARA(opt_r_cushin_controls));
}
/* ----------------------------------------------------------------*/
/**
 * @brief createFormElectricChair 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormElectricChair(HWND hMainWnd)
{
	HWND Form = Screen.Find(form_base_priv.name);
	if(Form) {
		chair_disp_type = CHAIR_LEFT;
		chair_dir_type = CHAIR_DIR_LEFT;
		chair_mode_type = CHAIR_BACKREST;
		form_base_priv.bmp_bkg = &bmp_bkg_l_backrest;
		SendMessage(Form,MSG_UPDATESTATUS,0,0);
		SendMessage(Form,MSG_ELECTRIC_CHAIR_TYPE,0,0);
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
		form_base_priv.hwnd = hMainWnd;
		form_base_priv.bmp_bkg = &bmp_bkg_l_backrest;
		form_base = formBaseCreate(&form_base_priv);
		CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				form_base->priv->hwnd,
				form_base->priv->dlgProc,
				0);
	}

	return 0;
}

