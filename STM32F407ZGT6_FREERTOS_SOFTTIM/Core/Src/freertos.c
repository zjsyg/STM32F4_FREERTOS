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
osThreadId defaultTaskHandle;
osTimerId myTimer01Handle;
osTimerId myTimer02Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint8_t key_scan(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Callback01(void const * argument);
void Callback02(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

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

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

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

  /* Create the timer(s) */
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, Callback01);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);

  /* definition and creation of myTimer02 */
  osTimerDef(myTimer02, Callback02);
  myTimer02Handle = osTimerCreate(osTimer(myTimer02), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  xTimerChangePeriod(myTimer01Handle,500,200);
  xTimerChangePeriod(myTimer02Handle,1000,200);
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTimerStop(myTimer01Handle,100);
  xTimerStop(myTimer02Handle,100);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
    {
      osDelay(20);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
      {
        printf("KEY1 DOWN\r\n");
        xTimerStart(myTimer01Handle,100);
        xTimerStart(myTimer02Handle,100);
        printf("timer on\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0);
    }
    osDelay(100);
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
    {
      osDelay(20);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
      {
        printf("KEY2 DOWN\r\n");
        xTimerStop(myTimer01Handle,100);
        xTimerStop(myTimer02Handle,100);
        printf("timer off\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0);
    }
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Callback01 function */
void Callback01(void const * argument)
{
  /* USER CODE BEGIN Callback01 */
  printf("softtimer1\r\n");
  HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
  /* USER CODE END Callback01 */
}

/* Callback02 function */
void Callback02(void const * argument)
{
  /* USER CODE BEGIN Callback02 */
  printf("softtimer2\r\n");
  HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10);
  /* USER CODE END Callback02 */
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
  return key_val;
}
/* USER CODE END Application */
