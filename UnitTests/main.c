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

// This function will initialize SPI5
//SPI5:	
//- NSS:  PF6, PH5
//- SCK:  PF7
//- MISO: PF8
//- MOSI: PF9
// MEM CS: PC1
void SPI_Init(void){
	// Init PF7-9
	GPIO_GeneralConfig portFConfig;
	portFConfig.pinMask = GPIO_PIN(7) | GPIO_PIN(8) | GPIO_PIN(9);
	portFConfig.mode = ALTERNATE_FUNCTION;
	portFConfig.AFSelect = 5;
	portFConfig.pull = GPIO_PULL_DOWN;
	portFConfig.outputType = GPIO_OTYPE_PUSHPULL;
	portFConfig.outputSpeed = GPIO_OSPEED_VERY_HIGH;
	GPIO_setupPort(GPIOF, portFConfig);	
	
	// Init PC1
	GPIO_GeneralConfig portCConfig;
	portCConfig.pinMask = GPIO_PIN(1);
	portCConfig.mode = OUTPUT;
	//portCConfig.pull = GPIO_PULL_NONE;
	portCConfig.outputType = GPIO_OTYPE_PUSHPULL;
	portCConfig.outputSpeed = GPIO_OSPEED_VERY_HIGH;
	GPIO_setupPort(GPIOC, portCConfig);	
	
	// Initialize SPI5
	SPI_GeneralConfigType spi5Config;
	spi5Config.wireMode = SPI_2_LINE;
	spi5Config.operationMode = SPI_MASTER;
	spi5Config.baudPrescaler = SPI_BAUD_DIV_32;
	spi5Config.clockPhase = SPI_CPHA_1_FALLING_EGDE;
	spi5Config.clockPol = SPI_CPOL_1;
	spi5Config.dataFormat = SPI_MSBFIRST;
	spi5Config.softwareNSSEnable = false;
	SPI_Config(SPI5, spi5Config);
}


#define L3GD20_I2C I2C3
#define L3GD20_W_ADDRESS  0xD4
#define L3GD20_R_ADDRESS  0xD5
#define L3GD20_WHO_AM_I 0x0F

#define MPU6050_I2C I2C1
#define MPU6050_ADDRESS_WRITE 0x68 << 1 | 0
#define MPU6050_ADDRESS_READ 0x68 << 1 | 1
#define MPU6050_ADDRESS_WHOAMI 0x75 


#define I2C_DISPLAY_ADRESS 0x4E

void readL3GD20(){
	// Start
	_I2C_sendStart(L3GD20_I2C);
	// Adress
	_I2C_sendAddress(L3GD20_I2C, L3GD20_R_ADDRESS);
	
	// Receive Single Byte from L3GD20	
	// uint8_t rxByte = I2C_receiveByte(L3GD20_I2C, L3GD20_R_ADDRESS);
}


void readMPU6050(){
	// Start
	_I2C_sendStart(MPU6050_I2C);
	
	// Send write to MPU6050
	_I2C_sendAddress(MPU6050_I2C, MPU6050_ADDRESS_WRITE);

	// Send Internal Register Address
	_I2C_sendSingleByte(MPU6050_I2C, MPU6050_ADDRESS_WHOAMI);
	
	// Send Restart
	_I2C_sendStart(MPU6050_I2C);

	// Send read to MPU6050
	_I2C_sendAddress(MPU6050_I2C, MPU6050_ADDRESS_READ);
	
	// Receive Single Byte from MPU	
	uint8_t rxByte = I2C_receiveByte(MPU6050_I2C);
	
	_I2C_sendStop(MPU6050_I2C);
	
	// Send through UART for debugging
	UART_transmitByte(USART6, rxByte);
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
	//  I2C_Config(I2C1);
	//	I2C_transmit(I2C1, slaveAddress, i2cTXBuffer, i2cTXLen);
	//	uint8_t slaveAddress = I2C_DISPLAY_ADRESS;
	//	const uint8_t i2cTXBuffer[1] = {0b00000001};
	//	size_t i2cTXLen = 1;

	I2C_Config(MPU6050_I2C);
	I2C_Config(L3GD20_I2C);

	// SPI
	SPI_Init();
	uint8_t spiTXBuffer[6] = {'S', 'P', 'I'};

	// Loop
	while(1){
		// GPIO
		GPIO_pinToggle(GPIOG, 13);

		// UART
		//		UART_transmitString(USART1, "USART 1! ");
		//		UART_transmitString(USART2, "USART 2! ");
		//		UART_transmitString(USART3, "USART 3! ");
		//		UART_transmitString(USART6, "USART 6! ");
		uint8_t byteRead = UART_readByte(USART6);	
		if(byteRead != 0)
			UART_transmitByte(USART6, byteRead);
		
		// I2C
		//I2C_transmit(I2C1, slaveAddress, &i2cTXBuffer[0], i2cTXLen);
		readMPU6050();	
		
		
		// SPI
		uint8_t txByte = 0x73;
		// CS down: PC1
		GPIO_digitalWrite(GPIOC, 1, LOW);
		//SPI_transmitByte(SPI5, txByte);
		//SPI_DisableTransmission(SPI5);	
		SPI_transmitString(SPI5, "Hello SPI!", strlen("Hello SPI!"));
		// User still has to disable the söave selection port manually
		GPIO_digitalWrite(GPIOC, 1, HIGH);

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

