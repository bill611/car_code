/*
 * =============================================================================
 *
 *       Filename:  protocol.c
 *
 *    Description:  协议处理
 *
 *        Version:  1.0
 *        Created:  2018-02-26 15:25:45
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include "externfunc.h"
#include "UDPServer.h"
#include "UartDeal.h"
#include "debug.h"
#include "protocol.h"

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/
extern uint32_t getDiffSysTick(uint64_t new,uint64_t old);

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static void proUdpSendStatus(void);
static void proDbgPrint(const char *fmt, ...);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
//------------协议A,串口协议
#define COM_HEAD			0xFE  // 命令头
#define COM_ORDER_CONNECT	0xFF  // 联机和联机应答命令
#define COM_ORDER_STATUS	0xFC  // 获取客户设备状态和状态应答命令

#pragma pack(1)
// 发送联机或者获取客户设备状态命令数据包
typedef struct{
	u8 head;
	u8 order;
	u8 check; // 累加和校验
}st_connect;

/*
   例子（联机命令,7寸屏上电完成时间隔100ms发送3次，直至收到同样应答回复）：
   0xFE 0xFF 0xFC
   例子（获取客户设备状态命令，联机完成后需要获取一次）：
   0xFE 0xFC 0xFA
   */

// 发送操作命令数据包（如果操作成功，客户设备会回复设备状态应答数据包）
typedef struct{
	u8 device;  // 界面机器码
	u8 opCode;  // 界面功能动作码
}st_op;
/*
   例子：0x08 0x80
   */

typedef struct{
	u8 head;
	u8 order;

	/*
	   bit7座椅加热状态：0关闭，1开启
	   bit6座椅制冷状态：0关闭，1开启
	   bit5、4位按摩状态：00关闭，01腿部，10背部，11全身
	   */
	u8 leftSeat;
	u8 rightSeat;

	u8 mute; // rev01 // 静音状态
	u8 rev02;
	u8 rev03;
	u8 rev04;

	/*
bit7:1为ON,0为OFF;
bit6-4: 1为亮1格,2为亮2格,3为亮3格，4为亮4格,5为亮5格；
bit3:1为单色显示，0为循环显示；
bit2-0: 1为闪烁2秒，2为闪烁5秒，3为闪烁10秒，4为15秒，5为20秒，6为30秒。
*/
	u8 light1; // 0~20
	u8 light2; // 0~20
	u8 light3; // 0~20

	/*
	   高4位：8为电源关闭，9为电源打开
	   低4位：0电视上升中，1到顶，2下降中，3到底，5停止
	   */
	u8 tvPower;

	u8 monitor; // 0为非监控状态，1为监控状态
	u8 rev05;
	u8 ucMedia; // 0:cd, 1:dvd, 2:cmmb, 3:satelliteTV
	u8 rev1;
	u8 rev2;
	u8 rev3;
	u8 rev4;
	u8 rev5;
	u8 rev6;
	u8 check; // 累加和校验
}st_status;
//------------协议A,串口协议---end

//------------协议B,UDP协议
#define LOCAL_PORT		8080 // 本地udp接收端口
#define REMOTE_PORT		6060 // app端udp接收端口

#define SERIAL_REC_PORT		1000 // 调试用接收到串口数据后发送广播
#define SERIAL_SEDN_PORT	1001 // 调试用发送串口数据后发送广播
#define DEBUG_PORT	        1002 // 调试用发送打印信息
// 7寸屏和app端接收IP是均广播方式：255.255.255.255
//一般网络udp通讯，双方同一个数据包均发送3次，数据包ID不变，以确保对方收到。

// 1、发送获取客户设备状态命令数据包
#define NET_HEAD                0x55aa66bb
#define NET_END                 0xbb66aa55
typedef struct {
	u32 flagStart; // 0x55aa66bb

	u16 id; // 数据包ID，发送方累加，接收方检测以区分重发包

	u8 order; // false:获取客户设备状态命令, true:界面操作命令
	u16 tag; // 界面控件tag
	u8 data; // 界面控件区分一个部件多个部分或者多个功能

	u32 flagEnd; // 0xbb66aa55
}st_set;
/*
   当 order = 0 时是发送状态命令。
   7寸屏收到后，转换成“协议A”通过串口发送获取客户设备状态命令。
   */

