/*
 * =====================================================================================
 *
 *       Filename:  Mylistbox.h
 *
 *    Description:  菜单控件函数头文件
 *
 *        Version:  2.0
 *        Created:  2015-10-30 09:11:22 
 *       Revision:  1.0 创建自定义菜单控件
 *       			2.0 增加类型选择，增加呼叫列表类型
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */

#ifndef _MY_CTRL_LISTBOX_H
#define _MY_CTRL_LISTBOX_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define CTRL_MYLISTBOX        ("mylistbox")

#define LIGHT 1
#define INLIGHT 0

#define MSG_UP			MSG_USER+1
#define MSG_DOWN		MSG_USER+2
#define MSG_SELECT		MSG_USER+3
#define MSG_ADD_ITEM	MSG_USER+4
#define MSG_DELETE_ALL	MSG_USER+5
#define MSG_RERESH		MSG_USER+6

#define	MAX_CALL_LIST_NUM	4

typedef enum {
	TYPE_MENU = 0,		// 菜单
	TYPE_CALL_LIST,		// 呼叫列表
}ENUM_LISTBOX_TYPE;

typedef struct _MYMENUITEM
{
	NOTIFPROC notif_proc;				/* callback */
    char*   name;                // item string
} MYMENUITEM;

// 菜单控件所需图片
typedef struct _MY_MENU_BMPS {
    BITMAP *title_background;  	// 标题背景
    BITMAP *title_ball_light;	// 标题指示页码小球非选择状态
    BITMAP *title_ball_deep;	// 标题指示页码小球选择状态
    BITMAP *menu_deep;			// 内容选择状态
    BITMAP *menu_light;			// 内容非选择状态
    BITMAP *menu_bottom_deep;	// 内容尾行选择状态  因为尾行有圆弧状，所以用不同的图片
    BITMAP *menu_bottom_light;	// 内容尾行非选择状态
    BITMAP *menu_select;		// 内容每行末尾的绿色勾
}MY_MENU_BMPS;

// 菜单控件所需字体
typedef struct _MY_MENU_FONTS {
	PLOGFONT 	*title; 	// 菜单标题字体
	PLOGFONT 	*text;  	// 菜单内容字体
}MY_MENU_FONTS;

// 呼叫列表控件所需图片
typedef struct _MY_CALL_LIST_BMPS {
    BITMAP *menu_deep;			// 内容选择状态
    BITMAP *menu_light;			// 内容非选择状态
}MY_CALL_LIST_BMPS;

// 呼叫列表控件所需字体
typedef struct _MY_CALL_LIST_FONTS {
	PLOGFONT 	*text;  	// 呼叫列表内容字体
}MY_CALL_LIST_FONTS;

typedef struct _MY_MENUTEMPLATE
{
	/** Dialog box style */
    DWORD       dwStyle;
    /** Dialog box extended style */
    DWORD       dwExStyle;
    /** Dialog box position */
    int         x, y, w, h;
    /** Dialog box caption */
    const char* caption;
	int id;

    int         controlnr;   	// 菜单总数量
    MYMENUITEM   *menulist; 	// 菜单列表
	MY_MENU_FONTS *fonts; 	// 菜单用到的字体资源
	MY_MENU_BMPS *bmps; 	// 菜单用到的图片资源
} MY_MENUTEMPLATE;

typedef struct _MY_CALL_LIST_TEMPLATE
{
	/** Dialog box style */
    DWORD       dwStyle;
    /** Dialog box extended style */
    DWORD       dwExStyle;
    /** Dialog box position */
    int         x, y, w, h;
    /** Dialog box caption */
	int id;

	MY_CALL_LIST_FONTS *fonts; 	// 呼叫列表用到的字体资源
	MY_CALL_LIST_BMPS *bmps; 	// 呼叫列表用到的图片资源
} MY_CALL_LIST_TEMPLATE;

typedef struct _MyListBoxWidget
{
    BITMAP *bmp_light;
	BITMAP *bmp_inlight;
	int x,y,w,h;
	int interval;
	int index;
	int index_old;
} MyListBoxWidget;

typedef struct _MyMenuTitle
{
    BITMAP *bmp;
	const char *text;
	PLOGFONT font;

	MyListBoxWidget ball;
} MyMenuTitle;


typedef struct _MyMenuItem
{
    BITMAP *bmp_light;
	BITMAP *bmp_inlight;

	BITMAP *bmp_b_light;
	BITMAP *bmp_b_inlight;

	PLOGFONT font;

	MyListBoxWidget Hook;
} MyMenuItem;

typedef struct _MyCallListItem {
	PLOGFONT font;
    BITMAP *bmp;
	char list[MAX_CALL_LIST_NUM][64];
}MyCallListItem;

typedef struct MYtagLISTBOXDATA
{
	ENUM_LISTBOX_TYPE type; // 窗口类型 
    int itemCount; 			// 每页最多显示行数 
	int itemWidth;			// 单个项目宽度
	int itemHeight;			// 单个项目高度
	int listcnt;		// 总项目数
	int page_max;			// 总页数=listcnt/itemCount
    int selItem;			// 当前选择项目

//  菜单类
    MYMENUITEM   *menulist; // 菜单类结构
	MyMenuTitle title;		// 菜单标题结构
	MyMenuItem item;		// 菜单项目结构

//  呼叫列表类
	MyCallListItem call_list_item;
} MYLISTBOXDATA;

#define _LIST_BALL_INTERVAL_X(AreaWidth,BallWitth,Num) \
	((AreaWidth - (Num * BallWitth)) / (Num + 1) )

#define _LIST_BALL_X( Start,AIndex, AreaWidth, Num,BallWitth) \
	(Start + (_BALL_INTERVAL_X(AreaWidth,BallWitth,Num) * (AIndex + 1)) + (BallWitth * (AIndex)))

HWND CreateMyListBox(void *Data,HWND hOwner,ENUM_LISTBOX_TYPE type,int MaxItemNum);
BOOL RegisteMyListboxControl (void);
void MyListboxControlCleanup (void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CTRL_LISTBOX_H */

