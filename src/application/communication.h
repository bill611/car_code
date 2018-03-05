#ifndef ServicesDefH
#define ServicesDefH

#include <stddef.h>

#ifndef UINT32
#define UINT32 unsigned int
#endif
//定义UDP接收数据包超时值
#define UDPTIMEOUT 1000
//定义UDP发送、接收数据时最大的包长,必须整除4
#define UDPMAXLENGTH 1000
//定义UDP如果没有收到应答包一共发送的次数
#define TRYSENDTIMES 3

//UDP每包最长的长度

#define UDPSERVER_PORT 7800		//远端UDP服务端口号
#define UDPLOCAL_PORT  7800		//本机UDP服务端口号
#define	SQLSERVER_PORT 7890		//数据服务中心SQL_SERVER服务端口号(TCP)
#define RTPLISTEN_PORT 7891		//保留
#define TCPSERVER_PORT 7892		//数据服务中心TCP文件服务端口号
#define SQL_UDP_PORT   7896		//数据服务中心连接测试端口号
#define WAPTCP_PORT    7898		//WAP服务器传输图像端口
#define TC_DNS_PORT    9888		//太川DNS服务器端口号
#define TC_WEB_PORT	   7895		//太川网站后台服务器端口号

#define PROTOCOL_MAJOR   1
#define PROTOCOL_MINOR	 0
#define PROTOCOL_RELEASE 1
#define VERSION(a,b,c) (((a)<<16) | ((b)<<8) | (c))
/*---------------------------------------------------------------------------*/
enum {
	TP_CALL,			//呼叫对讲
	TP_MAIL,			//邮件通知
	TP_SIP,				//保留
	TP_WAP,				//WAP通讯，保留
	TP_ALARM,			//安防报警
	TP_GPRS,			//GRPS发送短信
	TP_PING,			//连接测试，保留
	TP_IPDOWN,			//IP地址下载，门口机及户门口机用
	TP_PICTURE,			//图片下载，门口机用
	TP_AFFICHE,			//门口机保留用
	TP_GUARD,			//门口机保留用
	TP_GUARDCARD,		//门口机巡更卡信息
	TP_USERCARD,		//门口机用户刷卡发送信息
	TP_DOWNMUSIC,		//管理中心下载音乐通知门口机
	TP_IMAGE,			//管理中心下载图片通知门口机
	TP_USERREFRESH,		//保留
	TP_GETUSERIP,		//获取用户IP地址，与TC-DNS通讯
	TP_VERIFYPWD,		//MD5密码校验
	TP_MONITOR,			//远程监视中控主机命令
	TP_MONITOROVER,		//远程监视中控主机结束命令
	TP_ELECTRIC,		//远程电器控制命令
	TP_BURROW,			//远程呼叫对讲前路由器穿透命令
	TP_CALLUSER,		//保留
	TP_SETSYSTIME,		//管理中心发送同步时间命令  0x17(23d)
	TP_IDCARDDOWN,		//ID卡信息下载命令
	TP_READIP,			//读户门口机IP地址命令
	TP_USERIDRECORD,	//门口机ID卡记录命令
	TP_PUBLICMAIL,		//门口机发布公共信息通知命令
	TP_MEDIA,                //多美体控制
	TP_DOWNANSWER,               //下载信息应答
	TP_COMPARELIST,               //下载门口机房号对照表  0x1e(30d)
	TP_MP3TIMECTRL,                  //设置MP3的控制时间，自动关闭打开时间
	TP_SETSYSTEMAUDIO,  //设置音量  0x20(32d)
	TP_LOGMSG = 0x101,  //动作报警日志 20130709
	TP_TESTROUTE=0x200,	//路由环回测试命令，类似于PING命令的作用
	TP_NATBURROW,		//NAT路由器穿透命令
	TP_RTPBURROW,		//RTP音视频穿透命令
	TP_DEVCHECK,		//设备测试
	TP_UPDATEPROC,		//程序更新通知
	TP_UPDATEEND,		//返回数据服务中心更新完成的消息
	TP_GETAPPVERSION,   //取程序版本号
	TP_ELEVATOR,        //电梯联动
	TP_UPDATEMSG,		//服务器IP更新信息,房号配置表更换信息
	TP_GETREMOTEPWD,	//获取设置服务器远程密码
	TP_SETREMOTEPWD,	//获取设置服务器远程密码
	TP_SENDMACTOSRV,	//发送MAC地址，IP地址及房号信息到服务器
	TP_CHANGEROOMID,	//更改房号
	TP_TRANCMDBYSRV,	//通过服务器转发包
	TP_TRANSRTP,		//发送RTP包
	TP_TRANSELECTRIC,	//转发送电器控制包
	TP_SENDSOFTVER,					//发送本设备版本信息
	TP_RETUPDATEMSG,				//返回升级信息  0x211
	TP_BUTTONALARM,		//紧急按钮报警
	TP_LOCALDEVID,		//获取本机唯一编号
	TP_ADVERTISEMENT,    //中控机接收远程广告发布， 为了向U9中控机兼容，才更改值
	TP_SYS_CONFIG=0x500,	//
	TP_ELEVATOR_3000=0x550,	//

