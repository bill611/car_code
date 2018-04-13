/*
 * =============================================================================
 *
 *       Filename:  FormWlan.c
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
static int formWlanProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void btKyeboardDeletPress(HWND hwnd, int id, int nc, DWORD add_data);

static void btKyeboardNumPress(HWND hwnd, int id, int nc, DWORD add_data);
static void btKyeboardEnterPress(HWND hwnd, int id, int nc, DWORD add_data);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define IDC_FOMR_TIMER 0xff

#define BMP_LOCAL_PATH BMP_RES_PATH"WLAN/"
enum {

	IDC_LABER_ERR,
	IDC_BUTTON_ERR_CONFIRM,

	IDC_EDIT_ACCOUNT = 50,
	IDC_EDIT_PASSWORD ,
	IDC_LABER_BACKGROUND ,
};


/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_title; 
static int current_edit = IDC_EDIT_ACCOUNT;
static char key_num[] = "0123456789abcdefghijklmnopqrstuvwxyz ";

static int bmp_load_finished = 0;
static pthread_mutex_t mutex;		//队列控制互斥信号
static pthread_mutexattr_t mutexattr2;

static BmpLocation bmp_load[] = {
    {&bmp_title, BMP_LOCAL_PATH"WLAN(x8 ，y91).JPG"},
};

static MY_CTRLDATA ChildCtrls [] = {
    STATIC_IMAGE(8,91,472,612,IDC_LABER_BACKGROUND,(DWORD)&bmp_title),
    EDIT_L(148,220,244,36,IDC_EDIT_ACCOUNT,"",0,NULL),
    EDIT_PSD_L(148,264,244,36,IDC_EDIT_PASSWORD,"",0,NULL),
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
	.name = "FWlan",
	.dlgProc = formWlanProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
};

static MgCtrlButton opt_controls[] = {
	{0,	0,"0",402,325,39,49},
	{0,	0,"1",39,325,39,49},
	{0,	0,"2",81,325,37,49},
	{0,	0,"3",121,325,38,49},
	{0,	0,"4",161,325,38,49},
	{0,	0,"5",201,325,39,49},
	{0,	0,"6",241,325,38,49},
	{0,	0,"7",281,325,39,49},
	{0,	0,"8",321,325,39,49},
	{0,	0,"9",361,325,38,49},
	{0,	0,"a",60,434,39,49},
	{0,	0,"b",262,487,39,50},
	{0,	0,"c",181,487,39,50},
	{0,	0,"d",140,434,40,49},
	{0,	0,"e",121,378,38,50},
	{0,	0,"f",181,434,39,49},
	{0,	0,"g",221,434,38,49},
	{0,	0,"h",262,434,38,49},
	{0,	0,"i",321,378,39,50},
	{0,	0,"j",302,434,40,49},
	{0,	0,"k",341,434,40,49},
	{0,	0,"l",382,434,39,49},
	{0,	0,"m",341,487,40,50},
	{0,	0,"n",302,487,38,50},
	{0,	0,"o",361,378,38,50},
	{0,	0,"p",402,378,39,50},
	{0,	0,"q",39,378,39,50},
	{0,	0,"r",161,378,38,50},
	{0,	0,"s",101,434,38,49},
	{0,	0,"t",201,378,39,50},
	{0,	0,"u",281,378,39,50},
	{0,	0,"v",221,487,38,50},
	{0,	0,"w",81,378,37,50},
	{0,	0,"x",140,487,38,50},
	{0,	0,"y",241,378,38,50},
	{0,	0,"z",101,487,38,50},
	{0,	0,"space",140,543,160,49},
	{0,	0,"enter",302,543,139,49,btKyeboardEnterPress},
	{0,	0,"delete",39,543,100,49,btKyeboardDeletPress},
};

static FormBase* form_base = NULL;

static void editNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	char text[50];
	if (nc == EN_SETFOCUS){
		int ip_len = SendMessage (GetDlgItem(GetParent(hwnd),id), MSG_GETTEXT, 50,  (LPARAM)text);
		SendMessage (GetDlgItem(GetParent(hwnd),id), EM_SETCARETPOS, 0, ip_len);
	} else if (nc == EN_KILLFOCUS) {
		current_edit = id;
	}
}
/* ----------------------------------------------------------------*/
/**
 * @brief btKyeboardNumPress 获取设备ID按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void btKyeboardNumPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	SendMessage(GetDlgItem(GetParent(hwnd),current_edit),MSG_CHAR,key_num[id],0);
}

static void btKyeboardDeletPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc == BN_PUSHED) {
		SetTimer(GetParent (hwnd),IDC_FOMR_TIMER,TIME_100MS);
		return;
	}
	if (nc == BN_UNPUSHED) {
		KillTimer (GetParent (hwnd),IDC_FOMR_TIMER);
        SendMessage(GetDlgItem(GetParent(hwnd),current_edit),MSG_CHAR,'\b',0);
	}
}

static void btKyeboardEnterPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    char pwd[32] = {0};
    char account[32] = {0};
	FILE *fp;

    GetWindowText(GetDlgItem (GetParent (hwnd), IDC_EDIT_ACCOUNT),
            account,sizeof(account));
    GetWindowText(GetDlgItem (GetParent (hwnd), IDC_EDIT_PASSWORD),
            pwd,sizeof(pwd));
    saveLog("account:%s,password:%s\n", account,pwd);
	fp = fopen("network_config","wb");
	if(fp) {
#if 1
		fprintf(fp,"SSID %s\n",account);
		fprintf(fp,"AUTH_KEY %s\n",pwd);
#else
		// fprintf(fp,"SSID TC_OFFICE\n",account);
		// fprintf(fp,"AUTH_KEY TC.86kb.com\n",pwd);
        fprintf(fp,"SSID LanBin\n",account);
        fprintf(fp,"AUTH_KEY lan102520\n",pwd);
#endif
        fflush(fp);
		fclose(fp);
		fp = NULL;
		sync();
	} else
		printf("Can't open network_config\n");
	if(fp)
		fclose(fp);
    restartNetwork();
    topMessage(GetParent (hwnd),TOPBOX_WIFI_CONNECTING,NULL );
    ShowWindow(GetParent(hwnd),SW_HIDE);
}

void formWlanLoadLock(void)
{
    INIT_MUTEX_LOCK(mutexattr2,mutex);
}

void formWlanLoadBmp(void)
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
		sprintf(image_path,BMP_LOCAL_PATH"%s(X%d，Y%d).JPG",opt_controls[i].img_name,
				opt_controls[i].x,
				opt_controls[i].y);
		bmpLoad(&opt_controls[i].image_normal, image_path);
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
	HWND hCtrl;
    // formWlanLoadBmp();
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_PASSWORD),"");
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_ACCOUNT),"");
    // CreateWindowEx2 (CTRL_STATIC, "",
            // WS_CHILD|WS_VISIBLE|SS_BITMAP,
            // WS_EX_TRANSPARENT,
            // IDC_LABER_BACKGROUND, // 避免与toolbar冲突 
            // ,
            // hDlg, NULL, NULL,
            // (DWORD)&bmp_title);
	for (i=0; i<NELEMENTS(opt_controls); i++) {
        opt_controls[i].idc = i;
		if (opt_controls[i].notif_proc == NULL)
			opt_controls[i].notif_proc = btKyeboardNumPress;
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
	SetWindowElementAttr(GetDlgItem(hDlg,IDC_EDIT_ACCOUNT), WE_FGC_WINDOW,MY_WHIGHT);
	SendDlgItemMessage(hDlg,IDC_EDIT_ACCOUNT, EM_LIMITTEXT, 20, 0L);
	SetNotificationCallback (GetDlgItem(hDlg,IDC_EDIT_ACCOUNT), editNotify);
	SetWindowElementAttr(GetDlgItem(hDlg,IDC_EDIT_PASSWORD), WE_FGC_WINDOW,MY_WHIGHT);
	SendDlgItemMessage(hDlg,IDC_EDIT_PASSWORD, EM_LIMITTEXT, 20, 0L);
	SetNotificationCallback (GetDlgItem(hDlg,IDC_EDIT_PASSWORD), editNotify);
	formManiCreateToolBar(hDlg);
}

/* ----------------------------------------------------------------*/
/**
 * @brief formWlanProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formWlanProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
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
				
                SendMessage(GetDlgItem(hDlg,current_edit),MSG_CHAR,'\b',0);
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
 * @brief createFormWlan 创建窗口
 *
 * @param hMainWnd
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
int createFormWlan(HWND hMainWnd)
{
	HWND Form = Screen.Find(form_base_priv.name);
	if(Form) {
        current_edit = IDC_EDIT_ACCOUNT;
		SetWindowText(GetDlgItem(Form,IDC_EDIT_PASSWORD),"");
		SetWindowText(GetDlgItem(Form,IDC_EDIT_ACCOUNT),"");
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

