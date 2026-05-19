/*
 * ssd1306_spi.h
 *
 *  Created on: Apr 22, 2026
 *      Author: Admin
 */

#ifndef INC_SSD1306_SPI_H_
#define INC_SSD1306_SPI_H_
#ifndef __SSD1306_SPI_H
#define __SSD1306_SPI_H

#include "stm32f4xx_hal.h"

void SSD1306_Init(SPI_HandleTypeDef *hspi);
void SSD1306_Clear(void);
void SSD1306_UpdateScreen(void);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_WriteString(char *str);

#endif


#endif /* INC_SSD1306_SPI_H_ */