	TP_VIDEO_CONFIG=0x600, 		//视频编码头信息
	TP_UNLOCK_INFO = 0x601, // 开锁信息
	TP_GETDEVADDR=0x8000, 			//读设备地址信息
	TP_RESPDEVADDR,  				//返回设备地址信息
	TP_GETCONFIGADDR,				//读取配置地址
	TP_RESCONFIGADDR,				//返回配置地址
	TP_WORKREG,						//设备每一分钟在管理中心登记一次工作状态
	TP_ONLINE,          			//管理理中心或门口机报告本设备在线
	TP_DEVCTRLCOM,					//控制设备 ，例如重启或关机 xiehb 20120613
	TP_WORKREG2,					//设备每一分钟在管理中心登记一次工作状态(新版本)

	TP_RTPDATA=0x8050,              //新的门口机随监数据包            
	TP_CALLUSER2,                   //门口机请求发送随监视频          
	TP_RETCALLUSER2,                //管理中心回复或控制门口机随监命令
	TP_CALLACKREC=0x806A,           //门口机返回呼叫状态到管理中心
	TP_NONE				//空命令，保留
};

//网络转发器命令定义
enum {
	CMD_SYS_SET_REQ,CMD_SYS_SET_REQ_REP,
	CMD_SYS_SET_DONE,CMD_SYS_SET_DONE_REP,
	CMD_SYS_SET_CANCEL,CMD_SYS_SET_CANCEL_REP,
	CMD_PWD_SET,CMD_PWD_SET_REP,
	CMD_IP_SET,CMD_IP_SET_REP,
	CMD_IP_GET,CMD_IP_GET_REP,
	CMD_DDNS_SET,CMD_DDNS_SET_REP,
	CMD_DDNS_GET,CMD_DDNS_GET_REP,
	CMD_DEV_SET,CMD_DEV_SET_REP,
	CMD_DEV_GET,CMD_DEV_GET_REP,
	CMD_IMEI_SET,CMD_IMEI_SET_REP,
	CMD_IMEI_GET,CMD_IMEI_GET_REP,
	CMD_ROOM_WRITE,CMD_ROOM_WRITE_REP,
	CMD_DEV_WRITE,CMD_DEV_WRITE_REP,
	CMD_RECORD_GET,CMD_RECORD_GET_REP,
	CMD_RF_FREQ_CHG, CMD_RF_FREQ_CHG_REP,
	CMD_RF_FREQ_GET, CMD_RF_FREQ_GET_REP,
	CMD_RF_DATA_TRAN, CMD_RF_DATA_TRAN_REP
};
/*---------------------------------------------------------------------------*/
/* 包头 */
typedef struct
{
    unsigned int ID;
    unsigned int Size;
    unsigned int Type;
}COMMUNICATION;
/*---------------------------------------------------------------------------*/
/* 应答包 */
typedef struct
{
    unsigned int ID;
}ASKPACKET;
/*---------------------------------------------------------------------------*/
/* 呼叫命令 */

