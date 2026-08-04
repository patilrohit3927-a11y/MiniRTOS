#include "cortex_m.h"
#include <stdint.h>


#define ICSR   (*(volatile uint32_t *)0xE000ED04)



/*
    Disable global interrupts
*/
void Disable_Interrupts(void)
{
    __asm volatile
    (
        "cpsid i"
    );
}



/*
    Enable global interrupts
*/
void Enable_Interrupts(void)
{
    __asm volatile
    (
        "cpsie i"
    );
}



/*
    Trigger PendSV exception
*/
void Trigger_PendSV(void)
{
    ICSR = (1 << 28);
    __asm volatile("dsb");
    __asm volatile("isb");
}



/*
    Start first task
*/
__attribute__((naked))
void Start_First_Task(uint32_t *stackPointer)
{
    __asm volatile
    (
        "msr psp, r0          \n"

        "mov r0, #2           \n"
        "msr CONTROL, r0      \n"

        "isb                  \n"

        /* Restore R4-R11 */
        "pop {r4-r11}         \n"

        /* Restore exception frame */
        "pop {r0-r3}          \n"
        "pop {r12}            \n"
        "pop {lr}             \n"
        "pop {pc}             \n"
    );
}