/**
  ******************************************************************************
  * @file    Dual_Combined_Reg_Inj_Simultaneous/src/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    03/09/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "stm32f10x_it.h"

/** @addtogroup Dual_Combined_Reg_Inj_Simultaneous
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BufferLenght       128

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t   ADC_DualConvertedValueTab[BufferLenght];
extern __IO uint16_t   ADC1_InjectedConvertedValueTab[BufferLenght];
extern __IO uint16_t   ADC2_InjectedConvertedValueTab[BufferLenght];
__IO uint32_t Index = 0, Index1=0, Index2 = 0;
__IO uint8_t  RegularValuesReadyTobeTreated = 0; 
__IO uint8_t  InjectedValuesReadyTobeTreated = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval None
  */
void ADC1_2_IRQHandler(void)
{
  if (ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC))
  {
    /* Get injected channel11 converted value (potentiometer) */
    ADC1_InjectedConvertedValueTab[Index1++] = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1); 

    /* Get injected channel8 (external signal) */
    ADC2_InjectedConvertedValueTab[Index2++] = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1);

    if (Index2 == BufferLenght)
    {
     /* Initialize Index1 variable */
     Index1 = 0;
    
     /* Initialize Index2 variable */
     Index2 = 0;
     
     /* Set the software flag for injected conversion */
     InjectedValuesReadyTobeTreated = 1;
     
     /* TIM1 counter Disable */
     TIM_Cmd(TIM1, DISABLE);
    }
    
    ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);
  }
  else if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
  {
    /* Get regular values converted by ADC1 and ADC2 in dual mode */
    ADC_DualConvertedValueTab[Index++] = ADC_GetDualModeConversionValue();
    
    if (Index == BufferLenght)
    {
     /* Initialize Index variable */
     Index = 0;
     
     /* Set the software flag for regular conversion (using interrupt method) */
     RegularValuesReadyTobeTreated = 1;
    }
    
   ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s)                 */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles Dual_Combined_Reg_Inj_Simultaneous_src interrupt request.
  * @param  None
  * @retval None
  */
/*void Dual_Combined_Reg_Inj_Simultaneous_src_IRQHandler(void)
{
}*/


/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
