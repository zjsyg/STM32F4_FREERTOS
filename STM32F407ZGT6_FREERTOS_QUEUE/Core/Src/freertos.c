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
osThreadId FS1Handle;
osThreadId FS2Handle;
osThreadId JSHandle;
osMessageQId myQueue01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint8_t key_scan(void);
/* USER CODE END FunctionPrototypes */

void FS1_Task(void const * argument);
void FS2_Task(void const * argument);
void JS_Task(void const * argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 2, uint16_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of FS1 */
  osThreadDef(FS1, FS1_Task, osPriorityNormal, 0, 128);
  FS1Handle = osThreadCreate(osThread(FS1), NULL);

  /* definition and creation of FS2 */
  osThreadDef(FS2, FS2_Task, osPriorityNormal, 0, 128);
  FS2Handle = osThreadCreate(osThread(FS2), NULL);

  /* definition and creation of JS */
  osThreadDef(JS, JS_Task, osPriorityNormal, 0, 128);
  JSHandle = osThreadCreate(osThread(JS), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_FS1_Task */
/**
  * @brief  Function implementing the FS1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_FS1_Task */
void FS1_Task(void const * argument)
{
  /* USER CODE BEGIN FS1_Task */
  uint16_t buf = 1024;
  BaseType_t status = pdFAIL;
  /* Infinite loop */
  for(;;)
  {
    if(key_scan() == 1)
    {
      status = xQueueSend(myQueue01Handle,&buf,0);
      if(status == pdPASS)
        printf("write list:%d\r\n",buf);
      else
        printf("write list fail\r\n");
    }
    osDelay(100);
  }
  /* USER CODE END FS1_Task */
}

/* USER CODE BEGIN Header_FS2_Task */
/**
* @brief Function implementing the FS2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FS2_Task */
void FS2_Task(void const * argument)
{
  /* USER CODE BEGIN FS2_Task */
  uint16_t buf = 666;
  BaseType_t status = pdFAIL;
  /* Infinite loop */
  for(;;)
  {
    if(key_scan() == 2)
    {
      status = xQueueSend(myQueue01Handle,&buf,0);
      if(status == pdPASS)
        printf("write list:%d\r\n",buf);
      else
        printf("write list fail\r\n");
    }
    osDelay(100);
  }
  /* USER CODE END FS2_Task */
}

/* USER CODE BEGIN Header_JS_Task */
/**
* @brief Function implementing the JS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_JS_Task */
void JS_Task(void const * argument)
{
  /* USER CODE BEGIN JS_Task */
  uint16_t buf;
  BaseType_t status = pdFAIL;
  /* Infinite loop */
  for(;;)
  {
    if(key_scan() == 3)
    {
      status = xQueueReceive(myQueue01Handle,&buf,0);
      if(status == pdPASS)
        printf("read list:%d\r\n",buf);
      else
        printf("read list fail\r\n");
    }
    osDelay(100);
  }
  /* USER CODE END JS_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
uint8_t key_scan(void)
{
  uint8_t key_val = 0;
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == GPIO_PIN_SET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == GPIO_PIN_SET)
    {
      key_val = 1;
    }
  }
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == GPIO_PIN_SET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == GPIO_PIN_SET)
    {
      key_val = 2;
    }
  }
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == GPIO_PIN_SET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == GPIO_PIN_SET)
    {
      key_val = 3;
    }
  }
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) == GPIO_PIN_SET)
  {
    osDelay(20);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) == GPIO_PIN_SET)
    {
      key_val = 4;
    }
  }
  return key_val;
}
/* USER CODE END Application */
