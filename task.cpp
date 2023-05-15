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
	/* ���� ���������� ������ ����� ������� TaskQueue,
	����� ���� ������ ����� ����� */
	int	occupy;

	printf("ActivateTask %s\n", name);

	// �� ������� FreeTask ������ � TaskQueue ����� �����
	occupy = FreeTaskRef;

	// � � FreeTask ������������� ����� ������ ������ ������ ��������� ���������
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

	task = RunningTaskRef;

	printf("TerminateTask %s\n", TaskQueue[task].name);

	if (RunningTaskRef == MostPriorityTaskRef)
	{
		MostPriorityTaskRef = TaskQueue[task].next;
	}

	if (TaskCount > 1)
	{
		int currentTask = MostPriorityTaskRef;
		while (currentTask != -1 && TaskQueue[currentTask].next != task)
		{
			currentTask = TaskQueue[currentTask].next;
		}
		TaskQueue[currentTask].next = TaskQueue[task].next;
	}
	
	TaskQueue[task].next = FreeTaskRef;

	FreeTaskRef = task;
	delete TaskQueue[task].entry;

	TaskCount--;

	printf("End of TerminateTask %s\n", TaskQueue[task].name);
}

void ReSchedule(int task)
{
	printf("Reschedule Task %d\n", task);

	if (MostPriorityTaskRef == -1 || TaskCount == 1 || TaskQueue[MostPriorityTaskRef].next == -1)
	{
		MostPriorityTaskRef = task;
		TaskQueue[task].next = -1;
		return;
	}

	int currentTask = MostPriorityTaskRef;
	if (task == currentTask)
	{
		MostPriorityTaskRef = TaskQueue[task].next;
		TaskQueue[task].next = FreeTaskRef;
		FreeTaskRef = task;
		return Schedule(task, INSERT_TO_HEAD);
	}

	// ������� ����� �� ����������
	while (currentTask != -1 && TaskQueue[currentTask].next != task)
	{
		currentTask = TaskQueue[currentTask].next;
	}
	
	TaskQueue[currentTask].next = TaskQueue[task].next;

	// ��������� ����� ����� � ����������� ������� ����������
	Schedule(task, INSERT_TO_HEAD);
}

void Schedule(int task, int mode)
{
	int cur, prev;
	// ��������� ��������� �������� ����� (task)
	int priority;

	printf("Schedule %s\n", TaskQueue[task].name);

	cur = MostPriorityTaskRef;
	prev = -1;

	priority = TaskQueue[task].ceiling_priority;

	/* ��� ���������� ����� �� ������ �����
	���� ����� ������ ��� (cur != -1) ��� �� ����� �����,
	��������� ������� ������ ���������� �����, �� �������, �������� � cur ������ �����,
	������� ������� <= �����*/
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

	// prev == -1 ������ ����� ����� ����� ������������
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
		
		if (TaskQueue[nextReadyTask].entry != nullptr)
		{
			RunningTaskRef = nextReadyTask;
			(*TaskQueue[nextReadyTask].entry)();
			RunningTaskRef = -1;
		}
	}
}
