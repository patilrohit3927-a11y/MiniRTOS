#ifndef UART_H
#define UART_H

#include <stdint.h>


void UART_Init(void);

void UART_PutChar(char c);

void UART_Print(const char *str);


#endif