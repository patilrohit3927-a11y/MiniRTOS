#include "mutex.h"
#include "scheduler.h"
#include "cortex_m.h"


/*-----------------------------------------------------------
 * Mutex Create
 *----------------------------------------------------------*/

void Mutex_Create(Mutex *mutex)
{
    if(mutex == 0)
        return;


    mutex->locked = 0;

    mutex->owner = 0;

    mutex->waitingTask = 0;
}


/*-----------------------------------------------------------
 * Mutex Take
 *----------------------------------------------------------*/

int Mutex_Take(Mutex *mutex)
{
    Disable_Interrupts();


    if(mutex == 0)
    {
        Enable_Interrupts();

        return -1;
    }


    /*-------------------------------------------------------
     * Mutex is free
     *------------------------------------------------------*/

    if(mutex->locked == 0)
    {

        mutex->locked = 1;

        mutex->owner = currentTask;


        Enable_Interrupts();


        return 0;
    }


    /*-------------------------------------------------------
     * Mutex already owned by current task
     *
     * Simple recursive protection:
     * do not block the owner.
     *------------------------------------------------------*/

    if(mutex->owner == currentTask)
    {
        Enable_Interrupts();

        return 0;
    }


    /*-------------------------------------------------------
     * Mutex is owned by another task.
     *
     * Priority inheritance:
     *
     * If waiting task has higher priority than
     * mutex owner, temporarily raise owner's priority.
     *------------------------------------------------------*/

    if(mutex->owner != 0)
    {

        if(currentTask != 0)
        {

            if(currentTask->priority >
               mutex->owner->priority)
            {

                mutex->owner->priority =
                    currentTask->priority;

            }
        }
    }


    /*-------------------------------------------------------
     * Add current task to blocked state
     *------------------------------------------------------*/

    mutex->waitingTask = currentTask;


    if(currentTask != 0)
    {

        currentTask->blockReason = BLOCK_MUTEX;


        ReadyList_Remove(currentTask);


        BlockedList_Add(currentTask);

    }


    Trigger_PendSV();


    Enable_Interrupts();


    return -1;
}


/*-----------------------------------------------------------
 * Mutex Give
 *----------------------------------------------------------*/

int Mutex_Give(Mutex *mutex)
{
    Disable_Interrupts();


    if(mutex == 0)
    {
        Enable_Interrupts();

        return -1;
    }


    /*
        Only owner should release mutex.
    */

    if(mutex->owner != currentTask)
    {
        Enable_Interrupts();

        return -1;
    }


    /*-------------------------------------------------------
     * Save owner before releasing.
     *------------------------------------------------------*/

    TCB *owner = mutex->owner;


    mutex->locked = 0;

    mutex->owner = 0;


    /*-------------------------------------------------------
     * Restore original priority.
     *------------------------------------------------------*/

    owner->priority = owner->originalPriority;


    /*-------------------------------------------------------
     * Wake waiting task
     *------------------------------------------------------*/

    if(mutex->waitingTask != 0)
    {

        TCB *task = mutex->waitingTask;


        mutex->waitingTask = 0;


        BlockedList_Remove(task);


        task->blockReason = BLOCK_NONE;


        ReadyList_Add(task);


        /*
            If awakened task has higher priority
            than current task, request immediate
            context switch.
        */

        if(currentTask != 0)
        {

            if(task->priority > currentTask->priority)
            {
                Trigger_PendSV();
            }

        }
    }


    Enable_Interrupts();


    return 0;
}