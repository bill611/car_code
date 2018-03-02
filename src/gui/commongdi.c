#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "externfunc.h"

//#include "iwgetmsg.h"

//extern PConfigWIFI pWifi;
//---------------------------------------------------------------------------
void LoadAdverImage(HWND hWnd,int ID,BITMAP *Image,char *AdverType)
{
//	static int ImageIdx=0;
//	HWND hCtrl;
//	char FileName[64];
//	ImageIdx++;
//	sprintf(FileName,"./images/adver/%s%03d.bmp",AdverType,ImageIdx);
//	if(!fileexists(FileName))
//		ImageIdx = 1;
//	sprintf(FileName,"./images/adver/%s%03d.bmp",AdverType,ImageIdx);
//	UnloadBitmap (Image);
//	LoadBitmap (HDC_SCREEN, Image, FileName);
//	hCtrl = GetDlgItem(hWnd,ID);
//	SendMessage(hCtrl,STM_SETIMAGE,(DWORD)Image,0);
}
//---------------------------------------------------------------------------
//绘制一个白色的底图
//---------------------------------------------------------------------------
void DrawWhiteFrame (HWND hWnd, HDC hdc, const RECT* pClipRect,int Left,int Top,int Width,int Height)
{
//	BOOL fGetDC = FALSE;
//	RECT rcTemp;
//	RECT rcClient;

//	GetClientRect(hWnd,&rcClient);

//	if (hdc == 0) {
//		hdc = GetClientDC (hWnd);
//		fGetDC = TRUE;
//	}
//	if (pClipRect) {
//		rcTemp = *pClipRect;
//		ScreenToClient (hWnd, &rcTemp.left, &rcTemp.top);
//		ScreenToClient (hWnd, &rcTemp.right, &rcTemp.bottom);
//		IncludeClipRect(hdc,&rcTemp);
//	}
//	Draw3DControlFrame(hdc,Left,Top,Width,Height,COLOR_lightwhite,0);
//	if (fGetDC)
//		ReleaseDC (hdc);
}
//---------------------------------------------------------------------------
void DrawBackground(HWND hWnd, HDC hdc, const RECT* pClipRect,BITMAP *Image)
{
    BOOL fGetDC = FALSE;
    RECT rcTemp;
	RECT rcClient;

	GetClientRect(hWnd,&rcClient);

    if (hdc == 0) {
        hdc = GetClientDC (hWnd);
        fGetDC = TRUE;
    }
//    if (pClipRect)
//        ClipRectIntersect (hdc, pClipRect);
    if (pClipRect) {
        rcTemp = *pClipRect;
        ScreenToClient (hWnd, &rcTemp.left, &rcTemp.top);
        ScreenToClient (hWnd, &rcTemp.right, &rcTemp.bottom);
		IncludeClipRect(hdc,&rcTemp);
    }
	FillBoxWithBitmap(hdc,rcClient.left,rcClient.top,RECTW(rcClient),RECTH(rcClient),Image);
    if (fGetDC)
        ReleaseDC (hdc);
}
//----------------------------------------------------------------------------
void wndEraseBackground(HWND hWnd,HDC hdc, const RECT* pClipRect,BITMAP *pImage,int Left,int Top,int Width,int Height)
{
	BOOL fGetDC = FALSE;
	RECT rcTemp;

	if (hdc == 0) {
		hdc = GetClientDC (hWnd);
		fGetDC = TRUE;
	}
	if (pClipRect) {
		rcTemp = *pClipRect;
		ScreenToClient (hWnd, &rcTemp.left, &rcTemp.top);
		ScreenToClient (hWnd, &rcTemp.right, &rcTemp.bottom);
		IncludeClipRect(hdc,&rcTemp);
	}
	FillBoxWithBitmap(hdc,Left,Top,Width,Height,pImage);
	if (fGetDC)
		ReleaseDC (hdc);
}

