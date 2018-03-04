
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
    BUT_NORMAL, // ����
    BUT_CLICK,  // ����
    BUT_DISABLED, // ������
};
enum {
    BUT_STATE_SELECT, // ѡ��״̬
	BUT_STATE_UNSELECT, // ��ѡ��״̬
};

struct ButtonSelect {
	int mode; // 0����ģʽ 1����ѡģʽ
	int state;
};

typedef struct {
	BITMAP *image_normal;	// ����״̬ͼƬ
	BITMAP *image_press;	// ����״̬ͼƬ
	BITMAP *image_select;	// ѡ��ͼƬ
	BITMAP *image_unselect;	// ��ѡ��ͼƬ
	struct ButtonSelect select; //����ģʽʱ�Ƿ�ѡ
	int state;		//BUTTON״̬
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
