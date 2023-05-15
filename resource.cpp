/*************************************/
/*             resource.c            */
/*************************************/

#include "sys.h"
#include "rtos_api.h"
#include <stdio.h>

void GetResource(int priority, char* name)
{
	printf("GetResource %s\n", name);
	/* Указатель на голову списка свободных ресурсов*/
	int free_occupy;
	free_occupy = FreeResourceRef;
	
	/* Ставим в FreeResource индекс следующего свободного ресурса*/
	FreeResourceRef = ResourceQueue[FreeResourceRef].priority;

	ResourceQueue[free_occupy].priority = priority;
	ResourceQueue[free_occupy].task = MostPriorityTaskRef;
	ResourceQueue[free_occupy].name = name;

	/* Выставляем наивысший приоритет таске, захватившей ресурс*/
	if (TaskQueue[MostPriorityTaskRef].ceiling_priority < priority)
	{
		TaskQueue[MostPriorityTaskRef].ceiling_priority = priority;
	}

}

void ReleaseResource(int priority, char* name)
{
	//priority - приоритет освобождаемого ресурса

	int ResourceIndex;

	printf("ReleaseResource %s\n", name);

	if (TaskQueue[MostPriorityTaskRef].ceiling_priority == priority)
	{
		int res_priority, task_priority;
		int our_task;

		our_task = MostPriorityTaskRef;
		/* task_priority мб больше, чем priority - если брали ресурс внутри другого более приоритетного ресурса*/
		task_priority = TaskQueue[MostPriorityTaskRef].priority;

		for (int i = 0; i < MAX_RES; i++)
		{
			// находим индекс ресурса в списке ресурсов, который захвачет текущей таской
			if (ResourceQueue[i].task != MostPriorityTaskRef) continue;

			res_priority = ResourceQueue[i].priority;

			/* Ресур может быть захвачен внутри завата другого ресурса, причем оба ресурса имеют одинаковый приоритет,
			поэтому дополнительно проверям равенство имен (по ссылке на строку, а не по значению строки!!!)*/
			if (res_priority == priority && name == ResourceQueue[i].name)
				ResourceIndex = i;
			/*Если мы рассматриваем индекс i - ресурс, внутри которого захватили наш ресурс*/
			else if (res_priority > task_priority) 
				task_priority = res_priority;
		}
		/* ceiling_priority тут может принимать 2 значения:
		1. приоритет таски, если захватили ресурс НЕ внутри других ресурсов, то есть не было вложенных захватов ресурсов
		2. самый большой приоритет одного из ресурсов, захваченных текущей таской
		*/
		TaskQueue[MostPriorityTaskRef].ceiling_priority = task_priority;

		/* После завершения захвата ресурса наша таска либо не поменяла приоритет, либо его уменьшила,
		поэтому надо провести перепланировку. Для этого RunningTask поставим следующую по приоритету таску,
		а our_task отправим в Schedule в качестве новой таски*/
		MostPriorityTaskRef = TaskQueue[MostPriorityTaskRef].next;

		/* INSERT_TO_HEAD - значит наша таска будет самой "приоритетной" из тасок такого же приоритета*/
		Schedule(our_task, INSERT_TO_HEAD);

		/* Элемент массива ResourceQueue, который занимал захваченный ресурс, теперь свободная ячейка*/ 
		ResourceQueue[ResourceIndex].priority = FreeResourceRef;
		ResourceQueue[ResourceIndex].task = -1;
		FreeResourceRef = ResourceIndex;

		/* Если в результаты планирования наша таска оказалась менее приоритетной*/
		/*if (our_task != RunningTaskRef)
		{
			Dispatch(our_task);
		}*/

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