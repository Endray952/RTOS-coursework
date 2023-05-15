/*************************************/
/*             resource.c            */
/*************************************/

#include "sys.h"
#include "rtos_api.h"
#include <stdio.h>

void GetResource(int priority, char* name)
{
	printf("GetResource %s\n", name);
	/* ��������� �� ������ ������ ��������� ��������*/
	int free_occupy;
	free_occupy = FreeResourceRef;
	
	/* ������ � FreeResource ������ ���������� ���������� �������*/
	FreeResourceRef = ResourceQueue[FreeResourceRef].priority;

	ResourceQueue[free_occupy].priority = priority;
	ResourceQueue[free_occupy].task = MostPriorityTaskRef;
	ResourceQueue[free_occupy].name = name;

	/* ���������� ��������� ��������� �����, ����������� ������*/
	if (TaskQueue[MostPriorityTaskRef].ceiling_priority < priority)
	{
		TaskQueue[MostPriorityTaskRef].ceiling_priority = priority;
	}

}

void ReleaseResource(int priority, char* name)
{
	//priority - ��������� �������������� �������

	int ResourceIndex;

	printf("ReleaseResource %s\n", name);

	if (TaskQueue[MostPriorityTaskRef].ceiling_priority == priority)
	{
		int res_priority, task_priority;
		int our_task;

		our_task = MostPriorityTaskRef;
		/* task_priority �� ������, ��� priority - ���� ����� ������ ������ ������� ����� ������������� �������*/
		task_priority = TaskQueue[MostPriorityTaskRef].priority;

		for (int i = 0; i < MAX_RES; i++)
		{
			// ������� ������ ������� � ������ ��������, ������� �������� ������� ������
			if (ResourceQueue[i].task != MostPriorityTaskRef) continue;

			res_priority = ResourceQueue[i].priority;

			/* ����� ����� ���� �������� ������ ������ ������� �������, ������ ��� ������� ����� ���������� ���������,
			������� ������������� �������� ��������� ���� (�� ������ �� ������, � �� �� �������� ������!!!)*/
			if (res_priority == priority && name == ResourceQueue[i].name)
				ResourceIndex = i;
			/*���� �� ������������� ������ i - ������, ������ �������� ��������� ��� ������*/
			else if (res_priority > task_priority) 
				task_priority = res_priority;
		}
		/* ceiling_priority ��� ����� ��������� 2 ��������:
		1. ��������� �����, ���� ��������� ������ �� ������ ������ ��������, �� ���� �� ���� ��������� �������� ��������
		2. ����� ������� ��������� ������ �� ��������, ����������� ������� ������
		*/
		TaskQueue[MostPriorityTaskRef].ceiling_priority = task_priority;

		/* ����� ���������� ������� ������� ���� ����� ���� �� �������� ���������, ���� ��� ���������,
		������� ���� �������� ��������������. ��� ����� RunningTask �������� ��������� �� ���������� �����,
		� our_task �������� � Schedule � �������� ����� �����*/
		MostPriorityTaskRef = TaskQueue[MostPriorityTaskRef].next;

		/* INSERT_TO_HEAD - ������ ���� ����� ����� ����� "������������" �� ����� ������ �� ����������*/
		Schedule(our_task, INSERT_TO_HEAD);

		/* ������� ������� ResourceQueue, ������� ������� ����������� ������, ������ ��������� ������*/ 
		ResourceQueue[ResourceIndex].priority = FreeResourceRef;
		ResourceQueue[ResourceIndex].task = -1;
		FreeResourceRef = ResourceIndex;

		/* ���� � ���������� ������������ ���� ����� ��������� ����� ������������*/
		/*if (our_task != RunningTaskRef)
		{
			Dispatch(our_task);
		}*/

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