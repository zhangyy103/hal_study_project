/**
  ******************************************************************************
  * @file    Dual_Combined_Reg_Inj_Simultaneous/src/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    03/09/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/** @addtogroup Dual_Combined_Reg_Inj_Simultaneous
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USE_DMA_Transfer
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define BufferLenght       128

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef           ADC_InitStructure;
DMA_InitTypeDef           DMA_InitStructure;
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
TIM_OCInitTypeDef         TIM_OCInitStructure;
__IO uint32_t   ADC_DualConvertedValueTab[BufferLenght];
__IO uint16_t   ADC1_InjectedConvertedValueTab[BufferLenght];
__IO uint16_t   ADC2_InjectedConvertedValueTab[BufferLenght];
__IO uint16_t   ADC1_RegularConvertedValueTab[BufferLenght];
__IO uint16_t   ADC2_RegularConvertedValueTab[BufferLenght];
ErrorStatus HSEStartUpStatus;
extern __IO uint8_t  RegularValuesReadyTobeTreated; 
extern __IO uint8_t  InjectedValuesReadyTobeTreated; 
__IO uint32_t i;
uint32_t j=0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
  
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();

  /* TIM1 configuration ------------------------------------------------------*/ 
  /* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xFF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x2;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  /* TIM1 channel1 configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = 0x7F; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);

#ifdef  USE_DMA_Transfer  
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = BufferLenght;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
#endif
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegInjecSimult;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
   
  /* ADC1 Regular Channel Config */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_28Cycles5); 
  
  /* Set Injected sequencer length on ADC1 */
  ADC_InjectedSequencerLengthConfig(ADC1, 1);
  
  /* ADC1 Injected Channel Configuration */ 
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_7Cycles5);
  
  /* ADC1 injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);
  
  /* Enable injected external trigger conversion on ADC1*/
  ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

  /* ADC1 DMA enable */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* ADC2 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegInjecSimult; 
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC2, &ADC_InitStructure);
  
  /* Enable ADC2 external trigger conversion */
  ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  
   /* ADC1 Regular Channel Config */ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 1, ADC_SampleTime_28Cycles5);
  
  /* Set Injected sequencer length on ADC2 */
  ADC_InjectedSequencerLengthConfig(ADC2, 1);

  /*  ADC2 Injected Channel Configuration */ 
  ADC_InjectedChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_7Cycles5);

  /* ADC2 injected external trigger config */
  ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_T1_CC4);
  
  /* Enable injected external trigger conversion on ADC2 */
  ADC_ExternalTrigInjectedConvCmd(ADC2, ENABLE);

  /* Enable ADC1 JEOC interrupt */
  ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
  
#ifndef  USE_DMA_Transfer    
  /* Enable ADC1 JEOC interrupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
#endif
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Enable ADC2 reset calibaration register */   
  ADC_ResetCalibration(ADC2);
  
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));

  /* Start ADC2 calibaration */
  ADC_StartCalibration(ADC2);
  
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));

  /* ADC1 SoftwareStartConvCmd Test */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
  
  /* TIM1 main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  

#ifdef  USE_DMA_Transfer 
  /* Test on Channel 1 DMA1_FLAG_TC flag */
  while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
  
  /* Clear Channel 1 DMA1_FLAG_TC flag */
  DMA_ClearFlag(DMA1_FLAG_TC1);	
#else
  
  /* Wait for regular channels buffer to be full filled */
  while(RegularValuesReadyTobeTreated == 0);

#endif

  /* Wait for injected channels buffer to be full filled */
  while(InjectedValuesReadyTobeTreated == 0);
  
  /* Extract ADC1 regular values (CH12 conversion) */
  for(i=0;i<BufferLenght;i++)
  {
    ADC1_RegularConvertedValueTab[j++] = (uint16_t)(ADC_DualConvertedValueTab[i]);
  }
  
  /* Initialize j */
  j = 0;
 
  /* Extract ADC2 regular values (CH14: potentiometer conversion)*/
  for(i=0;i<BufferLenght;i++)
  {
    ADC2_RegularConvertedValueTab[j++] = (uint16_t)(ADC_DualConvertedValueTab[i] >> 16);
  }

  while (1)
  {
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
	
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  
#ifndef STM32F10X_CL  
    /* PLLCLK = 8MHz * 7 = 56 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);

#else
    /* Configure PLLs *********************************************************/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 7 = 56 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_7);
#endif

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  
/* Enable peripheral clocks --------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1, ADC2, GPIOA, GPIOB, GPIOC, and TIM1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
                         RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | 
                         RCC_APB2Periph_TIM1, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure TIM1_CH4 (PA11) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PC.01, PC.02 and PC.04 (ADC Channel11, Channel12 and Channel14) as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Configure PB.00  (ADC Channel8) as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

/**
  * @brief  Configures NVIC and Vector Table base location.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable ADC1_2 IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
