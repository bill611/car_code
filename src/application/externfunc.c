/*
 * =====================================================================================
 *
 *       Filename:  externfunc.c
 *
 *    Description:  外部函数
 *
 *        Version:  1.0
 *        Created:  2015-12-11 11:56:30
 *       Revision:  none
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
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include <strings.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/rtc.h>
#include <linux/fb.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <sys/statfs.h>
#include <sys/mman.h>

#include "externfunc.h"

/* ----------------------------------------------------------------*
 *                  extern variables declare
 *-----------------------------------------------------------------*/
extern TUDPClient *udpclient;
extern TUDPServer *UdpServer;

/* ----------------------------------------------------------------*
 *                  internal functions declare
 *-----------------------------------------------------------------*/

/* ----------------------------------------------------------------*
 *                        macro define
 *-----------------------------------------------------------------*/
#define	IOCTL_PD	0x1001
#define	IOCTL_PU	0x1002
#define	IOCTL_NM	0x1003

/* ----------------------------------------------------------------*
 *                      variables define
 *-----------------------------------------------------------------*/


//---------------------------------------------------------------------------
void WatchDogOpen(void)
{
#ifndef WATCHDOG_DEBUG
	if(Public.WatchDog_fd > 0) {
		return;
	}

	Public.WatchDog_fd = open("/dev/watchdog", O_WRONLY);
	if (Public.WatchDog_fd == -1) {
		perror("watchdog");
	} else {
		printf("Init WatchDog!!!!!!!!!!!!!!!!!!\n");
	}
#endif
}
void WatchDogFeed(void)
{
#ifndef WATCHDOG_DEBUG
	if(Public.WatchDog_fd <= 0) {
		return;
	}
	ioctl(Public.WatchDog_fd, WDIOC_KEEPALIVE);
#endif
}
//---------------------------------------------------------------------------
void WatchDogClose(void)
{
#ifndef WATCHDOG_DEBUG
	if(Public.WatchDog_fd <= 0) {
		return;
	}
	char * closestr="V";
	write(Public.WatchDog_fd,closestr,strlen(closestr));
	close(Public.WatchDog_fd);
	Public.WatchDog_fd = -2;
#endif
}

int GetLocalIP(char *net_name,char *IP,char *NetMask,char *MAC)
{
	struct ifreq ifr;
	int sock;
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return 0;
	}