enum {CMD_CALL,		/* CMD_CALL 呼叫 */
	CMD_ANSWER,		/* CMD_ANSWER 保留，测试时不用 */
	CMD_TALK,		/* CMD_TALK 摘机，被呼叫方发送该消息 */
	CMD_OVER,		/* CMD_OVER 挂机 */
	CMD_UNLOCK,		/* CMD_UNLOCK 开锁 */
	CMD_LEAVEWORD,	/* 留言 */
	CMD_FORTIFY,	//
	CMD_REPEAL,		//
	CMD_PICTURE,	//
	CMD_TEXT,		//
	CMD_CLOSEAD,	//
	CMD_MUSICPLAY,	//
	CMD_MUSICSTOP,	//
	CMD_CLOSEMEDIA,
	CMD_SHAKEHANDS=0x80,		// 呼叫前握手命令
	ASW_OK=0x100,	//呼叫回复成功
	ASW_FAIL,		//呼叫回复失败
	MNT_OK,			//监视成功
	MNT_BUSY,		//监视忙
	MNT_VERFAIL,	//监视检验MD5失败
	MNT_REFUSE,		//
	CMD_SENDPORT,	//
	CMD_TRANSCALL,	//转发呼叫命令
	CMD_TRANSOVER,	//转发呼叫结束命令
	CMD_SHAKEHANDS_ASW=0x300,	// 呼叫前握手包应答命令
};

/* call type */

enum {TYPE_CENTER,			//管理中心
	TYPE_DOOR,				//门口机
	TYPE_XC,				//保留
	TYPE_USER,				//中控主机
	TYPE_REMOTE,			//基于PC上的呼叫程序，如网站的对讲程序
	TYPE_XCDOOR,			//保留
	TYPE_FDOOR,				//户门口机
	TYPE_ZNBOX,				//智能箱
	TYPE_NETRF,				//网络RF模块
	TYPE_DEVID_SRV=0x100,	//取设备ID服务器
	TYPE_ALL=0xFFFFFFFF		//所有设备
};
typedef struct _COMMUNICATION_CALL
{
	unsigned int Cmd;					//Command
	char CallID[16];					//呼叫方编号
	char CallName[32];					//呼叫方名称
	unsigned int CallType;				//Type
	unsigned int VideoType;			//视频类型，0 H264, 1 Mpeg4
	unsigned short VideoWidth;			//视频类型  0 320X240 1 352X288 2 640X480 3 720X576
	unsigned short VideoHeight;			//视频类型  0 320X240 1 352X288 2 640X480 3 720X576
} COMMUNICATION_CALL;

typedef struct _OLD_COMMUNICATION_CALL
{
	unsigned int Cmd;					//Command
	char CallID[16];					//呼叫方编号
	char CallName[32];					//呼叫方名称
	unsigned int CallType;				//Type
}OLD_COMMUNICATION_CALL;

typedef struct _COMMUNICATION_TRANSCALL
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int Cmd;					//Command
	char CallID[16];					//呼叫方编号
	char CallName[32];					//呼叫方名称
	unsigned int CallType;				//Type
	char CallIP[16];					//呼叫方IP地址
	unsigned int CallPort;				//呼叫方端口号
}COMMUNICATION_TRANSCALL;
/*---------------------------------------------------------------------------*/
/* 呼叫应答 */
typedef struct
{
	unsigned int Answer;
}ANSWER_CALL;
/*---------------------------------------------------------------------------*/
enum {WAP_GETIMAGE,WAP_GETSCENE,WAP_MACCONTROL,WAP_UNLOCK};
typedef struct
{
	unsigned int Cmd;					//Command
	unsigned int Type;					//Type
}COMMUNICATION_WAP;
//---------------------------------------------------------------------------
// 远程电器控制
//---------------------------------------------------------------------------
enum {ELEC_GETDEVICE,		//读设备信息
	ELEC_RETDEVICE,			//返回设备信息
	ELEC_GETSTATUS,			//读设备状态
	ELEC_RETSTATUS,			//返回设备状态
	ELEC_CTRLDEVICE,		//控制设备
	ELEC_RETROOM,			//返回房间信息
	ELEC_GUARDMSG,			//警戒信息
	ELEC_GUARDSTATE,		//情景(布撤防)状态
};

//获取电器设备信息协议包结构
typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int NetID;
	unsigned char MD5Pwd[36];
	int Cmd;
	unsigned int GetType;			//获取信息内容,0发送房间结构及设备结构,1发送房间结构,2发送设备结构
	unsigned int PackIdx;			//0所有包上传,N 重传第几个包
	unsigned int Operate;			//忽略该字段
} TGetDeviceMsg;

