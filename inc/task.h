#ifndef TASK_H
#define TASK_H

#include <stdint.h>


#define MAX_TASKS          16
#define TASK_STACK_SIZE    256


/*-----------------------------------------------------------
 * Task States
 *----------------------------------------------------------*/

typedef enum
{
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SUSPENDED

} TaskState;


/*-----------------------------------------------------------
 * Block Reason
 *----------------------------------------------------------*/

typedef enum
{
    BLOCK_NONE,
    BLOCK_DELAY,
    BLOCK_SEMAPHORE,
    BLOCK_MUTEX,
    BLOCK_QUEUE

} BlockReason;


/*-----------------------------------------------------------
 * Task Control Block
 *----------------------------------------------------------*/

typedef struct TCB
{

    /* Current PSP */

    uint32_t *stackPointer;


    /* Bottom of allocated stack */

    uint32_t *stackBase;


    /* Stack size */

    uint32_t stackSize;


    /* Entry function */

    void (*taskFunction)(void);


    /* Delay ticks */

    uint32_t delayTicks;


    /* Time slice */

    uint32_t timeSlice;


    /* Task priority */

    uint8_t priority;


    /*
        Original priority.

        Used by priority inheritance.

        When a task temporarily inherits a higher
        priority, originalPriority remembers its
        normal priority.
    */

    uint8_t originalPriority;


    /* Current state */

    TaskState state;


    /* Why task is blocked */

    BlockReason blockReason;


    /* Task name */

    char name[16];


    /* Ready list link */

    struct TCB *next;


    /* Blocked list link */

    struct TCB *blockNext;

} TCB;


/*-----------------------------------------------------------
 * Current running task
 *----------------------------------------------------------*/

extern TCB *currentTask;


/*-----------------------------------------------------------
 * Task APIs
 *----------------------------------------------------------*/

void Task_Create
(
    TCB *task,
    void (*function)(void),
    uint32_t *stack,
    uint32_t size,
    uint8_t priority,
    const char *name
);


void Task_Delay(uint32_t ticks);


void Task_Yield(void);


#endif