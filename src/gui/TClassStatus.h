#ifndef TClassStatusH
#define TClassStatusH

#define MSG_HIDEKEYBOARD (MSG_USER+101)
#define MSG_SHOWKEYBOARD (MSG_USER+102)

typedef int (* CALLBACKPROC) (HWND hwnd, int id, int nc, DWORD add_data);

struct _TClassStatusPriv;
typedef struct _TClassStatus{
	struct _TClassStatusPriv *priv;
	void (*Destroy)(struct _TClassStatus * This);
	void (*EnKeyBoard)(struct _TClassStatus * This);
	void (*DisKeyBoard)(struct _TClassStatus * This);
	void (*Show)(struct _TClassStatus * This);
	void (*Hide)(struct _TClassStatus * This);
	void (*ChangeBackgrd)(struct _TClassStatus * This,const char *pImageFile);
	void (*ShowSaveButton)(struct _TClassStatus * This,BOOL bShow);
	CALLBACKPROC (*SetReturnFunc)(struct _TClassStatus * This,HWND hWnd,CALLBACKPROC Proc);
	CALLBACKPROC (*SetMainFormFunc)(struct _TClassStatus * This,HWND hWnd,CALLBACKPROC Proc);
	CALLBACKPROC (*SetSaveFunc)(struct _TClassStatus * This,HWND hWnd,CALLBACKPROC Proc);
}TClassStatus;

extern TClassStatus *ClassStatus;
TClassStatus *CreateClassStatus(HWND hWnd);

#endif
