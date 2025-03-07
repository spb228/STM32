#include "stm32f4xx.h"
#include "Sys_Clock_Config.h"

#define PLLM 25
#define PLLN 168
#define PLLP 0 // PLLP = 2;

void Sys_Clock_Config(void) {
	
	// 1) enable HSE
	RCC->CR |= RCC_CR_HSEON; 
	while (!(RCC->CR & RCC_CR_HSERDY)); 
	
	// 2) set power enable clock and voltage regulator
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	// 3) configure flash
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	
	// 4) configure prescalars
	// AHB 
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // 1
	// APB1 
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // 2
	// APB2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // 1
	
	// 5) configure pll
	// pllm
	RCC->PLLCFGR |= PLLM << 0;
	//PLLN
	RCC->PLLCFGR |= PLLN << 6; 
	//PLLP
	RCC->PLLCFGR |= PLLP << 16; 
	//HSE source
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	
	// 6) enable PLL and wait for ready
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	
	// 7) select the clock source - pll
	RCC->CFGR |= RCC_CFGR_SW_PLL; 
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}