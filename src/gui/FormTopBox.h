/*
 * =============================================================================
 *
 *       Filename:  FormTopBox.h
 *
 *    Description:  提示框
 *
 *        Version:  1.0
 *        Created:  2018-03-15 23:58:22 
 *       Revision:  
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
#ifndef _FORM_TOP_BOX_H
#define _FORM_TOP_BOX_H
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


    enum {
        TOPBOX_BED_LIE,
        TOPBOX_BED_UNLIE,
        TOPBOX_TABLE_FOLD,
        TOPBOX_TABLE_UNFOLD,
        TOPBOX_LEG_FOLD,
        TOPBOX_LEG_UNFOLD,
        TOPBOX_CHAIR_FOLD,
        TOPBOX_CHAIR_UNFOLD,
        TOPBOX_WIFI_CONNECTING,
        TOPBOX_WIFI_FAILED,
    };

    int topMessage(HWND hMainWnd,int type,void (*notif_proc)(void) );

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
