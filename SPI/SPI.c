#include "stm32f4xx.h"

void SPI_config(void) {
	// todo: change NSS pin to output now
	//////////////////////// SPI1 - NSS - PA15 Setup Start //////////////////////
	RCC->AHB1ENR |= (0b1<<0); // enable clock for GPIO port A
	GPIOA->MODER |= (0b10<<30); // AF mode
	GPIOA->OTYPER &= ~(0b1<<15); // push 0 value to output push pull
	GPIOA->OSPEEDR |= (0b10<<30); // high speed
	GPIOA->PUPDR &= ~(0b11<<30); // push 0 value for no pull up/down
	GPIOA->AFR[1] |= (0b0101<<28); // configuring AF5 alternate function for PA15
	//////////////////////// SPI1 - NSS - PA4 Setup End ////////////////////////
	
	//////////////////////// SPI1 - SCK - PB3 Setup Start //////////////////////
	RCC->AHB1ENR |= (0b1<<1); // enable clock for GPIO port B
	GPIOB->MODER |= (0b10<<6); // AF mode
	GPIOB->OTYPER &= ~(0b1<<3); // push 0 value to output push pull
	GPIOB->OSPEEDR |= (0b10<<6); // high speed
	GPIOB->PUPDR &= ~(0b11<<6); // push 0 value for no pull up/down
	GPIOB->AFR[0] |= (0b0101<<12); // configuring AF5 alternate function for PB3
	//////////////////////// SPI1 - SCK - PB3 Setup End ////////////////////////	
	
	//////////////////////// SPI1 - MISO - PA6 Setup Start /////////////////////
	RCC->AHB1ENR |= (0b1<<0); // enable clock for GPIO port A
	GPIOA->MODER |= (0b10<<12); // AF mode
	GPIOA->OTYPER &= ~(0b1<<6); // push 0 value to output push pull
	GPIOA->OSPEEDR |= (0b10<<12); // high speed
	GPIOA->PUPDR &= ~(0b11<<12); // push 0 value for no pull up/down
	GPIOA->AFR[0] |= (0b0101<<24); // configuring AF5 alternate function for PA6
	//////////////////////// SPI1 - MISO - PA6 Setup End ///////////////////////
	
	//////////////////////// SPI1 - MOSI - PA7 Setup Start /////////////////////
	RCC->AHB1ENR |= (0b1<<0); // enable clock for GPIO port A
	GPIOA->MODER |= (0b10<<14); // AF mode
	GPIOA->OTYPER &= ~(0b1<<7); // push 0 value to output push pull
	GPIOA->OSPEEDR |= (0b10<<14); // high speed
	GPIOA->PUPDR &= ~(0b11<<14); // push 0 value for no pull up/down
	GPIOA->AFR[0] |= (0b0101<<28); // configuring AF5 alternate function for PA7
	//////////////////////// SPI1 - MOSI - PA7 Setup End ///////////////////////	
	
	// todo: disable JTDI to enable PA15
	
	RCC->APB2ENR |= (0b1<<12); // enable clock for SPI1
	
	SPI1->CR1 |= (0b001<<3); // fpclk/2 = 84Mhz/2 = 42Mhz / this depends on slave chip
	SPI1->CR1 |= (0b1<<0); // first clock transition = first data capture edge
	SPI1->CR1 |= (0b1<<1); // CK to 0 when idle
	SPI1->CR1 &= ~(0b1<<10); // full duplex
	SPI1->CR1 &= ~(0b1<<11); // 8-bit data
	SPI1->CR1 &= ~(0b1<<7); // MSB transmitted first
	SPI1->CR2 |= (0b1<<2); // enable SSOE since SPI is in master mode
	SPI1->CR2 |= (0b1<<4); // enable TI mode
	SPI1->CR1 |= (0b1<<2); // enable master mode
	SPI1->CR1 |= (0b1<<8) | (0b1<<9); // SSM=1, SSI=1 - software slave management

	SPI1->CR2 = 0; 
}

void SPI_start(){
		SPI1->CR1 |= (0b1<<6); // enable SPI;
}

void SPI_CS_enable(){
	// GPIOA->BSRR |= ();
}

void SPI_CS_disable(){
	// GPIOA->BSRR |= ();
}

void SPI_write(uint8_t SPI1_write_data) {
	while (!(SPI1->SR & (1<<1))); // while TXE is 0 (not empty) - wait for TXE buffer to be set (it is empty when set)
	SPI1->DR = SPI1_write_data; 
	while (!(SPI1->SR & (1<<1))); 
	while (SPI1->SR & (1<<7)); // wait for BSY to go back to 0 indicating SPI is not busy

	uint8_t temp = SPI1->DR; // clear data register DR
	temp = SPI1->SR; // clear overrun flag by reading SR
}

void SPI_read(void) {
	uint8_t read_buffer = SPI1->DR;
	while(!(SPI1->SR & (1<<0))); // wait until receiver buffer is empty (RXNE=0)
}

void SPI_stop(void) {
	SPI1->CR1 &= ~(0b1<<6); // disable SPI
}