//远程电器控制命令协议包
typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int NetID;
	unsigned char MD5Pwd[36];
	int Cmd;
	unsigned int DevAddr;
	int DevUnit;
	unsigned int Operate;
} TRemoteCtrlDevice;

//远程监视命令协议包
typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned char MD5Pwd[36];
	int Way;
} TRemoteMonitor;

//中控主机返回房间数据协议包(旧版)
typedef struct
{
	int RoomID;			//房间号
	char RoomName[16];	//房间名
} TRemoRoomStruct;

typedef struct {
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int UserID;		//网络ID编号
	int Cmd;					//命令
	unsigned short PacketCnt;	//分包总数
	unsigned short PacketIdx;	//当前分包索引
	int RoomCnt;				//当前包内含房间数,-1MD5密码错误,-2请求包索引错误
	TRemoRoomStruct Rooms[1];		//房间数据，动态分配
} TPacketRoomStruct;

//中控主机返回设备信息结构体
typedef struct  {
	unsigned int DevID;			//设备ID
	int DevUnit;				//设备单元码(10101 1位楼层,4位单元码)
	int DevType;				//设备类型
	char DevName[16];			//设备名称
	unsigned short RoomID;		//所属房间编号
	unsigned short Status;		//状态
}TRemoDeviceStatus;

//中控主机返回设备信息协议包
typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int UserID;		//网络ID编号
	int Cmd;					//命令
	unsigned short PacketCnt;	//总包数
	unsigned short PacketIdx;	//包索引
	int DevCnt;					//包内含设备数量
	TRemoDeviceStatus Devs[1];
} TPacketDeviceStatus;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int NetID;
	int Cmd;
	unsigned int DevAddr;
	int DevUnit;
	int Status;
} TRetDevicePacket;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int NetID;
	int Cmd;                    //命令,必须是ELEC_GUARDMSG
	char UserCode[16];          //用户虚拟房号
	int TerminateType;          //终端类型
	unsigned int DevID;         //设备编号
	unsigned int DevAddr;       //设备单元号
	int DevUnit;                //设备楼层号
	char RoomName[24];          //房间名
	char DevName[36];           //设备名称
	unsigned int DevType;       //设备类型
	char cDevType[16];          //设备类型
	unsigned int Operate;       //操作
	unsigned int Success;       //操作是否成功
} TELECGUARDMSG;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int NetID;
	int Cmd;                    //命令,必须是ELEC_GUARDMSG
	char UserCode[16];          //用户虚拟房号
	int TerminateType;          //终端类型
	int AlarmState;				//布撤状态
	char AlarmContent[16];		//布撤防内容
} TELECGUARDSTATE;

//传输协议进行穿透时的包结构
typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int IP;
	unsigned int Port;
} TBURROW;

//RTP传输协议进行穿透时的包结构
typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int UserID;
	unsigned int IP;
	unsigned int Port;
	unsigned int Times;
} TRTPBURROW;

//搜索设备信息包定义
enum {
	CMD_GETSTATUS,		//读设备信息
	CMD_RETSTATUS			//返回设备信息
};
typedef struct
{
	unsigned int ID;		//包头ID号
	unsigned int Size;		//包大小
	unsigned int Type;		//包类型，必须为TP_DEVCHECK
	unsigned int Cmd;		//命令,必须为CMD_GETSTATUS
	unsigned int DevType;	//设备类型
	char Addition[20];		//附加信息
} TGETDEVICEMSG;

//返回设备信息包定义
typedef struct
{
	unsigned int ID;		//包头ID号
	unsigned int Size;		//包大小
	unsigned int Type;		//包类型，必须为TP_DEVCHECK
	unsigned int Cmd;		//命令,必须为CMD_RETSTATUS
	unsigned int DevType;	//设备类型(以数值方式描述)
	char Code[16];			//设备虚拟编号
	char Name[32];			//设备名称
	char IP[16];			//设备IP地址
	char dType[20];			//设备类型(以字符方式描述)
	char IMEI[24];			//设备机身号
	char Addition[20];		//附加信息
} TRETDEVICEMSG;

