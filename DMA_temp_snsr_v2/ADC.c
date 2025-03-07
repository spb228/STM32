#include "ADC.h"

void ADC_config(void){
	// 1) enable ADC and GPIO Clock
	RCC->AHB1ENR |= (1<<0); // enabling GPIO A clock
	RCC->APB2ENR |= (1<<8); // enabling ADC1
	
	// 2) set prescalar in common control register (CCR)
	ADC->CCR |= (0b10<<16); // using presc of 6; 84mhz/6 = 14mhz clocking
	
	// 3) set scan mode and resolution in CR1
	ADC1->CR1 |= (1<<8); // enable scan mode for multiple channels
	ADC1->CR1 &= ~(0b11<<24); // force 0 into 24/25 for 12 bit; 2^12=4095 values
	
	// 4) set cont conversion, EOC, DMA, Data alignment in CR2
	ADC1->CR2 |= (1<<1); // convert ADC values continously
	ADC1->CR2 |= (1<<8); // DMA mode enabled
	ADC1->CR2 |= (1<<10); // EOCS flag after each conversion
	ADC1->CR2 &= ~(1<<11); // right align 12 bits in the 16 bit regiser
	ADC1->CR2 |= (1<<9); // DMA is continous and will only disable if DMA itself is disabled
	
	// 5) set sampling time for channels
	// Tconv = (sampling time + cycles) / ADC clock
	// 			 = 144 + 12 / 14000000
	//			 =	~11 uS (but we want close to 10uS)
	ADC1->SMPR2 |= (0b110<<3); // 144 cycles sampling time for channel 1
	ADC1->SMPR2 |= (0b110<<12); // 144 cycles sampling time for channel 2	
	ADC1->SMPR1 |= (0b110<<24); // 144 cycles sampling time for channel 18

	// 6) set regular channel sequence length
	ADC1->SQR1 |= (0b10<<20); // converting three (0b10) channels 
	ADC1->SQR3 |= (1<<0); // SEQ 1 for ch1
	ADC1->SQR3 |= (4<<5); // SEQ 2 for ch4
	ADC1->SQR3 |= (18<<10); // SEQ 3 for ch18
	
	// Set the TSVREFE Bit to wake the sensor
	ADC->CCR |= (1<<23);
	
	// 7) set gpio pins in analog mode
	///////////////////////// ADC - PA1 and PA4 Setup Start ////////////////////
	GPIOA->MODER |= (3<<2); // analog mode for PA 1
	GPIOA->MODER |= (3<<8); // analog mode for PA 4
	///////////////////////// ADC - PA1 Setup Start ////////////////////////////
}

void ADC_enable(void) {
	ADC1->CR2 |= (1<<0); // enable ADC
	// wait for ADC to enable
	uint32_t delay = 10000;
	while (delay--);
}

void ADC_start(void) {
	ADC1->SR = 0; // clear status reg or conv will not start
	ADC1->CR2 |= (1<<30); // start conversion 
}

void DMA_config(void) {
	// 1) enable DMA2 clock 
	RCC->AHB1ENR |= (1<<22); // enable clock for DMA2
	
	// 2) configure the configuration register
	DMA2_Stream0->CR &= ~(0b11<<6); // peripheral to memory
	DMA2_Stream0->CR |= (1<<8); // circular mode enabled
	DMA2_Stream0->CR |= (1<<10); // Memeory Increment mode enabled
	DMA2_Stream0->CR |= (1<<11); // peripheral size data is half word/16bits
	DMA2_Stream0->CR |= (1<<13); // memory size data is half word/16bits
	DMA2_Stream0->CR &= ~(0b111<<25); // channel 0 selected
}

void DMA_enable(uint32_t src_add, uint32_t dest_add, uint16_t size) {
	// 1) set data size in NDTR reg 
	DMA2_Stream0->NDTR = size; 
	
	// 2) set peripheral addr and memory addr
	DMA2_Stream0->PAR = src_add; 
	DMA2_Stream0->M0AR = dest_add; 

	// 3) enable the DMA stream 
	DMA2_Stream0->CR |= (1<<0); 
}

