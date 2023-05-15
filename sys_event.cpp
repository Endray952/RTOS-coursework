/*********************************/
/* sys_event.cpp */
/*********************************/
#include <stdio.h>
#include <iostream>
#include "sys.h"
#include "rtos_api.h"

void SetSysEvent(TEventMask mask)
{
    printf("SetSysEvent %i\n", mask);
    WorkingEvents |= mask;
    // ���������� �� ���� �������
    for (int i = 0; i < MAX_TASK; i++)
    {
        // ��������, ��� �� ������ ��������� �������
        if (TaskQueue[i].task_state == TASK_WAITING &&
            (mask & TaskQueue[i].waiting_events))
        {
            // ������������ �� waiting_events ����, ��������������� �����
            TaskQueue[i].waiting_events &= ~mask;
            TaskQueue[i].task_state = TASK_READY;
            printf("Task \"%s\" is ready over event %u\n", 
                TaskQueue[i].name, mask);
        }
    }
    // ������������ �� WorkingEvents ����, ��������������� �����
    WorkingEvents &= ~mask;
}

void GetSysEvent(TEventMask* mask)
{
    *mask = WorkingEvents;
}

void _WaitSysEvent(TEventMask mask)
{
    printf("WaitSysEvent %u\n", mask);
    TaskQueue[RunningTaskRef].waiting_events = mask;
    if ((WorkingEvents & mask) == 0)
    {
        TaskQueue[RunningTaskRef].task_state = TASK_WAITING;
    }
}
