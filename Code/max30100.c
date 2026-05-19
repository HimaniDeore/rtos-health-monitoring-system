/*
 * max30100.c
 *
 *  Created on: Apr 21, 2026
 *      Author: Admin
 */


#include "max30100.h"

void MAX30100_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data;

    data = 0x40;
    HAL_I2C_Mem_Write(hi2c, MAX_ADDR, 0x06, 1, &data, 1, 100);
    HAL_Delay(200);

    data = 0x03;
    HAL_I2C_Mem_Write(hi2c, MAX_ADDR, 0x06, 1, &data, 1, 100);
    HAL_Delay(50);

    data = 0x27;
    HAL_I2C_Mem_Write(hi2c, MAX_ADDR, 0x07, 1, &data, 1, 100);
    HAL_Delay(50);

    data = 0xFF;
    HAL_I2C_Mem_Write(hi2c, MAX_ADDR, 0x09, 1, &data, 1, 100);
    HAL_Delay(50);
}

void MAX30100_Read(I2C_HandleTypeDef *hi2c, uint16_t *ir, uint16_t *red)
{
    uint8_t buffer[4];

    HAL_I2C_Mem_Read(hi2c, MAX_ADDR, 0x05, 1, buffer, 4, 100);

    *ir  = (buffer[0] << 8) | buffer[1];
    *red = (buffer[2] << 8) | buffer[3];
}
