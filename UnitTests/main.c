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



#define L3GD20_I2C I2C3
#define L3GD20_W_ADDRESS  0xD4
#define L3GD20_R_ADDRESS  0xD5
#define L3GD20_WHO_AM_I 0x0F

#define MPU6050_I2C I2C1
#define MPU6050_ADDRESS_READ 0x68 << 1 | 1

#define I2C_DISPLAY_ADRESS 0x4E

void readL3GD20(){
	// Start
	_I2C_sendStart(L3GD20_I2C);
	// Adress
	_I2C_sendAddress(L3GD20_I2C, L3GD20_R_ADDRESS);
	
	// Receive Single Byte from L3GD20	
	//uint8_t rxByte = I2C_receiveByte(L3GD20_I2C, L3GD20_R_ADDRESS);
}


void readMPU6050(){
	// Start
	_I2C_sendStart(MPU6050_I2C);
	// Adress
	_I2C_sendAddress(MPU6050_I2C, MPU6050_ADDRESS_READ);
	//_I2C_sendSingleByte(L3GD20_I2C, L3GD20_WHO_AM_I); // read from Who_am_I register
	
	// Receive Single Byte from L3GD20	
	//uint8_t rxByte = I2C_receiveByte(L3GD20_I2C, L3GD20_R_ADDRESS);
}

// GPIO and TIMER
int main(){
	SysClockConfig();
	
	// GPIO Output ************************
	GPIO_init();

	// GPIO interrupt *********************
	GPIO_Interrupt_Init();	
	GPIO_digitalWrite(GPIOG, 13, HIGH);	
	// Enable Interrupt
	GPIO_enableInterruptPin(0, EXTI0_IRQn);
	
	// Timer ******************************
	TIM4Config();
	TIM6Config();
	
	// USART & Debugger *******************
	uint32_t brr9600APB2 = 585 << 4 | 0xF;
	UART_begin(USART1, brr9600APB2);
	UART_begin(USART6, brr9600APB2);
	
	uint32_t brr9600APB1 = 24 << 4 | 7; // 115200
	UART_begin(USART2, brr9600APB1);
	UART_begin(USART3, brr9600APB1);
	
	// I2C ********************************
	//I2C_Config(I2C1);
	//	I2C_transmit(I2C1, slaveAddress, i2cTXBuffer, i2cTXLen);
	//	uint8_t slaveAddress = I2C_DISPLAY_ADRESS;
	//	const uint8_t i2cTXBuffer[1] = {0b00000001};
	//	size_t i2cTXLen = 1;

	I2C_Config(MPU6050_I2C);
	I2C_Config(L3GD20_I2C);

	// Loop
	while(1){
		// GPIO
		GPIO_pinToggle(GPIOG, 13);

		// UART
		//		UART_writeString(USART1, "USART 1! ");
		//		UART_writeString(USART2, "USART 2! ");
		//		UART_writeString(USART3, "USART 3! ");
		//		UART_writeString(USART6, "USART 6! ");
		uint8_t byteRead = UART_readByte(USART6);	
		if(byteRead != 0)
			UART_writeByte(USART6, byteRead);
		
		// I2C
		//I2C_transmit(I2C1, slaveAddress, &i2cTXBuffer[0], i2cTXLen);

		readMPU6050();	
		//readL3GD20();
		
		delay_ms(50);
	}
}


void EXTI0_IRQHandler(){
	// Must !
	GPIO_clearInterruptPin(0);
	
	// Do something
	// ...
	GPIO_pinToggle(GPIOG, 14);
}

