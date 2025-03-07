// DMA (circular mode) with Temp Sensor / ADC

#include "stm32f4xx.h"
#include "Sys_Clock_Config.h"
#include "GPIO_Config.h"
#include "Delay.h"
#include "uart.h"
#include "i2c.h"
#include "ADC.h"
#include "EXT_interrupt.h"
#include "SPI.h"

//uint16_t ADC_VAL[2] = {0,0}; // store ADC data

//int flag_EXTI = 0; // flag for ext interrupt 1

//int count = 0; // count for ext interrupt 1

//uint8_t SPI_write_data; 

uint16_t rx_data[3];

float temperature; 

//int check = 0; 

int main(void) {
	Sys_Clock_Config();
	//LED_PushButton_Config();
	TIM3_Config(); // has delay func
	ADC_config();
	ADC_enable();
	DMA_config();
	DMA_enable((uint32_t)&ADC1->DR, (uint32_t)rx_data, 3);
	ADC_start();
	
	while(1) {
		//check = 1;
		temperature = (((float)(3.3*rx_data[2]/(float)4095) - 0.76) / 0.0025) + 25;
		Delay_ms (1000);
	}
}	