//TC-DNS查询指定号码的地址
typedef struct {
	int ID;
	int Size;
	unsigned int Type;
	unsigned int UserID;
	unsigned char IMEI[24];
} TDNSQueryIP;
//TC-DNS返回查询的IP地址
typedef struct {
	int ID;
	int Size;
	unsigned int Type;
	unsigned int UserID;
	unsigned char IP[4];
	unsigned int Port;
	unsigned int Status;
} TDNSReturnIP;
//户门口机读写配置信息
typedef enum {
	IP_USER,
	IP_USERONE,           //用户1IP
	IP_USERTWO,           //用户2IP
	IP_USERTHREE,          //用户3IP
	IP_USERFOUR,          //用户4IP
	IP_DATACENTER,          //用户数据中心IP
	IP_MANAGECENTER,         //用户管理中心IP
	IP_DEVICE,           //设备自身IP
	IP_GATEWAY,           //网关
	IP_SUBNETMASK,		//设置子网掩码
	IP_NETCARMAC        //网卡物理地址
} HMKCODETYPE;
typedef enum {
	HMK_READWRITE=IP_USER,		//操作户门口机
	DMK_READWRITE=IP_DEVICE		//操作栋门口机
} HMKCMDTYPE;


//程序升级包
typedef struct {
	int ID;
	int Size;
	unsigned int Type;
	unsigned short Major;		//协议主版本号
	unsigned char Minor;		//协议副版本号
	unsigned char Release;		//协议修订号
	unsigned short PromptUpdate;	//是否提示升级
	unsigned short ForceUpdate;		//强制升级
	unsigned short PromptReboot;	//是否提示重启
	unsigned char ForceReboot;		//强制重启
	unsigned char RebootType;		//重启方式 0应用程序,1操作系统
	char UpdateMsg[128];			//程序升级时提示的文字
} TUPDATEPACK;
//程序更新完成通知包
typedef struct {
	int ID;
	int Size;
	unsigned int Type;
	unsigned short Major;			//协议主版本号
	unsigned char Minor;			//协议副版本号
	unsigned char Release;			//协议修订号
	unsigned short AppMajor;		//程序主版本号
	unsigned char AppMinor;			//程序副版本号
	unsigned char AppRelease;		//程序修改版本号
	char IMEI[20];					//主机IMEI号码
	char Code[16];					//主机编号
	char Name[32];					//主机名称
	char BuilderDate[32];			//编译日期时间
} TUPDATEENDPACK;


//add by Jack
typedef struct _TVersion
{
	int Major;		//主版本
	int Minor;		//从版本
	int Release;		//释放版本
	int Reserve;		//保留，对齐
} TVersion;



//取程序版本号包
typedef struct {
	int ID;
	int Size;
	unsigned int Type;
	unsigned short Major;			//协议主版本号
	unsigned char Minor;			//协议副版本号
	unsigned char Release;			//协议修订号
	unsigned short AppMajor;		//程序主版本号
	unsigned char AppMinor;			//程序副版本号
	unsigned char AppRelease;		//程序修改版本号
} TGETAPPVERSION;

//电梯控制协议
#pragma pack(1)
typedef struct _ELEVATOR
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned char  head;
	unsigned char  AddrH;
	unsigned char  AddrL;
	unsigned char  Function;
	unsigned char  CellCode;
	unsigned char  FloorCode;
	unsigned char  Reserved;
	unsigned char  RoomNumber;
	unsigned char  Lrc;
	unsigned char  Cr;
	unsigned char  Lf;
}TElevator;
#pragma pack ()

typedef struct _TUpdateMsg
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int UpdateType;		//更新类型,1服务器IP更新,2房号配置信息表更新
}TUpdateMsg;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int Cmd;		//0，取密码
}TGetRemotePwd;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int LocalID;
	unsigned int DevType;		//设备类型
}TSetRemotePwd;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int LocalID;
	unsigned int DevType;		//设备类型
	char Password[16];			//密码
}TSetRemotePwdEx;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	char ESSID[32];
	unsigned char IP[16];
	unsigned char MAC[6];
	unsigned char QH;		//期号
	unsigned char DH;		//栋号
	unsigned char DRH;		//单元号
	unsigned char CH;		//层号
	unsigned char FH;		//房号
	unsigned char FJH;		//分机号
} TSendMACToSrv;