/* ----------------------------------------------------------------*/
/**
 * @brief line_a2b 绘制直线
 *
 * @param hdc
 * @param a 起点
 * @param b 终点
 * @param color 颜色
 */
/* ----------------------------------------------------------------*/
void line_a2b (HDC hdc, POINT* a, POINT* b, int color)
{
	SetPenColor (hdc, color);
	MoveTo (hdc, a->x, a->y);
	LineTo (hdc, b->x, b->y);
}

/* ----------------------------------------------------------------*/
/**
 * @brief draw_rectangle 绘制矩形 左上点(x0,y0) 右下点(x1,y1)
 *
 * @param hdc
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color 颜色
 */
/* ----------------------------------------------------------------*/
void draw_rectangle (HDC hdc,  int x0, int y0, int x1, int y1, int color)
{
	SetPenColor (hdc, color);
	MoveTo (hdc, x0, y0);
	LineTo (hdc, x1, y0);

	MoveTo (hdc, x1, y0);
	LineTo (hdc, x1,y1);

	MoveTo (hdc, x1,y1);
	LineTo (hdc, x0,y1);

	MoveTo (hdc, x0,y1);
	LineTo (hdc, x0, y0);
}

/* ----------------------------------------------------------------*/
/**
 * @brief draw_Triangle
 * 填充三角形
 * @param hdc
 * @param rc
 * @param color
 * @param type 尖头方向 0:up 1:down 2:right 3:left
 */
/* ----------------------------------------------------------------*/
void draw_Triangle (HDC hdc, RECT rc, int color, int type)
{
	SetPenColor (hdc, color);
	POINT* pts = (POINT*)malloc(sizeof(POINT) * 3);
    switch (type)
	{
		case 0:
			{
				pts->x = rc.left + RECTW(rc) / 2;
				pts->y = rc.top;

				(pts + 1)->x = rc.left;
				(pts + 1)->y = rc.bottom;

				(pts + 2)->x = rc.right;
				(pts + 2)->y = rc.bottom;
			} break;

		case 1:
			{
				pts->x = rc.left + RECTW(rc) / 2;
				pts->y = rc.bottom;

				(pts + 1)->x = rc.left;
				(pts + 1)->y = rc.top;

				(pts + 2)->x = rc.right;
				(pts + 2)->y = rc.top;

			} break;

		case 2:
			{
				pts->x = rc.right;
				pts->y = rc.top + RECTH(rc) / 2;

				(pts + 1)->x = rc.left;
				(pts + 1)->y = rc.top;

				(pts + 2)->x = rc.left;
				(pts + 2)->y = rc.bottom;

			} break;

		case 3:
			{
				pts->x = rc.left;
				pts->y = rc.top + RECTH(rc) / 2;

				(pts + 1)->x = rc.right;
				(pts + 1)->y = rc.top;

				(pts + 2)->x = rc.right;
				(pts + 2)->y = rc.bottom;

			} break;
	}
	FillPolygon(hdc,pts,3);
	free(pts);
}
/* ----------------------------------------------------------------*/
/**
 * @brief myFillBox 填充矩形区域颜色
 *
 * @param hdc
 * @param rc 矩形区域
 * @param color 颜色
 */
/* ----------------------------------------------------------------*/
void myFillBox(HDC hdc, RECT rc, int color)
{
	SetBrushColor (hdc,color);
	FillBox (hdc, rc.left, rc.top, RECTW(rc),RECTH(rc));
}
//----------------------------------------------------------------------------
//  显示一帧视频,VideoBuf为decode的缓冲区
//  缓冲区格式为DC_WIDTHXDC_HEIGHTX16Bit的RGB像素集
//----------------------------------------------------------------------------
#ifndef TLINK_SIP

