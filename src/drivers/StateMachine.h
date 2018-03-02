#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


#define MAX_MSG 	5
#define TSK_BUSY    0x80
#define TSK_READY   0x01

#ifdef PC
#define ENTER_CRITICAL(x) 
#define EXIT_CRITICAL(x)  
#else
#define ENTER_CRITICAL(x) local_irq_save(x);
#define EXIT_CRITICAL(x)  local_irq_restore(x);
#endif

typedef struct _StateTable {
	int msg;			//
	int curstate;		//
	int nextstate;		//
	int run;			//
}StateTable;

typedef struct _MsgData {
	int msg;			//
	void *data;
}MsgData;

typedef struct _TCBStruct{
    int       status;
    int       msgcnt;
    int       msgcur;
    MsgData   msgque[MAX_MSG];
} TCBStruct;

typedef struct _State_machine {
	int	id;					//
	int G_curstate;			//
	int G_run;				//
	void *data;
	TCBStruct TCB;			//
	StateTable *funcentry;	//
	int table_num;			//
//	int Mutex_single;		//

	void  *(*initPara)(struct _State_machine *this,int size);
	int  (*getCurState)(struct _State_machine *this);
	void  (*MsgPost)(struct _State_machine *this,int msg,void *data);
	void  (*Handle)(struct _State_machine *this,void *data);
	void  (*Run)(struct _State_machine *this);
	void  (*Destroy)(struct _State_machine **this);
}State_machine;

State_machine* stateMachineCreate(int InitState, 
		void *state_table, 
		int num,
		int id,
		void (*Handle)(State_machine *this,void *data));
#endif
