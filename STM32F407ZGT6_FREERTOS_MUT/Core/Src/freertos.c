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
osThreadId MHandle;
osThreadId LHandle;
osThreadId HHandle;
osMutexId myMutex01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void M_Task(void const * argument);
void L_Task(void const * argument);
void H_Task(void const * argument);

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
  /* Create the mutex(es) */
  /* definition and creation of myMutex01 */
  osMutexDef(myMutex01);
  myMutex01Handle = osMutexCreate(osMutex(myMutex01));

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
  /* definition and creation of M */
  osThreadDef(M, M_Task, osPriorityNormal, 0, 128);
  MHandle = osThreadCreate(osThread(M), NULL);

  /* definition and creation of L */
  osThreadDef(L, L_Task, osPriorityBelowNormal, 0, 128);
  LHandle = osThreadCreate(osThread(L), NULL);

  /* definition and creation of H */
  osThreadDef(H, H_Task, osPriorityAboveNormal, 0, 128);
  HHandle = osThreadCreate(osThread(H), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_M_Task */
/**
  * @brief  Function implementing the M thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_M_Task */
void M_Task(void const * argument)
{
  /* USER CODE BEGIN M_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(2);
    printf("M Running\r\n");
  }
  /* USER CODE END M_Task */
}

/* USER CODE BEGIN Header_L_Task */
/**
* @brief Function implementing the L thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_L_Task */
void L_Task(void const * argument)
{
  /* USER CODE BEGIN L_Task */
  /* Infinite loop */
  for(;;)
  {
    xSemaphoreTake(myMutex01Handle,0);
    printf("L Running\r\n");
    xSemaphoreGive(myMutex01Handle);
    printf("L Over\r\n");
  }
  /* USER CODE END L_Task */
}

/* USER CODE BEGIN Header_H_Task */
/**
* @brief Function implementing the H thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_H_Task */
void H_Task(void const * argument)
{
  /* USER CODE BEGIN H_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
    if(xSemaphoreTake(myMutex01Handle,portMAX_DELAY) == pdTRUE)
      printf("H Running\r\n");
    else 
      printf("H fail\r\n");
    xSemaphoreGive(myMutex01Handle);
    printf("H Over\r\n");
  }
  /* USER CODE END H_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
int fputc(int ch,FILE *f)
{
  HAL_UART_Transmit(&huart1,(uint8_t *)(&ch),sizeof(ch),HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END Application */
