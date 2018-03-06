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
static void optControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optAssistControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optLBackrestControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optLLegControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optLCushinControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRBackrestControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRLegControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRCushinControlsNotify(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define BMP_LOCAL_PATH "res/image/电动座椅/"
enum {
    CHAIR_LEFT,
    CHAIR_RIGHT,
};

typedef struct _ButtonArray {
    MgCtrlButton *ctrl;
    int num;
}ButtonArray;

typedef struct _ElecChair {
    ButtonArray *chair;
}ElecChair;
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static FormBase* form_base = NULL;
static BITMAP bmp_bkg; 
static BITMAP bmp_bkg; 
static int chair_type = CHAIR_LEFT;

static BmpLocation bmp_load[] = {
    {&bmp_bkg, BMP_LOCAL_PATH"座椅功能.JPG"},
};

static MY_CTRLDATA ChildCtrls [] = {
};


static MY_DLGTEMPLATE DlgInitParam =
{
    WS_NONE,
    WS_EX_AUTOSECONDARYDC,
    // WS_EX_NONE,
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
	{0,	0x0,"辅助功能",305,509,119,51},
	{0,	0x82,"全收",58,509,60,51},
	{0,	0x83,"全躺",118,509,59,51},
	{0,	0x81,"腿托收",240,509,59,51},
	{0,	0x80,"腿托展开",181,509,59,51},
	{0,	0x0,"右椅",281,622,114,55},
	{0,	0x0,"左椅",83,622,113,54},
};

// 辅助功能
static MgCtrlButton opt_assist_controls[] = {
	{0,	0x88,"按犘打开",116,418,59,46},
	{0,	0x87,"按犘关闭",60,418,56,46},
	{0,	0x85,"加热打开",116,325,58,46},
	{0,	0x84,"加热关闭",59,325,58,46},
	{0,	0x9a,"制冷打开",116,233,58,47},
	{0,	0x99,"制冷关闭",59,233,57,47},
};

// 左靠背
static MgCtrlButton opt_l_backrest_controls[] = {
	{0,	0x0,"左座椅/左靠背/左靠背部份",348,121,71,81,optLBackrestControlsNotify},
	{0,	0x8b,"左座椅/左靠背/左靠背向上",258,262,62,67,optLBackrestControlsNotify},
	{0,	0x8c,"左座椅/左靠背/左靠背向下",139,309,63,66,optLBackrestControlsNotify},
	{0,	0x9d,"左座椅/左靠背/左头枕向上",146,150,66,62,optLBackrestControlsNotify},
	{0,	0x9e,"左座椅/左靠背/左头枕向下",126,218,66,65,optLBackrestControlsNotify},
};

// 左腿托
static MgCtrlButton opt_l_leg_controls[] = {
	{0,	0x0,"左座椅/左腿托/左腿托部份",348,121,71,81,optLLegControlsNotify},
	{0,	0x93,"左座椅/左腿托/左腿托向上",262,246,66,67,optLLegControlsNotify},
	{0,	0x94,"左座椅/左腿托/左腿托向下",154,319,64,68,optLLegControlsNotify},
};

// 左座垫
static MgCtrlButton opt_l_cushin_controls[] = {
	{0,	0x0,"左座椅/左座垫/左座垫部份",348,121,69,81,optLCushinControlsNotify},
	{0,	0x90,"左座椅/左座垫/左座垫向后",184,288,63,64,optLCushinControlsNotify},
	{0,	0x8f,"左座椅/左座垫/左座垫向前",266,266,62,64,optLCushinControlsNotify},
	{0,	0x8d,"左座椅/左座垫/左座垫向上",164,216,66,63,optLCushinControlsNotify},
	{0,	0x8e,"左座椅/左座垫/左座垫向下",191,359,64,65,optLCushinControlsNotify},
};

// 右靠背
static MgCtrlButton opt_r_backrest_controls[] = {
	{0,	0x0,"右座椅/右靠背/右靠背部份",348,121,71,81,optRBackrestControlsNotify},
	{0,	0x8b,"右座椅/右靠背/右靠背向上",160,281,63,69,optRBackrestControlsNotify},
	{0,	0x8c,"右座椅/右靠背/右靠背向下",274,308,69,67,optRBackrestControlsNotify},
	{0,	0x9d,"右座椅/右靠背/右头枕向上",266,148,70,67,optRBackrestControlsNotify},
	{0,	0x9e,"右座椅/右靠背/右头枕向下",288,217,67,64,optRBackrestControlsNotify},
};

// 右腿托
static MgCtrlButton opt_r_leg_controls[] = {
	{0,	0x0,"右座椅/右腿托/右腿托部份",348,121,69,84,optRLegControlsNotify},
	{0,	0x93,"右座椅/右腿托/右腿托向上",150,245,70,69,optRLegControlsNotify},
	{0,	0x94,"右座椅/右腿托/右腿托向下",262,319,66,66,optRLegControlsNotify},
};

// 右座垫
static MgCtrlButton opt_r_cushin_controls[] = {
	{0,	0x0,"右座椅/右座垫/右座垫部份",348,121,69,84,optRCushinControlsNotify},
	{0,	0x90,"右座椅/右座垫/右座垫向后",233,288,64,65,optRCushinControlsNotify},
	{0,	0x8f,"右座椅/右座垫/右座垫向前",150,266,66,66,optRCushinControlsNotify},
	{0,	0x8d,"右座椅/右座垫/右座垫向上",250,215,68,65,optRCushinControlsNotify},
	{0,	0x8e,"右座椅/右座垫/右座垫向下",224,359,68,64,optRCushinControlsNotify},
};


static ButtonArray chair_l[] = {
    {opt_l_backrest_controls},
    {opt_l_leg_controls},
    {opt_l_cushin_controls},
};

static ButtonArray chair_r[] = {
    {opt_r_backrest_controls},
    {opt_r_leg_controls},
    {opt_r_cushin_controls},
}; 

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
	if (nc != BN_CLICKED)
		return;
	saveLog("id:%d\n", id);
    if (chair_type == CHAIR_LEFT)
        pro_com->sendOpt(0x01, opt_controls[id].op_code);
    else
        pro_com->sendOpt(0x02, opt_controls[id].op_code);
}

