#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void SysTick_Init(uint32_t cpuFrequency);

void SysTick_Handler(void);

#endif