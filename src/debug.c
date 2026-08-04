#include "debug.h"
#include "uart.h"



void Debug_Init(void)
{
    UART_Init();
}



void Debug_Print(const char *message)
{
    UART_Print(message);
}