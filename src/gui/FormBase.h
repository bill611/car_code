/*
 * =====================================================================================
 *
 *       Filename:  FormBase.h
 *
 *    Description:  设置类基本窗口框架
 *
 *        Version:  1.0
 *        Created:  2016-02-19 15:24:17 
 *       Revision:  1.0
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _FORM_BASE_H
#define _FORM_BASE_H

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "MyDialog.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	enum {
		STOP = 0,
		CONTINUE = 1,
	};

	typedef struct _FormBasePriv {
		HWND hwnd;
		char *name;
		BITMAP *bmp_bkg;
		MY_DLGTEMPLATE *dlgInitParam;
		int (*dlgProc)(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);

		void (*initPara)(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
	}FormBasePriv;

	typedef struct _FormBase {
		FormBasePriv *priv;
		int auto_close_time;

		int (*baseProc)(struct _FormBase *this,HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
	}FormBase;

	FormBase * formBaseCreate(FormBasePriv *priv);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
