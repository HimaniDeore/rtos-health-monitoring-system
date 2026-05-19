/*
 * mpu6050.h
 *
 *  Created on: Apr 21, 2026
 *      Author: Admin
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f4xx_hal.h"
#define MPU_ADDR (0x68 << 1)
void MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_Read_Accel(I2C_HandleTypeDef *hi2c, int16_t *Ax, int16_t *Ay, int16_t *Az);

#endif
#endif /* INC_MPU6050_H_ */
