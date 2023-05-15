/*********************************/
/*          task.c               */
/*********************************/
#include <stdio.h>
#include <iostream>

#include "sys.h"
#include "rtos_api.h"
#include <stdlib.h>

void _ActivateTask(TTaskCall entry, int priority, char* name)
{
	/* Сюда записываем индекс бошки массива TaskQueue,
	чтобы туда сунуть новую таску */
	int	occupy;

	printf("ActivateTask %s\n", name);

	// По индексу FreeTask ставим в TaskQueue новую таску
	occupy = FreeTaskRef;

	// а в FreeTask устанавливаем новый индекс головы списка свободных элементов
	FreeTaskRef = TaskQueue[occupy].next;
	TaskQueue[occupy].next = -1;

	TaskQueue[occupy].priority = priority;
	TaskQueue[occupy].ceiling_priority = priority;
	TaskQueue[occupy].name = name;

	boost::coroutines2::coroutine<void>::push_type* coroutineEntry = nullptr;
	coroutineEntry = new boost::coroutines2::coroutine<void>::push_type(entry);

	TaskQueue[occupy].entry = coroutineEntry;
	TaskQueue[occupy].task_state = TASK_READY;

	TaskCount++;

	Schedule(occupy, INSERT_TO_TAIL);

	printf("End of ActivateTask %s\n", name);
}

void _TerminateTask(void)
{
	int task;

	task = MostPriorityTaskRef;

	printf("TerminateTask %s\n", TaskQueue[task].name);

	MostPriorityTaskRef = TaskQueue[task].next;

	TaskQueue[task].next = FreeTaskRef;

	FreeTaskRef = task;
	delete TaskQueue[task].entry;

	TaskCount--;

	printf("End of TerminateTask %s\n", TaskQueue[task].name);
}

void Schedule(int task, int mode)
{
	int cur, prev;
	// Наивысший приоритет входящей таски (task)
	int priority;

	printf("Schedule %s\n", TaskQueue[task].name);

	cur = MostPriorityTaskRef;
	prev = -1;

	priority = TaskQueue[task].ceiling_priority;

	/* Тут происходит спуск по списку тасок
	Если тасок вообще нет (cur != -1) или мы нашли таску,
	приоритет которой меньше приоритета новой, то выходим, запомнив в cur первую таску,
	приорит которой <= новой*/
	while (cur != -1 && TaskQueue[cur].ceiling_priority > priority)
	{
		prev = cur;
		cur = TaskQueue[cur].next;
	}

	if (mode == INSERT_TO_TAIL)
	{
		while (cur != -1 && TaskQueue[cur].ceiling_priority == priority)
		{
			prev = cur;
			cur = TaskQueue[cur].next;
		}
	}

	TaskQueue[task].next = cur;

	if (MostPriorityTaskRef == -1)
	{
		MostPriorityTaskRef = task;
	}

	// prev == -1 значит новая таска самая приоритетная
	if (prev != -1)
	{
		TaskQueue[prev].next = task;
	}
	else
	{
		MostPriorityTaskRef = task;
	}

	printf("End of Schedule %s\n", TaskQueue[task].name);
}

void Dispatch()
{
	while (TaskCount != 0)
	{
		int nextReadyTask = MostPriorityTaskRef;
		while (nextReadyTask != -1)
		{
			if (TaskQueue[nextReadyTask].task_state == TASK_READY)
			{
				break;
			}
			else
			{
				nextReadyTask = TaskQueue[nextReadyTask].next;
			}
		}
		
		if (nextReadyTask == -1)
		{
			std::cout << "There is no ready tasks!" << std::endl;
			exit(1);
		}
		//std::cout << TaskQueue[nextReadyTask].name;
		//std::cout << TaskQueue[nextReadyTask].entry;
		if (TaskQueue[nextReadyTask].entry != nullptr)
		{
			//std::cout << "Dura!";
			(*TaskQueue[nextReadyTask].entry)();
		}
	}
}

//void EventSystemDispatch(int task)
//{
//	printf("Event dispatch\n");
//
//	int nextTask = RunningTaskRef;
//	while ((nextTask != -1) && (TaskQueue[nextTask].task_state == TASK_WAITING))
//	{
//		nextTask = TaskQueue[nextTask].next;
//	}
//
//	if (nextTask == -1)
//	{
//		printf("Error: all task in waiting state");
//		return;
//	}
//
//	int tmp = nextTask;
//	while (TaskQueue[task].task_state == TASK_WAITING)
//	{
//		if (nextTask != -1)
//		{
//			if (TaskQueue[nextTask].task_state == TASK_WAITING)
//			{
//				nextTask = TaskQueue[nextTask].next;
//				continue;
//			}
//			tmp = nextTask;
//			nextTask = TaskQueue[tmp].next;
//			TaskQueue[tmp].entry();
//		}
//		else
//		{
//			printf("Error: there is no running tasks, while task %d is waiting", task);
//			exit(1);
//		}
//	}
//}
