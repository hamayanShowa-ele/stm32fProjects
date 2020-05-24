/* ----------------------------------------
  stm32f1xx dma utilities code
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
#include  <Dma.h>

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
volatile uint8_t dma2ch4TC_Update;

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
STM32F_DMA::STM32F_DMA()
{
}

STM32F_DMA::~STM32F_DMA()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */


/* ----------------------------------------
    from memory to peripheral
---------------------------------------- */
void STM32F_DMA::m2p(
  DMA_Channel_TypeDef* dmaCh,
  uint16_t *dst, const uint16_t *src, uint32_t count )
{
  DMA_InitTypeDef DMA_InitStruct;

  DMA_Cmd( dmaCh, DISABLE );
  DMA_DeInit( dmaCh );  /* Initialization of the DMA register */
  DMA_StructInit( &DMA_InitStruct );
  DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;  /* Enables transfer between memory to memory. */
//  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  /* Disables transfer between memory to memory. */
  DMA_InitStruct.DMA_Priority = DMA_Priority_Low;  /* set low priority.  */
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  /* 16bit data size */
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  /* 16bit data size */
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  /* increment memory address. */
//  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;  /* not increment memory address. */
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* not increment peripheral address */
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;  /* normal mode(one shot mode). */
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;  /* peripheral is destination. */

  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)dst; /* destination memory address */
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)src;  /* source memory address */;
  DMA_InitStruct.DMA_BufferSize = count;  /* transmit size */

  DMA_Init( dmaCh, &DMA_InitStruct );
  DMA_ITConfig( dmaCh, DMA_IT_TC | DMA_IT_TE, ENABLE );   /* enable transmit end and error interrupt. */
  DMA_Cmd( dmaCh, ENABLE );
}


void STM32F_DMA::p2p(
  DMA_Channel_TypeDef* dmaCh,
  uint16_t *dst, const uint16_t *src, uint32_t count )
{
  DMA_InitTypeDef DMA_InitStruct;

  DMA_Cmd( dmaCh, DISABLE );
  DMA_DeInit( dmaCh );  /* Initialization of the DMA register */
  DMA_StructInit( &DMA_InitStruct );
  DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;  /* Enables transfer between memory to memory. */
//  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  /* Disables transfer between memory to memory. */
  DMA_InitStruct.DMA_Priority = DMA_Priority_Low;  /* set low priority. */
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  /* 16bit data size */
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  /* 16bit data size */
//  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  /* increment memory address. */
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;  /* not increment memory address. */
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* not increment peripheral address */
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;  /* normal mode(one shot mode). */
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;  /* peripheral is destination. */

  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)dst; /* destination memory address */
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)src;  /* source memory address */;
  DMA_InitStruct.DMA_BufferSize = count;  /* transmit size */

  DMA_Init( dmaCh, &DMA_InitStruct );
  DMA_ITConfig( dmaCh, DMA_IT_TC | DMA_IT_TE, ENABLE );   /* enable transmit end and error interrupt. */
  DMA_Cmd( dmaCh, ENABLE );
}