void DrawFrame(HWND hWnd,char *VideoBuf)
{
	// int y;
    // int width, height, pitch;				//绘制的宽度，高度和每行的字节数
    // RECT rc = {0, 0, 352, 288};				//DC_WIDTHXDC_HEIGHT
    // HDC hdc = GetDC(hWnd);		//在主界面直接绘制
    // int bpp = GetGDCapability (hdc, GDCAP_BPP);
    // Uint8* frame_buffer = LockDC (hdc, &rc, &width, &height, &pitch);	//帧缓冲区地址,锁定
    // Uint8* row = frame_buffer;				//行缓冲区地址
// //	VideoBuf += DC_WIDTH*(DC_HEIGHT-1)*DC_PIXEL;
    // for (y = 0; y < height; y++) {
        // FastCopy (row, VideoBuf, width * bpp);		//高速拷屏
        // row += pitch;								//下一行
		// VideoBuf += 352*2;				//decode缓冲区每行字节数为DC_HEIGHT*2=576Byte
    // }
    // UnlockDC (hdc);
    // ReleaseDC(hdc);
}
#endif
//----------------------------------------------------------------------------
// 保存呼入的操作
// IP呼入的主机，Date呼叫日期，Answer是否应答
//----------------------------------------------------------------------------
void SaveCallInRec(const char *IP,const char *pCallID,const char *pCallName,
				const char *Date,BOOL Answer)
{
//	int i;
//	char oldname[64],newname[64];
//	char SqlStr[512];
//	char Host[16],RoomName[32];
//	char cTmp;

//	strcpy(Host,IP);
//	strcpy(RoomName,IP);

//	sprintf(SqlStr,"Select LocalID,Name From Indoor_Inf where IP='%s'",IP);
//	if(TempQuery->Connect(TempQuery)) {
//		QueryOpen(TempQuery,SqlStr);
//		if(TempQuery->RecordCount(TempQuery)) {
//			QueryOfChar(TempQuery,"LocalID",Host,sizeof(Host));
//			QueryOfChar(TempQuery,"Name",RoomName,sizeof(RoomName));
//		}
//		TempQuery->Close(TempQuery);
//		TempQuery->DisConnect(TempQuery,FALSE);
//	}

//	sprintf(SqlStr,"Update CallRecord Set sequence=sequence+1 where toid='%s' "
//		"And state=%d",Public.cRoomID,Answer);
//	LocalQueryExec(LocalQuery,SqlStr);
//	sprintf(SqlStr,"Delete From CallRecord where toid='%s' And state=%d And "
//		"sequence>20",Public.cRoomID,Answer);
//	LocalQueryExec(LocalQuery,SqlStr);

//	sprintf(SqlStr,"Insert INTO CallRecord (sequence,calltime,fromid,fromname,"
//		"toid,toname,type,state) Values (1,'%s','%s','%s','%s','%s',0,%d)",
//		Date,Host,RoomName,Public.cRoomID,Public.cRoomName,Answer);
//	LocalQueryExec(LocalQuery,SqlStr);

//	//更改图片名
//	if(Answer) cTmp='b';	//已接听
//	else cTmp='a';			//未接听
//	sprintf(newname,"capture/image%c20.pic",cTmp);
//	remove(newname);
//	for(i=20;i>1;i--) {
//		sprintf(oldname,"capture/image%c%02d.pic",cTmp,i-1);
//		sprintf(newname,"capture/image%c%02d.pic",cTmp,i);
//		rename(oldname,newname);
//	}
//	sprintf(newname,"capture/image%c01.pic",cTmp);
//	printf("answer:%d,name:%s\n",Answer,newname);
//	rename("capture/temp.pic",newname);
}
//----------------------------------------------------------------------------
// 保存呼出的操作
// IP呼叫的主机，Date呼叫日期，Answer是否应答
//----------------------------------------------------------------------------
void SaveCallOutRec(const char *IP,const char *pCallID,const char *pCallName,
						const char *Date,BOOL Answer)
{
	char SqlStr[512];
	char Host[16],RoomName[32];

//	strcpy(Host,IP);
//	strcpy(RoomName,IP);

//	sprintf(SqlStr,"Select LocalID,Name From Indoor_Inf where IP='%s'",IP);
//	if(TempQuery->Connect(TempQuery)) {
//		QueryOpen(TempQuery,SqlStr);
//		if(TempQuery->RecordCount(TempQuery)) {
//			QueryOfChar(TempQuery,"LocalID",Host,sizeof(Host));
//			QueryOfChar(TempQuery,"Name",RoomName,sizeof(RoomName));
//		}
//		TempQuery->Close(TempQuery);
//		sprintf(SqlStr,"Insert INTO CallRecord (sequence,calltime,fromid,fromname,toid,toname, "
//			"state,checked) Values (1,'%s','%s','%s','%s','%s',%d,0)",
//			Date,Public.cRoomID,Public.cRoomName,Host,RoomName,Answer);
//		QueryExec(TempQuery,SqlStr);
//		TempQuery->DisConnect(TempQuery,FALSE);
//	}

//	sprintf(SqlStr,"Update CallRecord Set sequence=sequence+1 where fromid='%s'",
//		Public.cRoomID);
//	LocalQueryExec(LocalQuery,SqlStr);
//	sprintf(SqlStr,"Delete From CallRecord where fromid='%s' And sequence>20",
//		Public.cRoomID);
//	LocalQueryExec(LocalQuery,SqlStr);

//	sprintf(SqlStr,"Insert INTO CallRecord (sequence,calltime,fromid,fromname,toid,toname, "
//		"state,checked) Values (1,'%s','%s','%s','%s','%s',%d,0)",
//		Date,Public.cRoomID,Public.cRoomName,Host,RoomName,Answer);
//	LocalQueryExec(LocalQuery,SqlStr);
}
//----------------------------------------------------------------------------
// 输入密码,成功则撤防
//----------------------------------------------------------------------------
BOOL ClearAlarmForm(HWND hWnd)
{
//	char cPwd[16];
//	if(CreateGetPwdForm(hWnd,"请输入撤防密码!",cPwd)==IDOK) {
//		if(strcmp(cPwd,Public.cSecurePassword)==0) {
//			//撤防
//			ElecCtrl->SetAlarmState(ElecCtrl,hWnd,0);
//			return TRUE;
//		} else if(strcmp(cPwd,"0000")==0) {			//受胁撤防
//			//
//		}
//		else {
//			MyMessageBox(hWnd,"密码错误!","提示",MB_OK|MB_ICONINFORMATION,10000);
//		}
//	}
//	return FALSE;
}
//----------------------------------------------------------------------------
void SetAlarmForm(HWND hWnd)
{
//	if(MyMessageBox(hWnd,"您确认要现在布防吗?","提示",MB_OKCANCEL|MB_ICONQUESTION,10000)==IDOK)
//	{
//		ElecCtrl->SetAlarmState(ElecCtrl,hWnd,1);		//布防
//		ShowMessage(hWnd,"已布防!");
//	}
}
//----------------------------------------------------------------------------
//前20位为电话号码,后XX位为信息内容,最多140字节
//---------------------------------------------------------------------------
int SendSMS(char *pPhone,char *Msg)
{
//	unsigned int t1,t2;
//	char cBuf[512];
//	memset(cBuf,0,sizeof(cBuf));
//	t1 = strlen(Msg);
//	if(t1>140)
//		return 0;
//	t2 = strlen(pPhone);
//	if(t2>18)
//		return 0;
//	strcpy(cBuf,pPhone);
//	strcpy(&cBuf[20],Msg);
//	//前20位为电话号码,后XX位为信息内容
//	return SendUdpCmd(Public.cDataSrvIP,SQL_UDP_PORT,TP_GPRS,cBuf,20+1+t1);
}
//---------------------------------------------------------------------------
//BOOL QueryOfBitmap(TMyQuery *Query,char *FieldName,BITMAP *img,const char *ext)
//{
//	int ret;
//	char *pBinary = (char*)malloc(102400);
//	ret = QueryOfBlobData(Query,FieldName,pBinary,102400);
//	if(ret>0) {
//        UnloadBitmap (img);
//		LoadBitmapFromMem(HDC_SCREEN,img,pBinary,ret,ext);
//	}
//	free(pBinary);
//	return ret>0;
//}
//----------------------------------------------------------------------------
// 从字符串中取ID号(取XXXX部分) 字符串格式为 XXXXX(xxxx)
int ExtraceID(const char *pStr,char *ID,int size)
{
	int ol=0;
	const char *ib;
	char *ob;
//	if(pStr==NULL || ID==NULL)
//		return -1;
//	ib = pStr;
//    ob = ID;
//	while(*ib) {
//        if('('==*ib) {
//            if(ol) {
//                *ob = 0;
//                return ol;
//            } else
//                return -2;
//        } else {
//            if(ol<size) {
//                *ob++ = *ib++;
//                ol++;
//            } else
//                return -3;
//        }
//	}
//    *ob = 0;
//    return ol;
}
//---------------------------------------------------------------------------
void DrawXpFrame(HWND hWnd,HDC hdc, const RECT* pClipRect,const char *Caption,int left,int top,int right,int bottom)
{
	int l = left;
	int t = top;
	int r = right;
	int b = bottom;
//	unsigned int Color;

    BOOL fGetDC = FALSE;
    RECT rcTemp;

    if (hdc == 0) {
        hdc = GetClientDC (hWnd);
        fGetDC = TRUE;
    }
    if (pClipRect) {
        rcTemp = *pClipRect;
        ScreenToClient (hWnd, &rcTemp.left, &rcTemp.top);
        ScreenToClient (hWnd, &rcTemp.right, &rcTemp.bottom);
		IncludeClipRect(hdc,&rcTemp);
    }

    if (hdc == 0) {
        hdc = GetClientDC (hWnd);
        fGetDC = TRUE;
    }

//	Color = SetBrushColor(hdc,COLOR_lightwhite);
	FillBox(hdc,l,t,r-l,b-t);

	SetPenColor (hdc, 0x02DD);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;
	SetPenColor (hdc, 0x3CBF);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;
	SetPenColor (hdc, 0x2C9F);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;
	SetPenColor (hdc, 0x039F);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x033E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;
	SetPenColor (hdc, 0x02FD);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;
	SetPenColor (hdc, 0x02DC);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	//填充
	SetBrushColor(hdc,0x02BC);
	FillBox(hdc,l,t,r-l,8);
	t+=8;

	SetPenColor (hdc, 0x02BD);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x02DD);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x02DE);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x031E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x033E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x035E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x035E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x037E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x035E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x031E);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x027C);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);
	t++;

	SetPenColor (hdc, 0x023A);
	MoveTo (hdc, l, t);
	LineTo (hdc, r, t);

	if(Caption) {
		SetTextColor(hdc,COLOR_yellow);
		SetBkMode (hdc, BM_TRANSPARENT);
		TextOut(hdc,left+5,top+4,Caption);
	}

	//绘制两侧
	SetPenColor (hdc, 0x0179);
	MoveTo (hdc, left, top);
	LineTo (hdc, left, bottom);
	MoveTo (hdc, left+1, top);
	LineTo (hdc, left+1, bottom);

	MoveTo (hdc, right, top);
	LineTo (hdc, right, bottom);
	MoveTo (hdc, right-1, top);
	LineTo (hdc, right-1, bottom);

	//绘制底边
	SetBrushColor(hdc,0x02BC);
	FillBox (hdc,left,bottom-24,
		right-left,24);

	if (fGetDC)
        ReleaseDC (hdc);
}
//---------------------------------------------------------------------------
void SendToUart(unsigned char Cmd,unsigned char Arg1,unsigned char Arg2)
{
//	char cBuf[12];
//	cBuf[0] = UART_HEAD;
//	cBuf[1] = Cmd;
//	cBuf[2] = Arg1;
//	cBuf[3] = Arg2;
//	cBuf[4] = UART_HEAD+Cmd+Arg1+Arg2;
//	uart->Send(uart,cBuf,5);
}


