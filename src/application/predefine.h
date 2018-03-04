#ifndef PredefineH
#define PredefineH

#define DEVICE_MODE "TC3000D-X1CH"

//-----------------------------------------------------------------------
#define SCR_WIDTH 480           /*LCD Width     */
#define SCR_HEIGHT 800          /*LCD Height    */
#define LOGFONTSIZE 20

#define MAX_VOLUME_EX   96 //最大音量百分比

#define INI_KEY_START   55 //配置文件标志
#define INI_KEY_END     99 //

// 最大显示数值个数
#define MAX_DIAL_NUM	14

#define  MAX_HOMELIST_NUM     	13

#define FLASH_DIR	"/mnt/nand1-2"

#define FIRST_UPDATE_TIME     60 // 首次获取管理中心时间间隔 60s
#define MY_BLACK    0x01010101
#define MY_BLACK_TOP    0x11111111
#define MY_WHIGHT    0XFFFFFF
#define MY_RED    0X0000FF
#define MIN_LCD_LIGHT 10
#define LCD_OFF     FALSE
#define LCD_ON      TRUE

#define FORM_SETTING_ONTIME      60 // 设置界面60秒无操作，则关闭窗口
#define DISP_STANDY_ONTIME      10 // 清空提示栏的时间 10s
#define KEY_PRESS_TIME      10 // 按键超时时间

#define MSG_KEYBOARD		 	(MSG_USER + 500)
#define MSG_SERIALPORT		 	(MSG_USER + 501)


/* ----------------------------------------------------------------*/
/**
 * @brief 格式化创建static控件
 */
