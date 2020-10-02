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
volatile uint16_t adc1Buffer[1];
volatile uint16_t adc1BufferWptr,adc1BufferRptr;
volatile uint8_t  adc1BufferUpdate;

volatile uint16_t adc2Buffer[1];
volatile uint16_t adc2BufferWptr,adc2BufferRptr;
volatile uint8_t  adc2BufferUpdate;

/* ----------------------------------------
  adc constructor destructor
---------------------------------------- */
STM32F_ADC::STM32F_ADC()
{
}

STM32F_ADC::STM32F_ADC( ADC_TypeDef *adcx )
{
  begin( adcx );
}

STM32F_ADC::~STM32F_ADC()
{
}

/* ----------------------------------------
  adc begin and end
---------------------------------------- */
void STM32F_ADC::begin( ADC_TypeDef *adcx )
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
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  /* right-aligned */
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /* No conversion by external events. */
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  /* SCAN mode is disable.*/
  ADC_InitStruct.ADC_NbrOfChannel = 1;  /* Number of conversion channels. */
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  /* If you make it ENABLE, you're in big trouble. */
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  /* independent mode */
  ADC_Init( ADCx, &ADC_InitStruct );

  ADC_Cmd( ADCx, ENABLE );
  // ADC calibration.
  ADC_ResetCalibration( ADCx );
  while( ADC_GetCalibrationStatus( ADCx ) != RESET ) rot_rdq();
  ADC_StartCalibration( ADCx );
  while( ADC_GetCalibrationStatus( ADCx ) != SET ) rot_rdq();
  ADC_Cmd( ADCx, DISABLE );
}


/* ----------------------------------------
  get adc and channel
---------------------------------------- */
void STM32F_ADC::whatADCType( int pin, uint8_t *channel )
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
  adc analogRead
---------------------------------------- */
uint16_t STM32F_ADC::analogRead( int pin, uint8_t cycle )
{
  GPIO::pinMode( pin, ANALOG, GPIO_SPEED_NORMAL );

  ADC_DeInit( ADCx );
  ADC_InitTypeDef ADC_InitStruct;
  ADC_StructInit( &ADC_InitStruct );
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  /* right-aligned */
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /* No conversion by external events. */
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  /* SCAN mode is disable. */
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

/* No! interrupt use. */
void STM32F_ADC::analogRead( ADC_PIN_AND_DATA ad[], size_t size )
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
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  /* SCAN mode is disable.*/
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

  uint16_t adc1BufferUpdateBase = adc1BufferUpdate;
  ADC_Cmd( ADCx, ENABLE );
  ADC_SoftwareStartConvCmd( ADCx, ENABLE );
  while( adc1BufferUpdate == adc1BufferUpdateBase ) rot_rdq();
  ADC_ITConfig( ADCx, ADC_IT_EOC, DISABLE );
  for( int i = 0; i < (int)size; i++ )
  {
    ad[i].data = adc1Buffer[i] & 0x0FFF;
  }
  ADC_Cmd( ADCx, DISABLE );
#endif
}

/* YES! dma use. */
void STM32F_ADC::analogRead(
  ADC_PIN_AND_DATA ad[], size_t size,
  volatile uint16_t *dst, size_t count,
  uint32_t trigger )
{
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
  ADC_InitStruct.ADC_ScanConvMode = (size <= 1) ? DISABLE : ENABLE;  /* SCAN mode */
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
  dma.p2m( (uint16_t *)dst, (const uint16_t *)&ADCx->DR, count, DMA_Mode_Circular );  /* configure the dma controller. */
//  dma.ITConfig( DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, ENABLE );  /* interrupt set. */
  dma.ITConfig( DMA_IT_TC | DMA_IT_TE, ENABLE );  /* interrupt set. */
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


/* ----------------------------------------
  dac constructor destructor
---------------------------------------- */
STM32F_DAC::STM32F_DAC()
{
}

STM32F_DAC::STM32F_DAC( uint32_t ch, uint32_t trig )
{
  begin( ch, trig );
}

STM32F_DAC::~STM32F_DAC()
{
}

/* ----------------------------------------
  dac begin and end
---------------------------------------- */
void STM32F_DAC::begin( uint32_t ch, uint32_t trig )
{
  channel = ch;  /* dac channel */
  trigger = trig;  /* trigger type */
  if( channel == DAC_Channel_1 )
  {
    pinMode( PA4, ANALOG );
  }
  else  /* DAC_Channel_2 */
  {
    pinMode( PA5, ANALOG );
  }
}

/* ----------------------------------------
  dac start and stop
---------------------------------------- */
void STM32F_DAC::start()
{
  /* DAC Peripheral clock enable */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE );

  /* DAC channel1 Configuration */
  DAC_Cmd( channel, DISABLE );
  DAC_DeInit();
  DAC_InitTypeDef DAC_InitStructure;
  DAC_InitStructure.DAC_Trigger = trigger;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init( channel, &DAC_InitStructure );

//  DAC->DHR12RD = (uint32_t)0;
  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
     automatically connected to the DAC converter. */
  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
     automatically connected to the DAC converter. */
  DAC_Cmd( channel, ENABLE );
}

void STM32F_DAC::stop()
{
  DAC_Cmd( channel, DISABLE );
}

/* ----------------------------------------
  dac data set
---------------------------------------- */
void STM32F_DAC::write( uint16_t data )
{
  if( channel == DAC_Channel_1 )
  {
    DAC_SetChannel1Data( DAC_Align_12b_R, data );
  }
  else  /* DAC_Channel_2 */
  {
    DAC_SetChannel2Data( DAC_Align_12b_R, data );
  }
  DAC_SoftwareTriggerCmd( channel, ENABLE );
}

