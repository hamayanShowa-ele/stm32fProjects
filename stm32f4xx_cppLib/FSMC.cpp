/* ----------------------------------------
 FSMC utilities.
  for STMicroelectronics SPL library

  Copyright (c) 2020 hamayan (hamayan@showa-ele.jp).
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Created 2020 by hamayan (hamayan@showa-ele.jp)
---------------------------------------- */

#include  "FSMC.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
---------------------------------------- */

/* ----------------------------------------
  class FSMC sram mode set.
---------------------------------------- */
void FSMC::sram(
  uint32_t bank,  // bank
  uint32_t width,  // bus width
  uint32_t rAS,  // read address setup time max=15
  uint32_t rDS,  // read data setup time max=255
  uint32_t wAS,  // write address setup time max=15
  uint32_t wDS,  // write data setup time max=255
  bool wait  // true : insert a wait. false : not insert a wait.
)
{
  /*-- FSMC Configuration ------------------------------------------------------*/
#if 0
  //  FSMC_NORSRAMDeInit( bank );
  FSMC_NORSRAMTimingInitTypeDef  rTiming,wTiming;
  /* This will cause a Hard Fault Error. Is this a bug? */
//  FSMC_NORSRAMStructInit( &FSMC_NORSRAMInitStructure );

  /* Enable sram area in sram bank1 */
  /* Configuring the NE1 SRAM 0x60000000 */
  rTiming.FSMC_AddressSetupTime = rAS;  // Define the number of HCLK cycles to configure the setup time of the address.
  rTiming.FSMC_AddressHoldTime = rAH;  // Define the number of HCLK cycles to configure the address's hold time.
  rTiming.FSMC_DataSetupTime = rDS;  // Define the number of HCLK cycles to configure the data setup time.
  rTiming.FSMC_BusTurnAroundDuration = rTA;  // This parameter is only used for multiplexed NOR Flash memory.
  rTiming.FSMC_CLKDivision = 15;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_DataLatency = 15;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  wTiming.FSMC_AddressSetupTime = wAS;  // Define the number of HCLK cycles to configure the setup time of the address.
  wTiming.FSMC_AddressHoldTime = wAH;  // Define the number of HCLK cycles to configure the address's hold time.
  wTiming.FSMC_DataSetupTime = wDS;  // Define the number of HCLK cycles to configure the data setup time.
  wTiming.FSMC_BusTurnAroundDuration = wTA;  // 1 This parameter is only used for multiplexed NOR Flash memory.
  wTiming.FSMC_CLKDivision = 15;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_DataLatency = 15;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMInitStructure.FSMC_Bank = bank;  // FSMC_Bank1_NORSRAM1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = width;  // FSMC_MemoryDataWidth_16b
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;

//  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //
//  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  //
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_DuringWaitState;  //
//  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Enable;

  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &rTiming;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &wTiming;
  FSMC_NORSRAMInit( &FSMC_NORSRAMInitStructure );
  /*!< Enable FSMC Bank1_SRAM1 Bank */
  FSMC_NORSRAMCmd( bank, ENABLE );  // FSMC_Bank1_NORSRAM1
#else
  /* FSMC clock enable */
  RCC_AHB3PeriphResetCmd( RCC_AHB3Periph_FSMC, DISABLE );
  RCC_AHB3PeriphClockCmd( RCC_AHB3Periph_FSMC, ENABLE );

  FSMC_NORSRAMDeInit( bank );

  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStruct;
  FSMC_NORSRAMTimingInitTypeDef rTiming,wTiming;
  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &rTiming;
  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct = &wTiming;
  FSMC_NORSRAMStructInit( &FSMC_NORSRAMInitStruct );

  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime = rAS;  // Define the number of HCLK cycles to configure the setup time of the address.
//  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime = 0xF;
  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime = rDS;  // Define the number of HCLK cycles to configure the data setup time.
//  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration = 0xF;
//  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct->FSMC_CLKDivision = 0xF;
//  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct->FSMC_DataLatency = 0xF;
//  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct->FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct->FSMC_AddressSetupTime = wAS;  // Define the number of HCLK cycles to configure the setup time of the address.
//  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct->FSMC_AddressHoldTime = 0xF;
  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct->FSMC_DataSetupTime = wDS;  // Define the number of HCLK cycles to configure the data setup time.
//  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct->FSMC_BusTurnAroundDuration = 0xF;
//  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct->FSMC_CLKDivision = 0xF;
//  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct->FSMC_DataLatency = 0xF;
//  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct->FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStruct.FSMC_Bank = bank;  // FSMC_Bank1_NORSRAM1
  FSMC_NORSRAMInitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
//  FSMC_NORSRAMInitStruct.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth = width;  // FSMC_MemoryDataWidth_16b
//  FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
//  FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
//  FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
//  FSMC_NORSRAMInitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable;
//  FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
//  FSMC_NORSRAMInitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  if( wait ) FSMC_NORSRAMInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Enable;
  else FSMC_NORSRAMInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
//  FSMC_NORSRAMInitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
//  FSMC_NORSRAMInitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

  FSMC_NORSRAMInit( &FSMC_NORSRAMInitStruct );
  /*!< Enable FSMC Bank1_SRAM1 Bank */
  FSMC_NORSRAMCmd( bank, ENABLE );  // FSMC_Bank1_NORSRAM1
#endif
}

extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
