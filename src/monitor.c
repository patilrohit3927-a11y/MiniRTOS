#include "monitor.h"
#include "scheduler.h"
#include "uart.h"
#include "cortex_m.h"



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
}






/*
    Print task name with fixed width

    NAME column = 16 characters
*/

static void PrintTaskName(char *name)
{
    int i = 0;



    UART_Print(name);



    while(name[i])
    {
        i++;
    }



    while(i < 16)
    {
        UART_Print(" ");
        i++;
    }

}







void RTOS_TaskMonitor(void)
{

    TCB *task;



    /*
        Protect monitor output

        Prevent other tasks
        from printing in between
    */

    Disable_Interrupts();





    UART_Print("\r\n");
    UART_Print("====================================\r\n");
    UART_Print("        MiniRTOS Task Monitor\r\n");
    UART_Print("====================================\r\n");



    UART_Print("NAME            STATE\r\n");

    UART_Print("------------------------------------\r\n");







    /*
        Current Running Task
    */


    if(currentTask != 0)
    {

        PrintTaskName(currentTask->name);


        PrintState(currentTask->state);


        UART_Print("\r\n");

    }







    /*
        Ready Tasks
    */


    task = Scheduler_GetReadyList();



    while(task)
    {

        /*
            Current task already printed
        */

        if(task != currentTask)
        {

            PrintTaskName(task->name);


            PrintState(task->state);


            UART_Print("\r\n");

        }



        task = task->next;

    }








    /*
        Blocked Tasks
    */


    task = Scheduler_GetBlockedList();



    while(task)
    {


        PrintTaskName(task->name);



        PrintState(task->state);



        UART_Print("\r\n");



        task = task->blockNext;

    }








    UART_Print("------------------------------------\r\n");



    Enable_Interrupts();


}