/*
 * max30100.h
 *
 *  Created on: Apr 21, 2026
 *      Author: Admin
 */

#ifndef INC_MAX30100_H_
#define INC_MAX30100_H_

#ifndef MAX30100_H
#define MAX30100_H

#include "stm32f4xx_hal.h"

#define MAX_ADDR (0x57<<1)

void MAX30100_Init(I2C_HandleTypeDef *hi2c);
void MAX30100_Read(I2C_HandleTypeDef *hi2c, uint16_t *ir, uint16_t *red);

#endif

#endif /* INC_MAX30100_H_ */
