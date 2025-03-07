#include "ADC.h"

//extern int uint16_t Rx_data[3];

void ADC_init(void){
	
	///////////////////////// ADC - PA1 and PA4 Setup Start ////////////////////
	GPIOA->MODER |= (3<<2); // analog mode for PA 1
	GPIOA->MODER |= (3<<8); // analog mode for PA 4
	///////////////////////// ADC - PA1 Setup Start ////////////////////////////
	
	// 1) Enable ADC and GPIO clock
	RCC->APB2ENR |= (0b1<<8); 
	RCC->AHB1ENR |= (0b1<<0);
	
	// 2) Set prescalar in CCR
	ADC->CCR |= (0b01<<16); //PCLK divide by 4
	
	// 3) Set the scan mode and resolution in CR1
	ADC1->CR1 |= (1<<8); // scan mode enabled
	ADC1->CR1 &= ~(1<<24); // 12 bit res
	
	// 4) Set continous conversion, EOC, and data alignment in CR2
	ADC1->CR2 |= (1<<1); // Cont conversion mode enabled
	ADC1->CR2 |= (1<<10); // EOC after each conversion
	ADC1->CR2 &= ~(1<<11); // Data alignment right
	
	// 5) Set sampling time for channels
	ADC1->SMPR2 &= ~((7<<3) | (7<<12)); // sampling time of 3 cycles for channel 1 and channel 4
	
	// 6) Set the regular channel sequence length
	ADC1->SQR1 |= (2<<20); // SQR1_L = 2 for 3 conversions
	
	// 7) Configure GPIO pins to analog pins - done above 
	
	ADC1->SMPR1 |= (7<<24); // sampling time of 23.5 uS
	
	ADC->CCR |= (1<<23); // set TSVREFE bit to wake temp sensor 
	
	// enable DMA for ADC 
	ADC1->CR2 |= (1<<8);
	
	// enable continous request
	ADC1->CR2 |= (1<<9);
	
	// channel sequence 
	ADC1->SQR3 |= (1<<0); // SEQ1 for Channel 1
	ADC1->SQR3 |= (4<<5); // SEQ2 for Channel 4
	ADC1->SQR3|= (18<<10); // SEQ3 for Channel 18
}

void ADC_enable(void){
	
	// 1)Enable ADC by setting ADON bit in CR2
	ADC1->CR2 |= (1<<0); 
	
	// 2) Wait for ADC to stabilize (approx 10us)
	uint32_t delay = 10000; 
	while (delay--);
}

void ADC_start(void) {

	// 1) Clear status register
	ADC1->SR = 0; 
	
	// 2) start conversion by setting SWSTART in CR2
	ADC1->CR2 |= (1<<30); 
	
}

uint16_t ADC_get_val(void) {
	
	return ADC1->DR; //read data register
	
}

void ADC_disable(void) {
	
	//disable ADC by clearing ADON bit in CR2
	ADC1->CR2 &= ~(1<<0);
	
}

void DMA_init(void) {
	RCC->AHB1ENR |= (1<<22); // enable DMA2 clock
	DMA2_Stream0->CR &= ~(3<<6); // DMA direction is peripheral to memory 
	DMA2_Stream0->CR |= (1<<8); // CIRC = 1 
	DMA2_Stream0->CR |= (1<<10); // MINC = 1
	DMA2_Stream0->CR |= (1<<11) | (1<<13); // PSIZE=01, MSIZE=01; 16 bit data
	DMA2_Stream0->CR &= ~(7<<25); // Channel 0 selected
}

void DMA_config(uint32_t src_add, uint32_t dest_add, uint16_t size) {
	DMA2_Stream0->NDTR = size; // set size of transfer
	DMA2_Stream0->PAR = src_add; // source address is peripheral address
	DMA2_Stream0->M0AR = dest_add; // destination address is memory address 
	
	DMA2_Stream0->CR |= (1<<0); // enable DMA
}
