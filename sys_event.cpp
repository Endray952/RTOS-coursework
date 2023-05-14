/*********************************/
/* sys_event.cpp */
/*********************************/
#include <stdio.h>
#include "sys.h"
#include "rtos_api.h"

void SetSysEvent(TEventMask mask)
{
    printf("SetSysEvent %i\n", mask);
    WorkingEvents |= mask;
    // Проходимся по всем задачам
    for (int i = 0; i < MAX_TASK; i++)
    {
        // Проверям, ждёт ли задача заданного события
        if (TaskQueue[i].task_state == TASK_WAITING &&
            (mask & TaskQueue[i].waiting_events))
        {
            // Сбрасываются из waiting_events биты, соответствующие маске
            TaskQueue[i].waiting_events &= ~mask;
            TaskQueue[i].task_state = TASK_READY;
            printf("Task \"%s\" is ready over event %u\n", 
                TaskQueue[i].name, mask);
        }
    }
    // Сбрасываются из WorkingEvents биты, соответствующие маске
    WorkingEvents &= ~mask;
    Dispatch(RunningTask);
    printf("End of SetSysEvent %u\n", mask);
}

void GetSysEvent(TEventMask* mask)
{
    *mask = WorkingEvents;
}

void WaitSysEvent(TEventMask mask)
{
    printf("WaitSysEvent %u\n", mask);
    TaskQueue[RunningTask].waiting_events = mask;
    if ((WorkingEvents & mask) == 0)
    {
        TaskQueue[RunningTask].task_state = TASK_WAITING;
        //printf("before dispatch\n");
        EventSystemDispatch(RunningTask);
    }
    printf("End of WaitSysEvent %i\n", mask);
}