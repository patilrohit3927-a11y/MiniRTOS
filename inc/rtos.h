#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>

#include "task.h"
#include "scheduler.h"
#include "queue.h"
#include "semaphore.h"
#include "mutex.h"
#include "event.h"
#include "heap.h"
#include "timer.h"



/*-----------------------------------------------------------
 * RTOS Status
 *----------------------------------------------------------*/

typedef enum
{
    RTOS_OK = 0,

    RTOS_ERROR,

    RTOS_INVALID_PARAMETER,

    RTOS_OUT_OF_TASKS

} RTOS_Status;



/*-----------------------------------------------------------
 * Task Priorities
 *----------------------------------------------------------*/

typedef enum
{
    PRIORITY_IDLE = 0,

    PRIORITY_LOW = 1,

    PRIORITY_MEDIUM = 2,

    PRIORITY_HIGH = 3

} TaskPriority;



/*-----------------------------------------------------------
 * Kernel APIs
 *----------------------------------------------------------*/

void RTOS_Init(void);

void RTOS_Start(void);



/*-----------------------------------------------------------
 * Task APIs
 *----------------------------------------------------------*/

RTOS_Status xTaskCreate
(
    void (*taskFunction)(void),
    const char *taskName,
    uint8_t priority
);



void vTaskDelay
(
    uint32_t ticks
);



void taskYIELD(void);



/*-----------------------------------------------------------
 * Critical Section
 *----------------------------------------------------------*/

void taskENTER_CRITICAL(void);

void taskEXIT_CRITICAL(void);



#endif