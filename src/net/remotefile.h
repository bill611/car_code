#ifndef RemoteFileH
#define RemoteFileH

#ifndef FALSE
    #define FALSE 0
    #define TRUE 1
    #define BOOL int
#endif


enum {UPDATE_FAIL,UPDATE_SUCCESS,UPDATE_POSITION};
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
enum {OPEN_READ=1,OPEN_WRITE,OPEN_READWRITE,FILE_CREATE_ALWAYS=0x100,
	FILE_CREATE_NEW=0x200,FILE_OPEN_ALWAYS=0x300,FILE_OPEN_EXISTING=0x400,
	FILE_OPEN_MASK=0xFF00};
//---------------------------------------------------------------------------
struct RemoteFilePrivate;        //私有数据

typedef struct _TRemoteFile
{
    struct RemoteFilePrivate * Private;
	BOOL (*Download)(struct _TRemoteFile* This,unsigned int hWnd,const char *SrcFile,
		const char *DstFile,int ExecuteMode);												//下载文件
	int (*Open)(struct _TRemoteFile* This,const char *FileName,int OpenMode);				//返回文件长度,-1打开失败
	int (*Read)(struct _TRemoteFile* This,void *Buffer,int Size);
	BOOL (*Write)(struct _TRemoteFile* This,void *Buffer,int Size);
	int (*Seek)(struct _TRemoteFile* This,int offset,int origin);
	BOOL (*SetEnd)(struct _TRemoteFile* This);
	int (*GetPos)(struct _TRemoteFile* This);
	void (*Close)(struct _TRemoteFile* This);
	void (*Destroy)(struct _TRemoteFile* This);
} TRemoteFile;

TRemoteFile * CreateRemoteFile(const char *IP,const char *TempFile);
//----------------------------------------------------------------------------
#endif

