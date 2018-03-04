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

#define MSG_MAIN_TIMER_START	    (MSG_USER + 1)
#define MSG_MAIN_TIMER_STOP	        (MSG_USER + 2)
#define MSG_MAIN_SHOW_PRESET	    (MSG_USER + 3)
#define MSG_MAIN_SHOW_NORMAL	    (MSG_USER + 4)

#define FORM_MAIN_KEY_DELETE 	KEYBOARD_FUNC1
#define FORM_MAIN_KEY_CALL  	KEYBOARD_FUNC2
#define FORM_MAIN_KEY_SWITCH 	KEYBOARD_FUNC3

	typedef enum _FormType {
		FORM_UPDATE,
		FORM_UNLOCK,
		FORM_SETTING,
	} FormType;

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
		unsigned int (*getTextNumber)(char *text_num);



		void (*createForm)(FormType type,void *para);
	} FormMain;


	FormMain * formMainCreate(
			int (*AppProc)(HWND,int,WPARAM,LPARAM),
			void (*keyboardHandle)(unsigned int,int),
			char * (*listboxNotify)(HWND , int , int , DWORD ),
			FormMainTimers *timers_table);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
