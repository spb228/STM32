#include "stm32f4xx.h"

void I2C_config(void) {
	// 1) Enable I2C clock and GPIO clock
	RCC->APB1ENR |= (1<<21);
	
	// 2) Configure I2C pins for alternate function
	
	// 3) Reset the I2C
	I2C1->CR1 |= (0b1<<15); // make 1 to reset
	I2C1->CR1 &= ~(0b1<<15); // make 0 to exit reset state
	
	// 4) Program peripheral input clock in I2C_CR2 
	I2C1->CR2 |= (42<<0);
	
	// 5) Configure clock control registers
	I2C1->CCR = 225<0; 
	
	// 6) Configure rise time register
	I2C1->TRISE = 43; 
	
	// 7) Program I2C_CR1 to enable peripheral
}

void I2C_start(void) {
	
	// 1) Send start condition
	I2C1->CR1 |= (1<<8);
	
	// 2) Wait for SB to set - this indicates that start condition is generated
	while (!(I2C1->SR1 & (1<<0)));
	
}

void I2C_write(uint8_t data) {
	
	// 1) wait for TXE to set - this indicates DR is empty
	while(!(I2C1->SR1 & (1<<7)));
	
	// 2) Send the data to DR register
	I2C1->DR = data; 
	
	//3) wait for BTF to set - this indicates the end of last data tranmission
	while (!(I2C1->SR1 & (1<<2))); 
}

void I2C_address(uint16_t address){
	
	// 1) Send slave address to the DR 
	I2C1->DR = address;
	
	// 2) Wait for ADDR to set - this indicates end of address transmission
	while (!(I2C1->SR1 & (1<<1)));
	
	// 3) clear ADDR by reading SR1 and SR2
	uint8_t temp = I2C1->SR1 | I2C1->SR2;

}

void I2C_stop(void) {
	
	I2C1->CR1 |= (1<<9); // stop I2C
	
}

void I2C_write_multi(uint8_t *data, uint8_t size) {
	
	// 1) Wait for txe to set - this indicates DR is empty
	while (!(I2C1->SR1 & (1<<7)));
	
	// 2) Keep sending datat to DR after performing check to see if TXE bit is set
	while (size) {
		while (!(I2C1->SR1 & (1<<7)));
		I2C1->DR = (volatile uint32_t) *data++;
		size--;
	}
	
	// 3) wait for btf to set - this indicates last data transmission
	while (!(I2C1->SR1 & (1<<2)));
	
}