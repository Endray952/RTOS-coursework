/*********************************/
/*          task.c               */
/*********************************/
#include <stdio.h>

#include "sys.h"
#include "rtos_api.h"

void ActivateTask(TTaskCall entry, int priority, char* name)
{
	/*����������� ���� ����� �� ������ ������������*/
	int task;
	/*���� ���������� ������ ����� ������� TaskQueue,
	����� ���� ������ ����� �����*/
	int	occupy;

	printf("ActivateTask %s\n", name);

	task = RunningTask;

	/*�� ������� FreeTask ������ � TaskQueue ����� �����,*/
	occupy = FreeTask;
	/* � � FreeTask ������������� ����� ������ ������ ������ ��������� ���������*/
	FreeTask = TaskQueue[occupy].ref;

	TaskQueue[occupy].priority = priority;
	TaskQueue[occupy].ceiling_priority = priority;
	TaskQueue[occupy].name = name;
	TaskQueue[occupy].entry = entry;

	Schedule(occupy, INSERT_TO_TAIL);

	/* ����� ���������� ����� �� ������������,
	���� ������� ����� (task) �� �������� ����� �����
	(������� Schedle ����� ��������� ����� ����� � RunningTask)*/
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

	/*����� � �������� ������� ����� ������ �� �������*/
	RunningTask = TaskQueue[task].ref;

	TaskQueue[task].ref = FreeTask;

	FreeTask = task;

	printf("End of TerminateTask %s\n", TaskQueue[task].name);

}

void Schedule(int task, int mode)
{
	int cur, prev;
	/* ��������� ��������� �������� ����� (task)*/
	int priority;

	printf("Schedule %s\n", TaskQueue[task].name);

	cur = RunningTask;
	prev = -1;

	priority = TaskQueue[task].ceiling_priority;

	/* ��� ���������� ����� �� ������ �����
	���� ����� ������ ��� (cur != -1) ��� �� ����� �����,
	��������� ������� ������ ���������� �����, �� �������, �������� � cur ������ �����,
	������� ������� <= �����*/
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

	// prev == -1 ������ ����� ����� ����� ������������
	if (prev == -1) RunningTask = task;
	else TaskQueue[prev].ref = task;

	printf("End of Schedule %s\n", TaskQueue[task].name);

}

void Dispatch(int task)
{
	// task - ����������� �����
	printf("Dispatch\n");

	do
	{
		TaskQueue[RunningTask].entry();
	} while (RunningTask != task);
	// ��������� � ���������� ����������� �����
	printf("End of Dispatch\n");

}