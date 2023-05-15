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
	// ��������� �� ������ ������ ��������� ��������
	int free_occupy;
	free_occupy = FreeResourceRef;
	
	// ������ � FreeResource ������ ���������� ���������� �������
	FreeResourceRef = ResourceQueue[FreeResourceRef].priority;
	
	ResourceQueue[free_occupy].priority = priority;
	ResourceQueue[free_occupy].task = RunningTaskRef;
	ResourceQueue[free_occupy].name = name;

	// ���������� ��������� ��������� �����, ����������� ������
	if (TaskQueue[RunningTaskRef].ceiling_priority < priority)
	{
		TaskQueue[RunningTaskRef].ceiling_priority = priority;
	}

	ReSchedule(RunningTaskRef);
}

void _ReleaseResource(int priority, char* name)
{
	//priority - ��������� �������������� �������

	int ResourceIndex;

	printf("ReleaseResource %s by Task %d\n", name, RunningTaskRef);

	if (TaskQueue[RunningTaskRef].ceiling_priority == priority)
	{
		volatile int res_priority, task_priority;
		volatile int our_task;

		our_task = RunningTaskRef;
		// task_priority �� ������, ��� priority - ���� ����� ������ ������ ������� ����� ������������� �������
		task_priority = TaskQueue[RunningTaskRef].priority;

		for (int i = 0; i < MAX_RES; i++)
		{
			// ������� ������ ������� � ������ ��������, ������� �������� ������� ������
			if (ResourceQueue[i].task != RunningTaskRef) continue;

			res_priority = ResourceQueue[i].priority;

			/* ����� ����� ���� �������� ������ ������ ������� �������, ������ ��� ������� ����� ���������� ���������,
			������� ������������� �������� ��������� ���� (�� ������ �� ������, � �� �� �������� ������!!!)*/
			if (res_priority == priority && (strcmp(name, ResourceQueue[i].name) == 0))
				ResourceIndex = i;
			/*���� �� ������������� ������ i - ������, ������ �������� ��������� ��� ������*/
			else if (res_priority > task_priority)
				task_priority = res_priority;
		}
		/* ceiling_priority ��� ����� ��������� 2 ��������:
		1. ��������� �����, ���� ��������� ������ �� ������ ������ ��������, �� ���� �� ���� ��������� �������� ��������
		2. ����� ������� ��������� ������ �� ��������, ����������� ������� ������
		*/
		TaskQueue[RunningTaskRef].ceiling_priority = task_priority;

		ReSchedule(RunningTaskRef);

		/* ������� ������� ResourceQueue, ������� ������� ����������� ������, ������ ��������� ������*/ 
		ResourceQueue[ResourceIndex].priority = FreeResourceRef;
		ResourceQueue[ResourceIndex].task = -1;
		FreeResourceRef = ResourceIndex;
	}
	/* ���� ceiling_priority > prioprity, �� ��� ������, ���
	�� ��������� ������ ������� ������ ������� ����� ������������� ������, ��-��
	�������� ceiling_priority ����� ���������� �������*/
	else
	{
		ResourceIndex = 0;
		/* ������� ������ �������� ������� � ������� ��������*/
		while (ResourceQueue[ResourceIndex].task != MostPriorityTaskRef ||
			ResourceQueue[ResourceIndex].priority != priority ||
			ResourceQueue[ResourceIndex].name != name)
		{
			ResourceIndex++;
		}

		/* ��������� ������� �������, ���������� ��������, � ������ ��������� ��������*/
		ResourceQueue[ResourceIndex].priority = FreeResourceRef;
		ResourceQueue[ResourceIndex].task = -1;
		FreeResourceRef = ResourceIndex;
	}
}