/*
 * =====================================================================================
 *
 *       Filename:  MyGpioCtr.c
 *
 *    Description:  GPIO控制
 *
 *        Version:  1.0
 *        Created:  2015-12-24 09:06:46
 *       Revision:  1.0
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =====================================================================================
 */
/* ----------------------------------------------------------------*
 *                      include head files
 *-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "externfunc.h"


/* ----------------------------------------------------------------*
 *                  extern variables declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                  internal functions declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                        macro define
 *-----------------------------------------------------------------*/
#define GPIO_GROUP_A 'a'
#define GPIO_GROUP_B 'b'
#define GPIO_GROUP_C 'c'
#define GPIO_GROUP_D 'd'
#define GPIO_GROUP_E 'e'
#define GPIO_GROUP_G 'g'
#define GPIO_GROUP_H 'h'


#define ENUM_GPIO_LCD		GPIO_GROUP_D,3

/* ----------------------------------------------------------------*
 *                      variables define
 *-----------------------------------------------------------------*/
MyGpio *Mygpio = NULL;

MyGpioPriv gpiotbl[]={
	{ENUM_GPIO_LCD,		"1",IO_ACTIVE,0},
};

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioFlashStop GPIO停止闪烁
 *
 * @param this
 * @param port IO口
 */
