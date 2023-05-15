/*************************************/
/*             resource.c            */
/*************************************/

#include "sys.h"
#include "rtos_api.h"
#include <stdio.h>
#include <string.h>

void _GetResource(int priority, char* name)
{
	printf("GetResource %s by Task %d\n", name, RunningTaskRef);
	// Указатель на голову списка свободных ресурсов
	int free_occupy;
	free_occupy = FreeResourceRef;
	
	// Ставим в FreeResource индекс следующего свободного ресурса
	FreeResourceRef = ResourceQueue[FreeResourceRef].priority;
	
	ResourceQueue[free_occupy].priority = priority;
	ResourceQueue[free_occupy].task = RunningTaskRef;
	ResourceQueue[free_occupy].name = name;

	// Выставляем наивысший приоритет таске, захватившей ресурс
	if (TaskQueue[RunningTaskRef].ceiling_priority < priority)
	{
		TaskQueue[RunningTaskRef].ceiling_priority = priority;
	}

	ReSchedule(RunningTaskRef);
}

void _ReleaseResource(int priority, char* name)
{
	//priority - приоритет освобождаемого ресурса

	int ResourceIndex;

	printf("ReleaseResource %s by Task %d\n", name, RunningTaskRef);

	if (TaskQueue[RunningTaskRef].ceiling_priority == priority)
	{
		volatile int res_priority, task_priority;
		volatile int our_task;

		our_task = RunningTaskRef;
		// task_priority мб больше, чем priority - если брали ресурс внутри другого более приоритетного ресурса
		task_priority = TaskQueue[RunningTaskRef].priority;

		for (int i = 0; i < MAX_RES; i++)
		{
			// находим индекс ресурса в списке ресурсов, который захвачет текущей таской
			if (ResourceQueue[i].task != RunningTaskRef) continue;

			res_priority = ResourceQueue[i].priority;

			/* Ресур может быть захвачен внутри завата другого ресурса, причем оба ресурса имеют одинаковый приоритет,
			поэтому дополнительно проверям равенство имен (по ссылке на строку, а не по значению строки!!!)*/
			if (res_priority == priority && (strcmp(name, ResourceQueue[i].name) == 0))
				ResourceIndex = i;
			/*Если мы рассматриваем индекс i - ресурс, внутри которого захватили наш ресурс*/
			else if (res_priority > task_priority)
				task_priority = res_priority;
		}
		/* ceiling_priority тут может принимать 2 значения:
		1. приоритет таски, если захватили ресурс НЕ внутри других ресурсов, то есть не было вложенных захватов ресурсов
		2. самый большой приоритет одного из ресурсов, захваченных текущей таской
		*/
		TaskQueue[RunningTaskRef].ceiling_priority = task_priority;

		ReSchedule(RunningTaskRef);

		/* Элемент массива ResourceQueue, который занимал захваченный ресурс, теперь свободная ячейка*/ 
		ResourceQueue[ResourceIndex].priority = FreeResourceRef;
		ResourceQueue[ResourceIndex].task = -1;
		FreeResourceRef = ResourceIndex;
	}
	/* Если ceiling_priority > prioprity, то это значит, что
	мы завершаем захват ресурса внутри захвата более приоритетного ресура, из-за
	которого ceiling_priority таски становится большим*/
	else
	{
		ResourceIndex = 0;
		/* Находим индекс текущего ресурса в массиве ресурсов*/
		while (ResourceQueue[ResourceIndex].task != MostPriorityTaskRef ||
			ResourceQueue[ResourceIndex].priority != priority ||
			ResourceQueue[ResourceIndex].name != name)
		{
			ResourceIndex++;
		}

		/* Добавляем элемент массива, занимаемый ресурсом, в список свободных ресурсов*/
		ResourceQueue[ResourceIndex].priority = FreeResourceRef;
		ResourceQueue[ResourceIndex].task = -1;
		FreeResourceRef = ResourceIndex;
	}
}