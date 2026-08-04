#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

extern TCB *currentTask;

/* Scheduler APIs */

void Scheduler_Init(void);

void Scheduler_Start(void);

void Scheduler_Tick(void);

void Scheduler_Yield(void);



/* Ready List */

void ReadyList_Add(TCB *task);

void ReadyList_Remove(TCB *task);



/* Blocked List */

void BlockedList_Add(TCB *task);

void BlockedList_Remove(TCB *task);



/* Task Selection */

TCB *Scheduler_GetNextTask(void);

TCB *Scheduler_SelectNextTask(void);

/* Idle Task */

void Scheduler_SetIdleTask(TCB *task);

// Task Monitor API
TCB *Scheduler_GetTaskList();
TCB *Scheduler_GetReadyList(void);

TCB *Scheduler_GetBlockedList(void);



#endif