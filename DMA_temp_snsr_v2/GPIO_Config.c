#include "stm32f4xx.h"

void LED_PushButton_Config(void) {

	//////////////////////// LED GPIO PA5 Setup Start /////////////////////////
	// 1) enable gpio port A clock
	RCC->AHB1ENR |= (1<<0);
	
	// 2) set pin as output
	GPIOA->MODER |= (1<<10);
	
	// 3) set pin output type
	GPIOA->OTYPER &= ~(1<<5); 
	GPIOA->OSPEEDR |= (1<<11);
	GPIOA->PUPDR &= ~((1<<10) | (1<<11));  
	//////////////////////// LED GPIO PA5 Setup End ////////////////////////////
	
	//////////////////////// Push Button GPIO PC13 Setup Start /////////////////
	RCC->AHB1ENR |= (1<<2);
	GPIOC->MODER |= (0<<26) | (0<<27);
	GPIOC->OTYPER &= ~(1<<13);
	GPIOC->OSPEEDR |= (1<<26);
	GPIOC->PUPDR |= ~((0<<26) | (0<<27));
	//////////////////////// Push Button GPIO PC13 Setup Start /////////////////

}

