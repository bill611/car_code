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

static void optPowerOnNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optPowerOffNotify(HWND hwnd, int id, int nc, DWORD add_data);
static void optBright1Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optBright2Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optBright3Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRate1Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRate2Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optRate3Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optColor1Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optColor2Notify(HWND hwnd, int id, int nc, DWORD add_data);
static void optColor3Notify(HWND hwnd, int id, int nc, DWORD add_data);
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
};

typedef struct _ButtonArray {
    MgCtrlButton *ctrl;
    int num;
    int y;
	NOTIFPROC notif_proc;
}ButtonArray;

typedef struct _StructLight {
    uint8_t device_id;
    char *path;
    ButtonArray *array;
    int array_num;
}StructLight;

typedef struct _StaticPower {
	char *path;
	int x,y,w,h;
	BITMAP bmp_active;
	BITMAP bmp_inactive;
	HWND idc;	
}StaticPower;
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

static StaticPower opt_static_power [] = {
	{"灯1",216,170,47,51},
	{"灯2",216,359,47,51},
	{"灯3",216,546,47,51},
};
static MgCtrlButton opt_power[] = {
	{0,0x81,"off",117,0,84,51,optPowerOffNotify},
	{0,0x80,"on",278,0,85,51,optPowerOnNotify},
};
static MgCtrlButton opt_bright[] = {
	{0,0x82,"亮度-0",50,0,125,59},
	{0,0x82,"亮度-1",50,0,125,59},
	{0,0x82,"亮度-2",50,0,125,59},
	{0,0x82,"亮度-3",50,0,125,59},
	{0,0x82,"亮度-4",50,0,125,59},
	{0,0x82,"亮度-5",50,0,125,59},
};
static MgCtrlButton opt_rate[] = {
	{0,0x84,"速率-1",305,0,123,59},
	{0,0x84,"速率-2",305,0,123,59},
	{0,0x84,"速率-5",305,0,123,59},
	{0,0x84,"速率-10",305,0,123,59},
	{0,0x84,"速率-15",305,0,123,59},
	{0,0x84,"速率-20",305,0,123,59},
	{0,0x84,"速率-30",305,0,123,59},
};
static MgCtrlButton opt_color[] = {
	{0,0x83,"颜色-1",178,0,124,59},
	{0,0x83,"颜色-2",178,0,124,59},
};

static ButtonArray array1[] = {
    {BMP_LOAD_PARA(opt_power),170},
    {BMP_LOAD_PARA(opt_bright),221,optBright1Notify},
    {BMP_LOAD_PARA(opt_rate),221,optRate1Notify},
    {BMP_LOAD_PARA(opt_color),221,optColor1Notify},
};
static ButtonArray array2[] = {
    {BMP_LOAD_PARA(opt_power),359},
    {BMP_LOAD_PARA(opt_bright),410,optBright2Notify},
    {BMP_LOAD_PARA(opt_rate),410,optRate2Notify},
    {BMP_LOAD_PARA(opt_color),410,optColor2Notify},
};
static ButtonArray array3[] = {
    {BMP_LOAD_PARA(opt_power),546},
    {BMP_LOAD_PARA(opt_bright),597,optBright3Notify},
    {BMP_LOAD_PARA(opt_rate),597,optRate3Notify},
    {BMP_LOAD_PARA(opt_color),597,optColor3Notify},
};
static StructLight light[] = {
    {0x03,"灯1",BMP_LOAD_PARA(array1)},
    {0x1e,"灯2",BMP_LOAD_PARA(array2)},
    {0x1f,"灯3",BMP_LOAD_PARA(array3)},
};
static FormBase* form_base = NULL;

static void updatePower(HWND hwnd,int light_n,int value)
{
	if (value)
		SendMessage(GetDlgItem(hwnd,opt_static_power[light_n].idc),
				STM_SETIMAGE,(WPARAM)&opt_static_power[light_n].bmp_active,0);
	else
		SendMessage(GetDlgItem(hwnd,opt_static_power[light_n].idc),
				STM_SETIMAGE,(WPARAM)&opt_static_power[light_n].bmp_inactive,0);
}
static MgCtrlButton *searchPowerCtrl(int id,int *light_n)
{
	int i,j;
    StructLight *p_light;
    MgCtrlButton *p_ctrl;
	for (i=0; i<NELEMENTS(light); i++) {
        p_light = &light[i];
		for (j=0; j<p_light->array->num; j++) {
			p_ctrl = p_light->array->ctrl+j;
			if (p_ctrl->idc == id)  {
				if (light_n)
					*light_n = i;
				return p_ctrl;
			}
		}
	}
	return NULL;
}

