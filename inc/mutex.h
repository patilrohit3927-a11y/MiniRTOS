#ifndef MUTEX_H
#define MUTEX_H

#include "task.h"


typedef struct
{
    uint8_t locked;

    TCB *owner;

    TCB *waitingTask;

} Mutex;


/*-----------------------------------------------------------
 * Mutex APIs
 *----------------------------------------------------------*/

void Mutex_Create(Mutex *mutex);


int Mutex_Take(Mutex *mutex);


int Mutex_Give(Mutex *mutex);


#endif