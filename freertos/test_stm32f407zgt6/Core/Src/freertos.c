/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "queue.h"

#include "my_func1.h"
#include "my_func2.h"
#include "my_default_func.h"
#include "my_applyfunc_of_func12.h"
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
static StackType_t g_pucStackOfTask2 [128]; // Stack: uint32_t
static StaticTask_t g_TCBOfTask2;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
	.name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartTask1(void *arg);
void StartTask2(void *arg);
void StartApplyTask(void *arg);

QueueHandle_t xQueue;

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	TaskHandle_t xTask1Handle;
	TaskHandle_t xTask2Handle;
	TaskHandle_t xApplyTaskHandle;
	
	BaseType_t ret;
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
	xQueue = xQueueCreate(10, sizeof(int));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	// creat my task1 threads with the dynamically manage memory
	ret = xTaskCreate(StartTask1, "Task1", 128, NULL, osPriorityNormal, &xTask1Handle);
	if (ret == pdPASS){;}
	else {;}
	
	// creat my task1 threads with the statically managed memory
	xTask2Handle = xTaskCreateStatic(StartTask2, "Task2", 128, NULL, osPriorityNormal, g_pucStackOfTask2, &g_TCBOfTask2);
	if (xTask2Handle != NULL){;}
	else {;}
		
	// creat my applytask threads with the dynamically manage memory
	ret = xTaskCreate(StartApplyTask, "ApplyTask", 128, NULL, osPriorityNormal, &xApplyTaskHandle);
	if (ret == pdPASS){;}
	else {;}
	
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
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	my_default_func_init();
  /* Infinite loop */
  for(;;)
  {
		my_default_func();
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartTask1(void *arg){
	my_func1_init();
#if 0	
	// get the handle of task
	TaskHandle_t xTaskHandle = xTaskGetCurrentTaskHandle();
	// get the high-water mark of the task stack
	UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(xTaskHandle);
	// print the high-water mark of the task stack
	printf("Task stack high water mark: %u\n", uxHighWaterMark);
  // handle stack usage as needed
	if (uxHighWaterMark < SOME_THRESHOLD) {

					printf("Warning: Task stack usage is high!\n");
			}
#endif	
	xQueueSend(xQueue, "task1", portMAX_DELAY);
	for(;;){
		my_func1();
	}
}

void StartTask2(void *arg){
	my_func2_init();
	
	xQueueSend(xQueue, "task2", portMAX_DELAY);
	for(;;){
		my_func2();
	}
}

void StartApplyTask(void *arg){
	my_applyfunc_of_func12_init();
	
	void* receivedData;
	
	if (xQueueReceive(xQueue, receivedData, portMAX_DELAY) == pdTRUE) {
			//printf("Received data: %d\n", receivedData);
	}
	
	
	for(;;){
		// apply of your argument
		my_applyfunc_of_func12(arg);
		
		// avoid conflicting access to global variables and peripheral
		vTaskDelay(100);
	}
}

#if 0
// if you want enable the detect of the out of stack resive the macro in FreeRTOSConfig.h else will lead the warnning of redefintion
// #define configCHECK_FOR_STACK_OVERFLOW 1  // enable the detect of the out of stack

// This is a hook func which will as seem as callback function 
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // deal the out of stack
    //printf("Stack overflow in task: %s\n", pcTaskName);
    for (;;);  // ????
}
#endif
/* USER CODE END Application */