//更改中控主机房号
typedef struct
{
	unsigned int ID;		//PacketID++
	unsigned int Size;		//sizeof(TChangeRoomID)
	unsigned int Type;		//TP_CHANGEROOMID
	unsigned char QH;		//期号
	unsigned char DH;		//栋号
	unsigned char DRH;		//单元号
	unsigned char CH;		//层号
	unsigned char FH;		//房号
	unsigned char FJH;		//分机号
} TChangeRoomID;

//发送数据到网络转发器
typedef struct
{
	unsigned int ID;		//PacketID++
	unsigned int Size;		//sizeof(TSendNetTrans)
	unsigned int Type;		//TP_CHANGEROOMID
	unsigned short Cmd;		//命令,CMD_RF_DATA_TRAN
	short Par;				//命令参数,CMD_RF_DATA_TRAN
	unsigned short Len;		//数据区长度
	unsigned char Data[46];		//数据,可变长度
	unsigned short TTL;
	unsigned char IP[4];
	unsigned short Port;
	unsigned short SysCode;
	unsigned short SysPwd;
} TSendNetTrans;

typedef struct
{
	unsigned int ID;		//PacketID++
	unsigned int Size;		//sizeof(TRecvNetTrans)
	unsigned int Type;		//TP_CHANGEROOMID
	unsigned short Cmd;		//命令
	short Par;				//命令参数,0:正常数据通信,-1:设备暂时没有数据传输通道租用空闲,-2：设备收到的是无效长度RF数据
	unsigned short Len;		//数据区长度
	unsigned char Data[46];		//数据,可变长度
} TRecvNetTrans;
//---------------------------------------------------------------------------
typedef struct
{
	unsigned int ID;		//PacketID++
	unsigned int Size;		//sizeof(TRecvNetTrans)
	unsigned int Type;		//TP_TRANCMDBYSRV/TP_TRANSRTP/TP_TRANSELECTRIC/
	unsigned int RecvIP;	//接收方IP
	unsigned int RecvPort;	//接收方Port
} TTransPacket;				//通过服务器转发包头
//---------------------------------------------------------------------------
//视频头传输协议 xiehb 20131013
typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	char Reserve[64];
	unsigned int VideoHeadSize;
	char VideoHeadData[4];
} TRTPVideoHead;
//---------------------------------------------------------------------------

//add by Jack
typedef struct
{
	unsigned int ID;		//包头ID号
	unsigned int Size;		//包大小
	unsigned int Type;		//包类型，必须为TP_DEVCHECK
	unsigned int Cmd;		//命令,必须为CMD_GETSTATUS
	unsigned int DevType;	//设备类型
	char Addition[20];		//附加信息
} TGetDeviceInfo;

//返回设备信息包定义
typedef struct
{
	unsigned int ID;		//包头ID号
	unsigned int Size;		//包大小
	unsigned int Type;		//包类型，必须为TP_DEVCHECK
	unsigned int Cmd;		//命令,必须为CMD_RETSTATUS
	unsigned int DevType;	//设备类型(以数值方式描述)
	char Code[16];			//设备虚拟编号
	char Name[32];			//设备名称
	char IP[16];			//设备IP地址
	char dType[20];			//设备类型(以字符方式描述)
	char IMEI[24];			//设备机身号
	char Addition[20];		//附加信息
} TRetDeviceInfo;

typedef struct
{
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
}SETSYSTIME;

typedef struct
{
	int KeyCode;
	char IP[16];
}CONFIGIP;

typedef struct {
	int ID;
	int Size;
	unsigned int Type;
	HMKCMDTYPE Cmd;			//
	int Cnt;
	CONFIGIP Cfg[0];
} THMKConfig;


typedef struct
{
    unsigned int ID;
    unsigned int Size;
    unsigned int Type;
	int Cmd;						//设备信息指令 1.下载
	int Count;						//数据数量
	char Data[1];					//数据首地址
}CFGDEVICE;

typedef struct
{
	char ip[64];			//升级的文件
	char file[256];               //提示信息
} TUpdateRevertFile;


enum
{
	REQSERVERINF,			// 终端向服务器申请 ID 命令号
	RESPONSESERVERINF		// 服务器向终端返回 ID 命令号
}; //Cmd


