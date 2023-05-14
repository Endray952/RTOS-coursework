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
	TASK_SUSPENDED,
	TASK_WAITING
};

extern TEventMask WorkingEvents; //����������� ������� (�� ��� �����������)


#define INSERT_TO_TAIL 1
#define INSERT_TO_HEAD 0

typedef struct Type_Task
{
	int ref;
	int priority;
	int ceiling_priority;
	void (*entry)(void);
	char* name;
	TTaskState task_state;
	TEventMask waiting_events;
} TTask;

typedef struct Type_resource
{
	/*������ �����, ����������� ������*/
	int task;
	/*������ �� ��������� ������� � ������� �������� ResourceQueue*/
	int priority;
	/*��� ������*/
	char* name;

} TResource;

extern TTask TaskQueue[MAX_TASK];

extern TResource ResourceQueue[MAX_RES];

/*�����, ������� ����������� ������*/
extern int RunningTask;

/* ��������� �� ������ ������ ��������� ���������
������� TaskQueue (������ ������� TaskQueue)*/
extern int FreeTask;

extern int FreeResource;

void Schedule(int task, int mode);

void Dispatch(int task);
void EventSystemDispatch(int task);
