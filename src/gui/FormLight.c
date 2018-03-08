/*
 * =============================================================================
 *
 *       Filename:  FormLight.c
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
static int formLightProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
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

#define BMP_LOCAL_PATH "res/image/灯光/"
#define POWER_MAX 2
#define BRIGHT_MAX 6
#define RATE_MAX 7
#define COLOR_MAX 2

enum {
	LIGHT1,
	LIGHT2,
	LIGHT3,
	LIGHT_NUM,
};
typedef struct _ButtonName {
	uint8_t op_code;
    char *name;
    int x,w,h;
	int y[LIGHT_NUM];
}ButtonName;

typedef struct _ButtonArray {
    MgCtrlButton *ctrl;
    int num;
}ButtonArray;

typedef struct _StructLight {
    MgCtrlButton **array;
	ButtonName *opt_base;
	int num;
}StructLight;
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_bkg; 

static BmpLocation bmp_load[] = {
    {&bmp_bkg, BMP_LOCAL_PATH"灯光.JPG"},
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
	.name = "FLight",
	.dlgProc = formLightProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

static ButtonName opt_power[] = {
	{0x81,"off",117,84,51,{170,359,546}},
	{0x80,"on",278,85,51,{170,359,546}},
};
static ButtonName opt_bright[] = {
	{0x82,"亮度",50,125,59,{221,410,597}},
};
static ButtonName opt_rate[] = {
	{0x84,"速率",305,123,59,{221,410,597}},
};
static ButtonName opt_color[] = {
	{0x83,"颜色",178,124,59,{221,410,597}},
};

static FormBase* form_base = NULL;

static MgCtrlButton power[LIGHT_NUM][POWER_MAX];
static MgCtrlButton bright[LIGHT_NUM][BRIGHT_MAX];
static MgCtrlButton rate[LIGHT_NUM][RATE_MAX];
static MgCtrlButton color[LIGHT_NUM][COLOR_MAX];
static StructLight light[] = {
	{power,opt_power,POWER_MAX},
	{bright,opt_bright,BRIGHT_MAX},
	{rate,opt_rate,RATE_MAX},
	{color,opt_color,COLOR_MAX},
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
	// saveLog("id:%d\n", id);
	// pro_com->sendOpt(opt_controls[id].device_id,
			// opt_controls[id].op_code);
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
	for (i=0; i<LIGHT_NUM; i++) {
		// opt_controls[i].idc = i;
		// opt_controls[i].device_id = 0x15;
		// opt_controls[i].notif_proc = optControlsNotify;
		// createSkinButton(hDlg,
				// opt_controls[i].idc,
				// opt_controls[i].x,
				// opt_controls[i].y,
				// opt_controls[i].w,
				// opt_controls[i].h,
				// &opt_controls[i].image_normal,
				// &opt_controls[i].image_press,
				// 1, 0,
				// opt_controls[i].notif_proc);
	}
	formManiCreateToolBar(hDlg);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formLightProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formLightProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
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

void formLightLoadBmp(void)
{
	int i,j,k;
	char image_path[128] = {0};
    MgCtrlButton **ctrl;
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
	for (i=0; i<LIGHT_NUM; i++) {
		for (k=0; k<NELEMENTS(light); k++) {
			for (j=0; j<light[k].num; j++) {
				ctrl = light[k].array;
				sprintf(image_path,BMP_LOCAL_PATH"灯%d/灯%d%s(x%d，y%d).JPG",i+1,i+1,opt_power[j].name,
						light[k].opt_base[j].x,
						light[k].opt_base[j].y[i]);
				saveLog("[%s]\n",image_path);
				// bmpLoad((*(ctrl+i)+j)->image_normal, image_path);
				sprintf(image_path,BMP_LOCAL_PATH"灯%d/灯%d%s-2(x%d，y%d).JPG",i+1,i+1,opt_power[j].name,
						light[k].opt_base[j].x,
						light[k].opt_base[j].y[i]);
				saveLog("[%s]\n",image_path);
				// bmpLoad(&power[i][j].image_press, image_path);
			}
		}
	}
}
/* ----------------------------------------------------------------*/
/**
 * @brief createFormLight 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormLight(HWND hMainWnd)
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

