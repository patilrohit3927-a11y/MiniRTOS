#include "uart.h"



#define UART0_BASE 0x4000C000



#define UART_DR     (*(volatile uint32_t *)(UART0_BASE + 0x000))

#define UART_FR     (*(volatile uint32_t *)(UART0_BASE + 0x018))



#define UART_FR_TXFF (1 << 5)



void UART_Init(void)
{
    /*
        QEMU LM3S6965 UART
        already initialized by machine firmware.

        No clock setup required.
    */
}





void UART_PutChar(char c)
{

    while(UART_FR & UART_FR_TXFF)
    {

    }


    UART_DR = c;

}





void UART_Print(const char *str)
{

    while(*str)
    {

        UART_PutChar(*str);

        str++;

    }

}