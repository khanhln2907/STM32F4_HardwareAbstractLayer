#include "../Library/GPIO_Library.h"
#include "../Library/UART_Library.h"
#include "../Library/TIM_Library.h"

// SystemCoreClock = ((INPUT_CLOCK (HSE_OR_HSI_IN_HZ) / PLL_M) * PLL_N) / PLL_P
// 180 Hz: PLL_N = 360, PLL_P = 2, PLL_M = 8
void SysClockConfig(){
		/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/
	
	// Template
//	#define PLL_M 	4
//	#define PLL_N 	180
//	#define PLL_P 	0  // PLLP = 2

//	// 1. ENABLE HSE and wait for the HSE to become Ready
//	RCC->CR |= RCC_CR_HSEON;  
//	while (!(RCC->CR & RCC_CR_HSERDY));
//	
//	// 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
//	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//	PWR->CR |= PWR_CR_VOS; 
//	
//	
//	// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
//	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
//	
//	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
//	// AHB PR
//	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
//	
//	// APB1 PR
//	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
//	
//	// APB2 PR
//	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
//	
//	
//	// 5. Configure the MAIN PLL
//	RCC->PLLCFGR = (PLL_M <<0) | (PLL_N << 6) | (PLL_P <<16) | (RCC_PLLCFGR_PLLSRC_HSE);

//	// 6. Enable the PLL and wait for it to become ready
//	RCC->CR |= RCC_CR_PLLON;
//	while (!(RCC->CR & RCC_CR_PLLRDY));
//	
//	// 7. Select the Clock Source and wait for it to be set
//	RCC->CFGR |= RCC_CFGR_SW_PLL;
//	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	
	#define PLL_M 4
	#define PLL_N 180
	#define PLL_P 0 // 2

	// 1. Set up PLL. Select HSE
	RCC->CR |= RCC_CR_HSEON;
	
	// Wait until HSE is stable
	while(!(RCC->CR & RCC_CR_HSERDY));
	
	// 2. Enable Power and Voltage Regulator
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; 
	PWR->CR |= PWR_CR_VOS; // Reset Value
	
	// 3. Setup Flash Register
	FLASH->ACR |= FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	
	// 4. Setup AHB, APB1 and APB2
	// AHB
	RCC->CFGR &= ~RCC_CFGR_HPRE_3; // 0xxx
	
	// APB1: /4 == 101
	RCC->CFGR |= RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE1_0;
	RCC->CFGR &= ~RCC_CFGR_PPRE1_1;
	// APB2: /2 == 100
	RCC->CFGR |= RCC_CFGR_PPRE2_2;
	RCC->CFGR &= ~(RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE2_1);

	
	// 5. COnfigure Main PPL
	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) | RCC_PLLCFGR_PLLSRC_HSE;
	
	// 6. Enable PLL and wait ofor it to be ready
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	// 7. Set CLK Source
	// Select PLL as used clk
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Wait for it to be ready
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}





