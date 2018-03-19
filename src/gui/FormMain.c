/*
 * =============================================================================
 *
 *       Filename:  FormMain.c
 *
 *    Description:  主窗口
 *
 *        Version:  1.0
 *        Created:  2016-02-23 15:32:24
 *       Revision:  1.0
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
#include "debug.h"
#include "commongdi.h"
#include "protocol.h"
#include "FormMain.h"
/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/
extern int createFormVersion(HWND hMainWnd);
extern int createFormCD(HWND hMainWnd);
extern int createFormElectricChair(HWND hMainWnd);
extern int createFormSecretaryChair(HWND hMainWnd);
extern int createFormRotChair(HWND hMainWnd);
extern int createFormCurtain(HWND hMainWnd);
extern int createFormGlassScreen(HWND hMainWnd);
extern int createFormTvScreen(HWND hMainWnd);
extern int createFormDVD(HWND hMainWnd);
extern int createFormMonitor(HWND hMainWnd);
extern int createFormDoor(HWND hMainWnd);
extern int createFormBed(HWND hMainWnd);
extern int createFormSaTv(HWND hMainWnd);
extern int createFormSkyLight(HWND hMainWnd);
extern int createFormLight(HWND hMainWnd);
extern int createFormProjection(HWND hMainWnd);
extern int createFormTable(HWND hMainWnd);
extern int createFormA12(HWND hMainWnd,int type);
extern int createFormWlan(HWND hMainWnd);

extern void formVersionLoadBmp(void);
extern void formPasswordLoadBmp(void);
extern void formPresetLoadBmp(void);
extern void formCDLoadBmp(void);
extern void formElectricChairLoadBmp(void);
extern void formSecretaryChairLoadBmp(void);
extern void formRotChairLoadBmp(void);
extern void formCurtainLoadBmp(void);
extern void formGlassScreenLoadBmp(void);
extern void formTvScreenLoadBmp(void);
extern void formDVDLoadBmp(void);
extern void formMonitorLoadBmp(void);
extern void formDoorLoadBmp(void);
extern void formBedLoadBmp(void);
extern void formSaTvLoadBmp(void);
extern void formSkyLightLoadBmp(void);
extern void formLightLoadBmp(void);
extern void formProjectionLoadBmp(void);
extern void formTableLoadBmp(void);
extern void formA12LoadBmp(void);
extern void formTopBoxLoadBmp(void);
extern void formWlanLoadBmp(void);

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static void formMainLoadBmp(void);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_MAIN > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

typedef void (*InitBmpFunc)(void) ;

#define BMP_LOCAL_PATH "res/image/主页/"

enum {
    IDC_MAIN_PAGE = 100,  // 主页
    IDC_VOLUEM_REDUCE, // 音量减少
    IDC_VOLUEM_ADD, // 音量增加
    IDC_MUTE,  // 静音
    IDC_CLOSE,  // 关闭
    IDC_WIFI,  // wifi 图标
};
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
BITMAP bmp_bkg1; // 背景1
BITMAP bmp_bkg2; // 背景2
static BITMAP bmp_wifi; // wifi

static BmpLocation bmp_load[] = {
	{&bmp_bkg1, "res/image/背景.JPG"},
	{&bmp_bkg2, "res/image/背景-2.JPG"},
	{&bmp_wifi, BMP_LOCAL_PATH"/wifi(x424，y5).JPG"},
};


// device 0x11
static MgCtrlButton opt_controls[] = {
	{IDC_CHAIR_ELE,			0x84,"电动座椅",25,96}, //电动座椅
	{IDC_CHAIR_SECRETARY,	0xab,"秘书椅",139,96}, //秘书椅
	{IDC_CHAIR_ROT,			0x83,"旋转座椅",254,96}, //旋转座椅
	{IDC_CURTAIN,			0xad,"窗帘",367,96}, //窗帘
	{IDC_SCREEN_GLASS,		0xaa,"玻璃屏",25,219}, //玻璃屏
	{IDC_SCREEN_TV,			0xa3,"电视屏幕",139,219}, //电视屏
	{IDC_CD,				0xaf,"CD",254,219}, //CD
	{IDC_DVD,				0x8e,"DVD",367,219}, //DVD
	{IDC_MONITOR,			0x82,"监控",25,340}, //监控
	{IDC_DOOR,				0x81,"车门",139,340}, //车门
	{IDC_BED_ELE,			0x85,"电动床",254,340}, //电动床
	{IDC_SATV,				0x83,"卫星电视",367,340}, //卫星电视
	{IDC_SKYLIGHT,			0x86,"天窗",25,461}, //天窗
	{IDC_LIGHT,				0xa2,"灯光",139,461}, //灯光
	{IDC_PROJECTION,		0x80,"屏幕投影",254,461}, //屏幕投影
	{IDC_TABLE,				0x87,"桌板",367,461}, //桌板
	{IDC_A1,				0x88,"A1",59,584}, //A1
	{IDC_A2,				0x89,"A2",139,584}, //A2
	{IDC_WALN,				0,"WLAN",25,584}, //WALN
	{IDC_SYSTEM,			0,"系统",139,584}, //系统
};

static MgCtrlButton opt_toolbar_controls[] = {
	{IDC_MAIN_PAGE,		0xd1,"主页",9,714,93,63},
	{IDC_VOLUEM_REDUCE,	0xb4,"声音-",102,714,92,63},
	{IDC_VOLUEM_ADD,	0xc4,"声音+",285,714,91,63},
	{IDC_MUTE,			0xb0,"静音",194,714,91,63},
	{IDC_CLOSE,			0,"关闭",376,714,94,63},
};

static InitBmpFunc loadBmps[] = {
    formMainLoadBmp,
    formVersionLoadBmp,
	formPasswordLoadBmp,
    formPresetLoadBmp,
	formCDLoadBmp,
	formElectricChairLoadBmp,
    formSecretaryChairLoadBmp,
    formRotChairLoadBmp,
    formCurtainLoadBmp,
	formGlassScreenLoadBmp,
	formTvScreenLoadBmp,
	formDVDLoadBmp,
	formMonitorLoadBmp,
	formDoorLoadBmp,
	formBedLoadBmp,
	formSaTvLoadBmp,
	formSkyLightLoadBmp,
	formLightLoadBmp,
	formProjectionLoadBmp,
	formTableLoadBmp,
	formA12LoadBmp,
    formTopBoxLoadBmp,
    formWlanLoadBmp,
};

static HWND hwnd_main = HWND_INVALID;
static FormMainTimers *timers_tbl;
static int (*mainAppProc)(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainTimerStart 开启定时器 单位100ms
 *
 * @param idc_timer 定时器id号，同时也是编号
 */
