#include "rtos.h"
#include "systick.h"
#include "semaphore.h"
#include "mutex.h"
#include "uart.h"
#include "debug.h"
#include "queue.h"
#include "monitor.h"

/*-----------------------------------------------------------
 * Task Function Prototypes
 *----------------------------------------------------------*/

void LEDTask(void);
void UARTTask(void);
void SensorTask(void);
void ApplicationTask(void);
void MemoryTask(void);

void SemaphoreReceiverTask(void);
void SemaphoreSenderTask(void);

void MutexTask1(void);
void MutexTask2(void);

void QueueSenderTask(void);
void QueueReceiverTask(void);


extern void IdleTask(void);



/*-----------------------------------------------------------
 * IPC Objects
 *----------------------------------------------------------*/

Semaphore testSemaphore;

Mutex testMutex;

Queue testQueue;


/*-----------------------------------------------------------
 * Application Tasks
 *----------------------------------------------------------*/


void LEDTask(void)
{
    while(1)
    {
        Debug_Print("[LED] Task Running\r\n");

        vTaskDelay(100);
    }
}




void UARTTask(void)
{
    while(1)
    {
        Debug_Print("[UART] Task Running\r\n");

        vTaskDelay(100);
    }
}


void SensorTask(void)
{
    while(1)
    {
        Debug_Print("[SENSOR] Task Running\r\n");

        vTaskDelay(100);
    }
}



void ApplicationTask(void)
{
    while(1)
    {
        Debug_Print("[APP] Task Running\r\n");


        RTOS_TaskMonitor();


        vTaskDelay(100);
    }
}



void MemoryTask(void)
{
    while(1)
    {
        Debug_Print("[MEMORY] Task Running\r\n");

        vTaskDelay(200);
    }
}







/*-----------------------------------------------------------
 * Semaphore Test
 *----------------------------------------------------------*/


void SemaphoreReceiverTask(void)
{
    while(1)
    {
        Semaphore_Take(&testSemaphore);

        Debug_Print("[SEM] Receiver Received Signal\r\n");

        vTaskDelay(50);
    }
}




void SemaphoreSenderTask(void)
{
    while(1)
    {
        Debug_Print("[SEM] Sender Giving Signal\r\n");

        Semaphore_Give(&testSemaphore);

        vTaskDelay(100);
    }
}







/*-----------------------------------------------------------
 * Mutex Test
 *----------------------------------------------------------*/
void MutexTask1(void)
{
    while(1)
    {
        if(Mutex_Take(&testMutex) == 0)
        {
            Debug_Print("[MUTEX] Task1 Locked\r\n");


            vTaskDelay(50);


            Mutex_Give(&testMutex);


            Debug_Print("[MUTEX] Task1 Released\r\n");
        }


        vTaskDelay(100);
    }
}

void MutexTask2(void)
{
    while(1)
    {
        if(Mutex_Take(&testMutex) == 0)
        {
            Debug_Print("[MUTEX] Task2 Acquired\r\n");


            vTaskDelay(50);


            Mutex_Give(&testMutex);


            Debug_Print("[MUTEX] Task2 Released\r\n");
        }
        else
        {
            Debug_Print("[MUTEX] Task2 Waiting\r\n");
        }


        vTaskDelay(100);
    }
}

/*-----------------------------------------------------------
 * Queue Test
 *----------------------------------------------------------*/
void QueueSenderTask(void)
{
    uint32_t count = 0;


    while(1)
    {

        if(Queue_Send(&testQueue, count) == 0)
        {
            Debug_Print("[QUEUE] Sent: ");

            UART_PutChar('0' + (count % 10));

            Debug_Print("\r\n");
        }


        count++;


        vTaskDelay(100);

    }
}

void QueueReceiverTask(void)
{
    uint32_t data;


    while(1)
    {

        if(Queue_Receive(&testQueue, &data) == 0)
        {

            Debug_Print("[QUEUE] Received: ");

            UART_PutChar('0' + (data % 10));

            Debug_Print("\r\n");

        }


        vTaskDelay(50);

    }
}



/*-----------------------------------------------------------
 * Main
 *----------------------------------------------------------*/


int main(void)
{
    RTOS_Status status;


    SysTick_Init(16000000);


    Debug_Print("\r\n");
    Debug_Print("====================================\r\n");
    Debug_Print("             MiniRTOS \r\n");
    Debug_Print("       ARM Cortex-M4 RTOS \r\n");
    Debug_Print("====================================\r\n");


    Debug_Print("\r\n");
    Debug_Print("System Initialization...\r\n");


    Debug_Print("[OK] UART Driver\r\n");
    Debug_Print("[OK] SysTick Timer\r\n");
    Debug_Print("[OK] Scheduler\r\n");
    Debug_Print("[OK] Memory Manager\r\n");
    Debug_Print("[OK] IPC Objects\r\n");


    Debug_Print("\r\n");


    RTOS_Init();
    Debug_Init();

    Debug_Print("================================\r\n");
    Debug_Print("        MiniRTOS Boot\r\n");
    Debug_Print("================================\r\n");



    Semaphore_Create
    (
        &testSemaphore,
        0
    );



    Mutex_Create
    (
        &testMutex
    );


    Queue_Create(&testQueue);


    status = xTaskCreate
    (
        LEDTask,
        "LED",
        PRIORITY_HIGH
    );

    if(status != RTOS_OK)
        while(1);






    status = xTaskCreate
    (
        UARTTask,
        "UART",
        PRIORITY_MEDIUM
    );

    if(status != RTOS_OK)
        while(1);






    status = xTaskCreate
    (
        SensorTask,
        "SENSOR",
        PRIORITY_MEDIUM
    );

    if(status != RTOS_OK)
        while(1);






    status = xTaskCreate
    (
        ApplicationTask,
        "APP",
        PRIORITY_LOW
    );

    if(status != RTOS_OK)
        while(1);






    status = xTaskCreate
    (
        MemoryTask,
        "MEMORY",
        PRIORITY_LOW
    );

    if(status != RTOS_OK)
        while(1);







    status = xTaskCreate
    (
        SemaphoreReceiverTask,
        "SEM_RX",
        PRIORITY_MEDIUM
    );

    if(status != RTOS_OK)
        while(1);







    status = xTaskCreate
    (
        SemaphoreSenderTask,
        "SEM_TX",
        PRIORITY_MEDIUM
    );

    if(status != RTOS_OK)
        while(1);








    status = xTaskCreate
    (
        MutexTask1,
        "MUTEX1",
        PRIORITY_MEDIUM
    );

    if(status != RTOS_OK)
        while(1);




    status = xTaskCreate
    (
        MutexTask2,
        "MUTEX2",
        PRIORITY_MEDIUM
    );

    if(status != RTOS_OK)
        while(1);
    

    status = xTaskCreate
    (
    QueueSenderTask,
    "QUEUE_TX",
    PRIORITY_MEDIUM
    );

    if(status != RTOS_OK)
       while(1);



 status = xTaskCreate
 (
    QueueReceiverTask,
    "QUEUE_RX",
    PRIORITY_MEDIUM
 );


  if(status != RTOS_OK)
    while(1);




 /*-----------------------------------------------------------
  * Scheduler Start Message
  *----------------------------------------------------------*/
 
 Debug_Print("====================================\r\n");
 Debug_Print("Tasks Created Successfully\r\n");
 Debug_Print("Starting Scheduler...\r\n");
 Debug_Print("====================================\r\n");
 Debug_Print("\r\n");
 


 RTOS_Start();
 
 

 while(1)
 {

 }
} 