/*********************************/
/*          task.c               */
/*********************************/
#include <stdio.h>

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
	TaskQueue[occupy].entry = entry;
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

	RunningTaskRef = TaskQueue[task].next;

	TaskQueue[task].next = FreeTaskRef;

	FreeTaskRef = task;

	TaskCount--;

	printf("End of TerminateTask %s\n", TaskQueue[task].name);
}

void Schedule(int task, int mode)
{
	int cur, prev;
	// ��������� ��������� �������� ����� (task)
	int priority;

	printf("Schedule %s\n", TaskQueue[task].name);

	cur = RunningTaskRef;
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

	// prev == -1 ������ ����� ����� ����� ������������
	if (prev != -1) TaskQueue[prev].next = task;

	printf("End of Schedule %s\n", TaskQueue[task].name);
}

void Dispatch()
{
	while (TaskCount != 0)
	{
		int nextReadyTask = RunningTaskRef;
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
		(*TaskQueue[nextReadyTask].entry)();
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
