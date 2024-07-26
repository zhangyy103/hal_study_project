/**
  @page Dual_FastInterleaved AN3116 Dual_FastInterleaved Readme file
  
  @verbatim
 ******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
 * @file     readme.txt
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     03/09/2010
 * @brief    Description of the AN3116 Application note's Dual_FastInterleaved.
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************
 @endverbatim
 
@par Description

This example describes how to use the ADC in Dual Fast Interleaved mode. 
ADC1 and ADC2 convert alternately the same ADC channel (channel11) with a period 
of 7 ADC clock cycles. The sampling rate of the conversion of channel11 is 2Msample/s.
The converted values are stored in ADC_DualConvertedValueTab[] array and separated in
ADCTab[] array. 

@par Directory contents 

  - Dual_FastInterleaved/inc/stm32f10x_conf.h  Library Configuration file
  - Dual_FastInterleaved/src/stm32f10x_it.c    Interrupt handlers
  - Dual_FastInterleaved/inc/stm32f10x_it.h    Interrupt handlers header file
  - Dual_FastInterleaved/src/main.c            Main program


@par Hardware and Software environment

  - This example runs on STM32 devices that have minimum two ADCs (ADC1 and ADC2).
  
  - This example has been tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity line), STM3210E-EVAL (STM32F10x High-Density) and STM3210B-EVAL
    (STM32F10x Medium-Density) evaluation boards and can be easily tailored to
    any other supported device and development board.

  - STM3210B-EVAL, STM3210E-EVAL and  STM3210C-EVAL Set-up  
    - Connect an external voltage (0-3.3v) to channel11 (GPIOC.01). The source can be a power
      supply or a GBF (it's preferable to use 100kHz frequency to well recover the signal).
      Note: the peak to peak amplitude of the signal should be in the range of 0v to 3.3v.
  
      
@par How to use it 

In order to load the Dual_FastInterleaved project, you have to do the
following:
  
 - EWARM (v5.40) 
    - Open the Dual_AlternateTrigger.eww workspace
    - In the workspace toolbar select the project config:
    - STM3210B-EVAL: to configure the project for STM32 Medium-Density devices
    - STM3210E-EVAL: to configure the project for STM32 High-Density devices
    - STM3210C-EVAL: to configure the project for STM32 Connectivity-line devices
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - RIDE (RIDE7)
    - Open the Dual_AlternateTrigger.rprj project
    - In the configuration toolbar(Project->properties) select the project config:
    - STM3210B-EVAL: to configure the project for STM32 Medium-Density devices
    - STM3210E-EVAL: to configure the project for STM32 High-Density devices
    - STM3210C-EVAL: to configure the project for STM32 Connectivity-line devices
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 - RVMDK (v4.00) 
    - Open the Dual_AlternateTrigger.Uv2 project
    - In the build toolbar select the project config:  
    - STM3210B-EVAL: to configure the project for STM32 Medium-Density devices
    - STM3210E-EVAL: to configure the project for STM32 High-Density devices
    - STM3210C-EVAL: to configure the project for STM32 Connectivity-line devices
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)

 - HiTOP (v5.31)
    - Open the HiTOP toolchain.
    - Browse to open the Dual_AlternateTrigger.htp
    - A "Download application" window is displayed, click "cancel".
    - STM3210B-EVAL: to configure the project for STM32 Medium-Density devices
    - STM3210E-EVAL: to configure the project for STM32 High-Density devices
    - STM3210C-EVAL: to configure the project for STM32 Connectivity-line devices
    - Rebuild all files: Project->Rebuild all
    - Load project image : Click "ok" in the "Download application" window.
    - Run the "RESET_GO_MAIN" script to set the PC at the "main"
    - Run program: Debug->Go(F5).

@note
 - Low-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 32 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.

 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 */