typedef struct
{
	unsigned int ID;		//包头ID号
	unsigned int Size;		//包大小
	unsigned int Type;		//包类型
	unsigned int Cmd;		//命令
	unsigned int ReqType;	//请求信息设备类型
	unsigned long long int DevID;            //设备ID
	char Addition[32];		//附加信息
} TResDeviceInf;

typedef struct {
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int Proctol;             //协议版本
	unsigned int DevType;           //设备类型
	unsigned int dwLoDevID;               //设备编号
	unsigned int dwHiDevID;
	char QH[8];                     //栋号
	char DH[8];                     //栋号
	char DYH[8];                    //单元号
	char CH[8];                     //层号
	char FH[8];                     //房号
	char FJH[8];                     //房号
	char Name[32];					//设备名称
	char MAC[20];                   //设备MAC地址
	char GateWay[16];               //设备网关地址
	unsigned short DoorOnLine;              //门口机是否在线
	unsigned short CenterOnLine;            //中心是否在线
	char DoorIP[4][16];             //门口机IP
	char CenterIP[4][16];           //中心IP地址
	unsigned int ActiveTick;              //程序运行时长
	char BuilderDate[32];			//程序编译日期时间
	TVersion HwVer;                 //硬件版本号
	TVersion EBootVer;              //EBOOT版本号
	TVersion KerVer;                //内核版本号
	char AppVer[256];                //程序版本号
	TVersion ResourceVer;           //资源文件版本号
	// BOOL AllocRomote;               //是否允许远程访问
	char RouteSSID[16];             //当前连接的路由器SSID
	char RouteMac[16];              //路由器MAC地址
	char CurrTime[20];              //设备当前时间
	int BatState;                   //电池状态，BatState = (是否充电<<16) | 电压*100;
	unsigned int RootDiskSize;            //根文件系统剩余空间单位KB
	unsigned int UserDiskSize;            //用户文件系统剩余空间单位KB
} TRespDevInfo2;
//管理中心向门口机(D-H)发送房号下载

// typedef struct
// {

	// char Code[24];	 //卡号
	// char IP[32];  	 //IP地址

// }TUserTable2;

//IP对照表操作指令
// enum{
	// IP_NULL,								//无任何操作
	// IP_DOWNLOAD,							//下载
	// IP_READALL,								//读取全部数据
	// IP_READONE,								//读取一个用户的数据
	// IP_DEL,									//删除一条数据,根据Code卡号
	// IP_ADD,									//增加一条数据，根据Code卡号
	// IP_MOIDFIE,								//修改一条数据，根据Code卡号
	// IP_DELALL,								//删除所有IP 信息 20110228 xiehb
	// IP_DOWNLOAD2,							//新版本下载 20130906
	// IP_OK=0x100,							//OK应答
	// IP_ERROR								//失败应答
// };

// typedef struct
// {
	// unsigned int ID;		//包头ID号
	// unsigned int Size;		//包大小
	// unsigned int Type;		//包类型 TP_COMPARELIST
	// unsigned int Cmd;       //IP_DOWNLOAD2
	// unsigned int UserCount; //下载用户数据总数
	// unsigned int PacketNum; //下载包号
	// unsigned int Count;     //有多少条用户数据
	// TUserTable2 Users[1];     //用户数据
// }TComparelistPack;

typedef struct
{
	unsigned int ID;		//包头ID号
	unsigned int Size;		//包大小
	unsigned int Type;		//包类型
	char Data[1];           //cmd + vol
}TSetVolumePack;


// 设备控制命令字
enum
{
	DEVCTRL_IDLE, 		// 什么也没做，防止干扰的
	DEVCTRL_RESET,		// 复位
	DEVCTRL_POWERDOWN,  // 关机
};
// 复位命令数据包
typedef struct {
	unsigned int ID;		//包头ID号
	unsigned int Size;		//包大小
	unsigned int Type;		//包类型
    unsigned int DevType;                 //设备类型
    unsigned int Command;                 //命令
	char Addition[32];				//附加信息
}TDevCtrlCommand;

