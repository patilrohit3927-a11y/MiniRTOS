#include "task.h"
#include "scheduler.h"
#include "cortex_m.h"



void Task_Create
(
    TCB *task,
    void (*taskFunction)(void),
    uint32_t *stack,
    uint32_t stackSize,
    uint8_t priority,
    const char *name
)
{
    uint32_t *sp;


    Disable_Interrupts();



    task->taskFunction = taskFunction;

    task->priority = priority;

    task->state = TASK_READY;

    task->blockReason = BLOCK_NONE;

    task->delayTicks = 0;

task->timeSlice = 10;

task->next = 0;



int i = 0;


while(name[i] && i < 15)
{
    task->name[i] = name[i];
    i++;
}


task->name[i] = '\0';



task->stackBase = stack;

    task->stackSize = stackSize;



    /*
        Stack grows downward
    */

    sp = stack + stackSize;



    /* Hardware stacked frame */

    *(--sp) = 0x01000000;                  // xPSR
    *(--sp) = ((uint32_t)taskFunction);    // PC
    *(--sp) = 0x00000000;                  // LR


    *(--sp) = 0x12121212;                  // R12
    *(--sp) = 0x03030303;                  // R3
    *(--sp) = 0x02020202;                  // R2
    *(--sp) = 0x01010101;                  // R1
    *(--sp) = 0x00000000;                  // R0



    /* Software frame */

    *(--sp) = 0x11111111; // R11
    *(--sp) = 0x10101010; // R10
    *(--sp) = 0x09090909; // R9
    *(--sp) = 0x08080808; // R8
    *(--sp) = 0x07070707; // R7
    *(--sp) = 0x06060606; // R6
    *(--sp) = 0x05050505; // R5
    *(--sp) = 0x04040404; // R4



    task->stackPointer = sp;



    ReadyList_Add(task);



    Enable_Interrupts();

}






void vTaskDelay(uint32_t ticks)
{
    Disable_Interrupts();


    currentTask->delayTicks = ticks;


    currentTask->blockReason = BLOCK_DELAY;


    ReadyList_Remove(currentTask);


    BlockedList_Add(currentTask);


    Trigger_PendSV();


    Enable_Interrupts();
}






void Task_Yield(void)
{
    Disable_Interrupts();

    Trigger_PendSV();

    Enable_Interrupts();
}