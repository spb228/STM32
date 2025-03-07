#include "Delay.h"
#include "Sys_Clock_Config.h"
#include "stm32f4xx.h"

void TIM3_Config (void) {
	
	// 1) enable timer clock
	RCC->APB1ENR |= (1<<1); // enable TIM3
	
	// 2) set the prescalar and the ARR
	TIM3->PSC = 42 - 1;	// 42Mhz / 42 = 1 MHz = 1uS delay
	TIM3->ARR = 0xffff; // MAX ARR Value
	
	//3) enable the timer and wait for update flag to set
	TIM3->CR1 |= (1<<0); // enable counter
	while (!(TIM3->SR & (1<<0))); // update interrupt flag
 	
}

void Delay_us(uint16_t us) {
	
	// 1) reset the counter
	TIM3->CNT = 0;
	
	// 2) wiat for counter to reach entered value
	while (TIM3->CNT < us);
}
	
void Delay_ms(uint16_t ms){
	for (uint16_t i=0; i<ms; i++) {
		Delay_us(1000); //delay of 1ms
	}
	
}