/* ----------------------------------------------------------------*/
#define STATIC_LB(x,y,w,h,id,caption,dwAddData,font)    \
{CTRL_STATIC,WS_CHILD|WS_VISIBLE|SS_CENTER,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

#define STATIC_IMAGE(x,y,w,h,id,dwAddData)    \
{CTRL_STATIC,WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTER|SS_NOTIFY,\
	x,y,w,h,id,"",dwAddData,WS_EX_TRANSPARENT,NULL,NULL,NULL}

#define STATIC_LB_DISVISIBLE(x,y,w,h,id,caption,dwAddData,font)    \
{CTRL_STATIC,WS_CHILD|SS_CENTER,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

#define STATIC_LB_L_DISVISIBLE(x,y,w,h,id,caption,dwAddData,font)    \
{CTRL_STATIC,WS_CHILD|SS_LEFT,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

#define STATIC_LB_L(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_STATIC,WS_CHILD|WS_VISIBLE|SS_LEFT,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

#define STATIC_LB_L_NOTIFY(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_STATIC,WS_CHILD|WS_VISIBLE|SS_LEFT|SS_NOTIFY,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

/* ----------------------------------------------------------------*/
/**
 * @brief 格式化创建CTRL_BUTTON控件
 * GROUP 为一组控件的开头
 */
/* ----------------------------------------------------------------*/
#define GROUP_BUTTON_LB(x,y,w,h,id,caption,dwAddData,font)    \
{CTRL_BUTTON,WS_CHILD|WS_VISIBLE|WS_GROUP|BS_AUTORADIOBUTTON ,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

#define BUTTON_LB(x,y,w,h,id,caption,dwAddData,font)    \
{CTRL_BUTTON,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON ,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

#define BUTTON_CHECKVOX(x,y,w,h,id,caption,dwAddData,font)    \
{CTRL_BUTTON,WS_CHILD|WS_VISIBLE |BS_AUTOCHECKBOX | WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

#define GROUP_BUTTON_CHECKVOX(x,y,w,h,id,caption,dwAddData,font)    \
{CTRL_BUTTON,WS_CHILD|WS_VISIBLE |BS_AUTOCHECKBOX | WS_TABSTOP| WS_GROUP,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

/* ----------------------------------------------------------------*/
/**
 * @brief 格式化创建CTRL_SLEDIT控件
 * GROUP 为一组控件的开头
 */
/* ----------------------------------------------------------------*/
#define EDIT(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_CENTER| WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define EDIT_L(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT| WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define EDIT_L_DISVISIBLE(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|WS_BORDER|ES_LEFT| WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define GROUP_EDIT_L(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT| WS_TABSTOP | WS_GROUP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define GROUP_EDIT(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_CENTER| WS_TABSTOP | WS_GROUP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define EDIT_PSD(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|ES_PASSWORD|WS_VISIBLE|ES_CENTER|WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define EDIT_PSD_L(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|ES_PASSWORD|WS_VISIBLE|WS_BORDER|ES_LEFT|WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define EDIT_PSD_L_DISVISIBLE(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|ES_PASSWORD|WS_BORDER|ES_LEFT|WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define GROUP_EDIT_PSD_L(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SLEDIT,WS_CHILD|ES_PASSWORD|WS_VISIBLE|WS_BORDER|ES_LEFT|WS_TABSTOP|WS_GROUP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

/* ----------------------------------------------------------------*/
/**
 * @brief 格式化创建CTRL_SCROLLVIEW控件
 * GROUP 为一组控件的开头
 */
/* ----------------------------------------------------------------*/
#define SCROLLVIEW(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_SCROLLVIEW,WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL|SVS_AUTOSORT,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

/* ----------------------------------------------------------------*/
/**
 * @brief 格式化创建CTRL_LISTBOX控件
 * GROUP 为一组控件的开头
 */
/* ----------------------------------------------------------------*/
#define LISTBOX(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_LISTBOX,WS_VISIBLE|WS_VSCROLL|WS_BORDER,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

/* ----------------------------------------------------------------*/
/**
 * @brief 格式化创建CTRL_COMBOBOX控件
 * GROUP 为一组控件的开头
 */
/* ----------------------------------------------------------------*/
#define GROUP_COMBOBOX(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_COMBOBOX,WS_CHILD|WS_VISIBLE|WS_GROUP|CBS_DROPDOWNLIST|CBS_READONLY|WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}

#define COMBOBOX(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_COMBOBOX,WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_READONLY|ES_CENTER|WS_TABSTOP,\
	x,y,w,h,id,caption,dwAddData,WS_EX_TRANSPARENT,NULL,NULL,font}

/* ----------------------------------------------------------------*/
/**
 * @brief 格式化创建CTRL_LISTVIEW控件
 * GROUP 为一组控件的开头
 */
/* ----------------------------------------------------------------*/
#define LISTVIEW(x,y,w,h,id,caption,dwAddData,font)  \
{CTRL_LISTVIEW,WS_CHILD|WS_VISIBLE|ES_CENTER|WS_VSCROLL|WS_HSCROLL,\
	x,y,w,h,id,caption,dwAddData,0,NULL,NULL,font}


/* ----------------------------------------------------------------*/
/**
 * @brief _INTERVAL  平均排列间隔
 *
 * @param Total_size 总区域宽度
 * @param unit_size 单位区域宽度
 * @param Num 单位个数
 *
 */
/* ----------------------------------------------------------------*/
#define _INTERVAL(Total_size,unit_size,Num)  \
	((Total_size - (Num * unit_size)) / (Num + 1) )

/* ----------------------------------------------------------------*/
/**
 * @brief _LOCATION 平均分布坐标
 *
 * @param Total_size 总区域宽度
 * @param unit_size 单位区域宽度
 * @param Start 区域起始位置
 * @param AIndex 图标位置
 * @param Num 单位个数
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
#define _LOCATION( Total_size,unit_size,Start, AIndex, Num ) \
	(Start + (_INTERVAL(Total_size,unit_size,Num) * (AIndex + 1)) + (unit_size * (AIndex)))

#define NELEMENTS(array)        /* number of elements in an array */ \
	(sizeof (array) / sizeof ((array) [0]))

#define TIME_1S (10 * 5)
#define TIME_100MS (TIME_1S / 10)

#define DBG_RC(x) printf("l:%d,t:%d,r:%d,b:%d\n",x.left,x.top,x.right,x.bottom)
#define DBG_STR(x)  printf("flag------->[%s]\n",x)
#define DBG_F(x) printf("flag[%s]------->[%d]\n",__FUNCTION__,x)

//#define ENTERFUNCTION printf("[%s]\n",__FUNCTION__);
//#define EXITFUNCTION printf("[%s]EXIT\n",__FUNCTION__);
#define ENTERFUNCTION
#define EXITFUNCTION
#define PRINTF( x... )  printf( x )

//-----------------------------------------------------------------------
#endif