//---------------------------------------------------------------------------
#ifdef WIN32
#pragma   pack(push,   1)
typedef struct BITMAPFILEHEADER
{
	unsigned short bfType;
	unsigned long  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long  bfOffBits;
} BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER  /* size: 40 */
{
	unsigned long  biSize;
	unsigned long  biWidth;
	unsigned long  biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	unsigned long  biXPelsPerMeter;
	unsigned long  biYPelsPerMeter;
	unsigned long  biClrUsed;
	unsigned long  biClrImportant;
} BITMAPINFOHEADER;
#pragma   pack(pop)
#else
typedef struct BITMAPFILEHEADER
{
	unsigned short bfType;
	unsigned long  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long  bfOffBits;
} __attribute__ ((packed)) BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER  /* size: 40 */
{
	unsigned long  biSize;
	unsigned long  biWidth;
	unsigned long  biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	unsigned long  biXPelsPerMeter;
	unsigned long  biYPelsPerMeter;
	unsigned long  biClrUsed;
	unsigned long  biClrImportant;
} __attribute__ ((packed)) BITMAPINFOHEADER;

#endif

//---------------------------------------------------------------------------
void GetPartFromBmp(const BITMAP *bmp,BITMAP *DstBitmap,int Left,int Top,int Width,int Height)
{
	int i,j;
	int DstLineCnt;
	int LineCnt;
	char *pBmpBuf;
	BITMAPFILEHEADER *head;
	BITMAPINFOHEADER *info;
	char *Pix;	//目标像素
	char *pSrc;	//源文件像素

	//目标每行缓冲区大小
	DstLineCnt = Width*3;
	if(DstLineCnt%4)
		DstLineCnt += 4-DstLineCnt%4;
	//分配空间
	pBmpBuf = (char*)malloc(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+DstLineCnt*Height);
	head = (BITMAPFILEHEADER *)pBmpBuf;
	info = (BITMAPINFOHEADER *)&pBmpBuf[sizeof(BITMAPFILEHEADER)];

	head->bfType = 0x4d42;
	head->bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+DstLineCnt*Height;
	head->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	head->bfReserved1 = 0;
	head->bfReserved2 = 0;

	info->biSize = sizeof(BITMAPINFOHEADER);
	info->biWidth = Width;
	info->biHeight = Height;
	info->biPlanes = 1;
	info->biBitCount = 24;
	info->biCompression = 0;
	info->biSizeImage = 0;
	info->biXPelsPerMeter = 0;
	info->biYPelsPerMeter = 0;
	info->biClrUsed = 0;
	info->biClrImportant = 0;
	//一行数据多少
	LineCnt = bmp->bmWidth*2;
	if(LineCnt%4)
		LineCnt += 4-LineCnt%4;
	//首数据
	pSrc = &((char*)bmp->bmBits)[Top*LineCnt+Left*3];
	//最后一行
	Pix = &pBmpBuf[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)]+(Height-1)*DstLineCnt;
	for(i=0;i<Height;i++) {
		char *pRGB = Pix;
		unsigned short *pRGB2 = (unsigned short *)pSrc;
		for(j=0;j<Width;j++) {
			Pixel2RGB(HDC_SCREEN,*pRGB2,&pRGB[2],&pRGB[1],&pRGB[0]);
			pRGB+=3;
			pRGB2++;
		}
		pSrc+=LineCnt;
		Pix-=DstLineCnt;
	}
	LoadBitmapFromMem(HDC_SCREEN,DstBitmap,pBmpBuf,head->bfSize,"png");
	free(pBmpBuf);
}

