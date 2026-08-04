#include "rtos.h"
#include "cortex_m.h"
#include "idle.h"


static TCB taskPool[MAX_TASKS];

static uint32_t taskStacks[MAX_TASKS][TASK_STACK_SIZE];

static uint32_t taskCount = 0;


/*
    Idle Task TCB and Stack
*/

static TCB idleTCB;

static uint32_t idleStack[TASK_STACK_SIZE];



void RTOS_Init(void)
{
    taskCount = 0;

    Scheduler_Init();


    /*
        Create Idle Task

        Lowest priority task
        Runs only when no other task is READY
    */

Task_Create
(
    &idleTCB,
    IdleTask,
    idleStack,
    TASK_STACK_SIZE,
    0,
    "IDLE"
);


ReadyList_Remove(&idleTCB);




    Scheduler_SetIdleTask(&idleTCB);
}




void RTOS_Start(void)
{
    Scheduler_Start();
}





RTOS_Status xTaskCreate
(
    void (*taskFunction)(void),
    const char *taskName,
    uint8_t priority
)
{

    



    if(taskFunction == 0)
    {
        return RTOS_INVALID_PARAMETER;
    }



    if(taskCount >= MAX_TASKS)
    {
        return RTOS_OUT_OF_TASKS;
    }



Task_Create
(
    &taskPool[taskCount],
    taskFunction,
    taskStacks[taskCount],
    TASK_STACK_SIZE,
    priority,
    taskName
);


    taskCount++;


    return RTOS_OK;
}







void taskYIELD(void)
{
    Task_Yield();
}





void taskENTER_CRITICAL(void)
{
    Disable_Interrupts();
}





void taskEXIT_CRITICAL(void)
{
    Enable_Interrupts();
}