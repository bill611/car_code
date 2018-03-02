#ifndef PublicH
#define PublicH

#include "communication.h"
#include "UDPServer.h"
#define MAX_CENTERNUM	4	// 协议规定的管理中心数量
#define MAX_MKJNUM		9	// 协议规定的门口机数量8, 多留一个给户门口机

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
    IDC_NUM,
};

typedef struct _ConfigFile {
   int device_main_controls[IDC_NUM];
}ConfigFile;

typedef struct _PUBLIC_DATA
{
	//程序版本信息
	TVersion SoftVer;				//软件版本号
    char build_date[32];

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
extern unsigned int PacketID;
extern ConfigFile g_config;

#define MSG_REFRESHSTATUS (MSG_USER+100)

#endif
