/* ----------------------------------------
 stm32f peripheral header
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
#ifndef  __STM32F_PERIPHERAL_H__
#define  __STM32F_PERIPHERAL_H__

#if defined (STM32F4XX)
#include  <stm32f4xx.h>
#endif  /* STM32F10X_xx */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    defines
---------------------------------------- */
enum STM32F_PIN_NAME
{
  PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
  PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
  PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
  PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
  PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
  PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
  PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
  PH0,PH1,PH2,PH3,PH4,PH5,PH6,PH7,PH8,PH9,PH10,PH11,PH12,PH13,PH14,PH15,
  PI0,PI1,PI2,PI3,PI4,PI5,PI6,PI7,PI8,PI9,PI10,PI11,PI12,PI13,PI14,PI15,
  PORT_END,
};


enum STM32F_ADC_NAME
{
//  ADC123_IN0,ADC123_IN1,ADC123_IN2,ADC123_IN3,
//  ADC12_IN4,ADC12_IN5,ADC12_IN6,ADC12_IN7,
//  ADC12_IN8,ADC12_IN9,
//  ADC123_IN10,ADC123_IN11,ADC123_IN12,ADC123_IN13,
//  ADC12_IN14,ADC12_IN15,

  ADC1_IN0,ADC1_IN1,ADC1_IN2,ADC1_IN3,
  ADC1_IN4,ADC1_IN5,ADC1_IN6,ADC1_IN7,
  ADC1_IN8,ADC1_IN9,ADC1_IN10,ADC1_IN11,
  ADC1_IN12,ADC1_IN13,ADC1_IN14,ADC1_IN15,

  ADC2_IN0,ADC2_IN1,ADC2_IN2,ADC2_IN3,
  ADC2_IN4,ADC2_IN5,ADC2_IN6,ADC2_IN7,
  ADC2_IN8,ADC2_IN9,ADC2_IN10,ADC2_IN11,
  ADC2_IN12,ADC2_IN13,ADC2_IN14,ADC2_IN15,

  ADC3_IN0,ADC3_IN1,ADC3_IN2,ADC3_IN3,
  ADC3_IN10,ADC3_IN11,ADC3_IN12,ADC3_IN13,
};


enum STM32F_ADC_PIN
{
  ADC1_IN0_PIN = PA0,
  ADC1_IN1_PIN = PA1,
  ADC1_IN2_PIN = PA2,
  ADC1_IN3_PIN = PA3,
  ADC1_IN4_PIN = PA4,
  ADC1_IN5_PIN = PA5,
  ADC1_IN6_PIN = PA6,
  ADC1_IN7_PIN = PA7,
  ADC1_IN8_PIN = PB0,
  ADC1_IN9_PIN = PB1,
  ADC1_IN10_PIN = PC0,
  ADC1_IN11_PIN = PC1,
  ADC1_IN12_PIN = PC2,
  ADC1_IN13_PIN = PC3,
  ADC1_IN14_PIN = PC4,
  ADC1_IN15_PIN = PC5,

  ADC2_IN0_PIN = PA0,
  ADC2_IN1_PIN = PA1,
  ADC2_IN2_PIN = PA2,
  ADC2_IN3_PIN = PA3,
  ADC2_IN4_PIN = PA4,
  ADC2_IN5_PIN = PA5,
  ADC2_IN6_PIN = PA6,
  ADC2_IN7_PIN = PA7,
  ADC2_IN8_PIN = PB0,
  ADC2_IN9_PIN = PB1,
  ADC2_IN10_PIN = PC0,
  ADC2_IN11_PIN = PC1,
  ADC2_IN12_PIN = PC2,
  ADC2_IN13_PIN = PC3,
  ADC2_IN14_PIN = PC4,
  ADC2_IN15_PIN = PC5,

  ADC3_IN0_PIN = PA0,
  ADC3_IN1_PIN = PA1,
  ADC3_IN2_PIN = PA2,
  ADC3_IN3_PIN = PA3,
  ADC3_IN10_PIN = PC0,
  ADC3_IN11_PIN = PC1,
  ADC3_IN12_PIN = PC2,
  ADC3_IN13_PIN = PC3,
};

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

#endif  /* __STM32F_PERIPHERAL_H__ */

