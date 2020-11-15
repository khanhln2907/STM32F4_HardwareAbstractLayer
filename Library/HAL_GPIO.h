#include "stm32f429xx.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct{
	//GPIO_TypeDef *port;
	uint32_t pinMask;
	uint32_t modeIO;
	uint32_t pull;
	uint32_t outputType; // Input or Output
	uint32_t outputSpeed;
	uint32_t useAF;
}GPIO_GeneralConfig;

typedef enum {
	INPUT = 0x00,
	OUTPUT = 0x01,
	ALTERNATE_FUNCTION = 0x10,
	ANALOG = 0x11
}GPIO_portMode;	

typedef enum {
	GPIO_PULL_NONE = 0x00,
	GPIO_PULL_UP = 0x01,
	GPIO_PULL_DOWN = 0x10, 
} GPIO_pull;

typedef enum {
	GPIO_OTYPE_PUSHPULL = 0x0,
	GPIO_OTYPE_OPENDRAIN = 0x1
}GPIO_outputType;
	
typedef enum {
	GPIO_OSPEED_LOW = 0x00,
	GPIO_OSPEED_MED = 0x01,
	GPIO_OSPEED_HIGH = 0x10,
	GPIO_OSPEED_VERY_HIGH = 0x11
} GPIO_ouputSpeed;


// ERROR AND ASSERTION HANDLING
#define HAL_GPIO_OK 1
#define HAL_GPIO_PORT_UNDEFINED 2
#define HAL_GPIO_PORT_MODE_UNDEFINED 3 
#define HAL_GPIO_PIN_UNDEFINED 4 
#define HAL_GPIO_INTERRUPT_TRIGGER_UNDEFINED 5 


// MACRO FOR PINNUMBER AND LEVEL
#define HAL_GPIO_PIN_POS(x) 0x01 << x
#define GPIO_PIN(x) HAL_GPIO_PIN_POS(x) 

#define HAL_GPIO_PIN_SET_POS(x) HAL_GPIO_PIN_POS(x)
#define HAL_GPIO_PIN_RESET_POS(x) 0x01 << (x + 16)

#define HAL_GPIO_STATE_HIGH 0x01
#define HIGH 	HAL_GPIO_STATE_HIGH
#define HAL_GPIO_STATE_LOW 0x00
#define LOW 	HAL_GPIO_STATE_LOW

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






