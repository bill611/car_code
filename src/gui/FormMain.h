/*
 * =====================================================================================
 *
 *       Filename:  FormMain.h
 *
 *    Description:  主窗口
 *
 *        Version:  1.0
 *        Created:  2016-02-23 15:34:38
 *       Revision:  1.0
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _FORM_MAIN_H
#define _FORM_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	enum {
		IDC_TIMER_1S ,	// 1s定时器
		IDC_TIMER_NUM,
	};

	typedef struct _FormMainTimers {
		void (*proc)(void);
		int time;
	}FormMainTimers;
	
	typedef struct _FormMain {
		int status;
		FormMainTimers *timer;
		int (*loop)(void);

		int (*timerProc1s)(void);
		void (*timerStart)(int idc_timer);
		void (*timerStop)(int idc_timer);
		int (*timerGetState)(int idc_timer);

	} FormMain;


	FormMain * formMainCreate(
			int (*AppProc)(HWND,int,WPARAM,LPARAM),
			FormMainTimers *timers_table);
    extern BITMAP bmp_bkg1; // 背景1
    extern BITMAP bmp_bkg2; // 背景2

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
