/*
 * =============================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  调试端口
 *
 *        Version:  1.0
 *        Created:  2016-08-10 18:03:27
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

// #include "Public.h"

#define DBG_SAVE_LOG 	0
#define DBG_UART_DEAL 	0
#define DBG_SOCKETREAD 	0
#define DBG_HTTP_REC 	0
#define DBG_HTTP_SEND 	0

#define Log(fmt,arg...)   \
	do { \
		FILE *log_fd; \
		log_fd = fopen("log.txt","a+"); \
		fprintf(log_fd,fmt,##arg); \
		fflush(log_fd); \
		fclose(log_fd); \
	} while(0)

/*
#define Log(fmt,arg...)   \
	do { \
		//char RecDate[50]; \
		FILE *log_fd; \
		//Public.GetDate(RecDate,sizeof(RecDate)); \
		log_fd = fopen("log.txt","a+"); \
		//fprintf(log_fd,"[%s]",RecDate); \
		fprintf(log_fd,fmt,##arg); \
		fflush(log_fd); \
		fclose(log_fd); \
	} while(0)
*/
#if DBG_SAVE_LOG > 0
#define saveLog(fmt,arg...)  Log(fmt,##arg)
#else
#ifdef PC
#define saveLog(fmt,arg...) fprintf(stderr,fmt,##arg)
#else
#define saveLog(fmt,arg...) printf(fmt,##arg)
#endif
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
