/*
 * mpu6050.c
 *
 *  Created on: Apr 21, 2026
 *      Author: Admin
 */


#include "mpu6050.h"

void MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t who;

    HAL_I2C_Mem_Read(hi2c, MPU_ADDR, 0x75, 1, &who, 1, 1000);

    // Accept multiple variants
    if (who != 0x68 && who != 0x70 && who != 0x71)
    {
        // still continue for debugging
    }

    uint8_t data = 0x00;

    // Wake up
    HAL_I2C_Mem_Write(hi2c, MPU_ADDR, 0x6B, 1, &data, 1, 1000);
    HAL_Delay(100);

    // Config
    data = 0x07; HAL_I2C_Mem_Write(hi2c, MPU_ADDR, 0x19, 1, &data, 1, 1000);
    data = 0x00; HAL_I2C_Mem_Write(hi2c, MPU_ADDR, 0x1C, 1, &data, 1, 1000);
    data = 0x00; HAL_I2C_Mem_Write(hi2c, MPU_ADDR, 0x1B, 1, &data, 1, 1000);
}
void MPU6050_Read_Accel(I2C_HandleTypeDef *hi2c, int16_t *Ax, int16_t *Ay, int16_t *Az)
{
    uint8_t data[6];

    HAL_I2C_Mem_Read(hi2c, MPU_ADDR, 0x3B, 1, data, 6, 1000);

    *Ax = (int16_t)(data[0] << 8 | data[1]);
    *Ay = (int16_t)(data[2] << 8 | data[3]);
    *Az = (int16_t)(data[4] << 8 | data[5]);
}
