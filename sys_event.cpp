/*********************************/
/* sys_event.cpp */
/*********************************/
#include <stdio.h>
#include "sys.h"
#include "rtos_api.h"

void SetSysEvent(TEventMask mask) {
    printf("SetSysEvent %i\n", mask);
    WorkingEvents |= mask;
    for (int i = 0; i < MAX_TASK; i++) {
        if (TaskQueue[i].task_state == TASK_WAITING &&
            // ћб WorkingEvents -> mask
            (WorkingEvents & TaskQueue[i].waiting_events)) {
            /* —брасываютс€ из waiting_events биты, соответствующие маске*/
            TaskQueue[i].waiting_events &= !mask;
            TaskQueue[i].task_state = TASK_READY;
            printf("Task \"%s\" is ready\n", TaskQueue[i].name);
        }
    }
    /* —брасываютс€ из WorkingEvents биты, соответствующие маске*/
    WorkingEvents &= !mask;
    Dispatch(RunningTask);
    printf("End of SetSysEvent %i\n", mask);
}

void GetSysEvent(TEventMask* mask) {
    *mask = WorkingEvents;
}

void WaitSysEvent(TEventMask mask) {
    printf("WaitSysEvent %i\n", mask);
    TaskQueue[RunningTask].waiting_events = mask;
    if ((WorkingEvents & mask) == 0) {//ждемс
        TaskQueue[RunningTask].task_state = TASK_WAITING;
        //printf("before dispatch\n");
        Dispatch(RunningTask);
    }
    printf("End of WaitSysEvent %i\n", mask);
}