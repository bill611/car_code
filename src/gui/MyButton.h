
#ifndef _MGUI_CTRL_MYBUTTON_H
#define _MGUI_CTRL_MYBUTTON_H
 
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define CTRL_MYBUTTON         ("mybutton")

enum {
    BUT_NORMAL, // 正常
    BUT_CLICK,  // 按下
    BUT_DISABLED, // 不启用
};
enum {
    BUT_STATE_SELECT, // 选择状态
	BUT_STATE_UNSELECT, // 非选择状态
};

struct ButtonSelect {
	int mode; // 0正常模式 1带勾选模式
	int state;
};

typedef struct {
	BITMAP *image_normal;	// 正常状态图片
	BITMAP *image_press;	// 按下状态图片
	BITMAP *image_select;	// 选中图片
	BITMAP *image_unselect;	// 非选中图片
	struct ButtonSelect select; //设置模式时是否勾选
	int state;		//BUTTON状态
}ButtonCtrlInfo;

#define MSG_MYBUTTON_GET_SELECT_STATE MSG_USER+1
#define MSG_MYBUTTON_SET_SELECT_MODE MSG_USER+2

BOOL RegisterMyButtonControl (void);
void MyButtonControlCleanup (void);

HWND createSkinButton(HWND hWnd,int id ,
		int x, int y, int w, int h,
		BITMAP *image_normal,BITMAP *image_press,
		int display,
		int mode,
		NOTIFPROC notif_proc);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CTRL_ICONVIEW_H */
