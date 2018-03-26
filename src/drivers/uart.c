#include <stdio.h>          //printf
#include <fcntl.h>          //open
#include <string.h>         //bzero
#include <stdlib.h>         //exit
#include <sys/times.h>	//times
#include <sys/types.h>	//pid_t
#include <termios.h>	//termios.tcgetattr(),tcsetattr
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>	//ioctl

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "externfunc.h"

#define MAX_COMMAND_QUEUE_SIZE      8
#define TTY_DEV "/dev/ttyS"
// extern UartServer *uart;
static void uartReceiveCreate(PUartServer This);
static void uartSendCreate(PUartServer This);
/**********************************
 * *
 ***********************************/
static int convbaud(unsigned long int baudrate)
{
	switch(baudrate)
	{
		case 2400 :  return B2400;
		case 4800 :  return B4800;
		case 9600 :  return B9600;
		case 19200:  return B19200;
		case 38400:  return B38400;
		case 57600:  return B57600;
		case 115200: return B115200;
		default:      return B9600;
	}
}
/**********************************
 * Setup comm attr
 * fdcon????????????,pportinfo:
 ***********************************/
static int PortSet(UartServer *This)
	//int PortSet(int This->hCom,const pportinfo_t pportinfo)
{
	struct termios termios_old,termios_new;
	int    baudrate,tmp,temp;
	char   databit,stopbit,parity,fctl;
	temp = This->hCom;
	bzero(&termios_old,sizeof(termios_old));
	bzero(&termios_new,sizeof(termios_new));
	cfmakeraw(&termios_new);
	tcgetattr(temp,&termios_old);

	baudrate = convbaud(This->comparam.baudrate);
	cfsetispeed(&termios_new,baudrate);
	cfsetospeed(&termios_new,baudrate);
	termios_new.c_cflag |= CLOCAL;
	termios_new.c_cflag |= CREAD;
	termios_new.c_iflag |= IXON|IXOFF|IXANY;
	termios_new.c_cflag&=~CSIZE;
	databit = This->comparam.databit;
	switch(databit){
		case '5' :
			termios_new.c_cflag |= CS5;
		case '6' :
			termios_new.c_cflag |= CS6;
		case '7' :
			termios_new.c_cflag |= CS7;
		default:
			termios_new.c_cflag |= CS8;

	}
	parity = This->comparam.parity;
	switch(parity)
	{
		default:
		case '0' :
			termios_new.c_cflag &= ~PARENB;
			break;
		case '1' :
			termios_new.c_cflag |= PARENB;
			termios_new.c_cflag &=~PARODD;
			break;
		case '2' :
			termios_new.c_cflag |= PARENB;
			termios_new.c_cflag |= PARODD;
			break;
	}

	stopbit =This->comparam.stopbit;
	if(stopbit == '2')
	{
		termios_new.c_cflag |= CSTOPB;                //2 stop bits
	}
	else{
		termios_new.c_cflag &=~CSTOPB;             //1 stop bits
	}
	termios_new.c_lflag &=~(ICANON|ECHO|ECHOE|ISIG);
	termios_new.c_oflag &= OPOST;                   //
	termios_new.c_cc[VMIN]  = 1;                    //
	termios_new.c_cc[VTIME] = 0;		      //
	termios_new.c_lflag &= (ICANON|ECHO|ECHOE|ISIG);

	tcflush(temp,TCIFLUSH);                              //
	tmp = tcsetattr(temp,TCSANOW,&termios_new);           //
	tcgetattr(temp,&termios_old);
	return(tmp);

}
/************************************************
 *  Open serial port
 *  tty:ttyS0,ttyS1,
 *
 ************************************************/
