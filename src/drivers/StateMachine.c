/*
 * =============================================================================
 *
 *       Filename:  StateMachine.c
 *
 *    Description:  状态机
 *
 *        Version:  1.0
 *        Created:  2016-03-09 11:22:34
 *       Revision:  1.0
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifndef PC
#include <alsa/iatomic.h>
#endif

#include "externfunc.h"


/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_MACHINE > 0
	#define DBG_P( x... ) printf( x )

static char *debug_msg[] = {
	"EVENT_SHAKEHANDS",
	"EVENT_SHAKEHANDS_ASW",
	"EVENT_TYPE_CENTER",
	"EVENT_TYPE_HM",
	"EVENT_DIAL",        	
	"EVENT_CALL",        	
	"EVENT_TALK",        	
	"EVENT_MONITOR",    	
	"EVENT_FAIL_COMM",    	
	"EVENT_FAIL_SHAKE",    		
	"EVENT_FAIL_BUSY",    	
	"EVENT_FAIL_ABORT",    	
	"EVENT_HANDS_UP",    	
};

static char *debug_state[] = {
	"ST_IDLE",		
	"ST_SHAKEHANDS",
	"ST_SHAKEHANDS_ASW",
	"ST_JUDGE_TYPE",
	"ST_DIAL",	
	"ST_CALL",
	"ST_TALK",
	"ST_MONITOR",
};

static char *debug_do[] = {
	"DO_NO",
	"DO_SHAKEHANDS",
	"DO_SHAKEHANDS_ASW",
	"DO_JUDGE_TYPE",
	"DO_DIAL",
	"DO_CALL",
	"DO_TALK",
	"DO_MONITOR",
	"DO_FAIL_COMM",
	"DO_FAIL_SHAKE",
	"DO_FAIL_BUSY",
	"DO_FAIL_ABORT",
	"DO_RET_FAIL",    	
	"DO_HANDS_UP",
};
#else
	#define DBG_P( x... )
#endif

/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/


/* ---------------------------------------------------------------------------*/
/**
 * @brief stmMsgPost 状态机发送事件消息
 *
 * @param this
 * @param msg 消息
 * @param data 附加参数
 */
/* ---------------------------------------------------------------------------*/
static void stmMsgPost(State_machine* this,int msg,void *data)
{
    int pos;
	unsigned long volatile flags;
    if (this->TCB.msgcnt >= MAX_MSG)
		return;

	ENTER_CRITICAL(flags);

	pos = this->TCB.msgcur + this->TCB.msgcnt;
	if (pos >= MAX_MSG) {
		pos -= MAX_MSG;
	}
	this->TCB.msgque[pos].msg = msg;
	this->TCB.msgque[pos].data = data;
	this->TCB.status |= TSK_READY;
	this->TCB.msgcnt++;

	EXIT_CRITICAL(flags);
}


/* ---------------------------------------------------------------------------*/
/**
 * @brief stmMsgRead 状态机读取消息队列
 *
 * @param this
 *
 * @returns 消息队列地址
 */
/* ---------------------------------------------------------------------------*/
static MsgData *stmMsgRead(State_machine *this)
{
    int pos;
	unsigned long volatile flags;

    if ((this->TCB.status & TSK_BUSY) == TSK_BUSY)
		return NULL;
    if (this->TCB.msgcnt == 0)
		return NULL;
	ENTER_CRITICAL(flags);

    pos = this->TCB.msgcur;
    if (++this->TCB.msgcur >= MAX_MSG) {
        this->TCB.msgcur = 0;
    }
    this->TCB.msgcnt--;

	EXIT_CRITICAL(flags);
    return &this->TCB.msgque[pos];
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief stmExecEntry 状态机执行状态判断和转换
 *
 * @param this
 * @param msg 消息
 *
 * @returns 1成功 0失败
 */
/* ---------------------------------------------------------------------------*/
static int stmExecEntry(State_machine *this,MsgData * msg)
{
	int num = this->table_num;
	StateTable *funcentry = this->funcentry;

    for (; num > 0; num--,funcentry++) {
		if (		(msg->msg == funcentry->msg) 
				&& 	(this->G_curstate == funcentry->curstate)) {
			DBG_P("[ST]msg:%s,cur:%s,next:%s,do:%s\n",
					debug_msg[msg->msg],
					debug_state[funcentry->curstate],
					debug_state[funcentry->nextstate],
					debug_do[funcentry->run]);
			this->G_run = funcentry->run ;
			this->data = msg->data;
			this->G_curstate = funcentry->nextstate;
			this->TCB.status |= TSK_BUSY;
			return 1;
		}
	}
    return 0;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief stmSchedule 状态机处理
 *
 * @param this
 */
/* ---------------------------------------------------------------------------*/
static void stmSchedule(State_machine *this)
{
    MsgData * msg;
	unsigned long volatile flags;

    if ((this->TCB.status & TSK_BUSY) == TSK_BUSY)
		return ;

	if ((this->TCB.status & TSK_READY) == TSK_READY) {
//		ENTER_CRITICAL(flags);
		msg = stmMsgRead(this);

		if (msg >= 0) {
			stmExecEntry(this,msg);
		}
		if (this->TCB.msgcnt == 0) {
			this->TCB.status &= (~TSK_READY);
		}
//		EXIT_CRITICAL(flags);
	}

}

/* ---------------------------------------------------------------------------*/
/**
 * @brief smtRunMachine 运行状态机 在线程里执行
 *
 * @param this
 */
/* ---------------------------------------------------------------------------*/
static void smtRunMachine(State_machine* this)
{
	stmSchedule(this);
	this->Handle(this,this->data);

	if (this->data) {
		free(this->data);
		this->data = NULL;
	}
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief stmInitPara 初始化发送状态机消息时带的参数
 *
 * @param this
 * @param size 参数大小
 *
 * @returns 
 */
/* ---------------------------------------------------------------------------*/
static void *stmInitPara(State_machine *this,int size)
{
	void *para = NULL;
	if (size) {
		para = (void *) calloc (1,size);
	}
	return para;
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief stmGetCurState 获取状态机当前状态
 *
 * @param this
 *
 * @returns 
 */
/* ---------------------------------------------------------------------------*/
static int stmGetCurState(State_machine *this)
{
	return this->G_curstate;	
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief stmDestroy 销毁状态机
 *
 * @param this
 */
/* ---------------------------------------------------------------------------*/
static void stmDestroy(State_machine **this)
{
	free(*this);
	*this = NULL;	
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief stateMachineCreate 创建状态机
 *
 * @param InitState 初始状态
 * @param state_table 状态机表
 * @param num 状态机表的数量
 * @param id 状态机的ID，区别多个状态机同时运行
 * @param Handle 状态机处理，
 *
 * @returns 
 */
/* ---------------------------------------------------------------------------*/
State_machine* stateMachineCreate(int InitState, 
		void *state_table, 
		int num,
		int id,
		void (*Handle)(State_machine *this,void *data))
{

	State_machine *this = (State_machine *)calloc(1,sizeof(State_machine));
	this->funcentry = (StateTable *)state_table;
	this->table_num = num;
	this->G_curstate = InitState;
	this->id = id;
//	this->Mutex_single = 0;

	this->MsgPost = stmMsgPost;
	this->Handle = Handle;
	this->Run = smtRunMachine;
	this->initPara = stmInitPara;
	this->getCurState = stmGetCurState;
	this->Destroy = stmDestroy;
	return this;
}

