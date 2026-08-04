#include "rtos.h"
#include "idle.h"


#include "task.h"

void IdleTask(void)
{
    while(1)
    {
        Task_Yield();
    }
}