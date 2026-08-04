#include <stdint.h>


/*-----------------------------------------------------------
 * External symbols
 *----------------------------------------------------------*/

extern int main(void);


/*
    Real handlers from other files
*/

extern void PendSV_Handler(void);
extern void SysTick_Handler(void);



/*-----------------------------------------------------------
 * Stack Top
 *----------------------------------------------------------*/

extern uint32_t _estack;



/*-----------------------------------------------------------
 * Default Handler
 *----------------------------------------------------------*/

void Default_Handler(void)
{
    while(1)
    {

    }
}



/*-----------------------------------------------------------
 * Reset Handler
 *----------------------------------------------------------*/

void Reset_Handler(void)
{

    main();


    while(1)
    {

    }
}



/*-----------------------------------------------------------
 * Weak handlers for unused interrupts
 *----------------------------------------------------------*/

void NMI_Handler(void)
__attribute__((weak, alias("Default_Handler")));

void HardFault_Handler(void)
__attribute__((weak, alias("Default_Handler")));

void MemManage_Handler(void)
__attribute__((weak, alias("Default_Handler")));

void BusFault_Handler(void)
__attribute__((weak, alias("Default_Handler")));

void UsageFault_Handler(void)
__attribute__((weak, alias("Default_Handler")));

void SVC_Handler(void)
__attribute__((weak, alias("Default_Handler")));

void DebugMon_Handler(void)
__attribute__((weak, alias("Default_Handler")));




/*-----------------------------------------------------------
 * Vector Table
 *----------------------------------------------------------*/

__attribute__((section(".isr_vector")))
const uint32_t vector_table[] =
{

    /* Initial Stack Pointer */

    (uint32_t)&_estack,


    /* Cortex-M Exceptions */

    (uint32_t)Reset_Handler,

    (uint32_t)NMI_Handler,

    (uint32_t)HardFault_Handler,

    (uint32_t)MemManage_Handler,

    (uint32_t)BusFault_Handler,

    (uint32_t)UsageFault_Handler,


    0,

    0,

    0,

    0,


    (uint32_t)SVC_Handler,

    (uint32_t)DebugMon_Handler,


    0,


    /*
        PendSV
    */

    (uint32_t)PendSV_Handler,


    /*
        SysTick
    */

    (uint32_t)SysTick_Handler

};