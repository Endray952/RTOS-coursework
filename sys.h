/****************************************/
/*               sys.h                   /
/****************************************/
#pragma once

#include "defs.h"
#include "rtos_api.h"

/* System Event */
enum TTaskState {
	TASK_RUNNING,
	TASK_READY,
	TASK_WAITING
};

extern TEventMask WorkingEvents; // сработавшие событи€ (не мой комментарий)


#define INSERT_TO_TAIL 1
#define INSERT_TO_HEAD 0

typedef struct Type_Task
{
	int next;
	int priority;
	int ceiling_priority;
	TTaskCall entry;
	char* name;
	TTaskState task_state;
	TEventMask waiting_events;
} TTask;

typedef struct Type_resource
{
	/*индекс таски, захватившей ресурс*/
	int task;
	/*ссылка на следующий элемент в очереди ресурсов ResourceQueue*/
	int priority;
	/*им€ ресурс*/
	char* name;

} TResource;

extern TTask TaskQueue[MAX_TASK];
extern int TaskCount;

extern TResource ResourceQueue[MAX_RES];

/*“аска, котора€ выполн€етс€ сейчас*/
extern int RunningTaskRef;

/* ”казатель на голову списка свободных элементов
массива TaskQueue (индекс массива TaskQueue)*/
extern int FreeTaskRef;

extern int FreeResourceRef;

void Schedule(int task, int mode);

void Dispatch();
void EventSystemDispatch(int task);
