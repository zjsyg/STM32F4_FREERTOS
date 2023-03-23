/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stdio.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId CJHandle;
osThreadId DISPLAYHandle;
osSemaphoreId myBinarySem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint8_t key_scan(void);
/* USER CODE END FunctionPrototypes */

void CJ_Task(void const * argument);
void DISPLAY_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of CJ */
  osThreadDef(CJ, CJ_Task, osPriorityNormal, 0, 128);
  CJHandle = osThreadCreate(osThread(CJ), NULL);

  /* definition and creation of DISPLAY */
  osThreadDef(DISPLAY, DISPLAY_Task, osPriorityNormal, 0, 128);
  DISPLAYHandle = osThreadCreate(osThread(DISPLAY), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_CJ_Task */
/**
  * @brief  Function implementing the CJ thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_CJ_Task */
void CJ_Task(void const * argument)
{
  /* USER CODE BEGIN CJ_Task */
  /* Infinite loop */
  for(;;)
  {
    if(key_scan() == 1)
    {
      if(xSemaphoreGive(myBinarySem01Handle) == pdTRUE)
        printf("give sem\r\n");
      else
        printf("give fail\r\n");
    }
  }
  /* USER CODE END CJ_Task */
}

/* USER CODE BEGIN Header_DISPLAY_Task */
/**
* @brief Function implementing the DISPLAY thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DISPLAY_Task */
void DISPLAY_Task(void const * argument)
{
  /* USER CODE BEGIN DISPLAY_Task */
  /* Infinite loop */
  for(;;)
  {
    if(key_scan() == 2)
    {
      if(xSemaphoreTake(myBinarySem01Handle,0) == pdTRUE)
        printf("take sem\r\n");
      else
        printf("take fail\r\n");
    }
    osDelay(100);
  }
  /* USER CODE END DISPLAY_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
int fputc(int ch,FILE *f)
{
  HAL_UART_Transmit(&huart1,(uint8_t *)(&ch),sizeof(ch),HAL_MAX_DELAY);
  return ch;
}

uint8_t key_scan(void)
{
  uint8_t key_val = 0;
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == GPIO_PIN_RESET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == GPIO_PIN_RESET)
    {
      key_val = 1;
    }
    while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == GPIO_PIN_RESET);
  }
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == GPIO_PIN_RESET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == GPIO_PIN_RESET)
    {
      key_val = 2;
    }
    while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == GPIO_PIN_RESET);
  }
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == GPIO_PIN_RESET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == GPIO_PIN_RESET)
    {
      key_val = 3;
    }
    while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == GPIO_PIN_RESET);
  }
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) == GPIO_PIN_RESET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) == GPIO_PIN_RESET)
    {
      key_val = 4;
    }
    while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) == GPIO_PIN_RESET);
  }
  return key_val;
}
/* USER CODE END Application */
