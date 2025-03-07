#include "stm32f4xx.h"

void I2C_config(void);

void I2C_start(void);

void I2C_write(uint8_t data);

void I2C_address(uint16_t address);

void I2C_stop(void);

void I2C_write_multi(uint8_t *data, uint8_t size);