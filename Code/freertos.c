/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "max30100.h"
#include "mpu6050.h"
#include "ssd1306_spi.h"
#include <stdio.h>
#include "tasks.h"
extern UART_HandleTypeDef huart2;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;
osMutexId_t i2cMutexHandle;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

volatile uint8_t touch_flag = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
// KEEP THESE ONLY ONCE

volatile uint8_t fall_detected = 0;
volatile uint8_t system_active = 0;

uint8_t mpu_status = 0;

int16_t global_Ax = 0;
int16_t global_Ay = 0;
int16_t global_Az = 0;

int32_t global_acc = 0;

uint16_t global_IR = 0;

float bpm = 0;
float spo2 = 0;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MAX_Task */
osThreadId_t MAX_TaskHandle;
const osThreadAttr_t MAX_Task_attributes = {
  .name = "MAX_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MPU_Task */
osThreadId_t MPU_TaskHandle;
const osThreadAttr_t MPU_Task_attributes = {
  .name = "MPU_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
osThreadId_t Touch_TaskHandle;

const osThreadAttr_t Touch_Task_attributes = {
  .name = "Touch_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osSemaphoreId_t touchSemHandle;

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartMAXTask(void *argument);
void StartMPUTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	i2cMutexHandle = osMutexNew(NULL);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	touchSemHandle = osSemaphoreNew(1, 0, NULL);
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of MAX_Task */
  MAX_TaskHandle = osThreadNew(StartMAXTask, NULL, &MAX_Task_attributes);

  /* creation of MPU_Task */
  MPU_TaskHandle = osThreadNew(StartMPUTask, NULL, &MPU_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */

  Touch_TaskHandle = osThreadNew(StartTouchTask, NULL, &Touch_Task_attributes);
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */

void StartDefaultTask(void *argument);
void StartMAXTask(void *argument);
void StartMPUTask(void *argument);
void StartTouchTask(void *argument);
/* USER CODE END Header_StartDefaultTask */


/* USER CODE BEGIN Header_StartMAXTask */
/**
* @brief Function implementing the MAX_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAXTask */

/* USER CODE BEGIN Header_StartMPUTask */
/**
* @brief Function implementing the MPU_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMPUTask */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

