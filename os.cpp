/******************************/
/*          os.c              */
/******************************/

#include <stdio.h>
#include "sys.h"
#include "rtos_api.h"

int StartOS(TTaskCall entry, int priority, char* name)
{
	// В начале нет работающих тасок
	MostPriorityTaskRef = -1;
	FreeTaskRef = 0;

	FreeResourceRef = 0;

	printf("StartOS!\n");

	// Инициализация очереди тасок
	for (int i = 0; i < MAX_TASK; i++)
	{
		TaskQueue[i].next = i + 1;
	}
	TaskQueue[MAX_TASK - 1].next = 0;

	// Инициализация очереди ресурсов
	for (int i = 0; i < MAX_RES; i++)
	{
		ResourceQueue[i].priority = i + 1;
		ResourceQueue[i].task = -1;
	}
	ResourceQueue[MAX_RES - 1].priority = -1;

	_ActivateTask(entry, priority, name);
	Dispatch();

	return 0;
}

void ShutdownOS()
{
	printf("ShutdownOS!\n");
}