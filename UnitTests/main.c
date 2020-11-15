#include "stm32f429xx.h"
#include "main.h"

void GPIO_init(void){
	// Setup port G
	GPIO_GeneralConfig myPinConfig;
	myPinConfig.pinMask = GPIO_PIN(13) | GPIO_PIN(14);
	myPinConfig.modeIO = OUTPUT;
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

void fakeDelay()
{
	for(int i = 0; i < 250000; i++);
}

int main(){
	// Initiallize GPIO
	GPIO_init();
	
	// Initialize interrupt
	GPIO_Interrupt_Init();
	
	GPIO_digitalWrite(GPIOG, 13, HIGH);
	
	// Enable Interrupt
	GPIO_enableInterruptPin(0, EXTI0_IRQn);
	
	// Loop
	while(1){
		GPIO_pinToggle(GPIOG, 13);
		fakeDelay();
	}
}

void EXTI0_IRQHandler(){
	// Must !
	GPIO_clearInterruptPin(0);
	
	// Do something
	// ...
	GPIO_pinToggle(GPIOG, 14);
}

