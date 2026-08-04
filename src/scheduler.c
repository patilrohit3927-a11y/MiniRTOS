#include "scheduler.h"
#include "cortex_m.h"



static TCB *readyList = 0;
static TCB *blockedList = 0;
static TCB *idleTask = 0;



TCB *currentTask = 0;



static TCB *lastTask = 0;





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



void BlockedList_Add(TCB *task)
{
    if(task == 0)
        return;


    task->state = TASK_BLOCKED;


task->blockNext = blockedList;

blockedList = task;
}







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





void Scheduler_SetIdleTask(TCB *task)
{
    idleTask = task;
}



TCB *Scheduler_GetNextTask(void)
{
    TCB *task;

    if (readyList == 0)
        return idleTask;

    /*
        First run
    */
    if (lastTask == 0)
    {
        lastTask = readyList;
        return lastTask;
    }

    /*
        Start searching after the last task
    */
    task = lastTask->next;

    while (task)
    {
        if (task->state == TASK_READY)
        {
            lastTask = task;
            return task;
        }

        task = task->next;
    }

    /*
        Wrap around to the beginning
    */
    task = readyList;

    while (task != lastTask)
    {
        if (task->state == TASK_READY)
        {
            lastTask = task;
            return task;
        }

        task = task->next;
    }

    /*
        If only lastTask is READY
    */
    if (lastTask->state == TASK_READY)
        return lastTask;

    return idleTask;
}



TCB *Scheduler_SelectNextTask(void)
{
    if(currentTask != 0)
    {
        if(currentTask->state == TASK_RUNNING)
        {
            currentTask->state = TASK_READY;
            ReadyList_Remove(currentTask);
            ReadyList_Add(currentTask);
        }
    }

    currentTask = Scheduler_GetNextTask();
    lastTask = currentTask;
    currentTask->state = TASK_RUNNING;

    return currentTask;
}




void Scheduler_Tick(void)
{
    TCB *task;
    TCB *next;



    Disable_Interrupts();




    task = blockedList;



    while(task)
    {

        
    next = task->blockNext;



        /*
            Wake only delay blocked tasks

            Semaphore blocked tasks
            are released by Semaphore_Give()
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






    if(currentTask)
    {

        if(currentTask->timeSlice > 0)
        {
            currentTask->timeSlice--;
        }



        if(currentTask->timeSlice == 0)
        {

            currentTask->timeSlice = 10;



            if(currentTask->state == TASK_RUNNING)
            {
                currentTask->state = TASK_READY;
            }



            Trigger_PendSV();

        }

    }




    Enable_Interrupts();

}







void Scheduler_Yield(void)
{
    Trigger_PendSV();
}


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

TCB *Scheduler_GetTaskList(void)
{
    return readyList;
}


TCB *Scheduler_GetReadyList(void)
{
    return readyList;
}



TCB *Scheduler_GetBlockedList(void)
{
    return blockedList;
}