void STM32F_DAC::write( uint16_t data1, uint16_t data2 )
{
  DAC_SetDualChannelData( DAC_Align_12b_R, data2, data1 );
  DAC_SoftwareTriggerCmd( DAC_Channel_1 | DAC_Channel_2, ENABLE );
}


void STM32F_DAC::write( const uint16_t *data, size_t size, TIM_TypeDef *tim, bool repeat )
{
  /* configure a convert timing timer. */
  STM32F_TIMER timer( tim );
  timer.frequency( 50UL );
  timer.adcTrigger( TIMx_CH4, ADC1_IN0_PIN, 0 );

  /* configure a dma controller. */
  /* DAC1:dma2 channel 3, DAC2:dma2 channel 4  */
  STM32F_DMA dma;
  if( channel == DAC_Channel_1 )
  {
    dma.begin( DMA2_Channel3 );
    dma.m2p( (uint16_t *)&(DAC->DHR12R1), data, size );
  }
  else /* DAC_Channel_2 */
  {
    dma.begin( DMA2_Channel4 );
    dma.m2p( (uint16_t *)&(DAC->DHR12R2), data, size );
  }
  dma.command( ENABLE );
  DAC_DMACmd( channel, ENABLE );  /* DMA2_Channel3 */

  /* convert timing timer start. */
  timer.start();
}

/* 32bit */
void STM32F_DAC::write( const uint32_t *data, size_t size, uint32_t freq, TIM_TypeDef *tim )
{
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE );
  /* TIMx Periph clock enable */
  STM32F_TIMER timer( tim );

  /* configure DAC port */
  pinMode( PA4, ANALOG );
  pinMode( PA5, ANALOG );

  /* TIMx Configuration */
  /* Time base configuration */
  timer.frequency( freq );

  /* TIMx TRGO selection */
  timer.trigger( TIM_TRGOSource_Update );

  DAC_InitTypeDef DAC_InitStructure;
  /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = trigger;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init( DAC_Channel_1, &DAC_InitStructure );
  /* DAC channel2 Configuration */
  DAC_Init( DAC_Channel_2, &DAC_InitStructure );

  /* dma initialize */
  STM32F_DMA dma( DMA2_Channel3 );
  dma.m2p( (uint32_t *)&(DAC->DHR12RD), (const uint32_t *)data, size, DMA_Mode_Circular );
  dma.command( ENABLE );

  /* dac enable */
  uint32_t tempUL = *data;
  DAC->DHR12RD = tempUL;
  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
     automatically connected to the DAC converter. */
  DAC_Cmd( DAC_Channel_1, ENABLE );
  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
     automatically connected to the DAC converter. */
  DAC_Cmd( DAC_Channel_2, ENABLE );

  /* Enable DMA for DAC Channel2 */
  DAC_DMACmd( DAC_Channel_1, ENABLE );  /* DMA2_Channel3 */
  //DAC_DMACmd( DAC_Channel_2, ENABLE );  /* DMA2_Channel4 : not able */

  timer.start();
}

/* ----------------------------------------
  dac data get
---------------------------------------- */
uint16_t STM32F_DAC::read()
{
  return DAC_GetDataOutputValue( channel );
}

/* ----------------------------------------
  dac dma set
---------------------------------------- */
void STM32F_DAC::dmaSet( DMA_Channel_TypeDef *DMAx_Channelx, const uint16_t *data, uint16_t size )
{
#if 0
  /* dma initialize */
  DMA_DeInit( DMAx_Channelx );
  DMA_InitTypeDef DMA_InitStructure;
  if( channel == DAC_Channel_1 )
  {
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(DAC->DHR12R1));
  }
  else /* DAC_Channel_2 */
  {
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(DAC->DHR12R2));
  }
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)data;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = size;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init( DMAx_Channelx, &DMA_InitStructure );

  DMA_ClearFlag( DMAx_Flag_TCx | DMAx_Flag_HTx | DMAx_Flag_TEx );
  DMA_ITConfig( DMAx_Channelx, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE, ENABLE );   /*転送/エラー割り込み許可*/
  /* Enable DMAx Channelx */
  DMA_Cmd( DMAx_Channelx, ENABLE );
#endif
}




/* ----------------------------------------
  adc and dac interrupt
---------------------------------------- */
extern "C"
{
void ADC1_2_IRQHandler( void )
{
  if( ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC ) == SET )
  {
//    ADC_ClearFlag( ADC1, ADC_FLAG_EOC );
    adc1Buffer[ adc1BufferWptr ] = ADC_GetConversionValue( ADC1 ) & 0x0FFF;
    if( ++adc1BufferWptr >= (uint16_t)(sizeof(adc1Buffer) / sizeof(adc1Buffer[0])) )
    {
      adc1BufferWptr = 0;
    }
    adc1BufferUpdate++;
  }

  if( ADC_GetFlagStatus( ADC2, ADC_FLAG_EOC ) == SET )
  {
//    ADC_ClearFlag( ADC2, ADC_FLAG_EOC );
    adc2Buffer[ adc2BufferWptr ] = ADC_GetConversionValue( ADC2 ) & 0x0FFF;
    if( ++adc2BufferWptr >= (uint16_t)(sizeof(adc2Buffer) / sizeof(adc2Buffer[0])) )
    {
      adc2BufferWptr = 0;
    }
    adc2BufferUpdate++;
  }
}
}  /* extern "C" */
