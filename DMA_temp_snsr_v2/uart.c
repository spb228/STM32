#include "stm32f4xx.h"

void USART2_config(void){
	
	//////////////////////// UART-TX PA2 Setup Start ///////////////////////////
	RCC->AHB1ENR |= (1<<0); //enable GPIOA
	GPIOA->MODER |= (1<<5); // alternate function
	GPIOA->OSPEEDR |= ((0<<4)|(1<<5)); // very high speed
	GPIOA->AFR[0] |= (7<<8); // AF7 alternate function for PA2
	//////////////////////// UART-TX PA2 Setup End /////////////////////////////
	
	//////////////////////// UART-RX PA3 Setup Start ///////////////////////////
	RCC->AHB1ENR |= (1<<0);
	GPIOA->MODER |= (1<<7);
	GPIOA->OSPEEDR |= ((0<<6)|(1<<7)); 
	GPIOA->AFR[0] |= (7<<12); // AF7 alternate function for PA3
	//////////////////////// UART-RX PA3 Setup End /////////////////////////////
	
	// 1) Enable UART Clock and GPIO Clock
	RCC->APB1ENR |= (1<<17); //USART2 enabled
	
	// 2) Configure UART Pins for Alternate functions (PA2=RX AND PA3=TX)
	
	// 3) Enable USART by write UE bit in USASRT_CR1 register to 1
	USART2->CR1 = 0x00; //clear all
	USART2->CR1 |= (1<<13); // UE enabled
	
	// 4) Program M bit in USART_CR1 to define word length
	USART2->CR1 &= ~(1<<12); // 1 start bit, 8 data bits, n stop bit
	
	// 5) Select the desired baud rate using USART_BRR register
	//baud rate works for 57600 but configured for 115200. Need to fix this in the future.
	USART2->BRR = (13<<0) | (22<<4); // buadrate of 115200; mantissa=22; frac=13; apb1=42mhz
	// 6) Enable the tx/rx by setting the TE and RE bits in USART_CR1 register
	USART2->CR1 |= (1<<2); // rx enable
	USART2->CR1 |= (1<<3); // tx enable
}

void USART2_send_char(uint8_t c) {
	
	// 1) Write data to send in USART_DR
	USART2->DR = c; 
	
	// 2) After writing last data into USART_DR wait till TC=1. This means transmission of last data is successful.
	while (!(USART2->SR & (1<<6))); //wait for tc to set (data has been transmitted)
}

void USART2_send_string(char *string) {
	while (*string) USART2_send_char(*string++);
}

uint8_t USART2_get_char(void) {
	uint8_t temp; 
	while (!(USART2->SR & (1<<5))); //wait for RXNE bit to set
	temp = USART2->DR; // read data and clear RXNE
	return temp; 
}