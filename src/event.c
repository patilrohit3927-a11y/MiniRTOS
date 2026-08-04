#include "event.h"



void Event_Create(
        Event *event
)
{

    event->flags = 0;

}



void Event_Set(
        Event *event,
        uint32_t flag
)
{

    event->flags |= flag;

}



void Event_Clear(
        Event *event,
        uint32_t flag
)
{

    event->flags &= ~flag;

}



int Event_Wait(
        Event *event,
        uint32_t flag
)
{

    if((event->flags & flag) != 0)
    {

        return 0;

    }


    return -1;

}