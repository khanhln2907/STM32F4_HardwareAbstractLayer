#include "HAL_GPIO.h"
#include <assert.h>


int GPIO_setupPort(GPIO_TypeDef *port, GPIO_GeneralConfig pinConfig){
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
			if(_GPIO_initPin(port, curPinPos, pinConfig) != HAL_GPIO_OK)
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


bool GPIO_getOutputState(GPIO_TypeDef *port, uint32_t pinNumber){
	return (port->ODR & HAL_GPIO_PIN_POS(pinNumber));
}


void GPIO_pinToggle(GPIO_TypeDef *port, uint32_t pinNumber){
	//bool curState = GPIO_getOutputState(port, pinNumber);
	//GPIO_digitalWrite(port, pinNumber, !curState);
	port->ODR ^= HAL_GPIO_PIN_POS(pinNumber);
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
int _GPIO_initPin(GPIO_TypeDef *port, uint32_t pinNumber, GPIO_GeneralConfig pinConfig){

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


// Interrupt and Events Functions
void GPIO_setupInterruptPin(GPIO_TypeDef *port, GPIO_InterruptConfig myPinConfig){
	// Enable clock
	RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Setup GPIO SOurce for Interrupt and Event
	if(port == GPIOA){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PA;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PA;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PA;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PA;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PA;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PA;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PA;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PA;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PA;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOB){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PB;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PB;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PB;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PB;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PB;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PB;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PB;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PB;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PB;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PB;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}	
	}	
	else if(port == GPIOC){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PC;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PC;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PC;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PC;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PC;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PC;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PC;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PC;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PC;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PC;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PC;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOD){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PD;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PD;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PD;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PD;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PD;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PD;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PD;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PD;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PD;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PD;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PD;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PD;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PD;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PD;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PD;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PD;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOE){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PE;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PE;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PE;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PE;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PE;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PE;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PE;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PE;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PE;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PE;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PE;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PE;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PE;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PE;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PE;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PE;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOF){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PF;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PF;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PF;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PF;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PF;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PF;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PF;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PF;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PF;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PF;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PF;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PF;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PF;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PF;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PF;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PF;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOG){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PG;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PG;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PG;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PG;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PG;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PG;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PG;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PG;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PG;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PG;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PG;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PG;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PG;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PG;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PG;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PG;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOH){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PH;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PH;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PH;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PH;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PH;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PH;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PH;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PH;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PH;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PH;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PH;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PH;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PH;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PH;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PH;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PH;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOI){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PI;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PI;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PI;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PI;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PI;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PI;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PI;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PI;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PI;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PI;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PI;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PI;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PI;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PI;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PI;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PI;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOJ){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PJ;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PJ;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PJ;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PJ;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PJ;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PJ;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PJ;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PJ;
				break;
			case 8:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PJ;
				break;
			case 9:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PJ;
				break;
			case 10:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PJ;
				break;
			case 11:
				SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PJ;
				break;
			case 12:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PJ;
				break;
			case 13:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PJ;
				break;
			case 14:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PJ;
				break;
			case 15:
				SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PJ;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else if(port == GPIOK){
		switch(myPinConfig.pinNumber){
			case 0:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PK;
				break;
			case 1:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PK;
				break;
			case 2:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PK;
				break;
			case 3:
				SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PK;
				break;
			case 4:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PK;
				break;
			case 5:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PK;
				break;
			case 6:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PK;
				break;
			case 7:
				SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PK;
				break;
			default:
				assert(HAL_GPIO_PIN_UNDEFINED);
				break;
		}
	}
	else {
		assert(HAL_GPIO_PORT_UNDEFINED);
	}
	
	// Setup interrupt/event parameter and style
	// Setup Masked
	//EXTI->IMR |= HAL_GPIO_PIN_POS(myPinConfig.pinNumber);

	// Is Software triggered ?
	if(myPinConfig.isSoftwareIntEvt){
		EXTI->SWIER |= HAL_GPIO_PIN_POS(myPinConfig.pinNumber);
	}
	
	// Trigger style
	switch (myPinConfig.trigger){
		case GPIO_EXTI_TRIGGER_RISING:
			EXTI->RTSR |= HAL_GPIO_PIN_POS(myPinConfig.pinNumber);
			break;
		case GPIO_EXTI_TRIGGER_FALLING:
			EXTI->FTSR |= HAL_GPIO_PIN_POS(myPinConfig.pinNumber);
			break;
		case GPIO_EXTI_TRIGGER_BOTH:
			EXTI->RTSR |= HAL_GPIO_PIN_POS(myPinConfig.pinNumber);
			EXTI->FTSR |= HAL_GPIO_PIN_POS(myPinConfig.pinNumber);
			break;
		default:
			assert(HAL_GPIO_INTERRUPT_TRIGGER_UNDEFINED);
			break;
	}
}

void GPIO_enableInterruptPin(uint32_t pinNumber, IRQn_Type irqNumber){
	// Set mask for EXTI
	EXTI->IMR |= HAL_GPIO_PIN_POS(pinNumber);
	
	// Set mask for NVIC
	NVIC_EnableIRQ(irqNumber);
}

void GPIO_clearInterruptPin(uint32_t pinNumber){
	EXTI->PR |= HAL_GPIO_PIN_POS(pinNumber);
}



















