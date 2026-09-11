#include "scheduler.h"
#include "cortex_m.h"


/*-----------------------------------------------------------
 * Scheduler Lists
 *----------------------------------------------------------*/

static TCB *readyList = 0;

static TCB *blockedList = 0;

static TCB *idleTask = 0;


/*-----------------------------------------------------------
 * Current Running Task
 *----------------------------------------------------------*/

TCB *currentTask = 0;


/*-----------------------------------------------------------
 * Last Task Used For Round Robin
 *
 * Used only when multiple READY tasks have
 * the same highest priority.
 *----------------------------------------------------------*/

static TCB *lastTask = 0;


/*-----------------------------------------------------------
 * Scheduler Initialization
 *----------------------------------------------------------*/

void Scheduler_Init(void)
{
    Disable_Interrupts();

    readyList = 0;

    blockedList = 0;

    idleTask = 0;

    currentTask = 0;

    lastTask = 0;

    Enable_Interrupts();
}


/*-----------------------------------------------------------
 * Add Task To Ready List
 *----------------------------------------------------------*/

void ReadyList_Add(TCB *task)
{
    TCB *temp;


    if(task == 0)
        return;


    task->state = TASK_READY;

    task->next = 0;


    if(readyList == 0)
    {
        readyList = task;

        return;
    }


    temp = readyList;


    while(temp->next)
    {
        temp = temp->next;
    }


    temp->next = task;
}


/*-----------------------------------------------------------
 * Remove Task From Ready List
 *----------------------------------------------------------*/

void ReadyList_Remove(TCB *task)
{
    TCB *prev = 0;

    TCB *curr = readyList;


    while(curr)
    {
        if(curr == task)
        {
            if(prev == 0)
            {
                readyList = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }


            curr->next = 0;

            return;
        }


        prev = curr;

        curr = curr->next;
    }
}


/*-----------------------------------------------------------
 * Add Task To Blocked List
 *----------------------------------------------------------*/

void BlockedList_Add(TCB *task)
{
    if(task == 0)
        return;


    task->state = TASK_BLOCKED;


    task->blockNext = blockedList;

    blockedList = task;
}


/*-----------------------------------------------------------
 * Remove Task From Blocked List
 *----------------------------------------------------------*/

void BlockedList_Remove(TCB *task)
{
    TCB *prev = 0;

    TCB *curr = blockedList;


    while(curr)
    {
        if(curr == task)
        {
            if(prev == 0)
            {
                blockedList = curr->blockNext;
            }
            else
            {
                prev->blockNext = curr->blockNext;
            }


            curr->blockNext = 0;

            return;
        }


        prev = curr;

        curr = curr->blockNext;
    }
}


/*-----------------------------------------------------------
 * Set Idle Task
 *----------------------------------------------------------*/

void Scheduler_SetIdleTask(TCB *task)
{
    idleTask = task;
}


/*-----------------------------------------------------------
 * Get Next Task
 *
 * Priority Scheduling:
 *
 * 1. Find highest priority among READY tasks.
 *
 * 2. If only one task has that priority,
 *    select it.
 *
 * 3. If multiple tasks have the same highest
 *    priority, use Round Robin.
 *
 * 4. If no task is READY, run Idle Task.
 *----------------------------------------------------------*/

TCB *Scheduler_GetNextTask(void)
{
    TCB *task;

    TCB *selectedTask = 0;

    uint8_t highestPriority = 0;

    int foundPriority = 0;


    /*-------------------------------------------------------
     * Find Highest Priority READY Task
     *-------------------------------------------------------*/

    task = readyList;


    while(task)
    {
        if(task->state == TASK_READY)
        {
            if(!foundPriority ||
               task->priority > highestPriority)
            {
                highestPriority = task->priority;

                foundPriority = 1;
            }
        }


        task = task->next;
    }


    /*-------------------------------------------------------
     * No READY Task
     *-------------------------------------------------------*/

    if(!foundPriority)
    {
        return idleTask;
    }


    /*-------------------------------------------------------
     * Round Robin Among Highest Priority Tasks
     *
     * Start searching after lastTask.
     *-------------------------------------------------------*/

    if(lastTask != 0)
    {
        task = lastTask->next;


        while(task)
        {
            if(task->state == TASK_READY &&
               task->priority == highestPriority)
            {
                selectedTask = task;

                break;
            }


            task = task->next;
        }
    }


    /*-------------------------------------------------------
     * Wrap Around
     *-------------------------------------------------------*/

    if(selectedTask == 0)
    {
        task = readyList;


        while(task)
        {
            if(task->state == TASK_READY &&
               task->priority == highestPriority)
            {
                selectedTask = task;

                break;
            }


            task = task->next;
        }
    }


    /*-------------------------------------------------------
     * Update Round Robin Position
     *-------------------------------------------------------*/

    if(selectedTask != 0)
    {
        lastTask = selectedTask;

        return selectedTask;
    }


    /*-------------------------------------------------------
     * Safety Fallback
     *-------------------------------------------------------*/

    return idleTask;
}


