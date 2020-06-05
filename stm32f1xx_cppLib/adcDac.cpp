/* ----------------------------------------
 stm32f1xx ADC and DAC utility code
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
#include  "adcDac.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */


/* ----------------------------------------
    instances or global variables
---------------------------------------- */


/* ----------------------------------------
    constructor destructor
---------------------------------------- */
STM32F_ADC_DAC::STM32F_ADC_DAC()
{
}

STM32F_ADC_DAC::~STM32F_ADC_DAC()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void STM32F_ADC_DAC::begin( ADC_TypeDef *adcx )
{
  ADCx = adcx;
  if( ADCx == ADC1 )
  {
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_ADC1, DISABLE );
  }
  else if( ADCx == ADC2 )
  {
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2, ENABLE );
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_ADC2, DISABLE );
  }
  else if( ADCx == ADC3 )
  {
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC3, ENABLE );
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_ADC3, DISABLE );
  }
  else return;
  RCC_ADCCLKConfig( RCC_PCLK2_Div6 );  /* The ADC CLK's division ratio can be selected from 2, 4, 6, or 8. */

  /* Calibration */
  ADC_DeInit( ADCx );
  ADC_InitTypeDef ADC_InitStruct;
  ADC_StructInit( &ADC_InitStruct );
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  /*右詰*/
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /*外部イベントによる変換は行わない*/
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  /*SCAN禁止*/
  ADC_InitStruct.ADC_NbrOfChannel = 1;  /*変換チャンネル数*/
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  /*ENABLEにすると大変な事になる*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  /*独立モード*/
  ADC_Init( ADCx, &ADC_InitStruct );

  ADC_Cmd( ADCx, ENABLE );
  // A-Dコンバータ補正
  ADC_ResetCalibration( ADCx );
  while( ADC_GetCalibrationStatus( ADCx ) != RESET ) rot_rdq();
  ADC_StartCalibration( ADCx );
  while( ADC_GetCalibrationStatus( ADCx ) != SET ) rot_rdq();
  ADC_Cmd( ADCx, DISABLE );
}


/* ----------------------------------------
    get adc and channel
---------------------------------------- */
void STM32F_ADC_DAC::whatADCType( int pin, uint8_t *channel )
{
  if( pin >= ADC1_IN0_PIN && pin <= ADC1_IN7_PIN )
  {
    *channel = (uint8_t)(pin - ADC1_IN0_PIN) + 0;
  }
  else if( pin >= ADC1_IN8_PIN && pin <= ADC1_IN9_PIN )
  {
    *channel = (uint8_t)(pin - ADC1_IN8_PIN) + 8;
  }
  else if( pin >= ADC1_IN10_PIN && pin <= ADC1_IN15_PIN )
  {
    *channel = (uint8_t)(pin - ADC1_IN10_PIN) + 10;
  }
  else if( pin >= ADC2_IN0_PIN && pin <= ADC2_IN7_PIN )
  {
    *channel = (uint8_t)(pin - ADC2_IN0_PIN) + 0;
  }
  else if( pin >= ADC2_IN8_PIN && pin <= ADC2_IN9_PIN )
  {
    *channel = (uint8_t)(pin - ADC1_IN8_PIN) + 8;
  }
  else if( pin >= ADC2_IN10_PIN && pin <= ADC2_IN15_PIN )
  {
    *channel = (uint8_t)(pin - ADC2_IN10_PIN) + 10;
  }
  else if( pin >= ADC3_IN0_PIN && pin <= ADC3_IN3_PIN )
  {
    *channel = (uint8_t)(pin - ADC3_IN0_PIN) + 0;
  }
  else if( pin >= ADC3_IN10_PIN && pin <= ADC3_IN13_PIN )
  {
    *channel = (uint8_t)(pin - ADC3_IN10_PIN) + 10;
  }
}


/* ----------------------------------------
    analogRead
---------------------------------------- */
uint16_t STM32F_ADC_DAC::analogRead( int pin, uint8_t cycle )
{
  GPIO::pinMode( pin, ANALOG, GPIO_SPEED_NORMAL );

  ADC_DeInit( ADCx );
  ADC_InitTypeDef ADC_InitStruct;
  ADC_StructInit( &ADC_InitStruct );
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  /* right-aligned */
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /* No conversion by external events. */
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  /* SCAN mode */
  ADC_InitStruct.ADC_NbrOfChannel = 1;  /* Number of conversion channels. */
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  /* If you make it ENABLE, you're in big trouble. */
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  /* independent mode */
  ADC_Init( ADCx, &ADC_InitStruct );

  uint8_t channel;
  whatADCType( pin, &channel );
  ADC_RegularChannelConfig( ADCx, channel, 1, cycle );
  ADC_ExternalTrigConvCmd( ADCx, DISABLE );

  ADC_Cmd( ADCx, ENABLE );
  ADC_SoftwareStartConvCmd( ADCx, ENABLE );
  while( ADC_GetFlagStatus( ADCx, ADC_FLAG_EOC ) == RESET ) rot_rdq();
  ADC_ClearFlag( ADCx, ADC_FLAG_EOC );
  uint16_t tempUS = ADC_GetConversionValue( ADCx ) & 0x0FFF;
  ADC_SoftwareStartConvCmd( ADCx, DISABLE );
//  ADC_Cmd( ADCx, DISABLE );

  return tempUS;
}

