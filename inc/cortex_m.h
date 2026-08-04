#ifndef CORTEX_M_H
#define CORTEX_M_H

#include <stdint.h>

/*-----------------------------------------------------------
 * System Control Block Registers
 *----------------------------------------------------------*/

#define SCB_ICSR        (*(volatile uint32_t *)0xE000ED04UL)

/* PendSV Set Pending Bit */
#define SCB_ICSR_PENDSVSET_Msk    (1UL << 28)

/*-----------------------------------------------------------
 * Cortex-M Kernel Functions
 *----------------------------------------------------------*/

/* Trigger PendSV exception */
void Trigger_PendSV(void);

/* Start first task using PSP */
void Start_First_Task(uint32_t *stackPointer);

/* Critical Section */
void Disable_Interrupts(void);
void Enable_Interrupts(void);

#endif