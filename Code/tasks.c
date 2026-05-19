/*
 * tasks.c
 *
 *  Created on: Apr 22, 2026
 *      Author: Admin
 */
#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
#include "max30100.h"
#include "mpu6050.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;

extern osMutexId_t i2cMutexHandle;
extern osSemaphoreId_t touchSemHandle;
extern float bpm;
extern float spo2;
extern uint8_t fall_detected;

extern uint16_t global_IR;
extern int16_t global_Az;
extern int16_t global_Ax;
extern int16_t global_Ay;
extern uint8_t system_active;

#define FALL_THRESHOLD_LOW   3000
#define FALL_THRESHOLD_HIGH  20000
#define NO_MOVEMENT_TIME     2000
#include "ssd1306_spi.h"
extern SPI_HandleTypeDef hspi1;

void StartDefaultTask(void *argument)
{
  char line1[32];
  char line2[32];

  // 🔹 OLED INIT
  SSD1306_Init(&hspi1);

  for(;;)
  {
    // 🔴 SYSTEM OFF
    if(system_active == 0)
    {
        SSD1306_Clear();

        SSD1306_SetCursor(0,0);
        SSD1306_WriteString("SYSTEM OFF");

        SSD1306_SetCursor(0,16);
        SSD1306_WriteString("TOUCH TO START");

        SSD1306_UpdateScreen();

        osDelay(500);
        continue;
    }

    // 🚨 FALL DETECTED
    if(fall_detected)
    {
        SSD1306_Clear();

        SSD1306_SetCursor(0,0);
        SSD1306_WriteString("!! FALL ALERT !!");

        SSD1306_SetCursor(0,16);
        SSD1306_WriteString("HELP NEEDED!");

        SSD1306_UpdateScreen();

        osDelay(500);
        continue;
    }

    // 🚨 EMERGENCY (no finger)
    if(global_IR < 2000)
    {
        SSD1306_Clear();

        SSD1306_SetCursor(0,0);
        SSD1306_WriteString("!! EMERGENCY !!");

        SSD1306_SetCursor(0,16);
        SSD1306_WriteString("VITALS DROPPING");

        SSD1306_UpdateScreen();

        osDelay(500);
        continue;
    }

    // 🟢 NORMAL CONDITION
    sprintf(line1, "HR:%d  SPO2:%d", (int)bpm, (int)spo2);
    sprintf(line2,"IR:%d \n",  global_IR);

    SSD1306_Clear();

    SSD1306_SetCursor(0,0);
    SSD1306_WriteString(line1);

    SSD1306_SetCursor(0,16);
    SSD1306_WriteString(line2);

    SSD1306_UpdateScreen();

    osDelay(500);
  }
}