static void updateAddPower(HWND hwnd,int id,int value)
{
	uint8_t *public_light;
	int light_n;
	MgCtrlButton *p_ctrl = searchPowerCtrl(id,&light_n);
	if (light_n == 0)
		public_light = &Public.light1;
	else if (light_n == 1)
		public_light = &Public.light2;
	else if (light_n == 2)
		public_light = &Public.light3;
	*public_light = (*public_light & ~(1 << 7) | value << 7);
	updatePower(hwnd,light_n,value);
	pro_com->sendOpt(p_ctrl->device_id, p_ctrl->op_code);
}
static void optPowerOnNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddPower(GetParent(hwnd),id,1);
}
static void optPowerOffNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddPower(GetParent(hwnd),id,0);
}

static MgCtrlButton *updateBright(HWND hwnd,int light_n,int value)
{
	int i;
    ButtonArray *p_array;
    MgCtrlButton *p_ctrl;
	p_array = light[light_n].array + 1;
	for (i=0; i<p_array->num; i++) {
		p_ctrl = p_array->ctrl+i;
		if (value == i)
			ShowWindow(GetDlgItem(hwnd,p_ctrl->idc),SW_SHOWNORMAL);
		else
			ShowWindow(GetDlgItem(hwnd,p_ctrl->idc),SW_HIDE);
	}
	return p_ctrl;
}
static void updateAddBright(HWND hwnd,int light_n)
{
	uint8_t *public_light;
	if (light_n == 0)
		public_light = &Public.light1;
	else if (light_n == 1)
		public_light = &Public.light2;
	else if (light_n == 2)
		public_light = &Public.light3;
	int value = BIT3(*public_light,4);
	if (++value >= NELEMENTS(opt_bright))
		value = 0;
	*public_light = (*public_light & ~(7 << 4) | value << 4);
    MgCtrlButton *p_ctrl = updateBright(hwnd,light_n,value);
	pro_com->sendOpt(p_ctrl->device_id, p_ctrl->op_code);
}
static void optBright1Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddBright(GetParent(hwnd),0);
}
static void optBright2Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddBright(GetParent(hwnd),1);
}
static void optBright3Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddBright(GetParent(hwnd),2);
}

static MgCtrlButton *updateRate(HWND hwnd,int light_n,int value)
{
	int i;
    ButtonArray *p_array;
    MgCtrlButton *p_ctrl;
	p_array = light[light_n].array + 2;
	for (i=0; i<p_array->num; i++) {
		p_ctrl = p_array->ctrl+i;
		if (value == i)
			ShowWindow(GetDlgItem(hwnd,p_ctrl->idc),SW_SHOWNORMAL);
		else
			ShowWindow(GetDlgItem(hwnd,p_ctrl->idc),SW_HIDE);
	}
	return p_ctrl;
}
static void updateAddRate(HWND hwnd,int light_n)
{
	uint8_t *public_light;
	if (light_n == 0)
		public_light = &Public.light1;
	else if (light_n == 1)
		public_light = &Public.light2;
	else if (light_n == 2)
		public_light = &Public.light3;
	int value = BIT3(*public_light,0);
	if (++value >= NELEMENTS(opt_rate))
		value = 0;
	*public_light = (*public_light & ~(7 << 0) | value << 0);
    MgCtrlButton *p_ctrl = updateRate(hwnd,light_n,value);
	pro_com->sendOpt(p_ctrl->device_id, p_ctrl->op_code);
}

static void optRate1Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddRate(GetParent(hwnd),0);
}
static void optRate2Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddRate(GetParent(hwnd),1);
}
static void optRate3Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddRate(GetParent(hwnd),2);
}

static MgCtrlButton *updateColor(HWND hwnd,int light_n,int value)
{
	int i;
    ButtonArray *p_array;
    MgCtrlButton *p_ctrl;
	p_array = light[light_n].array + 3;
	for (i=0; i<p_array->num; i++) {
		p_ctrl = p_array->ctrl+i;
		if (value == i)
			ShowWindow(GetDlgItem(hwnd,p_ctrl->idc),SW_SHOWNORMAL);
		else
			ShowWindow(GetDlgItem(hwnd,p_ctrl->idc),SW_HIDE);
	}
	return p_ctrl;
}
static void updateAddColor(HWND hwnd,int light_n)
{
	uint8_t *public_light;
	if (light_n == 0)
		public_light = &Public.light1;
	else if (light_n == 1)
		public_light = &Public.light2;
	else if (light_n == 2)
		public_light = &Public.light3;
	int value = BIT(*public_light,3);
	if (++value >= NELEMENTS(opt_color))
		value = 0;
	*public_light = (*public_light & ~(1 << 3) | value << 3);
    MgCtrlButton *p_ctrl = updateColor(hwnd,light_n,value);
	pro_com->sendOpt(p_ctrl->device_id, p_ctrl->op_code);
}

