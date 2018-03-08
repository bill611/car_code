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

enum {
	LIGHT1,
	LIGHT2,
	LIGHT3,
	LIGHT_NUM,
};

typedef struct _ButtonArray {
    uint8_t device_id;
    char *path;
    MgCtrlButton *ctrl;
    int num;
    int y;
}ButtonArray;

typedef struct _StructLight {
    ButtonArray *array;
    int array_num;
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

static MgCtrlButton opt_power[] = {
	{0,0x81,"off",117,0,84,51},//{170,359,546}},
	{0,0x80,"on",278,0,85,51},//{170,359,546}},
};
static MgCtrlButton opt_bright[] = {
	{0,0x82,"亮度-0",50,0,125,59},//,{221,410,597}},
	{0,0x82,"亮度-1",50,0,125,59},//,{221,410,597}},
	{0,0x82,"亮度-2",50,0,125,59},//,{221,410,597}},
	{0,0x82,"亮度-3",50,0,125,59},//,{221,410,597}},
	{0,0x82,"亮度-4",50,0,125,59},//,{221,410,597}},
	{0,0x82,"亮度-5",50,0,125,59},//,{221,410,597}},
};
static MgCtrlButton opt_rate[] = {
	{0,0x84,"速率-1",305,0,123,59},//,{221,410,597}},
	{0,0x84,"速率-2",305,0,123,59},//,{221,410,597}},
	{0,0x84,"速率-5",305,0,123,59},//,{221,410,597}},
	{0,0x84,"速率-10",305,0,123,59},//,{221,410,597}},
	{0,0x84,"速率-15",305,0,123,59},//,{221,410,597}},
	{0,0x84,"速率-20",305,0,123,59},//,{221,410,597}},
	{0,0x84,"速率-30",305,0,123,59},//,{221,410,597}},
};
static MgCtrlButton opt_color[] = {
	{0,0x83,"颜色-1",178,0,124,59},//{221,410,597}},
	{0,0x83,"颜色-2",178,0,124,59},//{221,410,597}},
};

static ButtonArray array1[] = {
    {0x03,"灯1",BMP_LOAD_PARA(opt_power),170},
    {0x03,"灯1",BMP_LOAD_PARA(opt_bright),221},
    {0x03,"灯1",BMP_LOAD_PARA(opt_rate),221},
    {0x03,"灯1",BMP_LOAD_PARA(opt_color),221},
};
static ButtonArray array2[] = {
    {0x1e,"灯2",BMP_LOAD_PARA(opt_power),359},
    {0x1e,"灯2",BMP_LOAD_PARA(opt_bright),410},
    {0x1e,"灯2",BMP_LOAD_PARA(opt_rate),410},
    {0x1e,"灯2",BMP_LOAD_PARA(opt_color),410},
};
static ButtonArray array3[] = {
    {0x1f,"灯3",BMP_LOAD_PARA(opt_power),546},
    {0x1f,"灯3",BMP_LOAD_PARA(opt_bright),597},
    {0x1f,"灯3",BMP_LOAD_PARA(opt_rate),597},
    {0x1f,"灯3",BMP_LOAD_PARA(opt_color),597},
};
static StructLight light[] = {
    {BMP_LOAD_PARA(array1)},
    {BMP_LOAD_PARA(array2)},
    {BMP_LOAD_PARA(array3)},
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
	if (nc != BN_CLICKED)
		return;
    saveLog("id:%d\n", id);
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
	int i,j,k;
    StructLight *p_light;
    ButtonArray *p_array;
    MgCtrlButton *p_ctrl,*p;
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
	for (i=0; i<NELEMENTS(light); i++) {
        p_light = &light[i];
        for (j=0; j<p_light->array_num; j++) {
            p_array = p_light->array + j;
            for (k=0; k<p_array->num; k++) {
                p_ctrl = p_array->ctrl + k;
                createSkinButton(hDlg,
                        p_ctrl->idc,
                        p_ctrl->x,
                        p_ctrl->y,
                        p_ctrl->w,
                        p_ctrl->h,
                        &p_ctrl->image_normal,
                        &p_ctrl->image_press,
                        1, 0,
                        p_ctrl->notif_proc);
            }
        }
	}
	for (i=0; i<LIGHT_NUM; i++) {
		// opt_controls[i].device_id = 0x15;
		// opt_controls[i].notif_proc = optControlsNotify;
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
	int i,j,k,idc=0;
	char image_path[128] = {0};
    StructLight *p_light;
    ButtonArray *p_array;
    MgCtrlButton *p_ctrl,*new_ctrl,*p;
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
	for (i=0; i<NELEMENTS(light); i++) {
        p_light = &light[i];
        for (j=0; j<p_light->array_num; j++) {
            p_array = p_light->array + j;
            new_ctrl = (MgCtrlButton *)calloc(p_array->num,sizeof(MgCtrlButton));
            p = p_array->ctrl;
            p_array->ctrl = new_ctrl;
            for (k=0; k<p_array->num; k++) {
                p_ctrl = p + k;
                new_ctrl->idc = idc++;
                new_ctrl->x = p_ctrl->x;
                new_ctrl->y = p_array->y;
                new_ctrl->w = p_ctrl->w;
                new_ctrl->h = p_ctrl->h;
                new_ctrl->device_id = p_array->device_id;
                new_ctrl->op_code = p_ctrl->op_code;
                new_ctrl->notif_proc = optControlsNotify;
                sprintf(image_path,BMP_LOCAL_PATH"%s/%s%s(x%d，y%d).JPG",
                        p_array->path,
                        p_array->path,
                        p_ctrl->img_name,
                        p_ctrl->x,
                        p_array->y);
                saveLog("[%d][%d][%d][%s]\n",i,j,k,image_path);
                bmpLoad(&new_ctrl->image_normal, image_path);
                sprintf(image_path,BMP_LOCAL_PATH"%s/%s%s-2(x%d，y%d).JPG",
                        p_array->path,
                        p_array->path,
                        p_ctrl->img_name,
                        p_ctrl->x,
                        p_array->y);
                bmpLoad(&new_ctrl->image_press, image_path);
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

