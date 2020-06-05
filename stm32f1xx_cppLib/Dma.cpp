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
volatile uint8_t dma1ch1HT_Update;  /* half transmission complete */
volatile uint8_t dma1ch1TC_Update;  /* full transmission complete */
volatile uint8_t dma1ch1TE_Update;  /* transmission error */

volatile uint8_t dma2ch3HT_Update;  /* half transmission complete */
volatile uint8_t dma2ch3TC_Update;  /* full transmission complete */
volatile uint8_t dma2ch3TE_Update;  /* transmission error */

volatile uint8_t dma2ch4HT_Update;  /* half transmission complete */
volatile uint8_t dma2ch4TC_Update;  /* full transmission complete */
volatile uint8_t dma2ch4TE_Update;  /* transmission error */

volatile uint8_t dma2ch5HT_Update;  /* half transmission complete */
volatile uint8_t dma2ch5TC_Update;  /* full transmission complete */
volatile uint8_t dma2ch5TE_Update;  /* transmission error */

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
void STM32F_DMA::begin( DMA_Channel_TypeDef* ch )
{
  dmaCh = ch;
}


/* ----------------------------------------
    from memory to peripheral
---------------------------------------- */
void STM32F_DMA::m2p(
  uint16_t *dst, const uint16_t *src, uint32_t count, uint32_t mode )
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
  DMA_InitStruct.DMA_Mode = mode;  /* normal mode(one shot mode). */
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;  /* peripheral is destination. */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)dst; /* destination memory address */
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)src;  /* source memory address */;
  DMA_InitStruct.DMA_BufferSize = count;  /* transmit size */
  DMA_Init( dmaCh, &DMA_InitStruct );

  DMA_ITConfig( dmaCh, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, ENABLE );   /* enable transmit end and error interrupt. */
  DMA_Cmd( dmaCh, ENABLE );
}


/* ----------------------------------------
    from peripheral to peripheral
---------------------------------------- */
void STM32F_DMA::p2p(
  uint16_t *dst, const uint16_t *src, uint32_t count, uint32_t mode )
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
  DMA_InitStruct.DMA_Mode = mode;  /* normal mode(one shot mode). */
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;  /* peripheral is destination. */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)dst; /* destination memory address */
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)src;  /* source memory address */;
  DMA_InitStruct.DMA_BufferSize = count;  /* transmit size */
  DMA_Init( dmaCh, &DMA_InitStruct );

  DMA_ITConfig( dmaCh, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, ENABLE );   /* enable transmit end and error interrupt. */
  DMA_Cmd( dmaCh, ENABLE );
}

/* ----------------------------------------
    from peripheral to memory
---------------------------------------- */
void STM32F_DMA::p2m(
  uint16_t *dst, const uint16_t *src, uint32_t count, uint32_t mode )
{
  DMA_InitTypeDef DMA_InitStruct;
  DMA_Cmd( dmaCh, DISABLE );
  DMA_DeInit( dmaCh );  /* Initialization of the DMA register */
  DMA_StructInit( &DMA_InitStruct );
//  DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;  /* Enables transfer between memory to memory. */
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  /* Disables transfer between memory to memory. */
  DMA_InitStruct.DMA_Priority = DMA_Priority_Low;  /* set low priority.  */
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  /* 16bit data size */
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  /* 16bit data size */
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  /* increment memory address. */
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* fixed peripheral address. */
  DMA_InitStruct.DMA_Mode = mode;  /* normal mode(one shot mode). */
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;  /* peripheral is source. */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)src; /* source memory address */
//  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; /* source memory address */
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)dst;  /* destination memory address */;
  DMA_InitStruct.DMA_BufferSize = count;  /* transmit size */
  DMA_Init( dmaCh, &DMA_InitStruct );

  DMA_ITConfig( dmaCh, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, ENABLE );   /* enable transmit end and error interrupt. */
  DMA_Cmd( dmaCh, ENABLE );
}

/* ----------------------------------------
    dma interrupt configuration.
---------------------------------------- */
void STM32F_DMA::ITConfig( uint32_t it, FunctionalState enableDisable )
{
//  DMA_ITConfig( dmaCh, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, ENABLE );   /* enable transmit end and error interrupt. */
  DMA_ITConfig( dmaCh, it, enableDisable );   /* enable transmit end and error interrupt. */
}

/* ----------------------------------------
    dma command.
---------------------------------------- */
void STM32F_DMA::command( FunctionalState enableDisable )
{
  //DMA_Cmd( dmaCh, ENABLE );
  DMA_Cmd( dmaCh, enableDisable );
}
