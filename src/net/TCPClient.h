#ifndef TCPClientH
#define TCPClientH
//---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
    #define FALSE 0
    #define TRUE 1
    #define BOOL int
#endif

typedef struct _TTCPClient
{
#ifndef WIN32
	int m_socket;
#else
    unsigned int m_socket;
#endif
    void (* Destroy)(struct _TTCPClient *This);
    int (* Connect)(struct _TTCPClient *This,const char *IP,int port,int TimeOut);
    void (*DisConnect)(struct _TTCPClient *This);
	int (* RecvBuffer)(struct _TTCPClient *This,void *buf,int count,int TimeOut);
	int (* SendBuffer)(struct _TTCPClient *This,const void *buf,int count);
} TTCPClient;
//---------------------------------------------------------------------------

//  创建一个UDP客户端，Port为0则不绑定端口
TTCPClient* TTCPClient_Create(void);

#ifdef __cplusplus
}
#endif

#endif
