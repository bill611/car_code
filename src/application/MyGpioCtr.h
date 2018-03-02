/*
 * =====================================================================================
 *
 *       Filename:  MyGpioCtr.h
 *
 *    Description:  ����GPIO����
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
		ENUM_GPIO_ECHO_RESET,	// ������оƬ��λ
		ENUM_GPIO_CCD_POWER,	// ����ͷ��Դ
		ENUM_GPIO_CCD_LED,		// ����ͷ�����
		ENUM_GPIO_UNLOCK,		// ����
		ENUM_GPIO_RESET,		// �ָ��������ýӿڣ�����
	}GPIO_TBL;

	typedef enum {//200msΪ����
		FLASH_STOP = 0,
		FLASH_FAST = 1,	//200ms
		FLASH_SLOW =5,		//1s
	}STRUCT_SPEED;


	enum {
		IO_ACTIVE = 0,	// ��Ч
		IO_INACTIVE,	// ��Ч
		IO_INPUT,		// ����
		IO_NO_EXIST	//��IO������ʱ�����ڸ����ͺŲ����ڸ�IO��
	};


	struct _MyGpioPriv;

	typedef struct _MyGpio {
		struct _MyGpioPriv *Priv;
		int io_num;		//GPIO����
		void (*Init)(struct _MyGpio *this); //  GPIO��ʼ�����ڴ���IO��������ִ�� 
		void (*FlashStart)(struct _MyGpio *this,int port,int freq,int times); //  ����GPIO��˸����ִ�� 
		void (*FlashStop)(struct _MyGpio *this,int port); // GPIOֹͣ��˸ 
		void (*FlashTimer)(struct _MyGpio *this); //  GPIO��˸ִ�к������ڵ����Ķ�ʱ�߳���ִ�� 
		void (*SetValue)(struct _MyGpio *this,int port,int Value); // GPIO�������ֵ��������ִ�� 
		void (*SetValueNow)(struct _MyGpio *this,int port,int Value); // GPIO�������ֵ��������ִ��
		int (*IsActive)(struct _MyGpio *this,int port); //  ��ȡ����IOֵ�����ж��Ƿ�IO��Ч 
		void (*Handle)(struct _MyGpio *this); //  GPIO���ִ�к��� 
		void (*Destroy)(struct _MyGpio *this);
	}MyGpio;

	typedef struct _MyGpioPriv {
		const char      portid;
		const int       portmask;
		const char      *active;		//��Чֵ
		const int 	  default_value;	//Ĭ��ֵ
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
