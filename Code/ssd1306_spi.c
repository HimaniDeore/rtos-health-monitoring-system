/*
 * ssd1306_spi.c
 *
 *  Created on: Apr 22, 2026
 *      Author: Admin
 */
#include "ssd1306_spi.h"
#include <string.h>
#include <stdio.h>

extern SPI_HandleTypeDef hspi1;

// GPIO mapping (as per your wiring)
#define CS_PORT GPIOA
#define CS_PIN  GPIO_PIN_4

#define DC_PORT GPIOB
#define DC_PIN  GPIO_PIN_0

#define RST_PORT GPIOB
#define RST_PIN  GPIO_PIN_1

static uint8_t buffer[1024];
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

// ================= LOW LEVEL =================

void OLED_Select()   { HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); }
void OLED_Unselect() { HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET); }

void OLED_Reset()
{
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
}

void OLED_WriteCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET);
    OLED_Select();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
    OLED_Unselect();
}

void OLED_WriteData(uint8_t *data, size_t size)
{
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    OLED_Select();
    HAL_SPI_Transmit(&hspi1, data, size, 100);
    OLED_Unselect();
}

// ================= INIT =================

void SSD1306_Init(SPI_HandleTypeDef *hspi)
{
    OLED_Reset();

    OLED_WriteCommand(0xAE);
    OLED_WriteCommand(0x20);
    OLED_WriteCommand(0x10);
    OLED_WriteCommand(0xB0);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x10);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0xFF);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0xF0);
    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0x22);
    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x20);
    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);

    SSD1306_Clear();
    SSD1306_UpdateScreen();
}

// ================= FONT (5x7) =================

static const uint8_t font5x7[][5] = {
    {0,0,0,0,0}, // space (32)
    {0x7E,0x11,0x11,0x11,0x7E}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x09,0x01}, // F
    {0x3E,0x41,0x49,0x49,0x7A}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x04,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x3F,0x40,0x38,0x40,0x3F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x07,0x08,0x70,0x08,0x07}, // Y
    {0x61,0x51,0x49,0x45,0x43}, // Z
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}, // 9
    {0x00,0x36,0x36,0x00,0x00}  // :
};

// ================= DRAW =================

void SSD1306_Clear(void)
{
    memset(buffer, 0, sizeof(buffer));
}

void SSD1306_UpdateScreen(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        OLED_WriteCommand(0xB0 + i);
        OLED_WriteCommand(0x00);
        OLED_WriteCommand(0x10);

        OLED_WriteData(&buffer[128 * i], 128);
    }
}

void SSD1306_SetCursor(uint8_t x, uint8_t y)
{
    cursor_x = x;
    cursor_y = y / 8;   // 🔥 convert pixels → page
}

void SSD1306_WriteChar(char c)
{
    const uint8_t *ch = font5x7[0]; // default = space

    if (c >= 'A' && c <= 'Z')
        ch = font5x7[c - 'A' + 1];
    else if (c >= '0' && c <= '9')
        ch = font5x7[c - '0' + 27];
    else if (c == ':')
        ch = font5x7[37];
    else if (c == ' ')
        ch = font5x7[0];
    else
        ch = font5x7[0]; // unknown → space

    for (int i = 0; i < 5; i++)
    {
        if (cursor_x < 128)
            buffer[cursor_x + (cursor_y * 128)] = ch[i];
        cursor_x++;
    }

    cursor_x++; // spacing
}

void SSD1306_WriteString(char *str)
{
    while (*str)
    {
        SSD1306_WriteChar(*str++);
    }
}