//2、发送操作命令数据包（如果操作成功，客户设备会回复设备状态应答数据包）
//协议同上，其中当 order = 1 时是发送操作命令。
//7寸屏收到后，通过tag和data查表匹配查出转换码（含设备码和操作码）（见下面表代码），按“协议A”方式通过串口发送操作命令给客户设备。

// udp控制命令发送到com的转换
typedef struct{
	u16 udpTag;
	u8 udpData;
	u8 comDevice;
	u8 comOpCode;
}st_udp2com;

//转换表见：ConversionTable.c

//7寸屏 to App（udp通讯）：
//7寸屏收到“协议A”的联机命令应答或设备状态回复数据包均转换成如下 网络udp数据包 发app
typedef struct {
	u32 flagStart; // 0x55aa66bb

	u16 id; // 数据包ID，发送方累加，接收方检测以区分重发包

	u8 bConnect; // false:没有联机, true:已经联机

	u8 bLeftSeatKey; // cold 0:close, 1:open
	u8 ucLeftSeatHeating; // hot 0:close, 1:open
	u8 ucLeftSeatMassage; // 0:close, 1:leg, 2:rear, 3:body

	u8 bRightSeatKey; // cold 0:close, 1:open
	u8 ucRightSeatHeating; // hot 0:close, 1:open
	u8 ucRightSeatMassage; // 0:close, 1:leg, 2:rear, 3:body

	u8 ucVolumeCD; // use for mute status
	u8 ucVolumeDVD; // unuse
	u8 ucVolumeCMMB; // unuse
	u8 ucVolumeSatelliteTV; // unuse

	/*
bit7:1为ON,0为OFF;
bit6-4: 1为亮1格,2为亮2格,3为亮3格，4为亮4格,5为亮5格；
bit3:1为单色显示，0为循环显示；
bit2-0: 1为闪烁2秒，2为闪烁5秒，3为闪烁10秒，4为15秒，5为20秒，6为30秒。
*/
	u8 ucLight1;
	u8 ucLight2;
	u8 ucLight3;

	/*
	   高4位：8为电源关闭，9为电源打开
	   低4位：0电视上升中，1到顶，2下降中，3到底，5停止
	   */
	u8 ucPowerTV;

	u8 ucMonitor; // 0:no, 1:monitor

	u8 bCallSw; // unuse

	u8 ucMedia; // unuse //0:cd, 1:dvd, 2:cmmb, 3:satelliteTV

	u8 rev0;
	u8 rev1;
	u8 rev2;
	u8 rev3;
	u8 rev4;
	u8 rev5;

	u32 flagEnd; // 0xbb66aa55
}st_var;
#pragma pack()
//------------协议B,UDP协议---end

#define VAILDTIME 	(5000)				//5秒

typedef struct _PacketsID {
	char IP[16];
	u16 id;
	u16 dwTick;		//时间
}PacketsID;

/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
DevToCom *pro_com;
DevToApp *pro_app;
static int online_state;  //联机状态 0非联机 1联机
static int start;  // 是否开始串口通讯 1开始 0未开始
static int packet_pos;
static PacketsID packets_id[10];
static u16 packet_id = 0;

