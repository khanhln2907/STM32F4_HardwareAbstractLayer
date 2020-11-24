#ifndef TIM_LIBRARY
#define TIM_LIBRARY
#include "stm32f429xx.h"
#include "GPIO_Library.h"

void TIM4Config(void);
void TIM6Config(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif

