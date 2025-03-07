#include "stm32f4xx.h"

void I2C_config(void) {
	//////////////////////// I2C_1 SCL - PB8 Setup Start ///////////////////////
	RCC->AHB1ENR |= (0b1<<1);
	GPIOB->MODER |= (0b10<<16); // alternate function
	GPIOB->OTYPER |= (0b1<<8); // open drain
	GPIOB->OSPEEDR |= (0b10<<16); // high speed
	GPIOB->PUPDR |= (0b01<<16); //pull up resistor
	GPIOB->AFR[1] |= (0b0100<<0); 
	//////////////////////// I2C_1 SCL - PB8 Setup End /////////////////////////
	
	//////////////////////// I2C_1 SDA - PB9 Setup Start ///////////////////////
	RCC->AHB1ENR |= (0b1<<1); 
	GPIOB->MODER |= (0b10<<18); // alternate function
	GPIOB->OTYPER |= (0b1<<9); // open drain
	GPIOB->OSPEEDR |= (0b10<<18); // high speed
	GPIOB->PUPDR |= (0b01<<18); //pull up resistor
	GPIOB->AFR[1] |= (0b0100<<4); 
	//////////////////////// I2C_1 SDA - PB9 Setup End /////////////////////////
	
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

void I2C_read(uint8_t address, uint8_t *buffer, uint8_t size){
	
	int remaining = size; 
	
	// if one byte of data to be received
	if (size == 1) {
		
		I2C1->DR = address; // send address
		while (!(I2C1->SR1 & (1<<1))); // wait for ADDR bit to set
		
		I2C1->CR1 &= ~(1<<10); // clear ACK bit
		uint8_t temp = I2C1->SR1 | I2C1->SR2; // read SR1 and SR2 to clear ADDR bit...EV6 condition
		I2C1->CR1 |= (1<<9); // stop I2C
		
		while (!(I2C1->SR1 & (1<<6))); // wait for RxNE to set
		
		buffer[size-remaining] = I2C1->DR; // read data from DATA REG
		
	}
	
	else {
		
		I2C1->DR = address; // send address
		while (!(I2C1->SR1 & (1<<1))); // wait for ADDR bit to set
		
		uint8_t temp = I2C1->SR1 | I2C1->SR2; // read SR1 and SR2 to clear ADDR bit
		
		while (remaining>2) {
			
			while (!(I2C1->SR1 & (1<<6))); // wait for RxNE to set
			
			buffer[size-remaining] = I2C1->DR; //copy data into buffer
			
			I2C1->CR1 |= (1<<10); // set ACK bit to acknowledge data received
			
			remaining--; 
			
			//read last byte
			while (!(I2C1->SR1 & (1<<6))); // wait for RxNE to set 
			buffer[size-remaining] = I2C1->DR; // copy data into buffer
		}
		
	}
	
}

// accelerometer function - not used
void MPU_write(uint8_t address, uint8_t reg, uint8_t data) {
	
	I2C_start();
	I2C_address(address);
	I2C_write(reg);
	I2C_write(data);
	I2C_stop();
}

// accelerometer function - not used
void MPU_read(uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t size) {
	
	I2C_start();
	I2C_address(address);
	I2C_write(reg);
	I2C_start();
	I2C_read(address + 0x01, buffer, size);
	I2C_stop();
}
