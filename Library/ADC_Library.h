#ifndef ADC_LIBRARY
#define ADC_LIBRARY
#include "stm32f429xx.h"

#define ADC_OK 1

#define ADC_ERROR_PORT_UNDEFINED 100


void ADC_config(ADC_TypeDef *adcPort);




typedef enum {
	ADC_PRE_DIV2 = 0b00, 
	ADC_PRE_DIV4 = 0b01,
	ADC_PRE_DIV6 = 0b10,
	ADC_PRE_DIV8 = 0b11
}ADC_Prescaler;

typedef enum {
	ADC_RES_12 = 0b00, 
	ADC_RES_10 = 0b01,
	ADC_RES_8 = 0b10,
	ADC_RES_6 = 0b11
}ADC_Resolution;



#endif
