#ifndef TIMER_H
#define TIMER_H


#include <stdint.h>



typedef void (*TimerCallback)(void);



typedef struct
{

    uint32_t period;


    uint32_t counter;


    uint8_t active;


    TimerCallback callback;



} SoftwareTimer;



void Timer_Create(
        SoftwareTimer *timer,
        uint32_t period,
        TimerCallback callback
);



void Timer_Start(
        SoftwareTimer *timer
);



void Timer_Stop(
        SoftwareTimer *timer
);



void Timer_Tick(void);



#endif