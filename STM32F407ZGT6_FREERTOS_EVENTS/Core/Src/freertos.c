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
EventGroupHandle_t Eventshandle = NULL; //freertos v1版本没有创建事件组的cubemx v2版本有
/* USER CODE END Variables */
osThreadId KEYHandle;
osThreadId BIUHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint8_t key_scan(void);
/* USER CODE END FunctionPrototypes */

void KEY_Task(void const * argument);
void BIU_Task(void const * argument);

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of KEY */
  osThreadDef(KEY, KEY_Task, osPriorityNormal, 0, 128);
  KEYHandle = osThreadCreate(osThread(KEY), NULL);

  /* definition and creation of BIU */
  osThreadDef(BIU, BIU_Task, osPriorityNormal, 0, 128);
  BIUHandle = osThreadCreate(osThread(BIU), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  Eventshandle = xEventGroupCreate();
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_KEY_Task */
/**
  * @brief  Function implementing the KEY thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_KEY_Task */
void KEY_Task(void const * argument)
{
  /* USER CODE BEGIN KEY_Task */
  /* Infinite loop */
  for(;;)
  {
    if(key_scan() == 1)
    {
      printf("key1down\r\n");
      xEventGroupSetBits(Eventshandle,0x0001);
    }
    if(key_scan() == 2)
    {
      printf("key2down\r\n");
      xEventGroupSetBits(Eventshandle,0x0010);
    }
    if(key_scan() == 3)
    {
      printf("key3down\r\n");
      xEventGroupSetBits(Eventshandle,0x0100);
    }
  }
  /* USER CODE END KEY_Task */
}

/* USER CODE BEGIN Header_BIU_Task */
/**
* @brief Function implementing the BIU thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BIU_Task */
void BIU_Task(void const * argument)
{
  /* USER CODE BEGIN BIU_Task */ 
  EventBits_t event;
  /* Infinite loop */
  for(;;)
  {
    event = xEventGroupWaitBits(Eventshandle,0x0111,pdTRUE,pdTRUE,0);//第二个pdtrue是与操作,pdfalse是或操作
    if(event == 0x0111)
      printf("biu\r\n");
    // event = xEventGroupWaitBits(Eventshandle,0x0111,pdTRUE,pdFALSE,0);
    // if((event == 0x0001)||(event == 0x0010)||(event == 0x0100))
    //   printf("biu\r\n");
  }
  /* USER CODE END BIU_Task */
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
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
  {
    osDelay(10);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
    {
      key_val = 1;
    }
    while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0);
  }
  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
  {
    osDelay(10);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
    {
      key_val = 2;
    }
    while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0);
  }
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == 0)
  {
    osDelay(10);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == 0)
    {
      key_val = 3;
    }
    while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == 0);
  }
  return key_val;
}
/* USER CODE END Application */
