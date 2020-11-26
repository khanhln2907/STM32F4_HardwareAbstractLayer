#include "I2C_Library.h"

#include <assert.h>
/****************************************************************
					STEP TO INIT I2C
			1. Enable I2C Clock and GPIO Clock
			2. Configure GPIO into AF Function
				- Open drain output
				- High Speed Output
				- Pull up pin
				- Configure AFR register -> AF4
			3. Reset I2C
			4. Set up input clock in I2C_CR2
			5. Configure Clock Control Register
			6. Configure Rise Time Register
			7. Enable I2C through I2C_CR1 Register
*****************************************************************/

// To do: specific pin number as parameter ?
// To do: hard coded APB1 clock freq
int I2C_Config(I2C_TypeDef* i2cPort){	
	// 1,2. Setup Clock and GPIO
	if(i2cPort == I2C1){
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		
		// Setup port GPIOB
		// SMBA: PB5
		// SCL: PB6, PB8
		// SDA: PB7, PB9	
		GPIO_GeneralConfig configGPIOB;
		configGPIOB.pinMask = GPIO_PIN(6) | GPIO_PIN(7);
		configGPIOB.mode = ALTERNATE_FUNCTION;
		configGPIOB.AFSelect = 4;
		configGPIOB.pull = GPIO_PULL_UP;
		configGPIOB.outputSpeed = GPIO_OSPEED_HIGH;
		configGPIOB.outputType = GPIO_OTYPE_OPENDRAIN;
		GPIO_setupPort(GPIOB, configGPIOB);
	
	} else if (i2cPort == I2C2) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
		
		// Setup port GPIOF
		// SMBA: PH6
		// SCL: PF1, PH4
		// SDA: PF0, PH5	
		GPIO_GeneralConfig configGPIOF;
		configGPIOF.pinMask = GPIO_PIN(1) | GPIO_PIN(0);
		configGPIOF.mode = ALTERNATE_FUNCTION;
		configGPIOF.AFSelect = 4;
		configGPIOF.pull = GPIO_PULL_UP;
		configGPIOF.outputSpeed = GPIO_OSPEED_HIGH;
		configGPIOF.outputType = GPIO_OTYPE_OPENDRAIN;
		GPIO_setupPort(GPIOF, configGPIOF);
		
	} else if (i2cPort == I2C3) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
	
		// Setup port GPIOH
		// SMBA: PH9
		// SCL: PH7, PA8 
		// SDA: PH8, PC9
		// ?? PH7, PH8 not found on STM32F429ZIT6U
		// Config PA8
		GPIO_GeneralConfig configGPIOA;
		configGPIOA.pinMask = GPIO_PIN(8);
		configGPIOA.mode = ALTERNATE_FUNCTION;
		configGPIOA.AFSelect = 4;
		configGPIOA.pull = GPIO_PULL_UP;
		configGPIOA.outputSpeed = GPIO_OSPEED_HIGH;
		configGPIOA.outputType = GPIO_OTYPE_OPENDRAIN;
		GPIO_setupPort(GPIOA, configGPIOA);
		
		// Config PC9
		GPIO_GeneralConfig configGPIOC;
		configGPIOC.pinMask = GPIO_PIN(9);
		configGPIOC.mode = ALTERNATE_FUNCTION;
		configGPIOC.AFSelect = 4;
		configGPIOC.pull = GPIO_PULL_UP;
		configGPIOC.outputSpeed = GPIO_OSPEED_HIGH;
		configGPIOC.outputType = GPIO_OTYPE_OPENDRAIN;
		GPIO_setupPort(GPIOC, configGPIOC);
		
	} else {
		assert(!I2C_PORT_ERROR);
		return I2C_PORT_ERROR;
	}
	
	// 3. Reset I2C
	i2cPort->CR1 |= I2C_CR1_SWRST;	// Put in reset state
	i2cPort->CR1 &= ~I2C_CR1_SWRST; // Take out of reset state -> reset done
		
	// 4. Programm peripheral input clock in CR2 Register
	// I2C run under frequency of APB1 -> put it into FREQ Buffer in CR2 Register
	i2cPort->CR2 |= 45 << I2C_CR2_FREQ_Pos;
	
	// 5, 6. Configure Clock and Rise Time of I2C
	// Fast / Standard mode
	i2cPort->CCR = 225; // APB1: 45MHz
	i2cPort->TRISE = 46; // Check Manual RM9090
	
	// 6. Enable I2C
	i2cPort->CR1 = I2C_CR1_PE; // Peripheral Enable
	
	return I2C_OK;
}

