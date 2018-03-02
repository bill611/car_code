#ifndef UARTH
#define UARTH

#include <sys/msg.h>
#define MAX_STRARG_LEN 128
typedef struct _COMM{
	int  baudrate;
	char databit;        	   //data bits,5,6,7,8
	char parity;		   //parity 0:none,1:odd,2:even
	char stopbit;		   //stop bits,1,2
}portinfo_t;
typedef portinfo_t *pportinfo_t;

typedef struct {
	int	leng;
    char   send_data[MAX_STRARG_LEN];
} Command;

typedef struct {
	long int message_type ;
	Command cmd;
} MsgType;

typedef  struct _UARTSERVER{

	int hCom;
	int ComNum;
	HWND hWnd;
	BOOL Terminated;
	portinfo_t comparam;
	pthread_mutex_t mutex;		//���п��ƻ����ź�
	int msg_id;

	int (*Open)(struct _UARTSERVER *This,int port,int baudrate);
	int (*Set)(struct _UARTSERVER *This);
	int (*Send)(struct _UARTSERVER *This,void *Buf,int datalen);
	//���ջ������ڵ�����
	int (*Recv)(struct _UARTSERVER *This,char *Buf,DWORD BufSize);
	//��������,ֱ��5MS��û�����ݷ���
	int (*RecvBuffer)(struct _UARTSERVER *This,void *Buf,DWORD BufSize);
	int (*WaitFor)(struct _UARTSERVER *This,unsigned int Ms);
	void (*Clear)(struct _UARTSERVER *This);
	void (*Close)(struct _UARTSERVER *This);
	void (*Destroy)(struct _UARTSERVER *This);

}UartServer,*PUartServer;

extern UartServer *Create_UartServer(HWND hWnd);
// extern PUartServer uart;
#endif
