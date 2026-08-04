#include "heap.h"



#define HEAP_SIZE 1024



static uint8_t heap[HEAP_SIZE];


static uint32_t heapIndex = 0;



void Heap_Init(void)
{

    heapIndex = 0;

}



void *pvPortMalloc(
        uint32_t size
)
{

    void *ptr;



    if((heapIndex + size) > HEAP_SIZE)
    {

        return 0;

    }



    ptr = &heap[heapIndex];



    heapIndex += size;



    return ptr;

}



void vPortFree(
        void *ptr
)
{

    /*
        Simple heap

        Free not implemented yet

        Added for RTOS API compatibility

    */


}