/* ---------------------------------------------------------------------------*/
static void formMainTimerStart(int idc_timer)
{
	SendMessage(hwnd_main, MSG_MAIN_TIMER_START, (WPARAM)idc_timer, 0);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainTimerStop 关闭定时器
 *
 * @param idc_timer 定时器id号
 */
/* ---------------------------------------------------------------------------*/
static void formMainTimerStop(int idc_timer)
{
	SendMessage(hwnd_main, MSG_MAIN_TIMER_STOP, (WPARAM)idc_timer, 0);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainTimerGetState 返回定时器当前是否激活
 *
 * @param idc_timer
 *
 * @returns 1激活 0未激活
 */
/* ---------------------------------------------------------------------------*/
static int formMainTimerGetState(int idc_timer)
{
	return	IsTimerInstalled(hwnd_main,idc_timer);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainTimerProc1s 窗口相关定时函数
 *
 * @returns 1按键超时退出 0未超时退出
 */
/* ---------------------------------------------------------------------------*/
static int formMainTimerProc1s(void)
{
	return 0;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainSetNetWorkState 改变网口状态图标
 *
 * @param state 0未连接 1连接
 */
/* ---------------------------------------------------------------------------*/
static void formMainSetNetWorkState(int state)
{
	static int state_old = 0;
	if (state_old == state) {
		return;
	}
	state_old = state;
    if (state)
        ShowWindow(GetDlgItem (hwnd_main, IDC_WIFI), SW_SHOWNORMAL);
    else
        ShowWindow(GetDlgItem (hwnd_main, IDC_WIFI), SW_HIDE);

}

static void optControlsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	HWND parent_hwnd = GetParent(hwnd);
	saveLog("[%s]%d\n",__FUNCTION__, id);
	pro_com->sendOpt(opt_controls[id].device_id,
			opt_controls[id].op_code);
    switch (id) {
		case	IDC_CHAIR_ELE:
			createFormElectricChair(parent_hwnd);
			break;
		case	IDC_CHAIR_SECRETARY:
			createFormSecretaryChair(parent_hwnd);
			break;
		case	IDC_CHAIR_ROT:
	  		createFormRotChair(parent_hwnd);
			break;
		case	IDC_CURTAIN:
			createFormCurtain(parent_hwnd);
			break;
		case	IDC_SCREEN_GLASS:
			createFormGlassScreen(parent_hwnd);
			break;
		case	IDC_SCREEN_TV:
			createFormTvScreen(parent_hwnd);
			break;
		case	IDC_CD:
			createFormCD(parent_hwnd);
			break;
		case	IDC_DVD:
			createFormDVD(parent_hwnd);
			break;
		case	IDC_MONITOR:
			createFormMonitor(parent_hwnd);
			break;
		case	IDC_DOOR:
			createFormDoor(parent_hwnd);
			break;
		case	IDC_BED_ELE:
			createFormBed(parent_hwnd);
			break;
		case	IDC_SATV:
			createFormSaTv(parent_hwnd);
			break;
		case	IDC_SKYLIGHT:
			createFormSkyLight(parent_hwnd);
			break;
		case	IDC_LIGHT:
			createFormLight(parent_hwnd);
			break;
		case	IDC_PROJECTION:
			createFormProjection(parent_hwnd);
			break;
		case	IDC_TABLE:
			createFormTable(parent_hwnd);
			break;
		case	IDC_A1:
			createFormA12(parent_hwnd,0);
			break;
		case	IDC_A2:
			createFormA12(parent_hwnd,1);
			break;
		case	IDC_WALN:
            createFormWlan(parent_hwnd);
			break;
        case IDC_SYSTEM:
			createFormVersion(parent_hwnd);
		   	break;
        default:
            break;
    }
}

static void optToolbarsNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	saveLog("[%s]%d\n",__FUNCTION__, id);
	pro_com->sendOpt(opt_toolbar_controls[id-IDC_MAIN_PAGE].device_id,
			opt_controls[id-IDC_MAIN_PAGE].op_code);
    switch (id) {
		case	IDC_MAIN_PAGE:
			{
				SendMessage(Screen.hMainWnd, MSG_MAIN_SHOW_NORMAL, 0, 0);
			} break;
		case	IDC_MUTE:
			{
				Public.mute = SendMessage(GetDlgItem (GetParent (hwnd), id),
						MSG_MYBUTTON_GET_SELECT_STATE, 0, 0);
			} break;
		case	IDC_CLOSE:
			{
				SendMessage(Screen.hMainWnd, MSG_MAIN_SHOW_NORMAL, 0, 0);
				screensaverStart(LCD_OFF);
			} break;

        default:
            break;
    }
}

static void showNormal(HWND hWnd)
{
	int i,k;
    HWND ctrl;
	for (i=0,k=0; i<NELEMENTS(opt_controls); i++) {
        ctrl = GetDlgItem(hWnd,opt_controls[i].idc);
        SendMessage(ctrl, MSG_MYBUTTON_SET_SELECT_MODE, 0, 0);
		if (g_config.device_main_controls[i]) {
			k++;
			opt_controls[i].x = 25 + ((k-1) % 4)*114;
			opt_controls[i].y = 96 + ((k-1) / 4)*123;
			ShowWindow(ctrl,SW_SHOWNORMAL);
			MoveWindow(ctrl,
					opt_controls[i].x,
					opt_controls[i].y,
					opt_controls[i].w,
					opt_controls[i].h,TRUE);
		} else {
			ShowWindow(ctrl,SW_HIDE);
		}
	}
	for (i=0; i<NELEMENTS(opt_toolbar_controls); i++) {
        ctrl = GetDlgItem(hWnd,opt_toolbar_controls[i].idc);
        ShowWindow(ctrl,SW_SHOWNORMAL);
	}
	SendMessage(hWnd,MSG_UPDATESTATUS,0,0);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainUpdateMute 更新静音状态
 */
/* ---------------------------------------------------------------------------*/
void formMainUpdateMute(HWND hWnd)
{
	printf("update:%d\n",Public.mute);
	// 更新静音状态
	SendMessage(GetDlgItem(hWnd,opt_toolbar_controls[IDC_MUTE-IDC_MAIN_PAGE].idc),
			   MSG_MYBUTTON_SET_SELECT_STATE, Public.mute, 0);
}

void formManiCreateToolBar(HWND hWnd)
{
	int i;
	for (i=0; i<NELEMENTS(opt_toolbar_controls); i++) {
		opt_toolbar_controls[i].device_id = 0x11;
		opt_toolbar_controls[i].notif_proc = optToolbarsNotify;
        createSkinButton(hWnd,
                opt_toolbar_controls[i].idc,
                opt_toolbar_controls[i].x,
                opt_toolbar_controls[i].y,
                opt_toolbar_controls[i].w,
                opt_toolbar_controls[i].h,
                &opt_toolbar_controls[i].image_normal,
                &opt_toolbar_controls[i].image_press,
                1,
                0,
                opt_toolbar_controls[i].notif_proc);
	}
	SendMessage(GetDlgItem(hWnd,opt_toolbar_controls[IDC_MUTE-IDC_MAIN_PAGE].idc),
		   	MSG_MYBUTTON_SET_SELECT_MODE, 2, 0);
	SendMessage(GetDlgItem(hWnd,opt_toolbar_controls[IDC_MUTE-IDC_MAIN_PAGE].idc),
			   MSG_MYBUTTON_SET_SELECT_STATE, Public.mute, 0);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainCreateControl 创建控件
 *
 * @param hWnd
 */
/* ---------------------------------------------------------------------------*/
static void formMainCreateControl(HWND hWnd)
{
	int i,k = 0;
	char image_path[128] = {0};
	for (i=0; i<NELEMENTS(opt_controls); i++) {
		if (g_config.device_main_controls[i]) {
			k++;
			opt_controls[i].x = 25 + ((k-1) % 4)*114;
			opt_controls[i].y = 96 + ((k-1) / 4)*123;
		}
		opt_controls[i].device_id = 0x11;
		opt_controls[i].w = 118;
		opt_controls[i].h = 122;
		opt_controls[i].notif_proc = optControlsNotify;
        createSkinButton(hWnd,
                opt_controls[i].idc,
                opt_controls[i].x,
                opt_controls[i].y,
                opt_controls[i].w,
                opt_controls[i].h,
                &opt_controls[i].image_normal,
                &opt_controls[i].image_press,
                g_config.device_main_controls[i],
                0,
                opt_controls[i].notif_proc);
	}
	formManiCreateToolBar(hWnd);
    CreateWindowEx2 (CTRL_STATIC, "",
            WS_CHILD|WS_VISIBLE|SS_BITMAP,
            WS_EX_TRANSPARENT,
            IDC_WIFI,
            424,5,36,35,
            hWnd, NULL, NULL,
            (DWORD)&bmp_wifi);
    ShowWindow(GetDlgItem (hWnd, IDC_WIFI), SW_HIDE);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief bmpsMainButtonLoad 加载主界面图片
 *
 * @param controls
 * @param num
 */
/* ---------------------------------------------------------------------------*/
static void bmpsMainButtonLoad(MgCtrlButton *controls,int num)
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

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainLoadBmp 加载主界面图片
 */
/* ---------------------------------------------------------------------------*/
static void formMainLoadBmp(void)
{
	int i;
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
    bmpsMainButtonLoad(BMP_LOAD_PARA(opt_controls));
    bmpsMainButtonLoad(BMP_LOAD_PARA(opt_toolbar_controls));
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief fromLoadBmps 加载各个窗口的图片
 */
/* ---------------------------------------------------------------------------*/
static void fromLoadBmps(void)
{
    int i;
    for (i=0; i<NELEMENTS(loadBmps); i++) {
        loadBmps[i]();
    }
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ---------------------------------------------------------------------------*/
static int formMainProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	// 执行基本业务逻辑
	if (mainAppProc(hWnd, message, wParam, lParam) == 0) {
		return DefaultMainWinProc(hWnd, message, wParam, lParam);
	}

	switch (message)
	{
		case MSG_CREATE:
			{
                int i;
				Screen.Add(hWnd,"TFrmMain");
				hwnd_main = Screen.hMainWnd = hWnd;
				// 装载所有图片
				fromLoadBmps();
				// 创建主窗口控件
				formMainCreateControl(hWnd);
				formMainTimerStart(IDC_TIMER_1S);
				// screensaverStart(LCD_ON);
			} break;

		case MSG_ERASEBKGND:
			{
				DrawBackground(hWnd,
					   	(HDC)wParam,
					   	(const RECT*)lParam,&bmp_bkg1);
			} return 0;

		case MSG_MAIN_SHOW_NORMAL:
			{
                Screen.ReturnMain();
                showNormal(hWnd);
			} return 0;

		case MSG_MAIN_TIMER_START:
			{
				SetTimer(hWnd,
						wParam,
						(timers_tbl + wParam)->time * TIME_100MS);
			} return 0;

		case MSG_MAIN_TIMER_STOP:
			{
				if (IsTimerInstalled(hwnd_main,wParam) == TRUE) {
					KillTimer (hwnd_main,wParam);
				}
			} return 0;

		case MSG_TIMER:
			{
				if ((wParam >= IDC_TIMER_1S) && (wParam < IDC_TIMER_NUM)) {
					(timers_tbl + wParam)->proc();
				}
			} return 0;

		case MSG_LBUTTONDOWN:
			{
				// screensaverStart(LCD_ON);
			} break;

		case MSG_UPDATESTATUS:
			{
				formMainUpdateMute(hWnd);
			} break;

		case MSG_DESTROY:
			{
				Screen.Del(hWnd);
				DestroyAllControls (hWnd);
			} return 0;

		case MSG_CLOSE:
			{
				int i;
				for (i=IDC_TIMER_1S; i<IDC_TIMER_NUM; i++) {
					formMainTimerStop(i);
				}
				DestroyMainWindow (hWnd);
				PostQuitMessage (hWnd);
			} return 0;

		default:
			break;
	}
	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainLoop 主窗口消息循环
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
static int formMainLoop(void)
{
    MSG Msg;
	while (GetMessage(&Msg, Screen.hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	MyButtonControlCleanup();
    MainWindowThreadCleanup (Screen.hMainWnd);
	hwnd_main = Screen.hMainWnd = 0;
	bmpsRelease(bmp_load,NELEMENTS(bmp_load));
	return 0;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief formMainCreate 创建主窗口
 *
 * @param AppProc
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
FormMain * formMainCreate(
		int (*AppProc)(HWND,int,WPARAM,LPARAM),
		FormMainTimers *timers_table)
{
    MAINWINCREATE CreateInfo;
	mainAppProc = AppProc;
	timers_tbl = timers_table;
	RegisterMyButtonControl();

    CreateInfo.dwStyle = WS_NONE;
	CreateInfo.dwExStyle = WS_EX_AUTOSECONDARYDC;
	// CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "CarControl";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = formMainProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = SCR_WIDTH;
    CreateInfo.by = SCR_HEIGHT;
    CreateInfo.iBkColor = GetWindowElementColor(WE_MAINC_THREED_BODY);
    CreateInfo.dwAddData = 0;
    CreateInfo.dwReserved = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

	CreateMainWindow (&CreateInfo);
	if (hwnd_main == HWND_INVALID)
		return NULL;
	ShowWindow(hwnd_main, SW_SHOWNORMAL);

	FormMain * this = (FormMain *) calloc (1,sizeof(FormMain));
	this->timer = timers_table;
	this->loop = formMainLoop;
	this->timerStart = formMainTimerStart;
	this->timerStop = formMainTimerStop;
	this->timerGetState = formMainTimerGetState;
	this->timerProc1s = formMainTimerProc1s;
    this->setNetWorkState = formMainSetNetWorkState;

	return this;
}

