/* ----------------------------------------
 stm32f1xx ADC and DAC utility header
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
#ifndef  __ADC_DAC_H__
#define  __ADC_DAC_H__

#include  <stm32f10x.h>
#include  <derivertive.h>
#include  <gpio.h>
#include  <Dma.h>
#include  <Timer.h>

extern "C"
{
  #include  <stddef.h>
  #include  <nvic.h>
  #include  <mul_tsk.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
typedef struct
{
  uint8_t pin;
  uint8_t cycle;
} ADC_PIN_CONFIG;

typedef struct
{
  ADC_PIN_CONFIG config;
  uint16_t data;
} ADC_PIN_AND_DATA;

/* ----------------------------------------
    prototypes
---------------------------------------- */


/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class STM32F_ADC : public GPIO
{
public:
  STM32F_ADC();
  STM32F_ADC( ADC_TypeDef *adcx );
  ~STM32F_ADC();
  void begin( ADC_TypeDef *adcx );
//  void end();
  uint16_t analogRead( int pin, uint8_t cycle = ADC_SampleTime_7Cycles5 );
  void analogRead( ADC_PIN_AND_DATA data[], size_t size );
  void analogRead(
    ADC_PIN_AND_DATA ad[], size_t size,
    uint16_t *dst, size_t count,
    uint32_t trigger = ADC_ExternalTrigConv_T1_CC3 );

private:
  ADC_TypeDef *ADCx;

  void whatADCType( int pin, uint8_t *channel );
};

class STM32F_DAC : public GPIO
{
public:
  STM32F_DAC();
  STM32F_DAC( uint32_t ch, uint32_t trig = DAC_Trigger_Software );
  ~STM32F_DAC();

  void begin( uint32_t ch, uint32_t trig = DAC_Trigger_Software );
  void start();
  void stop();
  void write( uint16_t data );
  void write( uint16_t data1, uint16_t data2 );
  void write( const uint16_t *data, size_t size, TIM_TypeDef *tim = TIM7, bool repeat = true );
  void write( const uint32_t *data, size_t size, uint32_t freq, TIM_TypeDef *tim );

  uint16_t read();
  void dmaSet( DMA_Channel_TypeDef *DMAx_Channelx, const uint16_t *data, uint16_t size );

private:
//  DAC_TypeDef *DACx;
  uint32_t channel;
  uint32_t trigger;

  void whatADCType( int pin, uint8_t *channel );
};

#endif  /* __ADC_DAC_H__ */

