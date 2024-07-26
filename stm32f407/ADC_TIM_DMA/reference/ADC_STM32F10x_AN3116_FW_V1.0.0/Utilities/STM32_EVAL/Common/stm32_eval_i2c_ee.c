/**
  ******************************************************************************
  * @file    stm32_eval_i2c_ee.c
  * @author  MCD Application Team
  * @version V4.1.0
  * @date    03/01/2010
  * @brief   This file provides a set of functions needed to manage the I2C M24CXX 
  *          EEPROM memory mounted on STM32xx-EVAL board (refer to stm32_eval.h
  *          to know about the boards supporting this memory). 
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32 hardware resources (I2C and 
  *          GPIO) are defined in stm32xx_eval.h file, and the initialization is 
  *          performed in sEE_LowLevel_Init() function declared in stm32xx_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          sEE_LowLevel_Init() function. 
  *            
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |                 
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32 I2C Pins                       |   sEE     |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | .                                     |   E0(GND) |    1  (0V)  |
  *     | .                                     |   E1(GND) |    2  (0V)  |
  *     | .                                     |   E2(GND) |    3  (0V)  |
  *     | .                                     |   E0(VSS) |    4  (0V)  |
  *     | sEE_I2C_SDA_PIN/ SDA                  |   SDA     |    5        |
  *     | sEE_I2C_SCL_PIN/ SCL                  |   SCL     |    6        |
  *     | .                                     |   /WC(VDD)|    7 (3.3V) |
  *     | .                                     |   VDD     |    8 (3.3V) |
  *     +---------------------------------------+-----------+-------------+  
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
#include "stm32_eval_i2c_ee.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */
  
/** @addtogroup STM32_EVAL_I2C_EE
  * @brief      This file includes the I2C EEPROM driver of STM32-EVAL boards.
  * @{
  */ 

/** @defgroup STM32_EVAL_I2C_EE_Private_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_I2C_EE_Private_Defines
  * @{
  */  
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_I2C_EE_Private_Macros
  * @{
  */
/**
  * @}
  */ 
  

/** @defgroup STM32_EVAL_I2C_EE_Private_Variables
  * @{
  */
__IO uint16_t sEEAddress = 0;   
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_I2C_EE_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_I2C_EE_Private_Functions
  * @{
  */ 

/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit(); 
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_Init(void)
{ 
  I2C_InitTypeDef  I2C_InitStructure;
  
  sEE_LowLevel_Init();
  
  /*!< I2C configuration */
  /* sEE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  /* sEE_I2C Peripheral Enable */
  I2C_Cmd(sEE_I2C, ENABLE);
  /* Apply sEE_I2C configuration after enabling it */
  I2C_Init(sEE_I2C, &I2C_InitStructure);

#if defined (sEE_M24C64_32)
  /*!< Select the EEPROM address according to the state of E0, E1, E2 pins */
  sEEAddress = sEE_HW_ADDRESS;  
#elif defined (sEE_M24C08)
  /*!< depending on the sEE Address selected in the i2c_ee.h file */
 #ifdef sEE_Block0_ADDRESS
  /*!< Select the sEE Block0 to write on */
  sEEAddress = sEE_Block0_ADDRESS;
 #endif
  
 #ifdef sEE_Block1_ADDRESS
  /*!< Select the sEE Block1 to write on */
  sEEAddress = sEE_Block1_ADDRESS;
 #endif

 #ifdef sEE_Block2_ADDRESS
  /*!< Select the sEE Block2 to write on */
  sEEAddress = sEE_Block2_ADDRESS;
 #endif
  
 #ifdef sEE_Block3_ADDRESS
  /*!< Select the sEE Block3 to write on */
  sEEAddress = sEE_Block3_ADDRESS;
 #endif 
#endif /*!< sEE_M24C64_32 */    
}

/**
  * @brief  Writes one byte to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @retval None
  */
void sEE_WriteByte(uint8_t* pBuffer, uint16_t WriteAddr)
{
  /*!< Send STRAT condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
  }  

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
  
  /*!< Test on EV6 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
  }

#ifdef sEE_M24C08
  
  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(sEE_I2C, WriteAddr);
  
#elif defined(sEE_M24C64_32)
  
  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }
  
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< sEE_M24C08 */  
  
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }

  /*!< Send the byte to be written */
  I2C_SendData(sEE_I2C, *pBuffer); 
   
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }
  
  /*!< Send STOP condition */
  I2C_GenerateSTOP(sEE_I2C, ENABLE);
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EEPROM.
  * @retval None
  */
void sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{  
  /*!< While the bus is busy */
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
  }
  
  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
  }
   
  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
  }