#ifndef PC
	strcpy(ifr.ifr_name, net_name);
	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
		goto error;

	strcpy(IP,(char*)inet_ntoa(((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr));

	if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0)
		goto error;
	strcpy(NetMask,(char*)inet_ntoa(((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr));

	//获取MAC地址
	if (!(ioctl (sock, SIOCGIFHWADDR, &ifr))) {
		sprintf(MAC,"%02X:%02X:%02X:%02X:%02X:%02X",
			(unsigned char)ifr.ifr_hwaddr.sa_data[0],
			(unsigned char)ifr.ifr_hwaddr.sa_data[1],
			(unsigned char)ifr.ifr_hwaddr.sa_data[2],
			(unsigned char)ifr.ifr_hwaddr.sa_data[3],
			(unsigned char)ifr.ifr_hwaddr.sa_data[4],
			(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	}
   close(sock);
   return 1;
error:
	close(sock);
#endif
	return 0;
}


unsigned long long int GetMs(void)
{
	struct  timeval    tv;
    gettimeofday(&tv,NULL);
	return ((tv.tv_usec / 1000) + tv.tv_sec  * 1000 );

}


//启动或关闭屏保
//返回 TRUE=屏保模式  FALSE=退出屏保模式
BOOL screensaverStart(BOOL On)
{
	if(On) {// 点亮LCD，退出屏保模式
		Public.LCDLight = 1;
		Mygpio->SetValueNow(Mygpio,ENUM_GPIO_LCD,IO_ACTIVE);
		Public.ScrSaversTimer = 0;
		return FALSE;
	} else {
		Public.LCDLight = 0;
		Mygpio->SetValueNow(Mygpio,ENUM_GPIO_LCD,IO_INACTIVE);
		return TRUE;
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief recoverData 恢复数据
 *
 * @param file 文件名称
 * @param reboot 1需要重启 0不需要重启
 */
/* ---------------------------------------------------------------------------*/
void recoverData(char *file,int reboot)
{
	int size = strlen(file);
	char *backfile = (char *) malloc (sizeof(char) * size + 5);
	sprintf(backfile,"%s_bak",file);
	ExcuteCmd(1,"cp",backfile,file,NULL);
	sync();
	if (reboot) {
		// form_main->timerStart(IDC_TIMER_RECOVER);
	}
	free(backfile);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief backData   备份数据
 *
 * @param file
 */
/* ---------------------------------------------------------------------------*/
void backData(char *file)
{
	int size = strlen(file);
	char *backfile = (char *) malloc (sizeof(char) * size + 5);
	sprintf(backfile,"%s_bak",file);
	ExcuteCmd(1,"cp",file,backfile,NULL);
	sync();
	free(backfile);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief etcFileCheck 检查配置文件是否正确
 *
 * @param file 配置文件名称
 *
 * @returns 1成功 0失败
 */
/* ---------------------------------------------------------------------------*/
int etcFileCheck(char *file)
{
	int ret = TRUE;
	int hFile;
	int star_num,end_num;
	hFile = LoadEtcFile(file);

	if(hFile != ETC_FILENOTFOUND) {
		if(GetIntValueFromEtc(hFile,"START_CHECK","StartCheck",&star_num)!=ETC_OK) {
			ret = FALSE;
		}
		if(GetIntValueFromEtc(hFile,"END_CHECK","EndCheck",&end_num)!=ETC_OK) {
			ret = FALSE;
		}
		if ((star_num != INI_KEY_START) || (end_num != INI_KEY_END)) {
			ret = FALSE;
		}
		if (ret == FALSE) {
			recoverData(file,1);
		}
		UnloadEtcFile(hFile);
	} else {
		recoverData(file,1);
	}
	return ret;
}

//---------------------------------------------------------------------------
int ExcuteCmd(int bWait,char *Cmd,...)
{
	int i;
	FILE *fp;
	int ret;
	va_list argp;
	char *argv;
	char commond[512] = {0};
	strcat(commond,Cmd);
	va_start( argp, Cmd);
	for(i=1;i<20;i++) {
		argv = va_arg(argp,char *);
		if(argv == NULL)
			break;
		strcat(commond," ");
		strcat(commond,argv);
	}
	va_end(argp);
	printf("cmd :%s\n",commond);
#if 1
	if ((fp = popen(commond, "r") ) == 0) {
		perror("popen");
		return -1;
	}
	if ( (ret = pclose(fp)) == -1 ) {
		printf("close popen file pointer fp error!\n");
		// printf("popen ret is :%d\n", ret);
	}
	return ret;
#else

	// 用此函数导致产生僵尸进程
    system(commond);
    return 0;
#endif
}
//---------------------------------------------------------------------------
//判断本地IP与目标IP是否同一个网段,如果不在同一个网段将返回最小的广播域
//SrcIP 本地IP地址
//DestIP 目标IP地址
//Mask 本机子网掩码
//返回最小的广播域IP地址
//---------------------------------------------------------------------------
//unsigned long inet_addr(const char* cp);
const char * GetSendIP(const char *pSrcIP,const char *pDestIP,const char *pMask)
{
	//取广播域
	static char cIP[16];
	unsigned int SrcIP,DestIP,Mask;
	unsigned char *pIP = (unsigned char *)&DestIP;
	//转换字符形IP到整形IP地址
	SrcIP = inet_addr(pSrcIP);
	DestIP = inet_addr(pDestIP);
	Mask = inet_addr(pMask);
	if((SrcIP & Mask)!=(DestIP & Mask)) {
		DestIP = (SrcIP & Mask) | (~Mask & 0xFFFFFFFF);//DestIP = 0xFFFFFFFF;
	}
	//转换成字符形IP地址
	sprintf(cIP,"%d.%d.%d.%d",pIP[0],pIP[1],pIP[2],pIP[3]);
	return cIP;
}


/**
 * @brief net_detect 检测网线连接状态
 *
 * @param net_name 网卡名称
 *
 * @return 0正常 -1不正常
 */
int net_detect(void)
{
#ifdef PC
    return 0;
#endif
	int ret = access("ip_ok",0);
    if (ret == 0)
        return 0;
    else
        return -1;
#if 0
	int skfd = 0;
	struct ifreq ifr;
	struct sockaddr_in *pAddr = NULL;
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd < 0) {
		printf("%s:%d Open socket error!\n", __FILE__, __LINE__);
		return -1;

	}
	strcpy(ifr.ifr_name, net_name);
	if(ioctl(skfd, SIOCGIFFLAGS, &ifr) <0 ) {
		close(skfd);
		printf("%s:%d IOCTL error!\n", __FILE__, __LINE__);
		printf("Maybe ethernet inferface %s is not valid!", ifr.ifr_name);
		return -1;

	}
	close(skfd);
	if(ifr.ifr_flags & IFF_RUNNING) {
        printf("%s is running :)\n", ifr.ifr_name);
		return 0;
	} else {
        printf("%s is not running :(\n", ifr.ifr_name);
		return -1;
	}
#endif
}

void restartNetwork(void)
{
	ExcuteCmd(1,"./network.sh",">","debug.txt","&",NULL);
    Public.net_connect_times = 20;
}

/* ----------------------------------------------------------------*/
/**
 * @brief ClearFramebuffer 清除fb0
 */
/* ----------------------------------------------------------------*/
void ClearFramebuffer(void)
{
	int fd = -1;
	unsigned int VpostWidth=0, VpostHeight=0,VpostBpp=0;
	unsigned int g_fb_vaddress=0;
	unsigned int g_u32VpostBufMapSize=0;
	struct	fb_var_screeninfo g_fb_var;
	fd = open("/dev/fb0", O_RDWR);
	if (fd < 0) {
		perror("/dev/fb0");
		return;
	}
	if (ioctl(fd, FBIOGET_VSCREENINFO, &g_fb_var) < 0) {
		perror("/dev/fb0");
		close(fd);
		return;
	}
	VpostWidth = g_fb_var.xres;
	VpostHeight = g_fb_var.yres;
	VpostBpp = g_fb_var.bits_per_pixel/8;

	g_u32VpostBufMapSize = VpostWidth*VpostHeight*VpostBpp*2;

	g_fb_vaddress = (unsigned int)mmap( NULL, g_u32VpostBufMapSize,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fd,
			0);

	memset ((void*)g_fb_vaddress, 0x0, g_u32VpostBufMapSize );

	if (g_fb_vaddress)
		munmap((void *)g_fb_vaddress, g_u32VpostBufMapSize);

	close(fd);

}
