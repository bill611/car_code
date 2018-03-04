/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  公共自定义画图函数
 *
 *        Version:  1.0
 *        Created:  2015-11-11 11:50:08 
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _MY_COMMON_H
#define _MY_COMMON_H

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>

    typedef struct _BmpLocation {
        BITMAP 	*bmp;
        char 	*location;
    }BmpLocation;

	// 控件结构体
	typedef struct _MgCtrlButton{
		HWND idc;		// 控件ID
		uint8_t op_code;  	// 设备操作
		char *img_name; 	// 常态图片名字,不带扩展名,完整路径由循环赋值
		int16_t x,y,w,h;
		NOTIFPROC notif_proc; 	// 回调函数
		uint8_t device_id;	// 设备编号
		BITMAP image_normal;	// 正常状态图片
		BITMAP image_press;	// 按下状态图片
	}MgCtrlButton;

	void SetAlarmForm(HWND hWnd);
	BOOL ClearAlarmForm(HWND hWnd);
	void DrawBackground(HWND hWnd, HDC hdc, const RECT* pClipRect,BITMAP *Image);
	void DrawWhiteFrame(HWND hWnd, HDC hdc, const RECT* pClipRect,int Left,int Top,int Width,int Height);
	void wndEraseBackground(HWND hWnd,HDC hdc, const RECT* pClipRect,BITMAP *pImage,int Left,int Top,int Width,int Height);
	void LoadAdverImage(HWND hWnd,int ID,BITMAP *Image,char *AdverType);
	void DrawXpFrame(HWND hWnd,HDC hdc, const RECT* pClipRect,const char *Caption,
			int left,int top,int right,int bottom);
	void GetPartFromBmp(const BITMAP *bmp,BITMAP *DstBitmap,int Left,int Top,int Width,int Height);
	void myFillBox(HDC hdc, RECT rc, int color);
	void TranslateIconPart(HDC hdc,int x,int y,int w,int h,BITMAP *FgBmp,int LineIconCnt,int IconIdx,int t,
			BOOL Translate);

    void bmpLoad(BITMAP *bmp,char *path);
    void bmpsLoad(BmpLocation *bmp,int num);
    void bmpRelease(BITMAP *bmp);
    void bmpsRelease(BmpLocation *bmp,int num);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
