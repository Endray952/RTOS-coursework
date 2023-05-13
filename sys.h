/****************************************/
/*               sys.h                   /
/****************************************/
#pragma once

#include "defs.h"

#define INSERT_TO_TAIL 1
#define INSERT_TO_HEAD 0

typedef struct Type_Task
{
	int ref;
	int priority;
	int ceiling_priority;
	void (*entry)(void);
	char* name;

} TTask;

typedef struct Type_resource
{
	/*индекс таски, захватившей ресурс*/
	int task;
	/*ссылка на следующий элемент в очереди ресурсов ResourceQueue*/
	int priority;
	/*имя ресурс*/
	char* name;

} TResource;

extern TTask TaskQueue[MAX_TASK];

extern TResource ResourceQueue[MAX_RES];

/*Таска, которая выполняется сейчас*/
extern int RunningTask;

/* Указатель на голову списка свободных элементов
массива TaskQueue (индекс массива TaskQueue)*/
extern int FreeTask;

extern int FreeResource;

void Schedule(int task, int mode);

void Dispatch(int task);
