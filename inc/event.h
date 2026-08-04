#ifndef EVENT_H
#define EVENT_H


#include <stdint.h>



typedef struct
{

    uint32_t flags;


} Event;



void Event_Create(
        Event *event
);



void Event_Set(
        Event *event,
        uint32_t flag
);



void Event_Clear(
        Event *event,
        uint32_t flag
);



int Event_Wait(
        Event *event,
        uint32_t flag
);



#endif