//------------------------------------------------------------------
// 门口机信息发到管理中心
enum {LOG_DR_DOOR, LOG_UR_ALARM, LOG_DR_LOWPOW};
typedef struct
{
    UINT32 ID;
    UINT32 Size;
    UINT32 Type;  //TP_LOGMSG
    UINT32 WarnLevel;         //警告级别 0 一般,日志  1 警告，需要弹框提示
    UINT32 DevType;             //设备类型
    UINT32 LogType;             //日志类型
    char   LogMessage[255];     //日志内容
} TLogMessage;

// 广告协议
enum {
	AD_SEND,			//广告发布
	AD_RECV,			//广告发布应答
	AD_DELETE,			//删除某区域广告图片,区域在ADAreaNo中设置
	AD_CLEAR,			//清空所有广告图片
	AD_DEL_FILE,		//删除单张图片
	AD_GET,             //获取门口机已存在的广告图片
	AD_GET_RESP,        //返回门口机已存在的文件名
};

typedef struct 
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int UserID;
	char MD5Pwd[36];
	unsigned int Cmd;			//命令
	unsigned int ADAreaNo;		//广告区号
	char ADSrv[52];				//广告服务器
	char ADPath[96];			//广告路径
	unsigned int FileSize;		//文件大小
} TSendAdver;

typedef struct 
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;
	unsigned int UserID;
	unsigned int Cmd;			//命令
	unsigned int Status;		//状态返回广告发布状态。0失败  1成功
} TReponseAdver;

typedef struct
{
	unsigned int ADAreaNo;		//广告区号
	char ADFileName[32];        //广告文件名
} TADFileInfo;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;          //
	unsigned int UserID;
	unsigned int Cmd;			//命令 AD_GET_RESP
	unsigned int FreeSpace;     //剩余空间大小
	unsigned int PacketCnt;     //总分包数
	unsigned int PacktMaxFiles; //一个包最大包含文件数量
	unsigned int PacketIdx;     //当前索引
	unsigned int FileCnt;       //当前包内有多少个文件
	TADFileInfo AdFiles[1];     //返回的文件结构描述         
} TReponseAdGet;
// 开锁记录上传
typedef struct
{
	UINT32 DevType;			// 室外机类型
	UINT32 UnlockMode;		// 开锁方式: 刷卡开锁，呼室内机开锁，管理中心开锁，密码开锁等
	UINT32 UnlockStatus;	// =0 开锁失败， =1开锁成功
	char   DevName[32];		// 室外机或门禁设备名称
	char   QH[4];			// 期号
	char   DH[4];			// 栋号	
	char   DYH[4];			// 单元号
	char   Reserver[64]; 	// 保留
}TUnlockRec;

typedef struct
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;              //TP_CALLUSER  TP_CALLUSER2
	unsigned int Version;			//协议版本号
   	unsigned int CallType;			//
   	unsigned int IsFDoor;			// 1 户门口机 0 单元格门口机
	unsigned int VideoType;			//视频类型，0 H264, 1 Mpeg4
	unsigned short VideoWidth;		//视频类型  320
	unsigned short VideoHeight;		//视频类型  240
	char CALLIP[16];
	char Reserve[128];	
} TDoorCallMoniter;

typedef struct // 管理中心回复门口机随监请求
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Type;    // TP_RETCALLUSER2
	unsigned int Version; // 协议版本号
	unsigned int Command; // 管理中心发到门口机命令
	unsigned int RecordID;// 随监呼叫记录ID
	char Reserve[124];

} TCenterRetCallMoniter;

// 门口机呼叫室内机是否接听结果
typedef struct
{
	UINT32 ID;
	UINT32 Size;
	UINT32 Type; 			// TP_CALLACKREC=0x806A
	UINT32 RecordID;		// 记录ID
	UINT32 IsAnswer;		// 是否接听 ＝0未接听 =1已接听
	char   DoorIP[16];		// 门口机IP
	char   Reserver[64]; 	// 保留
}TDoorCallUserRec;

//------------------------------------------------------------------
#define MakeVerison(Ver,Major,Minor,Release) \
	(*(DWORD*)&Ver) = ((Major) | ((Minor)<<8) | ((Release)<<16))

#define CopyVerison(DestVer,SrcVer) \
	((*(DWORD*)&DestVer) = (*(DWORD*)&SrcVer))

#endif
