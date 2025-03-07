#include "stm32f4xx.h"

void I2C_config(void);

void I2C_start(void);

void I2C_write(uint8_t data);

void I2C_address(uint16_t address);

void I2C_stop(void);

void I2C_write_multi(uint8_t *data, uint8_t size);

void I2C_read(uint8_t address, uint8_t *buffer, uint8_t size);

void MPU_write(uint8_t address, uint8_t reg, uint8_t data);

void MPU_read(uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t size);