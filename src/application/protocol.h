/*
 * =============================================================================
 *
 *       Filename:  protocol.h
 *
 *    Description:  协议结构
 *
 *        Version:  1.0
 *        Created:  2018-02-26 15:16:36 
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

	// // 设备控件结构
	// typedef struct _StCtrl{
		// u8 ui_id;		// 控件ID
		// u8 x,y,h,w;
		// u8 device_id;	// 设备编号
		// u8 op_code;  	// 设备操作
	// }StCtrl;

	typedef struct _DevToCom {
		void (*initCom)(void);  // 发送联机命令
		void (*getStatus)(void);  // 发送获取状态命令
		void (*sendOpt)(int device,int opt);  // 发送操作命令
	}DevToCom;

	typedef struct _DevToApp {
		void (*getStatus)(void);  // 发送获取状态命令
		void (*sendOpt)(int device,int opt);  // 发送操作命令
	}DevToApp;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
