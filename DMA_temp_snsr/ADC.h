#include "stm32f4xx.h"

void ADC_init(void);

void ADC_enable(void);

void ADC_start(void);

uint16_t ADC_get_val(void);

void ADC_disable(void);

void DMA_init(void);

void DMA_config(uint32_t src_add, uint32_t dest_add, uint16_t size);