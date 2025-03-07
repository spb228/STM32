#include "stm32f4xx.h"

void ADC_config(void);

void ADC_enable(void);

void ADC_start(void);
	
void DMA_config(void);

void DMA_enable(uint32_t src_add, uint32_t dest_add, uint16_t size);