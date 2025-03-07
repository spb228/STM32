#include "stm32f4xx.h"

void USART2_config(void);
void USART2_send_char(uint8_t c);
void USART2_send_string(char *string);
uint8_t USART2_get_char(void);