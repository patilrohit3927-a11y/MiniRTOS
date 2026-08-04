#include "semaphore.h"
#include "scheduler.h"
#include "cortex_m.h"



void Semaphore_Create
(
    Semaphore *sem,
    uint8_t initialValue
)
{

    sem->count = initialValue;

    sem->waitingTask = 0;

}






int Semaphore_Take
(
    Semaphore *sem
)
{

    Disable_Interrupts();



    if(sem->count > 0)
    {

        sem->count--;

        Enable_Interrupts();

        return 0;

    }



    /*
        No semaphore available

        Block current task
    */


sem->waitingTask = currentTask;

currentTask->blockReason = BLOCK_SEMAPHORE;

ReadyList_Remove(currentTask);

BlockedList_Add(currentTask);



    Trigger_PendSV();



    Enable_Interrupts();



    return -1;

}








int Semaphore_Give(Semaphore *sem)
{
    Disable_Interrupts();


    sem->count++;


    if(sem->waitingTask != 0)
    {
       TCB *task = sem->waitingTask;

       BlockedList_Remove(task);

       task->blockReason = BLOCK_NONE;

      ReadyList_Add(task);

sem->waitingTask = 0;
    }


    Enable_Interrupts();


    return 0;
}