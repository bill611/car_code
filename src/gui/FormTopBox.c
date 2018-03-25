/*
 * =============================================================================
 *
 *       Filename:  FormTopBox.c
 *
 *    Description:  提示框
 *
 *        Version:  1.0
 *        Created:  2018-03-15 23:38:21
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
#include <stdlib.h>
#include <string.h>


#include "externfunc.h"

#include "public.h"
#include "predefine.h"


/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#define DISPLAY_TIME 	3000		//显示时间 2S

#define BMP_LOCAL_PATH "res/image/弹出提示/"

enum {
	IDC_TOPBOX_TIMER ,
	IDC_BUTTON_CONFIRM ,
	IDC_BUTTON_CANCEL
};

typedef struct _BedBoxButton{
    HWND idc;
    BITMAP bmp[2];
}BedBoxButton;
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static int form_type;
static void (*callbackFunc)(void) = NULL;
static BITMAP *bmp_bkg = NULL; 
static BITMAP bmp_bed_lie; // 电动床全躺
static BITMAP bmp_bed_unlie;// 电动床全收
static BITMAP bmp_table_fold;// 桌板展开
static BITMAP bmp_table_unfold;// 桌板收起
static BITMAP bmp_leg_fold;// 腿托展开
static BITMAP bmp_leg_unfold;// 腿托收起
static BITMAP bmp_chair_fold;// 座椅展开
static BITMAP bmp_chair_unfold;// 桌板收起
static BITMAP bmp_wifi_connecting;// wifi连接中
static BITMAP bmp_wifi_failed;// wifi连接失败
static BITMAP notice_confirm[2];// 2个键的确认
static BITMAP notice_cancel[2];// 2个键的取消
static BITMAP wifi_confirm[2];// wifi确认

static BmpLocation bmp_load[] = {
    {&bmp_bed_lie, BMP_LOCAL_PATH"电动床全躺提示(X70-Y304).JPG"},
    {&bmp_bed_unlie, BMP_LOCAL_PATH"电动床全收提示(X70-Y304).JPG"},
    {&bmp_table_fold, BMP_LOCAL_PATH"桌板全收提示(X70-Y304).JPG"},
    {&bmp_table_unfold, BMP_LOCAL_PATH"桌板全展提示(X70-Y304).JPG"},
    {&bmp_leg_fold, BMP_LOCAL_PATH"桌腿收拢提示(X70-Y304).JPG"},
    {&bmp_leg_unfold, BMP_LOCAL_PATH"桌腿展开提示(X70-Y304).JPG"},
    {&bmp_chair_fold, BMP_LOCAL_PATH"座椅全收提示(X70-Y304).JPG"},
    {&bmp_chair_unfold, BMP_LOCAL_PATH"座椅全躺提示(X70-Y304).JPG"},
    {&bmp_wifi_connecting, BMP_LOCAL_PATH"正在连接提示(X100，Y278).JPG"},
    {&bmp_wifi_failed, BMP_LOCAL_PATH"wifi失败(X100，Y278).JPG"},
    {&notice_confirm[0], BMP_LOCAL_PATH"确定(X240-Y404).JPG"},
    {&notice_confirm[1], BMP_LOCAL_PATH"确定-2(X240-Y404).JPG"},
    {&notice_cancel[0], BMP_LOCAL_PATH"取消(X70-Y404).JPG"},
    {&notice_cancel[1], BMP_LOCAL_PATH"取消-2(X70-Y404).JPG"},
    {&wifi_confirm[0], BMP_LOCAL_PATH"确定(X186，Y389).JPG"},
    {&wifi_confirm[1], BMP_LOCAL_PATH"确定-2(X186，Y389).JPG"},
};

//----------------------------------------------------------------------------
static CTRLDATA ChildCtrls [] = { };
//----------------------------------------------------------------------------

static DLGTEMPLATE DlgInitParam =
{
    WS_NONE,
    WS_EX_NONE,
    70,304,340,155,
    "",
    0, 0,       //menu and icon is null
    sizeof(ChildCtrls)/sizeof(CTRLDATA),
    ChildCtrls, //pointer to control array
    0           //additional data,must be zero
};
static void btConfirmPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
    callbackFunc();
	SendMessage(GetParent (hwnd), MSG_CLOSE, 0, 0);
}
static void btCancelPress(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != BN_CLICKED)
		return;
	SendMessage(GetParent (hwnd), MSG_CLOSE, 0, 0);
}
//----------------------------------------------------------------------------
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    if (form_type == TOPBOX_WIFI_CONNECTING || form_type == TOPBOX_WIFI_FAILED) {
        createSkinButton(hDlg,
                IDC_BUTTON_CANCEL,
                86,111,114,47,
                &wifi_confirm[0],
                &wifi_confirm[1],
                1, 0,
                btCancelPress);
    } else {
        createSkinButton(hDlg,
                IDC_BUTTON_CONFIRM,
                170,100,170,55,
                &notice_confirm[0],
                &notice_confirm[1],
                1, 0,
                btConfirmPress);
        createSkinButton(hDlg,
                IDC_BUTTON_CANCEL,
                0,100,170,55,
                &notice_cancel[0],
                &notice_cancel[1],
                1, 0,
                btCancelPress);
    }
}
static int MessageBoxWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_INITDIALOG:
            {
                Screen.Add(hWnd,"TFrmTopMessage");
                initPara(hWnd,message,wParam,lParam);
                SetTimer(hWnd,IDC_TOPBOX_TIMER,DISPLAY_TIME);
                break;
            }
        case MSG_TIMER:
            if (wParam != IDC_TOPBOX_TIMER){
                break;
            }
            KillTimer(hWnd,IDC_TOPBOX_TIMER);
            SendNotifyMessage(hWnd,MSG_CLOSE,0,0);
            break;
        case MSG_ERASEBKGND:
            if (bmp_bkg)
                DrawBackground (hWnd, (HDC)wParam, (const RECT*)lParam,bmp_bkg);
            return 0;
        case MSG_CLOSE:
            EndDialog(hWnd,lParam);
            return 0;
        case MSG_DESTROY:
            Screen.Del(hWnd);
            DestroyAllControls (hWnd);
            return 0;
        default:
            break;
    }
    return DefaultDialogProc(hWnd, message, wParam, lParam);
}

void formTopBoxLoadBmp(void)
{
	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(BMP_LOAD_PARA(bmp_load));
}
//----------------------------------------------------------------------------
int topMessage(HWND hMainWnd,int type,void (*notif_proc)(void) )
{
    callbackFunc = notif_proc;
    form_type = type;
    switch (form_type)
    {
        case TOPBOX_BED_LIE :      bmp_bkg = &bmp_bed_lie;break;
        case TOPBOX_BED_UNLIE :    bmp_bkg = &bmp_bed_unlie;break;
        case TOPBOX_TABLE_FOLD :   bmp_bkg = &bmp_table_fold;break;
        case TOPBOX_TABLE_UNFOLD : bmp_bkg = &bmp_table_unfold;break;
        case TOPBOX_LEG_FOLD :     bmp_bkg = &bmp_leg_fold;break;
        case TOPBOX_LEG_UNFOLD :   bmp_bkg = &bmp_leg_unfold;break;
        case TOPBOX_CHAIR_FOLD :   bmp_bkg = &bmp_chair_fold;break;
        case TOPBOX_CHAIR_UNFOLD : bmp_bkg = &bmp_chair_unfold;break;
        case TOPBOX_WIFI_CONNECTING : 
        case TOPBOX_WIFI_FAILED : 
                                   {
                                       if (type == TOPBOX_WIFI_CONNECTING)
                                           bmp_bkg = &bmp_wifi_connecting;
                                       else
                                           bmp_bkg = &bmp_wifi_failed;
                                       DlgInitParam.x = 100;
                                       DlgInitParam.y = 278;
                                       DlgInitParam.w = 286;
                                       DlgInitParam.h = 173;
                                   }break;
        default : break;
    }
       

    return DialogBoxIndirectParam(&DlgInitParam,hMainWnd,MessageBoxWinProc,0);

}
