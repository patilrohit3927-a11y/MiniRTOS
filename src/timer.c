#include "timer.h"



#define MAX_TIMERS 10



static SoftwareTimer *timerList[MAX_TIMERS];


static uint8_t timerCount = 0;



void Timer_Create(
        SoftwareTimer *timer,
        uint32_t period,
        TimerCallback callback
)
{

    timer->period = period;


    timer->counter = period;


    timer->active = 0;


    timer->callback = callback;

}



void Timer_Start(
        SoftwareTimer *timer
)
{

    if(timerCount < MAX_TIMERS)
    {

        timerList[timerCount] = timer;


        timerCount++;


        timer->counter = timer->period;


        timer->active = 1;

    }

}



void Timer_Stop(
        SoftwareTimer *timer
)
{

    timer->active = 0;

}



void Timer_Tick(void)
{

    uint8_t i;



    for(i = 0; i < timerCount; i++)
    {

        SoftwareTimer *timer = timerList[i];



        if(timer->active)
        {


            if(timer->counter > 0)
            {

                timer->counter--;

            }



            if(timer->counter == 0)
            {


                if(timer->callback != 0)
                {

                    timer->callback();

                }



                /*
                    Restart periodic timer

                */

                timer->counter = timer->period;


            }

        }

    }

}