// udp控制命令到串口命令的转换表
const st_udp2com udp2com[] = {
	// udp-tag, udp-data, com-device, com-opcode
	// 主界面
	{0x00,0x00,0x11,0x84},
	{0x01,0x00,0x11,0xab},
	{0x02,0x00,0x11,0xa2},
	{0x03,0x00,0x11,0xad},

	{0x04,0x00,0x11,0x80},
	{0x05,0x00,0x11,0xa3},
	{0x06,0x00,0x11,0xaf},
	{0x07,0x00,0x11,0x8e},

	{0x08,0x00,0x11,0x82},
	{0x09,0x00,0x11,0x81},
	{0x0a,0x00,0x11,0x85},
	{0x0b,0x00,0x11,0x83},

	{0x0c,0x00,0x11,0x86},
	//connect
	//help
	{0x0d,0x00,0x11,0x87},
	{0x10,0x00,0x11,0x88}, // 旋转座椅

	{0x32,0x00,0x11,0xd1},
	{0x33,0x00,0x11,0xb4},
	{0x34,0x00,0x11,0xb0},
	{0x35,0x00,0x11,0xc4},
	// 左座椅
	{0x64,0x00,0x01,0x8c},
	{0x65,0x00,0x01,0x8b},
	{0x66,0x00,0x01,0x8d},
	{0x67,0x00,0x01,0x8e},
	{0x68,0x00,0x01,0x90},
	{0x69,0x00,0x01,0x8f},
	{0x6a,0x00,0x01,0x94},
	{0x6b,0x00,0x01,0x93},
	{0x6c,0x00,0x01,0x95},
	{0x6d,0x00,0x01,0x96},

	{0x79,0x00,0x01,0x82},
	{0x7a,0x00,0x01,0x83},
	{0x7b,0x00,0x01,0x80},
	{0x7c,0x00,0x01,0x81},

	{0x81,0x00,0x01,0x99},
	{0x82,0x00,0x01,0x9a},

	{0x83,0x00,0x01,0x84},
	{0x84,0x00,0x01,0x85},

	{0x86,0x00,0x01,0x87},
	{0x87,0x00,0x01,0x88},
	{0x88,0x00,0x01,0x89},
	{0x89,0x00,0x01,0x8a},

	{0x8a,0x00,0x01,0x82},
	{0x8b,0x00,0x01,0x83},
	{0x8c,0x00,0x01,0x80},
	{0x8d,0x00,0x01,0x81},

	{0xaa,0x00,0x01,0x9d},
	{0xab,0x00,0x01,0x9e},
	{0xac,0x00,0x01,0x9c},
	{0xad,0x00,0x01,0x9b},
	// 右座椅
	{0x6f,0x00,0x02,0x8c},
	{0x6e,0x00,0x02,0x8b},
	{0x70,0x00,0x02,0x8d},
	{0x71,0x00,0x02,0x8e},
	{0x73,0x00,0x02,0x90},
	{0x72,0x00,0x02,0x8f},
	{0x75,0x00,0x02,0x94},
	{0x74,0x00,0x02,0x93},
	{0x76,0x00,0x02,0x95},
	{0x77,0x00,0x02,0x96},

	{0xa1,0x00,0x02,0x82},
	{0xa2,0x00,0x02,0x83},
	{0xa3,0x00,0x02,0x80},
	{0xa4,0x00,0x02,0x81},

	{0x8e,0x00,0x02,0x99},
	{0x8f,0x00,0x02,0x9a},

	{0x90,0x00,0x02,0x84},
	{0x91,0x00,0x02,0x85},

	{0x93,0x00,0x02,0x87},
	{0x94,0x00,0x02,0x88},
	{0x95,0x00,0x02,0x89},
	{0x96,0x00,0x02,0x8a},

	{0x97,0x00,0x02,0x82},
	{0x98,0x00,0x02,0x83},
	{0x99,0x00,0x02,0x80},
	{0x9a,0x00,0x02,0x81},

	{0xb4,0x00,0x02,0x9d},
	{0xb5,0x00,0x02,0x9e},
	{0xb6,0x00,0x02,0x9c},
	{0xb7,0x00,0x02,0x9b},
	// 秘书椅
	{0xc8,0x00,0x13,0x80},
	{0xc9,0x00,0x13,0x90},
	{0xca,0x00,0x13,0xaa},
	{0xcb,0x00,0x13,0xab},
	// 灯光
	{0x012c,0x00,0x03,0x81},
	{0x012e,0x00,0x03,0x80},
	{0x012f,0x00,0x03,0x82},
	{0x0130,0x00,0x03,0x83},
	{0x0131,0x00,0x03,0x84},

	{0x0132,0x00,0x1e,0x81},
	{0x0134,0x00,0x1e,0x80},
	{0x0135,0x00,0x1e,0x82},
	{0x0136,0x00,0x1e,0x83},
	{0x0137,0x00,0x1e,0x84},

	{0x0138,0x00,0x1f,0x81},
	{0x013a,0x00,0x1f,0x80},
	{0x013b,0x00,0x1f,0x82},
	{0x013c,0x00,0x1f,0x83},
	{0x013d,0x00,0x1f,0x84},
	// 窗帘
	{0x0190,0x00,0x15,0x80},
	{0x0191,0x00,0x15,0x8f},
	{0x0192,0x00,0x15,0xac},
	{0x0193,0x00,0x15,0x8d},
	{0x0194,0x00,0x15,0x96},
	{0x0195,0x00,0x15,0x93},
	{0x0196,0x00,0x15,0xa9},
	{0x0197,0x00,0x15,0x81},
	// apple tv
	{0x01f4,0x00,0x04,0x80},
	// 电视控制
	{0x0258,0x00,0x0c,0xab},
	{0x0259,0x00,0x0c,0x87},
	{0x025a,0x00,0x0c,0x8e},
	{0x025b,0x00,0x0c,0x88},
	{0x025c,0x00,0x0c,0xac},
	{0x025d,0x00,0x0c,0xa7},
	{0x025e,0x00,0x0c,0x80},
	{0x025f,0x00,0x0c,0xa6},

	{0x025d,0x01,0x0c,0xa9},
	{0x025e,0x01,0x0c,0xaa},
	{0x025f,0x01,0x0c,0xa8},
	// CD控制
	{0x02bc,0x00,0x0b,0xa6},
	{0x02bd,0x00,0x0b,0x87},
	{0x02be,0x00,0x0b,0x8e},
	{0x02bf,0x00,0x0b,0x88},
	{0x02c0,0x00,0x0b,0xa7},
	{0x02c1,0x00,0x0b,0x8f},
	{0x02c2,0x00,0x0b,0x8d},
	{0x02c3,0x00,0x0b,0xa9},
	{0x02c4,0x00,0x0b,0xaa},
	{0x02c5,0x00,0x0b,0x83},
	{0x02c6,0x00,0x0b,0x8a},
	{0x02c7,0x00,0x0b,0x8c},
	{0x02c8,0x00,0x0b,0x8b},
	{0x02c9,0x00,0x0b,0x84},
	// DVD音量
	{0x0320,0x00,0x14,0xa6},
	{0x0321,0x00,0x14,0x87},
	{0x0322,0x00,0x14,0x8e},
	{0x0323,0x00,0x14,0x88},
	{0x0324,0x00,0x14,0xa7},
	{0x0325,0x00,0x14,0x8f},
	{0x0326,0x00,0x14,0x8d},
	{0x0327,0x00,0x14,0xa9},
	{0x0328,0x00,0x14,0xaa},
	{0x0329,0x00,0x14,0x83},
	{0x032a,0x00,0x14,0x8a},
	{0x032b,0x00,0x14,0x8c},
	{0x032c,0x00,0x14,0x8b},
	{0x032d,0x00,0x14,0x84},
	{0x032e,0x00,0x14,0x96},
	{0x032f,0x00,0x14,0x81},
	{0x0330,0x00,0x14,0x93},
	// 监控
	{0x0384,0x00,0x0f,0x85},
	// 车门
	{0x03E8,0x00,0x05,0x90},
	{0x03E9,0x00,0x05,0x91},
	{0x03Ea,0x00,0x05,0x92},
	{0x03Eb,0x00,0x05,0x93},
	// 电动床
	{0x044c,0x00,0x06,0x80},
	{0x044d,0x00,0x06,0x81},
	{0x044e,0x00,0x06,0x82},
	{0x044f,0x00,0x06,0x83},
	// 卫星电视
	{0x04b0,0x00,0x0d,0x80},
	{0x04b1,0x00,0x0d,0x82},
	{0x04b2,0x00,0x0d,0x84},
	{0x04b3,0x00,0x0d,0x83},
	{0x04b4,0x00,0x0d,0x81},
	{0x04b5,0x00,0x0d,0x85},
	{0x04b7,0x00,0x0d,0x86},
	{0x04b8,0x00,0x0d,0x87},
	{0x04b9,0x00,0x0d,0x88},
	{0x04ba,0x00,0x0d,0x89},
	{0x04bb,0x00,0x0d,0x8a},
	{0x04bc,0x00,0x0d,0x8b},
	{0x04bd,0x00,0x0d,0x8c},
	// 天窗
	{0x0514,0x00,0x18,0xA7},
	{0x0515,0x00,0x18,0xA6},
	{0x0514,0x01,0x18,0xA9},
	{0x0515,0x01,0x18,0xA8},
	// 桌板控制
	{0x0640,0x00,0x07,0x80},
	{0x0641,0x00,0x07,0x81},
	{0x0642,0x00,0x07,0x82},
	{0x0643,0x00,0x07,0x83},
	{0x0644,0x00,0x07,0x84},
	{0x0645,0x00,0x07,0x85},
	// 旋转座椅
	{0x06A4,0x00,0x08,0x84},
	{0x06A5,0x00,0x08,0x85},
	{0x06AB,0x00,0x08,0x80},
	{0x06AA,0x00,0x08,0x81},
	{0x06AD,0x00,0x08,0x82},
	{0x06AC,0x00,0x08,0x83},

	{0x06A6,0x00,0x09,0x84},
	{0x06A7,0x00,0x09,0x85},
	{0x06AF,0x00,0x09,0x80},
	{0x06AE,0x00,0x09,0x81},
	{0x06B1,0x00,0x09,0x82},
	{0x06B0,0x00,0x09,0x83},
};

