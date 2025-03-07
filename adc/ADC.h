#include "stm32f4xx.h"

void ADC_init(void);

void ADC_enable(void);

void ADC_start(int channel);
	
void ADC_wait_for_conv(void);

uint16_t ADC_get_val(void);

void ADC_disable(void);

