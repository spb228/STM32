#include "stm32f4xx.h"
#include "Sys_Clock_Config.c"
#include "GPIO_Config.c"

void static delay(uint32_t time) {
	while (time--);
}

int main(void) {
	Sys_Clock_Config();
	GPIO_Config();
	
	while(1) {
		GPIOA->BSRR |= 1<<5; // set pin PA5
		delay(100000);
		GPIOA->BSRR |= 1<<21; // reset pin PA5
		delay(100000);
	}
	
//return 0;
}	