#include "HAL_GPIO.h"
#include <assert.h>


int GPIO_init(GPIO_TypeDef *port, GPIO_Config pinConfig){
	// Clock Init
	if(port == GPIOA){
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	} else if (port == GPIOB) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	} else if (port == GPIOC) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	} else if (port == GPIOD) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	} else if (port == GPIOE) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	} else if (port == GPIOF) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	} else if (port == GPIOG) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	} else if (port == GPIOH) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
	} else if (port == GPIOI) {
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
	} else if (port == GPIOJ) {	
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
	} else if (port == GPIOK) {	
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
	} else 
	{
			// Error or execption handling
			assert(HAL_GPIO_PORT_UNDEFINED);
			return HAL_GPIO_PORT_UNDEFINED;
	}
	
	// Get pinNumber
	for(unsigned char curPinPos = 0; curPinPos <16; curPinPos++){
		if((pinConfig.pinMask >> curPinPos) & 0x01){
			// Setup each pin according to the pin mask
			if(GPIO_setupPin(port, curPinPos, pinConfig) != HAL_GPIO_OK)
				// Return error pin immediately	
				return curPinPos;
		}
	}
	return HAL_GPIO_OK;
}	
	





void GPIO_digitalWrite(GPIO_TypeDef *port, uint32_t pinNumber, bool level){
	if(level == HAL_GPIO_STATE_HIGH){
		port->BSRR &= ~HAL_GPIO_PIN_RESET_POS(pinNumber);
		port->BSRR |= HAL_GPIO_PIN_SET_POS(pinNumber);
	}
	else if(level == HAL_GPIO_STATE_LOW){
		port->BSRR &= ~HAL_GPIO_PIN_SET_POS(pinNumber);
		port->BSRR |= HAL_GPIO_PIN_RESET_POS(pinNumber);
	}
}


bool GPIO_readOutput(GPIO_TypeDef *port, uint32_t pinNumber){
	return (port->ODR & HAL_GPIO_PIN_POS(pinNumber));
}


void GPIO_pinToggle(GPIO_TypeDef *port, uint32_t pinNumber){
	bool curState = GPIO_readOutput(port, pinNumber);
	GPIO_digitalWrite(port, pinNumber, !curState);
}



//  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
//  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
//  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
//  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
//  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
//  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
//  __IO uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
//  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
//  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
int GPIO_setupPin(GPIO_TypeDef *port, uint32_t pinNumber, GPIO_Config pinConfig){

	// Setup Mode
	//GPIO_setMode(port, pinNumber, portConfig.modeIO);
	port->MODER |= (pinConfig.modeIO << (pinNumber * 2)); // 2 bit for each pin
	
	// Setup Pull
	port->PUPDR |= (pinConfig.pull << (pinNumber * 2));
	
	// Setup mode for each pin
	port->MODER |= (pinConfig.modeIO << (pinNumber * 2)); // 2 bit for each pin
	// Setup according to GPIO function
	switch(pinConfig.modeIO){
		case INPUT:
		{
			// Do something
			break;
		}
		case OUTPUT: // Setup speed and output type
		{
			// Setup output type
			port->OTYPER |= (pinConfig.outputType << (pinNumber)); 
			// Setup output speed
			port->OSPEEDR |= (pinConfig.outputSpeed << (pinNumber * 2)); // 2 bit for each pin
			// Do something 
			break;
		}
		case ALTERNATE_FUNCTION:
		{
			// Do something
			break;
		}
		case ANALOG:
		{
			// Do something
			break;
		}
		default:
		{
			assert(HAL_GPIO_PORT_MODE_UNDEFINED);
			return HAL_GPIO_PORT_MODE_UNDEFINED;
		}
	}
	return HAL_GPIO_OK;
}


	
// Setting port to be input or output
void GPIO_setMode(GPIO_TypeDef* port, uint32_t pinNumber, GPIO_portMode mode){
	port->MODER |= (mode << (pinNumber * 2)); // 2 bit for each pin
}

// Setup port as output with desired configuration
void GPIO_configOutput(GPIO_TypeDef* port, uint32_t pinNumber, GPIO_outputType oType, GPIO_ouputSpeed oSpeed){
	// Setup as output
	port->MODER |= (OUTPUT << (pinNumber * 2));
	// Setup output type
	port->OTYPER |= (oType << (pinNumber)); 
	// Setup output speed
	port->OSPEEDR |= (oSpeed << (pinNumber * 2)); // 2 bit for each pin
}