/* ---------------------------------------------------------------------------*/
/**
 * @brief proComSendOpt 发送操作命令
 *
 * @param device 操作码
 * @param opt 命令
 */
/* ---------------------------------------------------------------------------*/
static void proComSendOpt(int device,int opt)
{
	if (device == 0 || opt == 0)
		return;
	uart->SndData[0] = device;
	uart->SndData[1] = opt;
	saveLog("%x,%x\n", device,opt);
	uart->ToSingleChip(uart,2);
}

static void proComStart(void)
{
   start = 1;
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief proComSendGetStatus 发送获取状态命令
 */
/* ---------------------------------------------------------------------------*/
static void proComSendGetStatus(void)
{
	uart->SndData[0] = COM_HEAD;
	uart->SndData[1] = COM_ORDER_STATUS;
	unsigned char check = (unsigned char)(uart->SndData[0] + uart->SndData[1]);
	uart->SndData[2] = check;
	uart->ToSingleChip(uart,3);
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief proComInitThread 上电每1s发送一次联机命令
 *
 * @param arg
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
static void* proComInitThread(void *arg)
{
	while (online_state == 0) {
		if (uart == NULL || start == 0) {
			sleep(1);
			continue;
		}

		uart->SndData[0] = COM_HEAD;
		uart->SndData[1] = COM_ORDER_CONNECT;
		unsigned char check = (unsigned char)(uart->SndData[0] + uart->SndData[1]);
		uart->SndData[2] = check;
		uart->ToSingleChip(uart,3);
		sleep(1);
	}
	return NULL;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief proUdpGetOnline 获取在线状态
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
static int proUdpGetOnline(void)
{
	return online_state ;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief proUdpGetRecivePort 获取接收端口
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
static int proUdpGetRecivePort(void)
{
	return LOCAL_PORT;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief proUdpGetSendPort 获取发送端口
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
static int proUdpGetSendPort(void)
{
	return REMOTE_PORT;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief proComCheckOnlineCmd 收到串口数据后，检测是否为联机命令
 *
 * @param data
 * @param leng
 */
/* ---------------------------------------------------------------------------*/
static void proComCheckOnlineCmd(unsigned char *data,int leng)
{
    if (leng != 3) {
        proDbgPrint("[%s]leng:%d != 3\n",__FUNCTION__,leng);
		return;
    }
	if (data[0] == COM_HEAD && data[1] == COM_ORDER_CONNECT) {
		unsigned char check = (unsigned char)(COM_HEAD + COM_ORDER_CONNECT);
		if (data[2] == check)	 {
			online_state = 1;
			// 联机后发送UDP数据包
			proUdpSendStatus();
			// 联机后发送获取状态命令
			proComSendGetStatus();
		}
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief proComCheckStateCmd 收到串口数据后，检测是否为获取状态命令
 *
 * @param data
 * @param leng
 */
/* ---------------------------------------------------------------------------*/
static void proComCheckStateCmd(unsigned char *data,int leng)
{
    if (leng != sizeof(st_status)) {
        proDbgPrint("[%s]leng:%d != st_status:%d\n",
                __FUNCTION__,leng,sizeof(st_status));
		return;
    }
	st_status *status = (st_status *)data;
	Public.leftSeat = status->leftSeat;
	Public.rightSeat = status->rightSeat;

	Public.mute = status->mute;
    Public.rev02 = status->rev02;
    Public.rev03 = status->rev03;
    Public.rev04 = status->rev04;

	Public.light1 = status->light1;
	Public.light2 = status->light2;
	Public.light3 = status->light3;
	Public.tvPower = status->tvPower;
	Public.monitor = status->monitor;
    Public.rev05 = status->rev05;
    Public.ucMedia = status->ucMedia;

    Public.rev1 = status->rev1;
    Public.rev2 = status->rev2;
    Public.rev3 = status->rev3;
    Public.rev4 = status->rev4;
    Public.rev5 = status->rev5;
    Public.rev6 = status->rev6;
    Screen.foreachForm(MSG_UPDATESTATUS,0,0);
	proUdpSendStatus();
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief proUdpFilter 判断Udp数据是否为重发
 *
 * @param ABinding
 * @param AData
 *
 * @returns
 */
/* ---------------------------------------------------------------------------*/
static int proUdpFilter(SocketHandle *ABinding,SocketPacket *AData)
{
	int i;
	unsigned int dwTick;
	if (ABinding == NULL || AData == NULL) {
        proDbgPrint("[%s]ABinding = %x != AData:%x\n",
                __FUNCTION__,AData,AData);
		return 0;
	}
	//回复数据包
	// for (i=0; i<AData->Size; i++) {
		// printf("[%d]%x\n", i,AData->Data[i]);
	// }
    if(AData->Size != sizeof(st_set)) {
        proDbgPrint("[%s]leng:%d != st_set:%d\n",
                __FUNCTION__,AData->Size,sizeof(st_set));
        return 0;
    }
    st_set *data = (st_set *)AData->Data;
	if (data->flagStart != NET_HEAD
            || data->flagEnd != NET_END) {
        proDbgPrint("[%s]data->flagStart:%#x != data->flagEnd:%#x\n",
                __FUNCTION__,data->flagStart,data->flagEnd);
		return 0;
    }
    //判断包是否重发
    dwTick = GetTickCount();
    for(i=0;i<10;i++) {
        if (strcmp(ABinding->IP,packets_id[i].IP) == 0
                && data->id == packets_id[i].id
                && (getDiffSysTick(dwTick,packets_id[i].dwTick) < VAILDTIME)) {
            // saveLog("Packet ID %d is already receive!\n",packets_id[i].id);
            return 0;
        }
    }

	//保存ID
    sprintf(packets_id[packet_pos].IP,"%s",ABinding->IP);
	packets_id[packet_pos].id = data->id;
	packets_id[packet_pos++].dwTick = dwTick;
	packet_pos %= 10;
    return 1;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief proUdpSendStatus 发送UDP到APP
 */
/* ---------------------------------------------------------------------------*/
static void proUdpSendStatus(void)
{
	st_var data;
	data.flagStart = NET_HEAD;
	data.id = packet_id++;
	data.bConnect = online_state;
	data.bLeftSeatKey = BIT(Public.leftSeat,6);
	data.ucLeftSeatHeating = BIT(Public.leftSeat,7);
	data.ucLeftSeatMassage = BIT2(Public.leftSeat,4);

	data.bRightSeatKey = BIT(Public.rightSeat,6);
	data.ucRightSeatHeating = BIT(Public.rightSeat,7);
	data.ucRightSeatMassage = BIT2(Public.rightSeat,4);

	data.ucVolumeCD = Public.mute;
    data.ucVolumeDVD = Public.rev02;
    data.ucVolumeCMMB = Public.rev03;
    data.ucVolumeSatelliteTV = Public.rev04;

	data.ucLight1 = Public.light1;
	data.ucLight2 = Public.light2;
	data.ucLight3 = Public.light3;

	data.ucPowerTV = Public.tvPower;

	data.ucMonitor = Public.monitor;
	data.bCallSw = Public.rev05;
	data.ucMedia = Public.ucMedia;

	data.rev0 = Public.rev1;
	data.rev1 = Public.rev2;
	data.rev2 = Public.rev3;
	data.rev3 = Public.rev4;
    data.rev4 = Public.rev5;
    data.rev5 = Public.rev6;

	data.flagEnd = NET_END;
	udp_server->AddTask(udp_server,"255.255.255.255",REMOTE_PORT,
			&data,sizeof(data),3,0,NULL,NULL);
}

static void proDbgRecSerial(unsigned char *data,int leng)
{
#ifndef WATCHDOG_DEBUG
    udp_server->AddTask(udp_server,"255.255.255.255",SERIAL_REC_PORT,
            data,leng,1,0,NULL,NULL);
#endif
}
static void proDbgSendSerial(unsigned char *data,int leng)
{
#ifndef WATCHDOG_DEBUG
    udp_server->AddTask(udp_server,"255.255.255.255",SERIAL_SEDN_PORT,
            data,leng,1,0,NULL,NULL);
#endif
}
static void proDbgPrint(const char *fmt, ...)
{
#ifdef PC
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);
    return;
#endif
#ifndef WATCHDOG_DEBUG
    va_list args;
    char buf[128] = {0};
    va_start(args, fmt);
    vsprintf(buf,fmt, args);
    va_end(args);
	udp_server->AddTask(udp_server,"255.255.255.255",DEBUG_PORT,
			(void *)buf,strlen(buf),1,0,NULL,NULL);
#endif
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief checOptCode 检查匹配码
 *
 * @param tag
 * @param data
 * @param device
 * @param code
 */
/* ---------------------------------------------------------------------------*/
static int checOptCode(u16 tag,u8 data, u8 *device, u8 *code)
{
	int i;
	for (i=0; i<NELEMENTS(udp2com); i++) {
		if (tag == udp2com[i].udpTag && data == udp2com[i].udpData) {
			*device = udp2com[i].comDevice;
			*code = udp2com[i].comOpCode;
			return 1;
		}
	}
	return 0;
}

static void udpSocketRead(SocketHandle *ABinding,SocketPacket *AData)
{
    if (proUdpFilter(ABinding,AData) == 0)
        return;
    st_set *data = (st_set *)AData->Data;
	if (data->order == 0) {
		proComSendGetStatus();
	} else if (data->order == 1) {
		u8 device = 0,code = 0;
		int ret = checOptCode(data->tag,data->data,&device,&code);
		if (ret == 0)
			return;
		proComSendOpt(device,code);
	}
}

void initProtocol(void)
{
	pro_com = (DevToCom *)calloc(1,sizeof(DevToCom));
	pro_app = (DevToApp *)calloc(1,sizeof(DevToApp));
	online_state = 0;
    start = 0;
	pro_com->start = proComStart;
	pro_com->sendOpt = proComSendOpt;
	pro_com->getOnline = proUdpGetOnline;
	pro_com->checkOnlineCmd = proComCheckOnlineCmd;
	pro_com->checkStateCmd = proComCheckStateCmd;

	pro_app->getSendPort = proUdpGetSendPort;
	pro_app->getRecivePort = proUdpGetRecivePort;
	pro_app->udpSocketRead = udpSocketRead;
	pro_app->dbgRecSerial = proDbgRecSerial;
	pro_app->dbgSendSerial = proDbgSendSerial;
	pro_app->dbgPrint = proDbgPrint;


#ifdef PC
	return;
#endif
	pthread_t id;
	pthread_attr_t  attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	int ret = pthread_create(&id,&attr,proComInitThread,NULL);
	if(ret) {
		printf("proComInitThread() pthread failt,Error code:%d\n",ret);
	}
	pthread_attr_destroy(&attr);
}
