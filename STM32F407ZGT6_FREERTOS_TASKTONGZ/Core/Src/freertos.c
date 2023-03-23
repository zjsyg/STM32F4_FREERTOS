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
uint16_t num = 0;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityNormal, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
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
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0) //模拟二值信号量
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
      {
        printf("key1 down\r\n");
        if(xTaskNotifyGive(myTask02Handle) == pdTRUE)
          printf("give task semb\r\n");
        else
          printf("give task semb fail\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0);
    }

  /*if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0) //模拟计数信号量
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
      {
        if(num < 20)
        {
          xTaskNotifyGive(myTask02Handle);
          num++;
          printf("num:%d\r\n",num);
        }
        else
          printf("num over\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0);
    }*/

  /*if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0) //模拟事件组
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
      {
        xTaskNotify(myTask03Handle,0x0001,eSetBits);
        printf("key1 down\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0);
    }*/

  /*if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0) //模拟队列
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)
      {
        printf("key1 down\r\n");
        if(xTaskNotify(myTask02Handle,(uint32_t)"zjs",eSetValueWithoutOverwrite) == pdTRUE)
          printf("give queue\r\n");
        else
          printf("queue over\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0);
    }*/
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  // uint32_t val;
  /* Infinite loop */
  for(;;)
  {
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0) //模拟二值信号量
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
      {
        printf("key2 down\r\n");
        if(ulTaskNotifyTake(pdTRUE,0) > 0)
          printf("take task semb\r\n");
        else
          printf("take task semb fail\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0);
    }

  /*if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0) //模拟计数信号量
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
      {
        if(ulTaskNotifyTake(pdFAIL,0) > 0)
        {
          num--;
          printf("num:%d\r\n",num);
        }
        else
          printf("num low\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0);
    }*/

  /*if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0) //模拟事件组
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
      {
        xTaskNotify(myTask03Handle,0x0010,eSetBits);
        printf("key2 down\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0);
    }*/

  /*if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0) //模拟队列
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0)
      {
        printf("key2 down\r\n");
        if(xTaskNotifyWait(0x00000000,0xffffffff,&val,0) == pdTRUE)
          printf("get val:%s\r\n",(uint8_t *)val);
        else
          printf("get fail\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5) == 0);
    }*/
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  // uint32_t val;
  /* Infinite loop */
  for(;;)
  {
  /*if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == 0) //模拟事件组
    {
      osDelay(10);
      if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == 0)
      {
        xTaskNotifyWait(0x00000000,0xffffffff,&val,0);
        printf("key3 down\r\n");
        if(val == 0x0011)
          printf("biu\r\n");
      }
      while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6) == 0);
    }*/
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
int fputc(int ch,FILE *f)
{
  HAL_UART_Transmit(&huart1,(uint8_t *)(&ch),sizeof(ch),HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END Application */
