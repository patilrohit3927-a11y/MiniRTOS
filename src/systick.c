#include "systick.h"
#include "scheduler.h"
#include "cortex_m.h"

#define SYSTICK_BASE    0xE000E010UL

#define SYST_CSR    (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYST_RVR    (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYST_CVR    (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))

#define SYSTICK_ENABLE      (1U << 0)
#define SYSTICK_TICKINT     (1U << 1)
#define SYSTICK_CLKSOURCE   (1U << 2)



void SysTick_Init(uint32_t cpuFrequency)
{
    uint32_t reload;

    /*
        Generate 1ms tick
    */

    reload = (cpuFrequency / 1000U) - 1U;

    SYST_RVR = reload;

    SYST_CVR = 0;

    SYST_CSR =
        SYSTICK_ENABLE |
        SYSTICK_TICKINT |
        SYSTICK_CLKSOURCE;
}



/*
    SysTick Interrupt
*/

void SysTick_Handler(void)
{
    Scheduler_Tick();
}