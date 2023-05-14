/****************************************/
/*           rtos_api.h                 */
/****************************************/
#pragma once

/* TASK */
#define DeclareTask(TaskID,priority)\
	TASK(TaskID);					\
enum {TaskID##prior=priority}

#define TASK(TaskID) void TaskID(void)

typedef void TTaskCall(void);

void ActivateTask(TTaskCall entry,int priority,char* name);
void TerminateTask(void);


/* OS */
int StartOS(TTaskCall entry,int priority,char* name);
void ShutdownOS();


/* Resource */
#define DeclareResource(ResID,priority) enum {ResID=priority}

void GetResource(int priority, char* name);
void ReleaseResource(int priority, char* name);


/* System event */
typedef unsigned int TEventMask;
#define DeclareSysEvent(Id) TEventMask Event_##Id = (Id)*(Id);

void SetSysEvent(TEventMask mask);
void GetSysEvent(TEventMask* mask);
void WaitSysEvent(TEventMask mask);