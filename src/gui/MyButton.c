/*
 * =====================================================================================
 *
 *       Filename:  MyButton.c
 *
 *    Description:  自定义按钮
 *
 *        Version:  1.0
 *        Created:  2015-12-09 16:22:37
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
/* ----------------------------------------------------------------*
 *                      include head files
 *-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "MyButton.h"
#include "cliprect.h"
#include "internals.h"
#include "ctrlclass.h"


/* ----------------------------------------------------------------*
 *                  extern variables declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                  internal functions declare
 *-----------------------------------------------------------------*/
static int myButtonControlProc (HWND hwnd, int message, WPARAM wParam, LPARAM lParam);

/* ----------------------------------------------------------------*
 *                        macro define
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                      variables define
 *-----------------------------------------------------------------*/
static BITMAP bmp_button_select ; 	// 按键选择图标
static BITMAP bmp_button_unselect ; 	// 按键非选择图标

/* ---------------------------------------------------------------------------*/
/**
 * @brief RegisterMyButtonControl 注册控件
 *
 * @returns 
 */
/* ---------------------------------------------------------------------------*/
BOOL RegisterMyButtonControl (void)
{
    WNDCLASS WndClass;

    WndClass.spClassName = CTRL_MYBUTTON;
    WndClass.dwStyle     = WS_NONE;
    WndClass.dwExStyle   = WS_EX_NONE;
    WndClass.hCursor     = GetSystemCursor (IDC_ARROW);
    WndClass.iBkColor    = 0;
    WndClass.WinProc     = myButtonControlProc;

    return RegisterWindowClass(&WndClass);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief MyButtonControlCleanup 卸载控件
 */
/* ---------------------------------------------------------------------------*/
void MyButtonControlCleanup (void)
{
    UnregisterWindowClass(CTRL_MYBUTTON);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief paint 主要绘图函数
 *
 * @param hWnd
 * @param hdc
 */
/* ---------------------------------------------------------------------------*/
static void paint(HWND hWnd,HDC hdc)
{
	RECT rcClient;
    PCONTROL    pCtrl;
    pCtrl = Control (hWnd);
	GetClientRect (hWnd, &rcClient);
	// printf("[%s]t:%d,b:%d,l:%d,r:%d\n",__FUNCTION__,rcClient.top,rcClient.bottom,rcClient.left,rcClient.right);

	if (pCtrl->dwAddData2) {
		ButtonCtrlInfo* pInfo = (ButtonCtrlInfo*)(pCtrl->dwAddData2);

		if (pInfo->select.mode) {
			FillBoxWithBitmap(hdc,rcClient.left,rcClient.top,pInfo->image_normal->bmWidth,pInfo->image_normal->bmHeight,pInfo->image_normal);
			if(pInfo->select.state == BUT_STATE_SELECT) 
				FillBoxWithBitmap(hdc,rcClient.left + 80,rcClient.top + 79,bmp_button_select.bmWidth,bmp_button_select.bmHeight,&bmp_button_select);
			else
				FillBoxWithBitmap(hdc,rcClient.left + 80,rcClient.top + 79,bmp_button_select.bmWidth,bmp_button_select.bmHeight,&bmp_button_unselect);
		} else {
			if(pInfo->state == BUT_NORMAL)
                FillBoxWithBitmap(hdc,rcClient.left,rcClient.top,pInfo->image_normal->bmWidth,pInfo->image_normal->bmHeight,pInfo->image_normal);
			else 
                FillBoxWithBitmap(hdc,rcClient.left,rcClient.top,pInfo->image_press->bmWidth,pInfo->image_press->bmHeight,pInfo->image_press);
			
		}
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief myButtonControlProc 控件主回调函数
 *
 * @param hwnd
 * @param message
 * @param wParam
 * @param lParam
 *
 * @returns 
 */
/* ---------------------------------------------------------------------------*/
static int myButtonControlProc (HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC         hdc;
    PCONTROL    pCtrl;
    DWORD       dwStyle;
    ButtonCtrlInfo* pInfo;

	pCtrl = Control (hwnd);
	pInfo = (ButtonCtrlInfo*)pCtrl->dwAddData2;
	dwStyle = GetWindowStyle (hwnd);

	switch (message) {
	case MSG_CREATE:
	{
		ButtonCtrlInfo* data = (ButtonCtrlInfo*)pCtrl->dwAddData;

		pInfo = (ButtonCtrlInfo*) calloc (1, sizeof (ButtonCtrlInfo));
		if (pInfo == NULL)
			return -1;
		memset(pInfo,0,sizeof(ButtonCtrlInfo));
		pInfo->image_press = data->image_press;
		pInfo->image_normal = data->image_normal;
		pInfo->state = data->state;
		pInfo->select.mode = data->select.mode;
		pInfo->select.state = data->select.state;
		pCtrl->dwAddData2 = (DWORD)pInfo;
		return 0;
	}
	case MSG_DESTROY:
		free(pInfo);
		break;

    case MSG_PAINT:
        hdc = BeginPaint (hwnd);
		paint(hwnd,hdc);
        EndPaint (hwnd, hdc);
        return 0;

    case MSG_MYBUTTON_GET_SELECT_STATE:
		return pInfo->select.state;

    case MSG_MYBUTTON_SET_SELECT_MODE:
		pInfo->select.mode = 1;
		InvalidateRect (hwnd, NULL, TRUE);
		return 0;

    // case MSG_LBUTTONDBLCLK:
        // if (dwStyle & SS_NOTIFY)
            // NotifyParent (hwnd, pCtrl->id, STN_DBLCLK);
        // break;

	case MSG_ENABLE:
		if (wParam && (dwStyle & WS_DISABLED)) {
			pCtrl->dwStyle &= ~WS_DISABLED;
			pInfo->state = BUT_NORMAL;
		}
		else if (!wParam && !(dwStyle & WS_DISABLED)) {
			pCtrl->dwStyle |= WS_DISABLED;
			pInfo->state = BUT_DISABLED;
		}
		else
			return 0;
		InvalidateRect (hwnd, NULL, TRUE);
		return 0;
	// case BM_GETCHECK:
		// if(dwStyle & BS_CHECKBOX) {
			// return pInfo->state==BUT_CLICK;
		// }
		// return 0;
	// case BM_SETCHECK:
		// if(dwStyle & BS_CHECKBOX) {
			// if(wParam)
				// pInfo->state = BUT_CLICK;
			// else
				// pInfo->state = BUT_NORMAL;
			// InvalidateRect (hwnd, NULL, FALSE);
		// }
		// return 0;
    case MSG_LBUTTONDOWN:
        if (GetCapture () == hwnd)
            break;

        if(pInfo->state == BUT_CLICK)
			break;			//已经是选中状态

        SetCapture (hwnd);
		pInfo->state = BUT_CLICK;
		InvalidateRect (hwnd, NULL, FALSE);
		if(dwStyle & SS_NOTIFY)
			NotifyParent (hwnd, pCtrl->id, BN_PUSHED);
        break;
	case MSG_LBUTTONUP:
	{
        int x, y;
		if (GetCapture() != hwnd) {
			if(!(dwStyle & BS_CHECKBOX)) {
				if(pInfo->state!=BUT_NORMAL) {
					pInfo->state = BUT_NORMAL;
					InvalidateRect (hwnd, NULL, FALSE);
				}
			}
            break;
		}

        ReleaseCapture ();
		if(!(dwStyle & BS_CHECKBOX)) {
			pInfo->state = BUT_NORMAL;
			if (pInfo->select.mode == 0)
				NotifyParent (hwnd, pCtrl->id, BN_CLICKED);
			else {
				if (pInfo->select.state == BUT_STATE_SELECT) 
					pInfo->select.state = BUT_STATE_UNSELECT;
				else
					pInfo->select.state = BUT_STATE_SELECT;
			}
			InvalidateRect (hwnd, NULL, FALSE);
		}
		// if (dwStyle & SS_NOTIFY) {
            // NotifyParent (hwnd, pCtrl->id, BN_UNPUSHED);
			// InvalidateRect (hwnd, NULL, TRUE);
		// }

		x = LOSWORD(lParam);
        y = HISWORD(lParam);
		//设置捕获后，坐标变成屏幕绝对值，需要转换为窗口坐标
		if(wParam & KS_CAPTURED) {
			ScreenToClient (GetParent (hwnd), &x, &y);
		}

		if (PtInRect ((PRECT) &(pCtrl->cl), x, y))
        {
			// playButtonSound();
			if (pInfo->select.mode == 0)
				NotifyParent (hwnd, pCtrl->id, BN_CLICKED);
			else {
				if (pInfo->select.state == BUT_STATE_SELECT) 
					pInfo->select.state = BUT_STATE_UNSELECT;
				else
					pInfo->select.state = BUT_STATE_SELECT;
			}
			InvalidateRect (hwnd, NULL, TRUE);
		} else if(dwStyle & BS_CHECKBOX) {
			pInfo->state = BUT_NORMAL;
			InvalidateRect (hwnd, NULL, FALSE);
		}
		break;
	}
	// case MSG_KEYDOWN:
		// switch(LOWORD(wParam))
		// {
		// case SCANCODE_CURSORUP:
			// break;
		// case SCANCODE_CURSORDOWN:
			// break;
		// default:
			// break;
		// }
		// break;
    // case MSG_FONTCHANGED:
        // InvalidateRect (hwnd, NULL, FALSE);
        // return 0;
    // case MSG_SETTEXT:
        // SetWindowCaption (hwnd, (char*)lParam);
        // InvalidateRect (hwnd, NULL, FALSE);
        // break;
	default:
		break;
    }

    return DefaultControlProc (hwnd, message, wParam, lParam);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief createSkinButton 创建单个皮肤按钮
 *
 * @param hWnd
 * @param id
 * @param x,y,w,h 坐标
 * @param image_normal 正常图片
 * @param image_press  按下图片
 * @param select_state 是否显示 0不显示 1显示
 * @param notif_proc   回调函数
 */
/* ---------------------------------------------------------------------------*/
void createSkinButton(HWND hWnd,int id ,
		int x, int y, int w, int h,
		BITMAP *image_normal,BITMAP *image_press,
		int select_state,
		NOTIFPROC notif_proc)
{
	HWND hCtrl;
	ButtonCtrlInfo pInfo;
	pInfo.image_normal = image_normal;
	pInfo.image_press = image_press;
	pInfo.select.mode = 0;
	pInfo.state = BUT_NORMAL;
	pInfo.select.state = select_state;
	if (bmp_button_select.bmBits == NULL) {
		if (LoadBitmap (HDC_SCREEN, &bmp_button_select, "res/image/func_select/button_select.JPG"))
			printf ("LoadBitmap(%s)fail.\n","button_select.JPG");
		if (LoadBitmap (HDC_SCREEN, &bmp_button_unselect, "res/image/func_select/button_unselect.JPG"))
			printf ("LoadBitmap(%s)fail.\n","button_unselect.JPG");

	}
	if (pInfo.select.state == BUT_STATE_SELECT)
		hCtrl = CreateWindowEx(CTRL_MYBUTTON,"",WS_CHILD,WS_EX_TRANSPARENT,
				id,x,y, w, h, hWnd,(DWORD)&pInfo);
	else
		hCtrl = CreateWindowEx(CTRL_MYBUTTON,"",WS_VISIBLE|WS_CHILD,WS_EX_TRANSPARENT,
				id,x,y, w, h, hWnd,(DWORD)&pInfo);
	if(notif_proc) {
		SetNotificationCallback (hCtrl, notif_proc);
	}
}