/*-----------------------------------------------------------
 * Select Next Task
 *----------------------------------------------------------*/

TCB *Scheduler_SelectNextTask(void)
{
    TCB *nextTask;


    /*-------------------------------------------------------
     * Current task becomes READY if it was RUNNING
     *-------------------------------------------------------*/

    if(currentTask != 0)
    {
        if(currentTask->state == TASK_RUNNING)
        {
            currentTask->state = TASK_READY;
        }
    }


    /*-------------------------------------------------------
     * Select next task using priority scheduler
     *-------------------------------------------------------*/

    nextTask = Scheduler_GetNextTask();


    if(nextTask == 0)
    {
        return 0;
    }


    currentTask = nextTask;


    currentTask->state = TASK_RUNNING;


    return currentTask;
}


/*-----------------------------------------------------------
 * Scheduler Tick
 *
 * Handles:
 *
 * - Delayed task wake-up
 * - Time slice management
 * - Preemption
 *----------------------------------------------------------*/

void Scheduler_Tick(void)
{
    TCB *task;

    TCB *next;


    Disable_Interrupts();


    /*-------------------------------------------------------
     * Process Blocked Tasks
     *-------------------------------------------------------*/

    task = blockedList;


    while(task)
    {
        next = task->blockNext;


        /*
         * Only delay-blocked tasks are automatically
         * released here.
         *
         * Semaphore, mutex and queue blocked tasks
         * are released by their respective IPC functions.
         */

        if(task->blockReason == BLOCK_DELAY)
        {
            if(task->delayTicks > 0)
            {
                task->delayTicks--;
            }


            if(task->delayTicks == 0)
            {
                BlockedList_Remove(task);


                task->blockReason = BLOCK_NONE;


                ReadyList_Add(task);
            }
        }


        task = next;
    }


    /*-------------------------------------------------------
     * Current Task Time Slice
     *-------------------------------------------------------*/

    if(currentTask != 0)
    {
        if(currentTask->timeSlice > 0)
        {
            currentTask->timeSlice--;
        }


        /*---------------------------------------------------
         * Time Slice Expired
         *---------------------------------------------------*/

        if(currentTask->timeSlice == 0)
        {
            currentTask->timeSlice = 10;


            if(currentTask->state == TASK_RUNNING)
            {
                currentTask->state = TASK_READY;
            }


            /*
             * PendSV will perform the context switch.
             *
             * Scheduler_SelectNextTask() will then select:
             *
             * 1. Highest priority READY task
             *
             * 2. Round Robin if priorities are equal
             */

            Trigger_PendSV();
        }
    }


    Enable_Interrupts();
}


/*-----------------------------------------------------------
 * Scheduler Yield
 *----------------------------------------------------------*/

void Scheduler_Yield(void)
{
    Trigger_PendSV();
}


/*-----------------------------------------------------------
 * Start Scheduler
 *----------------------------------------------------------*/

void Scheduler_Start(void)
{
    currentTask = Scheduler_GetNextTask();


    if(currentTask != 0)
    {
        currentTask->state = TASK_RUNNING;


        Trigger_PendSV();


        while(1)
        {
        }
    }
}


/*-----------------------------------------------------------
 * Get Complete Task List
 *----------------------------------------------------------*/

TCB *Scheduler_GetTaskList(void)
{
    return readyList;
}


/*-----------------------------------------------------------
 * Get Ready List
 *----------------------------------------------------------*/

TCB *Scheduler_GetReadyList(void)
{
    return readyList;
}


/*-----------------------------------------------------------
 * Get Blocked List
 *----------------------------------------------------------*/

TCB *Scheduler_GetBlockedList(void)
{
    return blockedList;
}