/* ----------------------------------------------------------------*/
static void myGpioFlashStop(MyGpio *this,int port)
{
	MyGpioPriv *Priv;
	Priv = this->Priv;
	if ((Priv+port)->default_value == IO_NO_EXIST) {
		return;
	}

	(Priv+port)->flash_delay_time = 0;
	(Priv+port)->flash_set_time = FLASH_STOP;
	(Priv+port)->flash_times = 0;
	(Priv+port)->flash_even_flag = 0;
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioSetValue GPIO口输出赋值，不立即执行
 *
 * @param this
 * @param port IO口
 * @param Value 有效IO_ACTIVE or 无效IO_INACTIVE
 */
/* ----------------------------------------------------------------*/
static void myGpioSetValue(MyGpio *this,int port,int  Value)
{
	MyGpioPriv *Priv;
	Priv = this->Priv;
	if (	((Priv+port)->default_value == IO_INPUT)
		||  ((Priv+port)->default_value == IO_NO_EXIST) ) {   //输出
		printf("[%d]set value fail,it is input or not exist!\n",port);
		return;
	}

	if (Value == IO_ACTIVE) {
		(Priv+port)->current_value = *((Priv+port)->active) - '0';
	} else {
		(Priv+port)->current_value = !(*((Priv+port)->active) - '0');
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioSetValueNow GPIO口输出赋值，并立即执行
 *
 * @param this
 * @param port IO口
 * @param Value 有效IO_ACTIVE or 无效IO_INACTIVE
 */
/* ----------------------------------------------------------------*/
static void myGpioSetValueNow(MyGpio *this,int port,int  Value)
{
	FILE *fp;
	char string_buf[50],buffer[10];
	MyGpioPriv *Priv;
	Priv = this->Priv;
	if (	((Priv+port)->default_value == IO_INPUT)
		||  ((Priv+port)->default_value == IO_NO_EXIST) ) {   //输出
		printf("[%d]set value fail,it is input!\n",port);
		return;
	}

	if (Value == IO_ACTIVE) {
		(Priv+port)->current_value = *((Priv+port)->active) - '0';
	} else {
		(Priv+port)->current_value = !(*((Priv+port)->active) - '0');
	}

#ifdef PC
    return;
#endif
	sprintf(string_buf,"/sys/class/gpio/gpio%d/value",(Priv+port)->portnum);
	if ((fp = fopen(string_buf, "rb+")) == NULL) {
		printf("SetValueNow[%d][%c%d]Cannot open value file.\n",
				port,(Priv+port)->portid,(Priv+port)->portmask);
		// exit(1);
	} else {
		// printf("SetValueNow[%c%d]\n",(Priv+port)->portid,(Priv+port)->portmask);
		sprintf(buffer,"%d",(Priv+port)->current_value);
		fwrite(buffer, sizeof(char), sizeof(buffer) - 1, fp);
		fclose(fp);
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioRead 读输入口的值
 *
 * @param this
 * @param port IO口
 *
 * @returns 真正的值 0或1
 */
/* ----------------------------------------------------------------*/
static int myGpioRead(MyGpio *this,int port)
{
	FILE *fp;
	char string_buf[50];
	char buffer[10];
	MyGpioPriv *Priv;
	Priv = this->Priv;
	if ((Priv+port)->default_value == IO_INPUT) {
		sprintf(string_buf,"/sys/class/gpio/gpio%d/value",(Priv+port)->portnum);
		if ((fp = fopen(string_buf, "rb")) == NULL) {
			printf("Read[%d][%c%d]Cannot open value file.\n",
					port,Priv->portid,Priv->portmask);
			// exit(1);
		} else {
			fread(buffer, sizeof(char), sizeof(buffer) - 1, fp);
			(Priv+port)->current_value = atoi(buffer);
			fclose(fp);
		}
	}
	return (Priv+port)->current_value;
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioIsActive 读取输入IO值，并判断是否IO有效
 *
 * @param this
 * @param port IO口
 *
 * @returns 有效IO_ACTIVE or 无效IO_INACTIVE
 */
/* ----------------------------------------------------------------*/
static int myGpioIsActive(MyGpio *this,int port)
{
	char value[2];
	MyGpioPriv *Priv;
	Priv = this->Priv;
	if ((Priv+port)->default_value == IO_NO_EXIST) {
		return IO_NO_EXIST;
	}
	sprintf(value,"%d",myGpioRead(this,port));
	if (strcmp(value,(Priv+port)->active) == 0){
		return IO_ACTIVE;
	} else {
		return IO_INACTIVE;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioFlashTimer GPIO闪烁执行函数，在单独的定时线程中执行
 * IO口电平变化到还原算一次
 *
 * @param this
 */
/* ----------------------------------------------------------------*/
static void myGpioFlashTimer(MyGpio *this)
{
	int i;
	MyGpioPriv *Priv;
	Priv = this->Priv;
	for (i=0; i<this->io_num; i++) {
		if (Priv->default_value == IO_NO_EXIST) {
			Priv++;
			continue;
		}
		if (Priv->default_value == IO_INPUT) {
			Priv++;
			continue;
		}
		if ((Priv->flash_delay_time == 0) || (Priv->flash_times == 0)) {
			Priv++;
			continue;
		}
		if (--Priv->flash_delay_time == 0) {
			Priv->flash_even_flag++;
			if (Priv->flash_even_flag == 2) {  //亮灭算一次
				Priv->flash_times--;
				Priv->flash_even_flag = 0;
			}

			Priv->flash_delay_time = Priv->flash_set_time;

			if (myGpioIsActive(this,i) == IO_ACTIVE)
				myGpioSetValue(this,i,IO_INACTIVE);
			else
				myGpioSetValue(this,i,IO_ACTIVE);
		}
		Priv++;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioFlashStart 设置GPIO闪烁，并执行
 *
 * @param this
 * @param port IO口
 * @param freq 频率 根据myGpioFlashTimer 执行时间决定
 * @param times 闪烁总次数 FLASH_FOREVER为循环闪烁
 */
/* ----------------------------------------------------------------*/
static void myGpioFlashStart(MyGpio *this,int port,int freq,int times)
{
	MyGpioPriv *Priv;
	Priv = this->Priv;
	if ((Priv+port)->default_value == IO_NO_EXIST) {
		return;
	}
	if ((Priv+port)->flash_set_time != freq) {
		(Priv+port)->flash_delay_time = freq;
		(Priv+port)->flash_set_time = freq;
		(Priv+port)->flash_times = times;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioInit GPIO初始化，在创建IO对象后必须执行
 *
 * @param this
 */
/* ----------------------------------------------------------------*/
static void myGpioInit(MyGpio *this)
{
	FILE *fp;
	int i;
	char string_buf[50];
	MyGpioPriv *Priv;
#ifdef PC
    return;
#endif
	Priv = this->Priv;
	for (i=0; i<this->io_num; i++) {
		if (Priv->default_value == IO_NO_EXIST) {
			Priv++;
			continue;
		}
		if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL) {
			printf("Init:/sys/class/gpio/export fopen failed.\n");
			Priv++;
			continue;
			// exit(1);
		}
		// printf("[%s](%d,df_value:%d)\n",__FUNCTION__,i,Priv->default_value);
		Priv->portnum = Priv->portmask;
		switch (Priv->portid)
		{
			case GPIO_GROUP_A : break;
			case GPIO_GROUP_B : Priv->portnum += 32;   break;
			case GPIO_GROUP_C : Priv->portnum += 32*2; break;
			case GPIO_GROUP_D : Priv->portnum += 32*3; break;
			case GPIO_GROUP_E : Priv->portnum += 32*4; break;
			case GPIO_GROUP_G : Priv->portnum += 32*5; break;
			case GPIO_GROUP_H : Priv->portnum += 32*6; break;
			default : printf("GPIO should be:a-h\n");break;
		}
		fprintf(fp,"%d",Priv->portnum);
		fclose(fp);

		sprintf(string_buf,"/sys/class/gpio/gpio%d/direction",Priv->portnum);

		if ((fp = fopen(string_buf, "rb+")) == NULL) {
			printf("Init:%s,fopen failed.\n",string_buf);
			Priv++;
			continue;
			// exit(1);
		}
		if (Priv->default_value != IO_INPUT) {
			fprintf(fp,"out");
		} else {
			fprintf(fp,"in");
		}
		fclose(fp);
		if (Priv->default_value != IO_INPUT)//设置默认值
			myGpioSetValueNow(this,i,Priv->default_value);

		Priv->flash_delay_time = 0;
		Priv->flash_set_time = FLASH_STOP;
		Priv->flash_times = 0;
		Priv->flash_even_flag = 0;

		Priv++;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioDestroy 销毁GPIO对象
 *
 * @param this
 */
/* ----------------------------------------------------------------*/
static void myGpioDestroy(MyGpio *this)
{
	free(this->Priv);
	free(this);
	this = NULL;
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioHandle GPIO输出执行函数，真正对IO口赋值，在单独线程
 * 中执行，IO输出以脉冲形式，防止非正常情况导致IO口电平一时错误
 *
 * @param this
 */
/* ----------------------------------------------------------------*/
static void myGpioHandle(MyGpio *this)
{
	FILE *fp;
	int i;
	char string_buf[50],buffer[10];
	MyGpioPriv *Priv;
	Priv = this->Priv;
	for (i=0; i<this->io_num; i++) {
		if (Priv->default_value == IO_NO_EXIST) {
			Priv++;
			continue;
		}
		if (Priv->default_value == IO_INPUT) {
			Priv++;
			continue;
		}

#ifdef PC
        Priv++;
        continue;
#endif
		sprintf(string_buf,"/sys/class/gpio/gpio%d/value",Priv->portnum);
		if ((fp = fopen(string_buf, "rb+")) == NULL) {
			printf("Handle GPIO[%d][%c%d]Cannot open value file.\n",
					i,Priv->portid,Priv->portmask);
			// exit(1);
		} else {
			sprintf(buffer,"%d",Priv->current_value);
			fwrite(buffer, sizeof(char), sizeof(buffer) - 1, fp);
			fclose(fp);
		}
		Priv++;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief myGpioPrivCreate 创建GPIO对象
 *
 * @param gpio_table GPIO列表
 *
 * @returns GPIO对象
 */
/* ----------------------------------------------------------------*/
MyGpio* myGpioPrivCreate(MyGpioPriv *gpio_table)
{
	MyGpio *this = (MyGpio *)malloc(sizeof(MyGpio));
	memset(this,0,sizeof(MyGpio));
    if (gpio_table == gpiotbl) {
		this->io_num = sizeof(gpiotbl) / sizeof(MyGpioPriv);
		// printf("gpio_tbl:%d\n",this->io_num);
	}
	this->Priv = (MyGpioPriv *)malloc(sizeof(MyGpioPriv) * this->io_num);
	memset(this->Priv,0,sizeof(MyGpioPriv) * this->io_num);

	this->Priv = gpio_table;
	this->Init = myGpioInit;
	this->SetValue = myGpioSetValue;
	this->SetValueNow = myGpioSetValueNow;
	this->FlashStart = myGpioFlashStart;
	this->FlashStop = myGpioFlashStop;
	this->FlashTimer = myGpioFlashTimer;
	this->Destroy = myGpioDestroy;
	this->Handle = myGpioHandle;
	this->IsActive = myGpioIsActive;
	return this;
}

