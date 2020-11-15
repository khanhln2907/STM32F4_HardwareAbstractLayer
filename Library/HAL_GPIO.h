#include "stm32f429xx.h"
#include <stdbool.h>

typedef struct{
	//GPIO_TypeDef *port;
	uint32_t pinMask;
	uint32_t modeIO;
	uint32_t pull;
	uint32_t outputType; // Input or Output
	uint32_t outputSpeed;
	uint32_t useAF;
}GPIO_Config;

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

// MACRO FOR PINNUMBER
#define HAL_GPIO_PIN_POS(x) 0x01 << x
#define HAL_GPIO_STATE_HIGH 0x01
#define HIGH HAL_GPIO_STATE_HIGH

#define HAL_GPIO_PIN_SET_POS(x) HAL_GPIO_PIN_POS(x)
#define HAL_GPIO_STATE_LOW 0x00
#define LOW HAL_GPIO_STATE_LOW
#define HAL_GPIO_PIN_RESET_POS(x) 0x01 << (x + 16)


// Public Function ********************************************************
// Init any specific GPIO, same function, set all pin by using pinMask
// ...
int GPIO_init(GPIO_TypeDef *port, GPIO_Config pinConfig);
 
 
void GPIO_digitalWrite(GPIO_TypeDef *port, uint32_t pinNumber, bool level);
void GPIO_pinToggle(GPIO_TypeDef *port, uint32_t pinNumber);
bool GPIO_readOutput(GPIO_TypeDef *port, uint32_t pinNumber);


int GPIO_setupPin(GPIO_TypeDef *port, uint32_t pinNumber, GPIO_Config portConfig);


// General
void GPIO_setMode(GPIO_TypeDef* port, uint32_t pinNumber, GPIO_portMode mode);
//void GPIO_setModeWithPUPD(GPIO_TypeDef* port, uint32_t pinNumber, GPIO_portMode mode, GPIO_PUPD pull);

// Output: Type, Speed
void GPIO_configOutput(GPIO_TypeDef* port, uint32_t pinNumber, GPIO_outputType oType, GPIO_ouputSpeed oSpeed);










