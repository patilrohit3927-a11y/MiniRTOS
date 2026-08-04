#ifndef SEMAPHORE_H
#define SEMAPHORE_H


#include <stdint.h>
#include "task.h"



typedef struct
{

    uint8_t count;

    TCB *waitingTask;


} Semaphore;




void Semaphore_Create
(
    Semaphore *sem,
    uint8_t initialValue
);



int Semaphore_Take
(
    Semaphore *sem
);



int Semaphore_Give
(
    Semaphore *sem
);



#endif