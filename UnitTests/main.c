#include "stm32f429xx.h"
#include "main.h"

int GPIO_startUp(){
	// Setup port G
	GPIO_Config myPinConfig;
	myPinConfig.pinMask = HAL_GPIO_PIN_POS(13) | HAL_GPIO_PIN_POS(14);
	myPinConfig.modeIO = OUTPUT;
	myPinConfig.pull = GPIO_PULL_NONE;
	myPinConfig.outputType = GPIO_OTYPE_PUSHPULL;
	myPinConfig.outputSpeed = GPIO_OSPEED_MED;
	GPIO_init(GPIOG, myPinConfig);
}

void fakeDelay()
{
	for(int i = 0; i < 500000; i++);
}

int main(){
	// Init
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
//	
//	GPIO_setMode(GPIOG, 13, OUTPUT);
//	GPIO_configOutput(GPIOG, 13, GPIO_OTYPE_PUSHPULL, MED);
	
	GPIO_startUp();
	
	GPIO_digitalWrite(GPIOG, 13, HIGH);
	GPIO_digitalWrite(GPIOG, 14, HIGH);

	// Loop
	while(1){
		GPIO_pinToggle(GPIOG, 13);
		fakeDelay();
	}
}

