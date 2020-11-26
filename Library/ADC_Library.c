#include "ADC_Library.h"
#include "GPIO_Library.h"
#include <assert.h>

/****************************************************************
					STEP TO INIT ADC
		1. Enable ADC and GPIO Clock
		2. Set prescaler in Common Control Register (CCR)
		3. Set the scan mode and resolution in Control Register 1 (CR1)
		4. Set continuous conversion, EOC and data allignment in CR2
		5. Set the sampling time for the channels in ADC_SMRPx
		6. Set the Regular channel sequence length in ADC_SQR1
		7. Set the respective GPIO Pins in the analog mode
		
*****************************************************************/

// Todo: Prescaler and sampling time are still hard coded
// Mode are not yet properly updated. Just fast init for using
// Sampling time must be set up independently for each channel, improve this function later

void ADC_config(ADC_TypeDef *adcPort){
	// 1. Enable ADC and GPIO Clock
	if (adcPort == ADC1){
		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	} else if(adcPort == ADC2) {
		RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;

	} else if(adcPort == ADC3) {
		RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;

	} else {
		assert(!ADC_ERROR_PORT_UNDEFINED);
	}
	
	// 2. Set prescaler in Common Control Register (CCR)
	// Clock is common for all register
	ADC->CCR |= ADC_CCR_ADCPRE_0; // div 4: 01
	
	// 3. Set the scan mode and resolution in Control Register 1 (CR1)
	adcPort->CR1 |= ADC_CR1_SCAN; // activate scanning group of input ?
	adcPort->CR1 &= ~(ADC_CR1_RES); // 00: 12 bit -> 15 cycles 
	
	// 4. Set continuous conversion, EOC and data allignment in CR2
	adcPort->CR2 |= ADC_CR2_CONT; // continuos conversion
	adcPort->CR2 |= ADC_CR2_EOCS; // EOCs at the end of conversion
	adcPort->CR2 &= ~(ADC_CR2_ALIGN); // ADC Allignment right
	
	// 5. Set the sampling time for the channels in ADC_SMPRxx
	//adcPort->SMPR1 = 0x00000000; // SMPR1 is for channel 10 -> 18
	adcPort->SMPR2 &= ~(ADC_SMPR1_SMP11 | ADC_SMPR1_SMP14); // SMPR2 is for channel 0 -> 9. Set up for 1 and 4 right now

	
	// 6. Set the Regular channel sequence length in ADC_SQR1
	adcPort->SQR1 |= ADC_SQR1_L_0; // L = 1 for 2 conversions
	
	// 7. Set the respective GPIO Pins in the analog mode
		// Setup port GPIOA PA1, PA4
		GPIO_GeneralConfig configGPIOA;
		configGPIOA.pinMask = GPIO_PIN(1) | GPIO_PIN(4);
		configGPIOA.mode = ANALOG; // TODO: Confirm GPIO Setup analog mode
		configGPIOA.outputSpeed = GPIO_OSPEED_HIGH;
		configGPIOA.outputType = GPIO_OTYPE_OPENDRAIN;
		GPIO_setupPort(GPIOA, configGPIOA);
}
