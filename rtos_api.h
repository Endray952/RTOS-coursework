/****************************************/
/*           rtos_api.h                 */
/****************************************/
#pragma once
#include <boost/coroutine2/all.hpp>

/* TASK */
#define DeclareTask(TaskID, priority)\
	TASK(TaskID);					\
enum {TaskID##prior=priority}

#define TASK(TaskID) \
	void TaskID(boost::coroutines2::coroutine<void>::pull_type& yield)


typedef boost::coroutines2::coroutine<void>::push_type* TTaskCallCoroutine;
typedef void TTaskCall(boost::coroutines2::coroutine<void>::pull_type&);

#define ActivateTask(entry, priority, name) \
	_ActivateTask(entry, priority, name); \
	yield();

#define TerminateTask() \
	_TerminateTask(); \
	yield();

void _ActivateTask(TTaskCall entry, int priority, char* name);
void _TerminateTask(void);


/* OS */
int StartOS(TTaskCall entry, int priority, char* name);
void ShutdownOS();


/* Resource */
#define DeclareResource(ResID, priority) enum {ResID=priority}

#define GetResource(priority, name) \
	_GetResource(priority, name); \
	yield();

#define ReleaseResource(priority, name) \
	_ReleaseResource(priority, name); \
	yield();

void _GetResource(int priority, char* name);
void _ReleaseResource(int priority, char* name);


/* System event */
typedef unsigned int TEventMask;
#define DeclareSysEvent(Id) TEventMask Event_##Id = (Id)*(Id);

#define WaitSysEvent(mask) \
	_WaitSysEvent(mask); \
	yield();

void SetSysEvent(TEventMask mask);
void GetSysEvent(TEventMask* mask);
void _WaitSysEvent(TEventMask mask);