#ifdef sEE_M24C08  
  
  /*!< Send the EEPROM's internal address to read from: Only one byte address */
  I2C_SendData(sEE_I2C, ReadAddr);  

#elif defined (sEE_M24C64_32)

  /*!< Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));    
  
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }

  /*!< Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));    
  
#endif /*!< sEE_M24C08 */
  
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }
  
  /*!< Send STRAT condition a second time */  
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
  }
  
  /*!< Send EEPROM address for read */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Receiver);
  
  /*!< Test on EV6 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
  }
  
  /*!< While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /*!< Disable Acknowledgement */
      I2C_AcknowledgeConfig(sEE_I2C, DISABLE);
      
      /*!< Send STOP Condition */
      I2C_GenerateSTOP(sEE_I2C, ENABLE);
    }

    /*!< Test on EV7 and clear it */
    if(I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /*!< Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(sEE_I2C);

      /*!< Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /*!< Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /*!< Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(sEE_I2C, ENABLE);
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
 
  /*!< If WriteAddr is sEE_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage == 0) 
    {
      sEE_WritePage(pBuffer, WriteAddr, NumOfSingle);
      sEE_WaitEepromStandbyState();
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        sEE_WritePage(pBuffer, WriteAddr, sEE_PAGESIZE); 
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        sEE_WritePage(pBuffer, WriteAddr, NumOfSingle);
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
  else 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /*!< Write the data conained in same page */
        sEE_WritePage(pBuffer, WriteAddr, count);
        sEE_WaitEepromStandbyState();      
        
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (NumByteToWrite - count));
        sEE_WaitEepromStandbyState();        
      }      
      else      
      {
        sEE_WritePage(pBuffer, WriteAddr, NumOfSingle);
        sEE_WaitEepromStandbyState();        
      }     
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
      
      if(count != 0)
      {  
        sEE_WritePage(pBuffer, WriteAddr, count);
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        sEE_WritePage(pBuffer, WriteAddr, sEE_PAGESIZE);
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        sEE_WritePage(pBuffer, WriteAddr, NumOfSingle); 
        sEE_WaitEepromStandbyState();
      }
    }
  }  
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{ 
  /*!< While the bus is busy */
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
  }
  
  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
  }
  
  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
  } 

#ifdef sEE_M24C08
  
  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(sEE_I2C, WriteAddr);
  
#elif defined(sEE_M24C64_32)
  
  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }
  
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< sEE_M24C08 */  

  /*!< Test on EV8 and clear it */
  while(! I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }

  /*!< While there is data to be written */
  while(NumByteToWrite--)  
  {
    /*!< Send the current byte */
    I2C_SendData(sEE_I2C, *pBuffer); 

    /*!< Point to the next byte to be written */
    pBuffer++; 
  
    /*!< Test on EV8 and clear it */
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    }
  }

  /*!< Send STOP condition */
  I2C_GenerateSTOP(sEE_I2C, ENABLE);
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
void sEE_WaitEepromStandbyState(void)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    /*!< Send START condition */
    I2C_GenerateSTART(sEE_I2C, ENABLE);

    /*!< Read sEE SR1 register to clear pending flags */
    SR1_Tmp = I2C_ReadRegister(sEE_I2C, I2C_Register_SR1);

    /*!< Send EEPROM address for write */
    I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(sEE_I2C, I2C_Register_SR1) & 0x0002));
  
  /*!< Clear AF flag */
  I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);
  
  /*!< STOP condition */    
  I2C_GenerateSTOP(sEE_I2C, ENABLE);  
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
