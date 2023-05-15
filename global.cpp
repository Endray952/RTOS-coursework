/****************************************/
/*        global.c                      */
/****************************************/
#pragma once

#include "sys.h"

TTask TaskQueue[MAX_TASK];

TResource ResourceQueue[MAX_RES];

int MostPriorityTaskRef;

int FreeTaskRef;

int FreeResourceRef;

TEventMask WorkingEvents;

int TaskCount = 0;
