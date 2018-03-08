#ifndef PublicH
#define PublicH

#include <stdint.h>
#include "communication.h"
#include "UDPServer.h"

#define CODE_VERSION	"V1.0.0.0"

typedef struct _FORMCLASS
{
	HWND hWnd;
	char Class[16];
	struct _FORMCLASS * next;
}FormClass,*PFormClass;

typedef struct _SCREEN_FORM
{
    HWND hMainWnd;												//主窗口 HWND hUpdate;                                               //更新窗口
	HWND hUpdate;
	FILE *DrangLogfp;
	int Width;
	int Height;
	int Count;													//合计数
	PFormClass head,tail;										//窗口链表头与尾
	BOOL (*Add)(HWND hWnd,const char *Class);					//添加窗口
	BOOL (*Del)(HWND hWnd);										//删除窗口
	HWND (*Find)(const char *Class);							//查找窗口
	void (*ReturnMain)(void);										//返回主窗口
	void (*foreachForm)(int iMsg, WPARAM wParam, LPARAM lParam); //遍历所有窗口发送消息
} SCREEN_FORM,*PSCREEN_FORM;
extern SCREEN_FORM Screen;

typedef struct
{
	char Name[32];
	char IP[16];
	int Type;				//类型,0门口机 1户门口机
} TServerMsg;

typedef struct _DisplayMsg
{
	int index;
	char buf[36];
} DisplayMsg;

enum {
	VEDIO_320x240,
	VEDIO_640x480,
	VEDIO_1280x720
};

typedef struct _EtcValueChar {
	const char* section;	
	const char* key;
	char *value;
	unsigned int leng;
	char *default_char;
} EtcValueChar;

typedef struct _EtcValueInt {
	const char* section;	
	const char* key;
	int *value;
	int default_int;
} EtcValueInt;

enum {
	IDC_CHAIR_ELE, //电动座椅
	IDC_CHAIR_SECRETARY, //秘书椅
	IDC_CHAIR_ROT, //旋转座椅
	IDC_CURTAIN, //窗帘
	IDC_SCREEN_GLASS, //玻璃屏
	IDC_SCREEN_TV, //电视屏
	IDC_CD, //CD
	IDC_DVD, //DVD
	IDC_MONITOR, //监控
	IDC_DOOR, //车门
	IDC_BED_ELE, //电动床
	IDC_SATV, //卫星电视
	IDC_SKYLIGHT, //天窗
	IDC_LIGHT, //灯光
	IDC_PROJECTION, //屏幕投影
	IDC_TABLE, //桌板
	IDC_A1, //A1
	IDC_A2, //A2
	IDC_WALN, //WALN
	IDC_SYSTEM, //系统
    IDC_MAIN_NUM,
};

typedef struct _ConfigFile {
   int device_main_controls[IDC_MAIN_NUM];
   char password[32];
}ConfigFile;

typedef struct _PublicData
{
	/*
	   bit7座椅加热状态：0关闭，1开启
	   bit6座椅制冷状态：0关闭，1开启
	   bit5、4位按摩状态：00关闭，01腿部，10背部，11全身
	   */
	uint8_t leftSeat;
	uint8_t rightSeat;

	uint8_t mute; // 静音状态
	/*
bit7:1为ON,0为OFF;
bit6-4: 1为亮1格,2为亮2格,3为亮3格，4为亮4格,5为亮5格；
bit3:1为单色显示，0为循环显示；
bit2-0: 1为闪烁2秒，2为闪烁5秒，3为闪烁10秒，4为15秒，5为20秒，6为30秒。
*/
	uint8_t light1; // 0~20
	uint8_t light2; // 0~20
	uint8_t light3; // 0~20

	/*
	   高4位：8为电源关闭，9为电源打开  
	   低4位：0电视上升中，1到顶，2下降中，3到底，5停止
	   */
	uint8_t tvPower;

	uint8_t monitor; // 0为非监控状态，1为监控状态
	uint8_t ucMedia; // 0:cd, 1:dvd, 2:cmmb, 3:satelliteTV
	uint8_t glass_power; // 玻璃屏幕电源 0关 1开


    int LCDLight;
	int ScrSaversTimer;		// 屏幕保护计数
	char MAC[20];			//本机MAC地址.
	char cLocalIP[16];		// 本机IP地址
	char cNetMask[16];		// 子网掩码
	char cGateWay[16];		// 默认网关

	int WatchDog_fd;

	void (*initTouchIrq)(void);
	void (*syncTimeHandle60s)(void); // 未同步时间时每分钟同步一次

    int (*saveConfig)(void); // 保存配置文件
    void (*getConfigInt)(char *section,char *key,int *value); // 读取配置文件
    void (*getConfigChar)(char *section,char *key,char *value); // 读取配置文件
 
} PUBLIC_DATA;


void publicInit(void);
void publicDestroy(void);

extern BOOL __mg_ime_minimize;
extern  PUBLIC_DATA Public;
extern ConfigFile g_config;

#endif