void StartMAXTask(void *argument)
{
  uint16_t ir, red;

  // 🔹 BPM variables
  static uint16_t prev_ir = 0;
  static uint16_t threshold = 0;
  uint32_t lastBeat = 0;

  // 🔹 SpO2 variables
  static float dc_ir = 0, dc_red = 0;
  static float ac_ir = 0, ac_red = 0;

  // 🔹 SpO2 timing + smoothing
  static uint32_t lastSpO2Time = 0;
  static float prev_spo2 = 95;

  MAX30100_Init(&hi2c3);
  osDelay(100);

  for(;;)
  {if(system_active == 0)
  {
      bpm = 0;
      spo2 = 0;
      osDelay(100);
      continue;
  }

    // 🔥 Read sensor
    osMutexAcquire(i2cMutexHandle, osWaitForever);
    MAX30100_Read(&hi2c3, &ir, &red);
    osDelay(2);   // 🔥 IMPORTANT for shared I2C
    osMutexRelease(i2cMutexHandle);

    global_IR = ir;
    static uint32_t lastValidBeat = 0;
    // 🚫 No finger detection
    if(ir < 800)
    {
        bpm = 0;
        spo2 = 0;
        threshold = 0;
        prev_ir = 0;
        osDelay(50);
        continue;
    }

    // ===================== ❤️ BPM =====================
    // Adaptive threshold
    threshold = (threshold * 9 + ir) / 10;

    // Rising edge detection
    if(ir > threshold + 200 && prev_ir <= threshold + 200)
    {
        uint32_t now = HAL_GetTick();
        uint32_t dt = now - lastBeat;

        if(dt > 300 && dt < 2000)
        {
            uint16_t new_bpm = 60000 / dt;

            if(new_bpm > 55 && new_bpm < 110)
            {
                static uint16_t prev_bpm = 0;
                bpm = (new_bpm + prev_bpm) / 2;
                prev_bpm = bpm;

                lastValidBeat = now;   // 🔥 important
            }
        }

        lastBeat = now;
    }
    // ===================== 🩸 SpO2 =====================
    // DC tracking (DO NOT reset this)
    dc_ir  = 0.95f * dc_ir  + 0.05f * ir;
    dc_red = 0.95f * dc_red + 0.05f * red;

    // AC components
    ac_ir  = ir  - dc_ir;
    ac_red = red - dc_red;

    // 🔥 Update SpO2 only every 1 second
    if(HAL_GetTick() - lastSpO2Time > 1000)
    {
        if(dc_ir > 0 && dc_red > 0 && ac_ir > 50)
        {
            float R = (ac_red / dc_red) / (ac_ir / dc_ir);
            float new_spo2 = 110 - 25 * R;

            // clamp realistic values
            if(new_spo2 > 100) new_spo2 = 100;
            if(new_spo2 < 90)  new_spo2 = 90;

            // smoothing
            spo2 = (new_spo2 + prev_spo2) / 2;
            prev_spo2 = spo2;
        }

        lastSpO2Time = HAL_GetTick();
    }

    osDelay(50);
  }
}

extern uint8_t fall_detected;

void StartMPUTask(void *argument)
{
    int16_t Ax, Ay, Az;

    static uint8_t fall_flag = 0;
    static uint32_t fall_time = 0;

    int32_t acc_sq;

    MPU6050_Init(&hi2c3);

    for(;;)
    {
        if(system_active == 0)
        {
            osDelay(100);
            continue;
        }

        // Read sensor
        osMutexAcquire(i2cMutexHandle, osWaitForever);
        MPU6050_Read_Accel(&hi2c1, &Ax, &Ay, &Az);
        osDelay(2);   // important for bus stability
        osMutexRelease(i2cMutexHandle);

        // Magnitude squared
        acc_sq = (int32_t)Ax*Ax + (int32_t)Ay*Ay + (int32_t)Az*Az;

        // -------- FALL LOGIC --------

        // Step 1: Free fall (drop in value)
        if(acc_sq < (14000 * 14000) && fall_flag == 0)
        {
            fall_flag = 1;
            fall_time = osKernelGetTickCount();
        }

        // Step 2: Impact (spike after drop)
        if(fall_flag == 1)
        {
            if(acc_sq > (20000 * 20000))
            {
                fall_detected = 1;

                // 🔴 OLED / LED / print here
                printf("FALL DETECTED\n");

                fall_flag = 0;
            }

            // Timeout reset
            if(osKernelGetTickCount() - fall_time > 700)
            {
                fall_flag = 0;
            }
        }

        // Reset when stable again
        if(acc_sq > (16000 * 16000) && fall_flag == 0)
        {
            fall_detected = 0;
        }

        osDelay(50);
    }
}


extern uint8_t touch_flag;
extern uint8_t system_active;

void StartTouchTask(void *argument)
{
    for(;;)
    {
        osSemaphoreAcquire(touchSemHandle, osWaitForever);

        // 🔥 If fall detected → reset it
        if(fall_detected == 1)
        {
            fall_detected = 0;
        }
        else
        {
            // normal toggle
            system_active = !system_active;
        }
    }
}