static void optColor1Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddColor(GetParent(hwnd),0);
}
static void optColor2Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddColor(GetParent(hwnd),1);
}
static void optColor3Notify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	updateAddColor(GetParent(hwnd),2);
}

static void refreshAll(HWND hDlg)
{
	updatePower(hDlg,	0,(Public.light1 & (1 << 7)) >> 7);
	updatePower(hDlg,	1,(Public.light2 & (1 << 7)) >> 7);
	updatePower(hDlg,	2,(Public.light3 & (1 << 7)) >> 7);

	updateBright(hDlg,	0,(Public.light1 & (7 << 4)) >> 4);
	updateBright(hDlg,	1,(Public.light2 & (7 << 4)) >> 4);
	updateBright(hDlg,	2,(Public.light3 & (7 << 4)) >> 4);

	updateRate(hDlg,	0,(Public.light1 & (7 << 0)) >> 0);
	updateRate(hDlg,	1,(Public.light2 & (7 << 0)) >> 0);
	updateRate(hDlg,	2,(Public.light3 & (7 << 0)) >> 0);

	updateColor(hDlg,	0,(Public.light1 & (1 << 3)) >> 3);
	updateColor(hDlg,	1,(Public.light2 & (1 << 3)) >> 3);
	updateColor(hDlg,	2,(Public.light3 & (1 << 3)) >> 3);
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
	int i,j,k,idc=0;
    StructLight *p_light;
    ButtonArray *p_array;
    MgCtrlButton *p_ctrl;
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
	for (i=0; i<NELEMENTS(light); i++) {
        p_light = &light[i];
        for (j=0; j<p_light->array_num; j++) {
            p_array = p_light->array + j;
            for (k=0; k<p_array->num; k++) {
                p_ctrl = p_array->ctrl + k;
				p_ctrl->idc = idc++;
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
	for (i=0; i<NELEMENTS(opt_static_power); i++) {
		opt_static_power[i].idc = idc++;
		CreateWindowEx2 (CTRL_STATIC, "",
				WS_CHILD|WS_VISIBLE|SS_BITMAP,
				WS_EX_TRANSPARENT,
				opt_static_power[i].idc,
				opt_static_power[i].x,
				opt_static_power[i].y,
				opt_static_power[i].w,
				opt_static_power[i].h,
				hDlg, NULL, NULL,
				(DWORD)&opt_static_power[i].bmp_inactive);
	}
	refreshAll(hDlg);
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
                new_ctrl->x = p_ctrl->x;
                new_ctrl->y = p_array->y;
                new_ctrl->w = p_ctrl->w;
                new_ctrl->h = p_ctrl->h;
                new_ctrl->device_id = p_light->device_id;
                new_ctrl->op_code = p_ctrl->op_code;
				if (p_array->notif_proc)
					new_ctrl->notif_proc = p_array->notif_proc;
				else if (p_ctrl->notif_proc)
					new_ctrl->notif_proc = p_ctrl->notif_proc;
                sprintf(image_path,BMP_LOCAL_PATH"%s/%s%s(x%d，y%d).JPG",
                        p_light->path,
                        p_light->path,
                        p_ctrl->img_name,
                        p_ctrl->x,
                        p_array->y);
                bmpLoad(&new_ctrl->image_normal, image_path);
                sprintf(image_path,BMP_LOCAL_PATH"%s/%s%s-2(x%d，y%d).JPG",
                        p_light->path,
                        p_light->path,
                        p_ctrl->img_name,
                        p_ctrl->x,
                        p_array->y);
                bmpLoad(&new_ctrl->image_press, image_path);
				new_ctrl++;
            }
        }
	}
	for (i=0; i<NELEMENTS(opt_static_power); i++) {
		sprintf(image_path,BMP_LOCAL_PATH"%s/%s(x%d，y%d).JPG",
				opt_static_power[i].path,
				opt_static_power[i].path,
				opt_static_power[i].x,
				opt_static_power[i].y);
		bmpLoad(&opt_static_power[i].bmp_inactive, image_path);
		sprintf(image_path,BMP_LOCAL_PATH"%s/%s-2(x%d，y%d).JPG",
				opt_static_power[i].path,
				opt_static_power[i].path,
				opt_static_power[i].x,
				opt_static_power[i].y);
		bmpLoad(&opt_static_power[i].bmp_active, image_path);
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
		refreshAll(Form);
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

