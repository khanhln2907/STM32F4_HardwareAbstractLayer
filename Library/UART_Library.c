#include "UART_Library.h"

#include <assert.h>

// For boolean
#include <stdbool.h>
#include <stddef.h>

// String intepretation for debugging
#include <stdint.h>
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

#define UART_OK 1

#define UART_ERROR 100
#define UART_PORT_ERROR UART_ERROR + 1

// Initialize UART Port with a specific baudrate
// Currently sysTick can not be read so there exist a parameter systemFreq
int UART_begin(USART_TypeDef* uartPort, uint32_t baudRateRegister){
	GPIO_TypeDef *port = NULL;

	// 1. Setup Clock and GPIO
	if(uartPort == USART1){
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		
		// Init GPIOA9: TX - GPIOA10: RX
		port = GPIOA;
		GPIO_GeneralConfig configGPIOA;
		configGPIOA.pinMask = GPIO_PIN(9) | GPIO_PIN(10);
		configGPIOA.mode = ALTERNATE_FUNCTION;
		configGPIOA.AFSelect = 7;
		configGPIOA.outputSpeed = GPIO_OSPEED_VERY_HIGH;
		configGPIOA.outputType = GPIO_OTYPE_OPENDRAIN;
		GPIO_setupPort(GPIOA, configGPIOA);
		
	} else if (uartPort == USART2) {
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		
		// Init GPIOA2: TX - GPIOA3: RX
		port = GPIOA;
		GPIO_GeneralConfig configGPIOA;
		configGPIOA.pinMask = GPIO_PIN(2) | GPIO_PIN(3);
		configGPIOA.mode = ALTERNATE_FUNCTION;
		configGPIOA.AFSelect = 7;
		configGPIOA.outputSpeed = GPIO_OSPEED_HIGH;
		configGPIOA.outputType = GPIO_OTYPE_PUSHPULL;
		GPIO_setupPort(GPIOA, configGPIOA);
		
	} else if (uartPort == USART3) {
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
		
		// Init GPIOB10: TX, Remap -> GPIOD8: TX - GPIOD9: RX
		port = GPIOB;
		GPIO_GeneralConfig configGPIOB;
		configGPIOB.pinMask = GPIO_PIN(10) | GPIO_PIN(11);
		configGPIOB.mode = ALTERNATE_FUNCTION;
		configGPIOB.AFSelect = 7;
		configGPIOB.outputSpeed = GPIO_OSPEED_VERY_HIGH;
		configGPIOB.outputType = GPIO_OTYPE_PUSHPULL;
		GPIO_setupPort(GPIOC, configGPIOB);
		
	} else if (uartPort == UART4) {
		RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
	} else if (uartPort == UART5) {
		RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
	} else if (uartPort == USART6) {
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
		
		// Init GPIOC6: TX - GPIOC7: RX, Remap -> GPIOD8: TX - GPIOD9: RX
		port = GPIOC;
		GPIO_GeneralConfig configGPIOC;
		configGPIOC.pinMask = GPIO_PIN(6) | GPIO_PIN(7); 
		configGPIOC.mode = ALTERNATE_FUNCTION;
		configGPIOC.AFSelect = 8;
		configGPIOC.outputSpeed = GPIO_OSPEED_VERY_HIGH;
		configGPIOC.outputType = GPIO_OTYPE_PUSHPULL;
		GPIO_setupPort(GPIOC, configGPIOC);
		
	} else if (uartPort == UART7) {
		RCC->APB1ENR |= RCC_APB1ENR_UART7EN;
	} else if (uartPort == UART8) {
		RCC->APB1ENR |= RCC_APB1ENR_UART8EN;
	} else {
		assert(!UART_PORT_ERROR);
		return UART_PORT_ERROR;
	}
	
	// Ensure that a specific GPIO was already assigned
	// If error occurs, gpio of UART must be defined above
	assert(port != NULL); 
	
	// 2. Enable UART 
	uartPort->CR1 = 00; // Clear register
	uartPort->CR1 |= USART_CR1_UE; // Enable UART Bit

	//	M: Word length
	//	This bit determines the word length. It is set or cleared by software.
	//	0: 1 Start bit, 8 Data bits, n Stop bit
	//	1: 1 Start bit, 9 Data bits, n Stop bit
	uartPort->CR1 &= ~USART_CR1_M;

	// 3. Configurate BaudRate
	/*
	Tx/Rx Baud = f_clk / (8 x 2 - OVER8) x USARTDIV
	
	USARTDIV[0:31]
	
	When OVER8=0, the fractional part is coded on 4 bits and programmed by the
	DIV_fraction[3:0] bits in the USART_BRR register
	
	When OVER8=1, the fractional part is coded on 3 bits and programmed by the
	DIV_fraction[2:0] bits in the USART_BRR register, and bit DIV_fraction[3] must be kept
	cleared.
	*/
	//assert(baudRate > 0);
	
	// Over8 is not yet implemented

	// TODO LATER
	//	#define OVER8 0
	//	double modulo = systemFreq % (8 * (2 - OVER8));
	//	int divMantissa = (systemFreq - modulo) / (8 * (2 - OVER8));
	//	double divFraction = modulo / (double)(8 * (2 - OVER8));
	//	
	//	double brrFloat = systemFreq / (8 * (2 - OVER8)); // iii.fff
	//		
	
	// Example
	// 90Mhz, Baud = 115200 
	// clkPer / (baudR * 16) = 90Mhz / (9600 x 16) = 585.9375
	// 585 << 4 | 0xF; 
	uartPort->BRR = baudRateRegister; //585 << 4 | 0xF; //0xF;  585 << 4
	
	// 4. Enable Transmitter / Receiver
	uartPort->CR1 |= USART_CR1_TE;
	uartPort->CR1 |= USART_CR1_RE;
	
	return UART_OK;
}

// Write 1 Byte through Serial Port
void UART_transmitByte(USART_TypeDef* uartPort, uint8_t txByte){
		uartPort->DR = txByte;
		while(!(uartPort->SR & USART_SR_TC));
}

void UART_transmitString(USART_TypeDef* uartPort, char *msg, ...){
	char txMsg[80];
	
	va_list args;
	// Parse arguments
	va_start(args, msg);
	
	// Put into buffer for transmission
	vsprintf(txMsg, msg, args);
	
	for (int i = 0; i< strlen(txMsg); i++){
		uartPort->DR = txMsg[i];
		while(!(uartPort->SR & USART_SR_TC)); // TC or TXE ?
	}
}

// Blocking method to get 1 Byte from UART_RX. Temporary
uint8_t UART_readByte(USART_TypeDef* uartPort){
//	// 1. Wait for RXNE bit to be set
//	while(!(uartPort->SR & USART_SR_RXNE)){
//		//UART_writeString(uartPort, "Waiting");
//	}
//	char rxByte = uartPort->DR;
//		return rxByte;
	
	if(!(uartPort->SR & USART_SR_RXNE))
		return 0; // Nothing to read
	else{
		uint8_t rxByte = uartPort->DR;
		return rxByte;
	}
}














