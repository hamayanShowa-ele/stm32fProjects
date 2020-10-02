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

STM32F_DMA::STM32F_DMA( DMA_Channel_TypeDef* ch )
{
  begin( ch );
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
}

void STM32F_DMA::m2p(
  uint32_t *dst, const uint32_t *src, uint32_t count, uint32_t mode )
{
  DMA_InitTypeDef DMA_InitStructure;
  DMA_Cmd( dmaCh, DISABLE );
  DMA_DeInit( dmaCh );
  DMA_StructInit( &DMA_InitStructure );

//  DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;  /* Enables transfer between memory to memory. */
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  /* Disables transfer between memory to memory. */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;  /* set low priority.  */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Mode = mode;  /* normal mode(one shot mode). */
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst; /* destination memory address */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src;  /* source memory address */;
  DMA_InitStructure.DMA_BufferSize = count;  /* transmit size */
  DMA_Init( dmaCh, &DMA_InitStructure );

  /* Enable DMAx Channelx */
//  DMA_Cmd( DMAx_Channelx, ENABLE );
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


/* ----------------------------------------
    dma interrupt.
---------------------------------------- */
extern "C"
{

/* ----------------------------------------
    DMA1 channel 1 interrupt handler.
    ADC1,TIM2_CH3,TIM4_CH1
---------------------------------------- */
void DMA1_Channel1_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA1_IT_GL1 ) == SET )
  {
    if( DMA_GetITStatus( DMA1_IT_HT1 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA1_IT_HT1 );
      dma1ch1HT_Update++;
    }
    if( DMA_GetITStatus( DMA1_IT_TC1 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA1_IT_TC1 );
      dma1ch1TC_Update++;
    }
    if( DMA_GetITStatus( DMA1_IT_TE1 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA1_IT_TE1 );
      dma1ch1TE_Update++;
    }
    DMA_ClearITPendingBit( DMA1_IT_GL1 );
  }
}

/* ----------------------------------------
    DMA1 channel 2 interrupt handler.
    SPI1_RX,USART3_TX,TIM1_CH1,TIM2_UP,TIM3_CH3
---------------------------------------- */

/* ----------------------------------------
    DMA1 channel 3 interrupt handler.
    SPI1_TX,USART3_RX,TIM1_CH2,TIM3_UP,TIM3_CH4
---------------------------------------- */

/* ----------------------------------------
    DMA1 channel 4 interrupt handler.
    SPI/I2S2_RX,USART1_TX,I2C2_TX,TIM1_CH4,TIM1_TRG,TIM1_COM,TIM4_CH2
---------------------------------------- */

/* ----------------------------------------
    DMA1 channel 5 interrupt handler.
    SPI/I2S2_TX,USART1_RX,I2C2_RX,TIM1_UP,TIM2_CH1,TIM4_CH3
---------------------------------------- */

/* ----------------------------------------
    DMA1 channel 6 interrupt handler.
    USART2_RX,I2C1_TX,TIM1_CH3,TIM3_CH1,TIM3_TRG
---------------------------------------- */

/* ----------------------------------------
    DMA1 channel 7 interrupt handler.
    USART2_TX,I2C1_RX,TIM2_CH2,TIM2_CH4,TIM4_UP
---------------------------------------- */

/* ----------------------------------------
    DMA2 channel 1 interrupt handler.
    SPI/I2S3_RX,TIM5_CH4,TIM5_TRIG,TIM8_CH3,TIM8_UP
---------------------------------------- */

/* ----------------------------------------
    DMA2 channel 2 interrupt handler.
    SPI/I2S3_TX,TIM5_CH3,TIM5_UP,TIM8_CH4,TIM8_TRIG,TIM8_COM
---------------------------------------- */

/* ----------------------------------------
    DMA2 channel 3 interrupt handler.
    UART4_RX,TIM6_UP,DAC1,TIM8_CH1
---------------------------------------- */
void DMA2_Channel3_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA2_IT_GL3 ) == SET )
  {
    if( DMA_GetITStatus( DMA2_IT_HT3 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_HT3 );
      dma2ch3HT_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TC3 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TC3 );
      dma2ch3TC_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TE3 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TE3 );
      dma2ch3TE_Update++;
    }
    DMA_ClearITPendingBit( DMA2_IT_GL3 );
  }
}


/* ----------------------------------------
    DMA2 channel 4 and channel 5 interrupt handler.
    SDIO,TIM5_CH2,TIM7_UP,DAC2

    DMA2 channel 5 and channel 5 interrupt handler.
    ADC3,UART4_TX,TIM5_CH1,TIM8_CH2
---------------------------------------- */
void DMA2_Channel4_5_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA2_IT_GL4 ) == SET )
  {
    if( DMA_GetITStatus( DMA2_IT_HT4 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_HT4 );
      dma2ch4HT_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TC4 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TC4 );
      dma2ch4TC_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TE4 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TE4 );
      dma2ch4TE_Update++;
    }
    DMA_ClearITPendingBit( DMA2_IT_GL4 );
  }

  if( DMA_GetITStatus( DMA2_IT_GL5 ) == SET )
  {
    if( DMA_GetITStatus( DMA2_IT_HT5 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_HT5 );
      dma2ch5HT_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TC5 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TC5 );
      dma2ch5TC_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TE5 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TE5 );
      dma2ch5TE_Update++;
    }
    DMA_ClearITPendingBit( DMA2_IT_GL4 );
  }
}

}  /* extern "C" */
