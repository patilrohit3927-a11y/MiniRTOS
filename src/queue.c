#include "queue.h"
#include "scheduler.h"
#include "cortex_m.h"

void Queue_Create(Queue *queue)
{
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;

    queue->waitingSender = 0;
    queue->waitingReceiver = 0;
}

int Queue_Send
(
    Queue *queue,
    uint32_t data
)
{
    Disable_Interrupts();

    /*
        Queue Full
    */
    if(queue->count >= QUEUE_SIZE)
    {
        queue->waitingSender = currentTask;

        ReadyList_Remove(currentTask);

        currentTask->blockReason = BLOCK_QUEUE;

        BlockedList_Add(currentTask);

        Trigger_PendSV();

        Enable_Interrupts();

        return -1;
    }

    /*
        Store Data
    */
    queue->buffer[queue->tail] = data;

    queue->tail++;

    if(queue->tail >= QUEUE_SIZE)
    {
        queue->tail = 0;
    }

    queue->count++;

    /*
        Wake waiting receiver
    */
    if(queue->waitingReceiver != 0)
    {
        TCB *task = queue->waitingReceiver;

        queue->waitingReceiver = 0;

        BlockedList_Remove(task);

        task->blockReason = BLOCK_NONE;
        task->state = TASK_READY;

        ReadyList_Add(task);
    }

    Enable_Interrupts();

    return 0;
}

int Queue_Receive
(
    Queue *queue,
    uint32_t *data
)
{
    Disable_Interrupts();

    /*
        Queue Empty
    */
    if(queue->count == 0)
    {
        queue->waitingReceiver = currentTask;

        ReadyList_Remove(currentTask);

        currentTask->blockReason = BLOCK_QUEUE;

        BlockedList_Add(currentTask);

        Trigger_PendSV();

        Enable_Interrupts();

        return -1;
    }

    /*
        Read Data
    */
    *data = queue->buffer[queue->head];

    queue->head++;

    if(queue->head >= QUEUE_SIZE)
    {
        queue->head = 0;
    }

    queue->count--;

    /*
        Wake waiting sender
    */
    if(queue->waitingSender != 0)
    {
        TCB *task = queue->waitingSender;

        queue->waitingSender = 0;

        BlockedList_Remove(task);

        task->blockReason = BLOCK_NONE;
        task->state = TASK_READY;

        ReadyList_Add(task);
    }

    Enable_Interrupts();

    return 0;
}