static void optAssistControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	saveLog("id:%d\n", id);
    int i;
    for (i=0; i<NELEMENTS(opt_assist_controls); i++) {
        if (opt_assist_controls[i].idc == id)
            break;
    }
    if (chair_type == CHAIR_LEFT)
        pro_com->sendOpt(0x01, opt_assist_controls[id].op_code);
    else
        pro_com->sendOpt(0x02, opt_assist_controls[id].op_code);
}

static void optLBackrestControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{

}
static void optLLegControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
    
}
static void optLCushinControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
    
}
static void optRBackrestControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
    
}
static void optRLegControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
    
}
static void optRCushinControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
    
}
static void creatButtonControl(HWND hDlg,MgCtrlButton *ctrl,int num,
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
				1, 0, p->notif_proc);
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
    chair_l[0].num = NELEMENTS(opt_l_backrest_controls);
    chair_l[1].num = NELEMENTS(opt_l_leg_controls);
    chair_l[2].num = NELEMENTS(opt_l_cushin_controls);
    chair_r[0].num = NELEMENTS(opt_r_backrest_controls);
    chair_r[1].num = NELEMENTS(opt_r_leg_controls);
    chair_r[2].num = NELEMENTS(opt_r_cushin_controls);
    creatButtonControl(hDlg, BMP_LOAD_PARA(opt_controls),
            optControlsNotify);
    creatButtonControl(hDlg,BMP_LOAD_PARA(opt_assist_controls),
            optAssistControlsNotify);
    int i;
    for (i=0; i<NELEMENTS(chair_l); i++) {
        creatButtonControl(hDlg, chair_l[i].ctrl,chair_l[i].num,
                chair_l[i].ctrl->notif_proc);
    } 
    for (i=0; i<NELEMENTS(chair_r); i++) {
        creatButtonControl(hDlg, chair_r[i].ctrl,chair_r[i].num,
                chair_r[i].ctrl->notif_proc);
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

