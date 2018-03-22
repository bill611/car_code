/*
 * =====================================================================================
 *
 *       Filename:  MyGpioCtr.h
 *
 *    Description:  创建GPIO对象
 *
 *        Version:  1.0
 *        Created:  2015-12-24 09:26:29 
 *       Revision:  1.0
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
#ifndef _MY_GPIO_H
#define _MY_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define FLASH_FOREVER	0x7FFFFFFF

	typedef enum {
		ENUM_GPIO_LCD,	// 消回音芯片复位
	}GPIO_TBL;

	typedef enum {//200ms为周期
		FLASH_STOP = 0,
		FLASH_FAST = 1,	//200ms
		FLASH_SLOW =5,		//1s
	}STRUCT_SPEED;


	enum {
		IO_ACTIVE = 0,	// 有效
		IO_INACTIVE,	// 无效
		IO_INPUT,		// 输入
		IO_NO_EXIST	//当IO不存在时，用于个别型号不存在该IO口
	};


	struct _MyGpioPriv;

	typedef struct _MyGpio {
		struct _MyGpioPriv *Priv;
		int io_num;		//GPIO数量
		void (*Init)(struct _MyGpio *this); //  GPIO初始化，在创建IO对象后必须执行 
		void (*FlashStart)(struct _MyGpio *this,int port,int freq,int times); //  设置GPIO闪烁，并执行 
		void (*FlashStop)(struct _MyGpio *this,int port); // GPIO停止闪烁 
		void (*FlashTimer)(struct _MyGpio *this); //  GPIO闪烁执行函数，在单独的定时线程中执行 
		void (*SetValue)(struct _MyGpio *this,int port,int Value); // GPIO口输出赋值，不立即执行 
		void (*SetValueNow)(struct _MyGpio *this,int port,int Value); // GPIO口输出赋值，并立即执行
		int (*IsActive)(struct _MyGpio *this,int port); //  读取输入IO值，并判断是否IO有效 
		void (*Handle)(struct _MyGpio *this); //  GPIO输出执行函数 
		void (*Destroy)(struct _MyGpio *this);
	}MyGpio;

	typedef struct _MyGpioPriv {
		const char      portid;
		const int       portmask;
		const char      *active;		//有效值
		const int 	  default_value;	//默认值
		int  	active_time;

		int		  current_value;
		int		  portnum;
		int 	  flash_times;
		int 	  flash_set_time;
		int 	  flash_delay_time;
		int 	  flash_even_flag;
		int		  delay_time;
	}MyGpioPriv;


	MyGpio* myGpioPrivCreate(MyGpioPriv *gpio_table);

	extern MyGpio *Mygpio;
	extern MyGpioPriv gpiotbl[];

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