static int PortOpen(UartServer *This,int com,int baudrate)
{
	char *ptty;

	switch(com){
		case 0:
			ptty = TTY_DEV"0";
			break;
		case 1:
			ptty = TTY_DEV"1";
			break;
		case 2:
			ptty = TTY_DEV"2";
			break;
		default:
			ptty = TTY_DEV"3";
			break;
	}
	This->ComNum = com;
	if(baudrate>=1200)
		This->comparam.baudrate = baudrate;
#ifndef PC
	This->hCom = open(ptty,O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
	This->Set(This);
#else
	This->hCom = 1;
#endif
	if(This->hWnd) {
		uartReceiveCreate(This);
		uartSendCreate(This);
	}
	return This->hCom>0;
}
/************************************************
 *  send data
 *  fdcom: ????????????,data:????????????,datalen:
 *
 ************************************************/
#if 0
static void printfbuf(void *pbuf,int size)
{
	int i;
	unsigned char *pData = (unsigned char *)pbuf;
	saveLog("SendData[%d]  ",size);
	for(i=0;i<size;i++) {
		saveLog("%02x ",pData[i]);
	}
	saveLog("\n");
}
#else
#define printfbuf(pbuf,size)
#endif

static int uartSend(UartServer *This,void *Buf,int datalen)
{
	MsgType msg;
	msg.message_type = 1;
	msg.cmd.leng = datalen;
	memset(msg.cmd.send_data,0,sizeof(msg.cmd.send_data));
	memcpy(msg.cmd.send_data,Buf,msg.cmd.leng);
	msgsnd(This->msg_id, &msg, sizeof (MsgType), 0);
	// printfbuf(msg.cmd.send_data,msg.cmd.leng);
	return 1;
}
static int PortSend(UartServer *This,void *Buf,int datalen)
{
	int len = 0;
	if(This->hCom<=0)
		return -1;
	pthread_mutex_lock (&This->mutex);		//加锁
	len = write(This->hCom,Buf,datalen);   //
	printfbuf(Buf,datalen);
	if(len == datalen) {
		pthread_mutex_unlock (&This->mutex);		//解锁
		usleep(5000);
		return(len);
	} else{
		tcflush(This->hCom,TCOFLUSH);
		usleep(5000);
		pthread_mutex_unlock (&This->mutex);		//解锁
		return -1;
	}
}
/************************************************
 *  receive data
 ************************************************/

static int PortRecv(UartServer *This,char *Buf,DWORD datalen)
{
	if(This->hCom<=0)
		return 0;
	return read(This->hCom,Buf,datalen);
}
//---------------------------------------------------------------------------
static int Uart_RecvBuffer(UartServer *This,void *pBuf,DWORD Size)
{
	int LeaveSize = Size;
	// char *pChar = (char*)pBuf;
	while(LeaveSize) {
		int RecvSize = PortRecv(This,&((char*)pBuf)[Size-LeaveSize],LeaveSize);
		if(RecvSize<=0)
			break;
		LeaveSize -= RecvSize;
		usleep(20000);
	}
	return Size-LeaveSize;
}
//---------------------------------------------------------------------------
static void Uart_Clear(UartServer *This)
{
	int result;
	char cBuf[128];
	do {
		result = This->Recv(This,cBuf,sizeof(cBuf));
	} while(result>0);
}
/***************************************
 *    close serial port
 ***************************************/
static void PortClose(UartServer *This)
{
	if(This->hCom) {
		close(This->hCom);
		This->hCom = 0;
		This->Terminated = TRUE;
	}
}
//---------------------------------------------------------------------------
static int UartWaitFor(UartServer *This,unsigned int Ms)
{
	if(This->hCom) {
		int fs_sel;
		fd_set fs_read;
		struct timeval tv_timeout;

		FD_ZERO(&fs_read);
		FD_SET(This->hCom,&fs_read);
		tv_timeout.tv_sec = Ms/1000;
		tv_timeout.tv_usec = (Ms%1000)*1000;
		fs_sel = select(This->hCom+1,&fs_read,NULL,NULL,&tv_timeout);
		if(fs_sel>0 && FD_ISSET(This->hCom,&fs_read))
			return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------
static void * uartReceiveThead(UartServer *This)
{
	int fs_sel;
	fd_set fs_read;
	struct timeval tv_timeout;

	while(!This->Terminated){
		FD_ZERO(&fs_read);
		FD_SET(This->hCom,&fs_read);
		tv_timeout.tv_sec = 3;
		tv_timeout.tv_usec = 0;
		fs_sel = select(This->hCom+1,&fs_read,NULL,NULL,&tv_timeout);
		if(fs_sel>0 && FD_ISSET(This->hCom,&fs_read)) {
			//			printf("%#x Have Data\n",This->hCom);
			SendMessage(This->hWnd,MSG_SERIALPORT,This->ComNum,0);
		}
	}
	pthread_exit(NULL);
}

static void * uartSendThead(UartServer *This)
{
	MsgType msg;
	while(!This->Terminated){
		msgrcv(This->msg_id, &msg, sizeof(MsgType), 0, 0);
		PortSend(This,msg.cmd.send_data,msg.cmd.leng);
		usleep(50000);
	}
	pthread_exit(NULL);
}

static void uartReceiveCreate(PUartServer This)
{
	int ret;
	pthread_t id;
	pthread_attr_t  attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&id,&attr,(void *)uartReceiveThead,This);
	if(ret) {
		printf("uartReceiveCreate() pthread failt,Error code:%d\n",ret);
	}
	pthread_attr_destroy(&attr);
}

static void uartSendCreate(PUartServer This)
{
	int ret;
	pthread_t id;
	pthread_attr_t  attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&id,&attr,(void *)uartSendThead,This);
	if(ret) {
		printf("uartReceiveCreate() pthread failt,Error code:%d\n",ret);
	}
	pthread_attr_destroy(&attr);
}

static void Uart_Destroy(PUartServer This)
{
	This->Close(This);
	pthread_mutex_destroy (&This->mutex);
	free(This);
}

UartServer *Create_UartServer(HWND hWnd)
{
	pthread_mutexattr_t mutexattr;
	PUartServer This = (PUartServer)calloc(sizeof(UartServer),1);
	if(This == NULL)
		return NULL;
	pthread_mutexattr_init(&mutexattr);
	/* Set the mutex as a recursive mutex */
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);

	/* create the mutex with the attributes set */
	pthread_mutex_init(&This->mutex, &mutexattr);
	/* destroy the attribute */
	pthread_mutexattr_destroy(&mutexattr);

	This->msg_id = msgget((key_t)1234, 0666  | IPC_CREAT);
	if (This->msg_id == -1)
		perror("msg_id");

	This->comparam.baudrate = 9600;
	This->comparam.databit = 8;
	This->comparam.parity = 0;
	This->comparam.stopbit = 1;
	This->hWnd = hWnd;
	This->Open = PortOpen;
	This->Close= PortClose;
	This->Send = uartSend;
	This->Recv = PortRecv;
	This->RecvBuffer = Uart_RecvBuffer;
	This->WaitFor = UartWaitFor;
	This->Clear = Uart_Clear;
	This->Set = PortSet;
	This->Destroy = Uart_Destroy;

	return This;
}
