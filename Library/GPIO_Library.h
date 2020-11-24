#ifndef GPIO_LIBRARY
#define GPIO_LIBRARY

#include "stm32f429xx.h"

#include <stdbool.h>
#include <stddef.h>

// ERROR AND ASSERTION HANDLING
#define HAL_GPIO_OK 1

#define HAL_GPIO_NOT_OK 100
#define HAL_GPIO_PORT_UNDEFINED 				HAL_GPIO_NOT_OK + 1
#define HAL_GPIO_PORT_MODE_UNDEFINED 			HAL_GPIO_PORT_UNDEFINED + 1 
#define HAL_GPIO_PORT_PULL_UNDEFINED    		HAL_GPIO_PORT_MODE_UNDEFINED + 1
#define HAL_GPIO_PIN_UNDEFINED  				HAL_GPIO_PORT_PULL_UNDEFINED + 1
#define HAL_GPIO_INTERRUPT_TRIGGER_UNDEFINED 	HAL_GPIO_PIN_UNDEFINED + 1
#define HAL_GPIO_OTYPE_UNDEFINED				HAL_GPIO_INTERRUPT_TRIGGER_UNDEFINED + 1
#define HAL_GPIO_OSPEED_UNDEFINED				HAL_GPIO_OTYPE_UNDEFINED + 1

// MACRO FOR PINNUMBER AND LEVEL
#define HAL_GPIO_PIN_POS(x) 0x1 << x
#define GPIO_PIN(x) HAL_GPIO_PIN_POS(x) 

#define HAL_GPIO_PIN_SET_POS(x) HAL_GPIO_PIN_POS(x)
#define HAL_GPIO_PIN_RESET_POS(x) 0x1 << (x + 16)

#define HAL_GPIO_STATE_HIGH 0x1
#define HIGH 	HAL_GPIO_STATE_HIGH
#define HAL_GPIO_STATE_LOW 0x0
#define LOW 	HAL_GPIO_STATE_LOW


typedef struct{
	//GPIO_TypeDef *port;
	uint16_t pinMask;
	uint8_t mode;
	uint32_t pull;
	uint8_t outputType; // Input or Output
	uint8_t outputSpeed;
	uint16_t AFSelect;
}GPIO_GeneralConfig;

typedef enum {
	INPUT = 0b00,
	OUTPUT = 0b01,
	ALTERNATE_FUNCTION = 0b10,
	ANALOG = 0b11
}GPIO_portMode;	

typedef enum {
	GPIO_PULL_NONE = 0b00,
	GPIO_PULL_UP = 0b01,
	GPIO_PULL_DOWN = 0b10, 
} GPIO_pull;

typedef enum {
	GPIO_OTYPE_PUSHPULL = 0b0,
	GPIO_OTYPE_OPENDRAIN = 0b1
}GPIO_outputType;
	
typedef enum {
	GPIO_OSPEED_LOW = 0b00,
	GPIO_OSPEED_MED = 0b01,
	GPIO_OSPEED_HIGH = 0b10,
	GPIO_OSPEED_VERY_HIGH = 0b11
} GPIO_ouputSpeed;

// Public Function ********************************************************
// Init any specific GPIO, same function, set the common pin by according to pinMask
int GPIO_setupPort(GPIO_TypeDef *port, GPIO_GeneralConfig pinConfig);

/// Function for set and reset pin 
void GPIO_digitalWrite(GPIO_TypeDef *port, uint32_t pinNumber, bool level);
void GPIO_pinToggle(GPIO_TypeDef *port, uint32_t pinNumber);
bool GPIO_getOutputState(GPIO_TypeDef *port, uint32_t pinNumber);

// Interrupt
typedef enum {
	GPIO_EXTI_TRIGGER_RISING = 0x0,
	GPIO_EXTI_TRIGGER_FALLING = 0x1,
	GPIO_EXTI_TRIGGER_BOTH = 0x2,
} GPIO_triggerSelection;


typedef struct {
	uint32_t pinNumber;
	GPIO_triggerSelection trigger;
	bool isSoftwareIntEvt;
}GPIO_InterruptConfig;

/// Used to setup interrupt for pin only 
/// Interrupt lines 0 -> 15
void GPIO_setupInterruptPin(GPIO_TypeDef *port, GPIO_InterruptConfig myPinConfig);

void GPIO_enableInterruptPin(uint32_t pinNumber, IRQn_Type irqNumber);
void GPIO_clearInterruptPin(uint32_t pinNumber);

// Private
/// Setup a single pin
/// portConfig.pinMask: ignored
int _GPIO_initPin(GPIO_TypeDef *port, uint32_t pinNumber, GPIO_GeneralConfig pinConfig);

#endif