void I2C_transmit(I2C_TypeDef* i2cPort, uint8_t rxAddress, const uint8_t* txMsg, uint8_t txLen){
	// Start
	_I2C_sendStart(i2cPort);
	// Adress
	_I2C_sendAddress(i2cPort, rxAddress);
	// Data
	if(txLen == 1){
		_I2C_sendSingleByte(i2cPort, txMsg[0]);
	} else if (txLen > 1) {
		_I2C_sendMultipleByte(i2cPort, txMsg, txLen);
	}
	// Stop
	_I2C_sendStop(i2cPort);	
}

/*
The master sends a NACK for the last byte received from the slave. After receiving this
NACK, the slave releases the control of the SCL and SDA lines. Then the master can send
a Stop/Restart condition.
1. To generate the nonacknowledge pulse after the last received data byte, the ACK bit
must be cleared just after reading the second last data byte (after second last RxNE
event).
2. In order to generate the Stop/Restart condition, software must set the STOP/START bit
after reading the second last data byte (after the second last RxNE event).

*/
void I2C_receiveString(I2C_TypeDef* i2cPort, uint8_t txAdress, volatile uint8_t* rxMsg, uint8_t rxLen){
	// Start
	_I2C_sendStart(i2cPort);
	// Adress
	_I2C_sendAddress(i2cPort, txAdress);
	// Data
	for(int i = 0; i < rxLen; i++){
		rxMsg[i] = _I2C_readByte(i2cPort);
		
		// Setup NACK at the second last Byte
		if(i == rxLen - 2){
			i2cPort->CR1 &= ~I2C_CR1_ACK;
			i2cPort->CR1 |= I2C_CR1_STOP;
		}
	}
}

/*
In case a single byte has to be received, the Acknowledge disable is made during EV6
(before ADDR flag is cleared) and the STOP condition generation is made after EV6.
EV6: ADDR = 1
*/
uint8_t I2C_receiveByte(I2C_TypeDef* i2cPort, uint8_t txAdress){
	// Start
	_I2C_sendStart(i2cPort);
	
	// Send Adress
	i2cPort->DR = txAdress; 	
	while(!(i2cPort->SR1 & I2C_SR1_ADDR));  // Wait for ACK from Slave TX
	
	// Setup NACK
	i2cPort->CR1 &= ~I2C_CR1_ACK;
	
	// Clear ADDR bit by reading SR1 and SR2	
	uint8_t clear = i2cPort->SR1 | i2cPort->SR2; 	

	// Set STOP condition
	i2cPort->CR1 |= I2C_CR1_STOP;
	
	// Read and return 1 received Byte
	uint8_t rxByte = i2cPort->DR; // This read DR will clear the RxNE bit
	return rxByte;
}

void _I2C_sendStart(I2C_TypeDef* i2cPort){
	// Send Startbit
	i2cPort->CR1 |= I2C_CR1_START;
	// Wait until Startbit is sent
	while(!(i2cPort->SR1 & I2C_SR1_SB)); // SB will be cleared by writing into DR
}

void _I2C_sendAddress(I2C_TypeDef* i2cPort, uint8_t Address){
	i2cPort->DR = Address; 							// Send Adress
	while(!(i2cPort->SR1 & I2C_SR1_ADDR));  		// Wait for ACK from slave
	uint8_t clear = i2cPort->SR1 | i2cPort->SR2; 	// Clear ADDR bit by reading SR1 and SR2	
}

void _I2C_sendSingleByte(I2C_TypeDef* i2cPort, uint8_t dataByte){
	// Wait for data register empty
	while(!(i2cPort->SR1 & I2C_SR1_TXE));
	// Add in Data Byte
	i2cPort->DR = dataByte;
	// Wait until BTF bit is set: Byte tranfer finished
	while(!(i2cPort->SR1 & I2C_SR1_BTF));
}

void _I2C_sendStop(I2C_TypeDef* i2cPort){
	i2cPort->CR1 |= I2C_CR1_STOP;
}

void _I2C_sendMultipleByte(I2C_TypeDef* i2cPort, uint8_t* txMsg, uint8_t txLen){
	// TXE must be checked every time
	// BTF can be checked only once at the end
	
	while(!(i2cPort->SR1 & I2C_SR1_TXE));
	for (int i = 0; i < txLen; i++){
		while(!(i2cPort->SR1 | I2C_SR1_TXE));
		i2cPort->DR = txMsg[i]; 					
	}
	while(!(i2cPort->SR1 & I2C_SR1_BTF));
}

uint8_t _I2C_readByte(I2C_TypeDef* i2cPort){
	uint8_t rxByte = 0;
	i2cPort->CR1 |= I2C_CR1_ACK;
	
	rxByte = i2cPort->DR; // This read DR will clear the RxNE bit
	return rxByte;
}










