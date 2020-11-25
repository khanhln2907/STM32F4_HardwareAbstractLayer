#include "stm32f429xx.h"
#include "main.h"

void GPIO_init(void){
	// Setup port G
	// Output for LED: GPIOG13_14
	GPIO_GeneralConfig myPinConfig;
	myPinConfig.pinMask = GPIO_PIN(13) | GPIO_PIN(14);
	myPinConfig.mode = OUTPUT;
	myPinConfig.pull = GPIO_PULL_NONE;
	myPinConfig.outputType = GPIO_OTYPE_PUSHPULL;
	myPinConfig.outputSpeed = GPIO_OSPEED_MED;
	GPIO_setupPort(GPIOG, myPinConfig);	
}

void GPIO_Interrupt_Init(void){
	GPIO_InterruptConfig myConfig;
	myConfig.isSoftwareIntEvt = false;
	myConfig.pinNumber = 0;
	myConfig.trigger = GPIO_EXTI_TRIGGER_RISING;
	GPIO_setupInterruptPin(GPIOA, myConfig);
}

// GPIO and TIMER
int main(){
	SysClockConfig();
	
	// Initiallize GPIO
	GPIO_init();

	// Initialize interrupt
	GPIO_Interrupt_Init();	
	GPIO_digitalWrite(GPIOG, 13, HIGH);
	
	// Enable Interrupt
	GPIO_enableInterruptPin(0, EXTI0_IRQn);
	
	// Timer 4
	//TIM4Config()();
	TIM6Config();
	
	// USART & Debugger
	uint32_t brr9600APB2 = 585 << 4 | 0xF;
	UART_begin(USART1, brr9600APB2);
	UART_begin(USART6, brr9600APB2);
	
	uint32_t brr9600APB1 = 24 << 4 | 7; // 115200
	UART_begin(USART2, brr9600APB1);
	UART_begin(USART3, brr9600APB1);
	
	// I2C
	I2C_Config(I2C1);
	I2C_Config(I2C3);

	uint8_t slaveAddress = 0x4E;
	uint8_t i2cTXBuffer[1] = {0b10101010};
	unsigned int i2cTXLen = (unsigned int) strlen(i2cTXBuffer);
	
	// Loop
	while(1){
		//I2C_transmit(I2C1, slaveAddress, i2cTXBuffer, i2cTXLen);
		I2C_transmit(I2C3, slaveAddress, i2cTXBuffer, i2cTXLen);

		GPIO_pinToggle(GPIOG, 13);
		//printMsg("Hello World !");
//		UART_writeString(USART1, "Hello USART 1! ");
//		UART_writeString(USART2, "Hello USART 2! ");
//		UART_writeString(USART3, "Hello USART 3! ");
//		UART_writeString(USART6, "Hello USART 6! ");
		uint8_t byteRead = UART_readByte(USART6);
		if(byteRead != 0)
			UART_writeByte(USART6, byteRead);
		delay_ms(20);
	}
}


void EXTI0_IRQHandler(){
	// Must !
	GPIO_clearInterruptPin(0);
	
	// Do something
	// ...
	GPIO_pinToggle(GPIOG, 14);
}