/* ----------------------------------------------------------------*/
/**
 * @brief Operation1 设置透明按钮
 *
 * @param bmWidth
 * @param bmHeight
 * @param FgLine
 * @param BkLine
 * @param MkLine
 * @param FgPitch
 * @param BkPitch
 * @param MkPitch
 */
/* ----------------------------------------------------------------*/
static void Operation1(DWORD bmWidth,DWORD bmHeight,char *FgLine,char *BkLine,char *MkLine,
		int FgPitch,int BkPitch,int MkPitch)
{
	DWORD i,j;
	WORD *BkBits,*FgBits,*MkBits;
	for(i = 0;i<bmHeight;i++) {
		FgBits = (WORD*)FgLine;
		BkBits = (WORD*)BkLine;
		MkBits = (WORD*)MkLine;
		for(j=0;j<bmWidth;j++) {
#if 0       //简单透明运算，速度快
			*FgBits = ((*BkBits)&(~(*MkBits))) | ((*FgBits) & (*MkBits));
#else       //支持透明度透明运算，速度较慢
			unsigned int dwTmp,A;
			A = (*MkBits>>5) & 0x3F;
			// R = (Bk * (64-A) + Fg * A) / 64;
			dwTmp = ((*BkBits & 0x1F)*(64-A) + (*FgBits & 0x1F)*A)>>6;      //R
			// R = ((Bk * (64-A) + Fg * A) / 64) << 5;
			dwTmp |= ((((*BkBits>>5) & 0x3F)*(64-A) + ((*FgBits>>5) & 0x3F)*A)>>1) & 0x7E0;     //G
			dwTmp |= ((((*BkBits>>11) & 0x1F)*(64-A) + ((*FgBits>>11) & 0x1F)*A)<<5) & 0xF800;      //B
			*BkBits = dwTmp;
#endif
			FgBits++;
			BkBits++;
			MkBits++;
		}
		FgLine += FgPitch;
		BkLine += BkPitch;
		MkLine += MkPitch;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief TranslateIconPart 绘制透明图标
 *
 * @param hdc
 * @param x
 * @param y
 * @param w
 * @param h
 * @param FgBmp
 * @param LineIconCnt
 * @param IconIdx 绘制行中第几元素
 * @param t
 * @param Translate
 */
/* ----------------------------------------------------------------*/
void TranslateIconPart(HDC hdc,int x,int y,int w,int h,BITMAP *FgBmp,int LineIconCnt,int IconIdx,int t,
		BOOL Translate)
{
	if((LineIconCnt==4 || LineIconCnt==8) && Translate) {
		BITMAP BkBmp;
		char *BkLine,*FgLine,*MkLine;
		memset(&BkBmp,0,sizeof(BITMAP));
		GetBitmapFromDC(hdc,x,y,w,h,&BkBmp);
		FgLine = FgBmp->bmBits+t*FgBmp->bmPitch+w*IconIdx*2;
		BkLine = BkBmp.bmBits;
		if(LineIconCnt==4) {
			MkLine = FgLine+w*2*2;
		} else {
			MkLine = FgLine+w*4*2;
		}
		// Operation1(w,h,FgLine,BkLine,MkLine,FgBmp->bmPitch,
				// BkBmp.bmPitch,FgBmp->bmPitch);
		FillBoxWithBitmap(hdc,x,y,w,h,&BkBmp);
		UnloadBitmap(&BkBmp);
	} else {
		if(LineIconCnt>IconIdx) {
			FillBoxWithBitmapPart (hdc, x, y, w, h, 0, 0, FgBmp,w*IconIdx, t);
		} else {
			FillBoxWithBitmapPart (hdc, x, y, w, h, 0, 0, FgBmp,w*(LineIconCnt-1), t);
		}
	}
}

//---------------------------------------------------------------------------
int PostMainWindowMessage(unsigned int Msg,unsigned int wParam,unsigned int lParam)
{
	return SendNotifyMessage(Screen.hMainWnd,Msg,wParam,lParam);
}

