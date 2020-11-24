#ifndef UART_LIBRARY
#define UART_LIBRARY
#include "stm32f429xx.h"

// APB2: USART1, USART6
// APB1: USART2, USART3, UART4, UART5, UART7, UART8
int UART_begin(USART_TypeDef* uartPort, uint32_t baudRateRegister);

void UART_writeByte(USART_TypeDef* uartPort, uint8_t txByte);
void UART_writeString(USART_TypeDef* uartPort, char *msg, ...);

#endif

