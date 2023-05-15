/****************************************/
/*           rtos_api.h                 */
/****************************************/
#pragma once
#include <boost/coroutine2/all.hpp>

/* TASK */
#define DeclareTask(TaskID, priority)\
	_TASK(TaskID);					\
	boost::coroutines2::coroutine<void>::pull_type C_##TaskID (_##TaskID); \
	boost::coroutines2::coroutine<void>::pull_type* TaskID = &(C_##TaskID); \
enum {TaskID##prior=priority}

#define _TASK(TaskID) void _##TaskID(boost::coroutines2::coroutine<void>::push_type& yield);

#define TASK(TaskID) \
	void _##TaskID(boost::coroutines2::coroutine<void>::push_type& yield)


typedef boost::coroutines2::coroutine<void>::pull_type* TTaskCall;

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

void GetResource(int priority, char* name);
void ReleaseResource(int priority, char* name);


/* System event */
typedef unsigned int TEventMask;
#define DeclareSysEvent(Id) TEventMask Event_##Id = (Id)*(Id);

void SetSysEvent(TEventMask mask);
void GetSysEvent(TEventMask* mask);
void WaitSysEvent(TEventMask mask);
