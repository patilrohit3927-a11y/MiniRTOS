#include "monitor.h"
#include "scheduler.h"
#include "uart.h"
#include "rtos.h"
#include "cortex_m.h"


/*-----------------------------------------------------------
 * Print Task State
 *----------------------------------------------------------*/

static void PrintState(TaskState state)
{
    if(state == TASK_READY)
    {
        UART_Print("READY");
    }
    else if(state == TASK_RUNNING)
    {
        UART_Print("RUNNING");
    }
    else if(state == TASK_BLOCKED)
    {
        UART_Print("BLOCKED");
    }
    else if(state == TASK_SUSPENDED)
    {
        UART_Print("SUSPENDED");
    }
    else
    {
        UART_Print("UNKNOWN");
    }
}


/*-----------------------------------------------------------
 * Print Task Name
 *
 * NAME column width = 17 characters
 *----------------------------------------------------------*/

static void PrintTaskName(const char *name)
{
    int i = 0;

    UART_Print(name);

    while(name[i])
    {
        i++;
    }

    while(i < 17)
    {
        UART_Print(" ");
        i++;
    }
}


/*-----------------------------------------------------------
 * Print Task Priority
 *
 * PRIORITY column width = 16 characters
 *----------------------------------------------------------*/

static void PrintPriority(uint8_t priority)
{
    const char *text;
    int i = 0;


    if(priority == PRIORITY_HIGH)
    {
        text = "HIGH";
    }
    else if(priority == PRIORITY_MEDIUM)
    {
        text = "MED";
    }
    else if(priority == PRIORITY_LOW)
    {
        text = "LOW";
    }
    else if(priority == PRIORITY_IDLE)
    {
        text = "IDLE";
    }
    else
    {
        text = "UNKNOWN";
    }


    UART_Print(text);


    while(text[i])
    {
        i++;
    }


    while(i < 16)
    {
        UART_Print(" ");
        i++;
    }
}


/*-----------------------------------------------------------
 * Print Complete Task Information
 *----------------------------------------------------------*/

static void PrintTaskInfo(TCB *task)
{
    if(task == 0)
        return;

    PrintTaskName(task->name);

    PrintPriority(task->priority);

    PrintState(task->state);

    UART_Print("\r\n");
}


/*-----------------------------------------------------------
 * RTOS Task Monitor
 *----------------------------------------------------------*/

void RTOS_TaskMonitor(void)
{
    TCB *task;


    /*
     * Protect the complete monitor output so another task
     * cannot interrupt the monitor while it is printing.
     */

    Disable_Interrupts();


    UART_Print("\r\n");

    UART_Print("==============================================\r\n");

    UART_Print("             MiniRTOS Task Monitor\r\n");

    UART_Print("==============================================\r\n");

    UART_Print("NAME             PRIORITY        STATE\r\n");

    UART_Print("----------------------------------------------\r\n");


    /*-------------------------------------------------------
     * Running Task
     *------------------------------------------------------*/

    if(currentTask != 0)
    {
        PrintTaskInfo(currentTask);
    }


    /*-------------------------------------------------------
     * Ready Tasks
     *------------------------------------------------------*/

    task = Scheduler_GetReadyList();

    while(task)
    {
        if(task != currentTask)
        {
            PrintTaskInfo(task);
        }

        task = task->next;
    }


    /*-------------------------------------------------------
     * Blocked Tasks
     *------------------------------------------------------*/

    task = Scheduler_GetBlockedList();

    while(task)
    {
        PrintTaskInfo(task);

        task = task->blockNext;
    }


    UART_Print("----------------------------------------------\r\n");


    /*
     * Resume interrupts and scheduling.
     */

    Enable_Interrupts();
}