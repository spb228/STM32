#include "ADC.h"

void ADC_init(void){
	
	///////////////////////// ADC - PA1 and PA4 Setup Start ////////////////////
	GPIOA->MODER |= (3<<2); // analog mode for PA 1
	GPIOA->MODER |= (3<<8); // analog mode for PA 4
	///////////////////////// ADC - PA1 Setup Start ////////////////////////////
	
	// 1) Enable ADC and GPIO clock
	RCC->APB2ENR |= (0b1<<8); 
	RCC->AHB1ENR |= (0b1<<0);
	
	// 2) Set prescalar in CCR
	ADC->CCR |= (0b01<<16);
	
	// 3) Set the scan mode and resolution in CR1
	ADC1->CR1 |= (1<<8); // scan mode enabled
	ADC1->CR1 &= ~(1<<24); // 12 bit res
	
	// 4) Set continous conversion, EOC, and data alignment in CR2
	ADC1->CR2 |= (1<<1); // Cont conversion mode enabled
	ADC1->CR2 |= (1<<10); // EOC after each conversion
	ADC1->CR2 &= ~(1<<11); // Data alignment right
	
	// 5) Set sampling time for channels
	ADC1->SMPR2 &= ~((1<<3) | (1<<12)); // sampling time of 3 cycles for channel 1 and channel 4
	
	// 6) Set the regular channel sequence length
	ADC1->SQR1 |= (1<<20);
	
	// 7) Configure GPIO pins to analog pins

}

void ADC_enable(void){
	
	// 1)Enable ADC by setting ADON bit in CR2
	ADC1->CR2 |= (1<<0); 
	
	// 2) Wait for ADC to stabilize (approx 10us)
	uint32_t delay = 10000; 
	while (delay--);
}

void ADC_start(int channel) {
	
	// 1) Set channel sequence in SQR
	ADC1->SQR3 = 0; // clear 
	ADC1->SQR3 |= (channel<<0); // conversion in regular sequence
	
	// 2) Clear status register
	ADC1->SR = 0; 
	
	// 3) start conversion by seting SWSTART in CR2
	ADC1->CR2 |= (1<<30); 
	
}

void ADC_wait_for_conv(void) {
	
	//wait for EOC to set
	while (!(ADC1->SR & (1<<1)));
	
}

uint16_t ADC_get_val(void) {
	
	return ADC1->DR; //read data register
	
}

void ADC_disable(void) {
	
	//disable ADC by clearing ADON bit in CR2
	ADC1->CR2 &= ~(1<<0);
	
}