volatile uint16_t adcBuffer[3];
volatile uint16_t adcBufferWptr,adcBufferRptr;
volatile uint16_t adcBufferUpdate;
void STM32F_ADC_DAC::analogRead( ADC_PIN_AND_DATA ad[], size_t size )
{
  for( int i = 0; i < (int)size; i++ )
  {
    GPIO::pinMode( ad[i].config.pin, ANALOG, GPIO_SPEED_NORMAL );
  }

  ADC_DeInit( ADCx );
  ADC_InitTypeDef ADC_InitStruct;
  ADC_StructInit( &ADC_InitStruct );
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  /* right-aligned */
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /* No conversion by external events. */
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  /* SCAN mode */
  ADC_InitStruct.ADC_NbrOfChannel = 1;  /* Number of conversion channels. */
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  /* If you make it ENABLE, you're in big trouble. */
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  /* independent mode */
  ADC_Init( ADCx, &ADC_InitStruct );

  ADC_Cmd( ADCx, ENABLE );
#if 1
  for( int i = 0; i < (int)size; i++ )
  {
    uint8_t channel;
    whatADCType( ad[i].config.pin, &channel );
    ADC_RegularChannelConfig( ADCx, channel, 1, ad[i].config.cycle );
    ADC_SoftwareStartConvCmd( ADCx, ENABLE );
    while( ADC_GetFlagStatus( ADCx, ADC_FLAG_EOC ) == RESET ) rot_rdq();
    ad[i].data = ADC_GetConversionValue( ADCx ) & 0x0FFF;
  }
  ADC_Cmd( ADCx, DISABLE );
#else
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
  ADC_ITConfig( ADCx, ADC_IT_EOC, ENABLE );

  uint16_t adcBufferUpdateBase = adcBufferUpdate;
  ADC_Cmd( ADCx, ENABLE );
  ADC_SoftwareStartConvCmd( ADCx, ENABLE );
  while( adcBufferUpdate == adcBufferUpdateBase ) rot_rdq();
  ADC_ITConfig( ADCx, ADC_IT_EOC, DISABLE );
  for( int i = 0; i < (int)size; i++ )
  {
    ad[i].data = adcBuffer[i] & 0x0FFF;
  }
  ADC_Cmd( ADCx, DISABLE );
#endif
}

void STM32F_ADC_DAC::analogRead(
  ADC_PIN_AND_DATA ad[], size_t size,
  uint16_t *dst, size_t count,
  uint32_t trigger )
{
  /* YES! dma use. */
  for( int i = 0; i < (int)size; i++ )
  {
    GPIO::pinMode( ad[i].config.pin, ANALOG, GPIO_SPEED_NORMAL );
  }

  ADC_Cmd( ADCx, DISABLE );
  ADC_DeInit( ADCx );
  ADC_InitTypeDef ADC_InitStruct;
  ADC_StructInit( &ADC_InitStruct );
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  /* right-aligned */
  ADC_InitStruct.ADC_ExternalTrigConv = trigger;  /**/
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;  /* SCAN mode */
  ADC_InitStruct.ADC_NbrOfChannel = size;  /* Number of conversion channels. */
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  /* If you make it ENABLE, you're in big trouble. */
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  /* independent mode */
  ADC_Init( ADCx, &ADC_InitStruct );
  for( int i = 0; i < (int)size; i++ )
  {
    uint8_t channel;
    whatADCType( ad[i].config.pin, &channel );
    ADC_RegularChannelConfig( ADCx, channel, i + 1, ad[i].config.cycle );
  }
  ADC_ExternalTrigConvCmd( ADCx, ENABLE );

  /* Initialization of DMA1 ch1 or DMA2 ch5 */
  DMA_Channel_TypeDef *DMAx_channelY;
  int DMAx_ChannelY_IRQn;
  if( ADCx == ADC1 || ADCx == ADC2 )
  {
    DMAx_channelY = DMA1_Channel1;
    DMAx_ChannelY_IRQn = DMA1_Channel1_IRQn;
  }
  else if( ADCx == ADC3 )
  {
    DMAx_channelY = DMA2_Channel5;
    DMAx_ChannelY_IRQn = DMA2_Channel4_5_IRQn;
  }
  STM32F_DMA dma;
  dma.begin( DMAx_channelY );
  dma.p2m( dst, (const uint16_t *)&ADCx->DR, count, DMA_Mode_Circular );  /* configure the dma controller. */
  dma.ITConfig( DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, ENABLE );  /* interrupt set. */
  dma.command( ENABLE );  /* enable dma. */

  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMAx_ChannelY_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  ADC_DMACmd( ADCx, ENABLE );
  ADC_Cmd( ADCx, ENABLE );
}


extern "C"
{
//uint16_t adcBuffer[3];
//int adcBufferWptr,adcBufferRptr;
void ADC1_2_IRQHandler( void )
{
  if( ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC ) == SET )
  {
//    ADC_ClearFlag( ADC1, ADC_FLAG_EOC );
#if 1
    adcBuffer[adcBufferWptr] = ADC_GetConversionValue( ADC1 ) & 0x0FFF;
    if( ++adcBufferWptr >= (uint16_t)(sizeof(adcBuffer) / sizeof(adcBuffer[0])) )
    {
      adcBufferWptr = 0;
    }
#else
    adcBuffer[0] = ADC_GetConversionValue( ADC1 ) & 0x0FFF;
    adcBuffer[1] = ADC_GetConversionValue( ADC1 ) & 0x0FFF;
    adcBuffer[2] = ADC_GetConversionValue( ADC1 ) & 0x0FFF;
#endif
    adcBufferUpdate++;
  }
}
}  /* extern "C" */
