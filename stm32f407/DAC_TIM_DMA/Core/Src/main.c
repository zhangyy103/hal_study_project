/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ARRAY_SIZE 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
int fputc(int ch,FILE *f)
{
    HAL_UART_Transmit(&huart1,(uint8_t *) &ch,1,HAL_MAX_DELAY);
    return ch;    
}
int fgetc(FILE *f)
{
    uint8_t ch;
    HAL_UART_Receive(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);
    return ch;
}

const uint16_t SineWave12bit[ARRAY_SIZE]={
		0x0800,
		0x0CB3,
		0x0F9B, 
		0x0F9B,
		0x0CB3,
		0x0800,
		0x034D,
		0x0065,
		0x0065,
		0x034D,
};

//const uint16_t SquareWave12bit[ARRAY_SIZE] = {
//    0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF,
//    0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF,
//    0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF,
//    0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF,
//    0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF,
//    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//};

//const uint16_t TriangleWave12bit[100] = {
//    0x0000, 0x028F, 0x051E, 0x07AE, 0x0A3D, 0x0CCC, 0x0F5B, 0x11EB, 0x147A, 0x1709, 
//	  0x1999, 0x1C28, 0x1EB7, 0x2147, 0x23D6, 0x2665, 0x28F5, 0x2B84, 0x2E13, 0x30A3, 
//	  0x3332, 0x35C1, 0x3851, 0x3AE0, 0x3D6F, 0x3FFF, 0x3D6F, 0x3AE0, 0x3851, 0x35C1, 
//	  0x3332, 0x30A3, 0x2E13, 0x2B84, 0x28F5, 0x2665, 0x23D6, 0x2147, 0x1EB7, 0x1C28,
//	  0x1999, 0x1709, 0x147A, 0x11EB, 0x0F5B, 0x0CCC, 0x0A3D, 0x07AE, 0x051E, 0x028F, 
//	  0x0000, 0x028F, 0x051E, 0x07AE, 0x0A3D, 0x0CCC, 0x0F5B, 0x11EB, 0x147A, 0x1709, 
//	  0x1999, 0x1C28, 0x1EB7, 0x2147, 0x23D6, 0x2665, 0x28F5, 0x2B84, 0x2E13, 0x30A3, 
//	  0x3332, 0x35C1, 0x3851, 0x3AE0, 0x3D6F, 0x3FFF, 0x3D6F, 0x3AE0, 0x3851, 0x35C1,
//	  0x3332, 0x30A3, 0x2E13, 0x2B84, 0x28F5, 0x2665, 0x23D6, 0x2147, 0x1EB7, 0x1C28, 
//	  0x1999, 0x1709, 0x147A, 0x11EB, 0x0F5B, 0x0CCC, 0x0A3D, 0x07AE, 0x051E, 0x028F
//};

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void WaveTran(char rece){
//    switch (rece) {
//        case 'S':
//            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)SineWave12bit, 100, DAC_ALIGN_12B_R);
//            break;
//        case 'T':
//            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)TriangleWave12bit, 100, DAC_ALIGN_12B_R);
//            break;
//        case 'M':
//            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)SquareWave12bit, 100, DAC_ALIGN_12B_R);
//            break;
//        default:
//            // 可�?�：处理未知的rece�?
//            break;
//    }
//}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	char rece_data;
	char pre_rece_data;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_TIM_Base_Start(&htim6);
  printf("Please enter: M-Square Wave T-Triangle Wave S-Sine Wave \r\n");
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)SineWave12bit, 10, DAC_ALIGN_12B_R);
  while (1)
  {
//		scanf("%c", &rece_data);
//		if(pre_rece_data != rece_data)HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
