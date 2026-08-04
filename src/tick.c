#include "tick.h"

#include "task.h"

#include "scheduler.h"



extern TCB *currentTask;



volatile uint32_t tickCount = 0;



void SysTick_Handler(void)
{

    tickCount++;


    Scheduler_Tick();


    if(currentTask != 0)
    {

        currentTask->state = TASK_READY;

    }


}