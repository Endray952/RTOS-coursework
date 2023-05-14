/*********************************/
/*          task.c               */
/*********************************/
#include <stdio.h>

#include "sys.h"
#include "rtos_api.h"
#include <stdlib.h>

void ActivateTask(TTaskCall entry, int priority, char* name)
{
	/*«апоминанем сюда таску до вызова планировщика*/
	int task;
	/*—юда записываем индекс бошки массива TaskQueue,
	чтобы туда сунуть новую таску*/
	int	occupy;

	printf("ActivateTask %s\n", name);

	task = RunningTask;

	/*ѕо индексу FreeTask ставим в TaskQueue новую таску,*/
	occupy = FreeTask;
	/* а в FreeTask устанавливаем новый индекс головы списка свободных элементов*/
	FreeTask = TaskQueue[occupy].ref;

	TaskQueue[occupy].priority = priority;
	TaskQueue[occupy].ceiling_priority = priority;
	TaskQueue[occupy].name = name;
	TaskQueue[occupy].entry = entry;

	Schedule(occupy, INSERT_TO_TAIL);

	/* ¬ызов диспетчера может не понадобитьс€,
	если текущую таску (task) не сместила нова€ таска
	(функци€ Schedle может поставить новую таску в RunningTask)*/
	if (task != RunningTask)
	{
		Dispatch(task);
	}

	printf("End of ActivateTask %s\n", name);

}

void TerminateTask(void)
{
	int task;

	task = RunningTask;

	printf("TerminateTask %s\n", TaskQueue[task].name);

	/*Ѕерем в качестве текущей таски первую из очереди*/
	RunningTask = TaskQueue[task].ref;

	TaskQueue[task].ref = FreeTask;

	FreeTask = task;

	printf("End of TerminateTask %s\n", TaskQueue[task].name);

}

void Schedule(int task, int mode)
{
	int cur, prev;
	/* Ќаивысший приоритет вход€щей таски (task)*/
	int priority;

	printf("Schedule %s\n", TaskQueue[task].name);

	cur = RunningTask;
	prev = -1;

	priority = TaskQueue[task].ceiling_priority;

	/* “ут происходит спуск по списку тасок
	≈сли тасок вообще нет (cur != -1) или мы нашли таску,
	приоритет которой меньше приоритета новой, то выходим, запомнив в cur первую таску,
	приорит которой <= новой*/
	while (cur != -1 && TaskQueue[cur].ceiling_priority > priority)
	{
		prev = cur;
		cur = TaskQueue[cur].ref;
	}

	if (mode == INSERT_TO_TAIL)
	{
		while (cur != -1 && TaskQueue[cur].ceiling_priority == priority)
		{
			prev = cur;
			cur = TaskQueue[cur].ref;
		}
	}

	TaskQueue[task].ref = cur;

	// prev == -1 значит нова€ таска сама€ приоритетна€
	if (prev == -1) RunningTask = task;
	else TaskQueue[prev].ref = task;

	printf("End of Schedule %s\n", TaskQueue[task].name);
}

void Dispatch(int task)
{
	// task - вытесненна€ таска
	printf("Dispatch\n");

	do
	{
		TaskQueue[RunningTask].entry();
	} while (RunningTask != task);
	// вернулись к выполнению вытесненной таски
	printf("End of Dispatch\n");

}

void EventSystemDispatch(int task)
{
	printf("Event dispatch\n");

	int nextTask = RunningTask;
	while ((nextTask != -1) && (TaskQueue[nextTask].task_state == TASK_WAITING))
	{
		nextTask = TaskQueue[nextTask].ref;
	}

	if (nextTask == -1)
	{
		printf("Error: all task in waiting state");
		return;
	}

	while (TaskQueue[task].task_state == TASK_WAITING)
	{
		if (nextTask != -1)
		{
			TaskQueue[nextTask].entry();
		}
		else
		{
			printf("Error: there is no running tasks, while task %d is waiting", task);
			exit(1);
		}
	}
}
