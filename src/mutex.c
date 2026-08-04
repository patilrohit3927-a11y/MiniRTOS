#include "mutex.h"
#include "scheduler.h"
#include "cortex_m.h"



void Mutex_Create(Mutex *mutex)
{
    mutex->locked = 0;

    mutex->owner = 0;

    mutex->waitingTask = 0;
}





int Mutex_Take(Mutex *mutex)
{

    Disable_Interrupts();



    /*
        Mutex is free
    */

    if(mutex->locked == 0)
    {

        mutex->locked = 1;

        mutex->owner = currentTask;


        Enable_Interrupts();


        return 0;
    }





    /*
        Mutex already taken
    */

    if(mutex->owner != currentTask)
    {

        mutex->waitingTask = currentTask;


        ReadyList_Remove(currentTask);


        BlockedList_Add(currentTask);



        Trigger_PendSV();

    }



    Enable_Interrupts();



    return -1;
}


int Mutex_Give(Mutex *mutex)
{
    Disable_Interrupts();

    mutex->locked = 0;
    mutex->owner = 0;

    if(mutex->waitingTask != 0)
    {
        TCB *task = mutex->waitingTask;

        mutex->waitingTask = 0;

        /* Remove from blocked list first */
        BlockedList_Remove(task);

        task->blockReason = BLOCK_NONE;
        task->state = TASK_READY;

        ReadyList_Add(task);
    }

    Enable_Interrupts();

    return 0;
}



