#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include "task.h"

#define QUEUE_SIZE 10

typedef struct
{
    uint32_t buffer[QUEUE_SIZE];

    uint32_t head;
    uint32_t tail;
    uint32_t count;

    TCB *waitingSender;
    TCB *waitingReceiver;

} Queue;

void Queue_Create(Queue *queue);

int Queue_Send
(
    Queue *queue,
    uint32_t data
);

int Queue_Receive
(
    Queue *queue,
    uint32_t *data
);

#endif