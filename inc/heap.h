#ifndef HEAP_H
#define HEAP_H


#include <stdint.h>



void *pvPortMalloc(
        uint32_t size
);



void vPortFree(
        void *ptr
);



void Heap_Init(void);



#endif