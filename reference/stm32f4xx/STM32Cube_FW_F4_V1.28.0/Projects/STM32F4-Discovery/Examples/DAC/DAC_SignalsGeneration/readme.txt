/**
  @page DAC_SignalsGeneration DAC Signals generation example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DAC/DAC_SignalsGeneration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DAC Signals generation example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
   @endverbatim

@par Example Description 

This example provides a description of how to use the DAC peripheral to 
generate several signals using DMA controller.

When the user presses the KEY push-button, DMA transfers the two selected 
waveforms to the DAC.
For each press on KEY button, a signal has been selected and can be monitored on  
the DAC channel one:
    - Triangle waveform (Channel 1).
    - Escalator waveform (Channel 1).

STM32 Discovery boards LEDs can be used to monitor the transfer status:
  - LED5 is ON when there are an error in initialization.
    
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, DAC, Signals generation, DMA, Triangle, Escalator, Waveform, Amplitude


@par Directory contents 

  - DAC/DAC_SignalsGeneration/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DAC/DAC_SignalsGeneration/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DAC/DAC_SignalsGeneration/Inc/main.h                  Main program header file  
  - DAC/DAC_SignalsGeneration/Src/stm32f4xx_it.c          Interrupt handlers
  - DAC/DAC_SignalsGeneration/Src/main.c                  Main program
  - DAC/DAC_SignalsGeneration/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - DAC/DAC_SignalsGeneration/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  
  
@par Hardware and Software environment
  
  - This example runs on STM32F407xx devices.
      
  - This example has been tested with STMicroelectronics STM32F4-Discovery RevB & RevC boards and can be 
    easily tailored to any other supported device and development board. 	

  - STM32F4-Discovery Set-up 	
     - Use KEY push-button connected to PA0.
     - Connect PA4 (DAC Channel1) pin to an oscilloscope.
     

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
