/* ----------------------------------------
 board 1715 utilities.
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

#include  "1715.h"

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
void BOARD_1715::begin()
{
  pinMode( PWM1, OUTPUT, GPIO_SPEED_FAST );  // PA0 : TIM5 CH1
  pinMode( PWM2, OUTPUT, GPIO_SPEED_FAST );  // PA1 : TIM5 CH2
  pinMode( PWM3, OUTPUT, GPIO_SPEED_FAST );  // PA2 : TIM5 CH3
  pinMode( PWM4, OUTPUT, GPIO_SPEED_FAST );  // PA3 : TIM5 CH4

  pinMode( PWM5, OUTPUT, GPIO_SPEED_FAST );  // PA6 : TIM3 CH1
  pinMode( PWM6, OUTPUT, GPIO_SPEED_FAST );  // PA7 : TIM3 CH2
  pinMode( PWM7, OUTPUT, GPIO_SPEED_FAST );  // PB0 : TIM3 CH3
  pinMode( PWM8, OUTPUT, GPIO_SPEED_FAST );  // PB1 : TIM3 CH4

  pinMode( PWM10, ANALOG );  // PA4 : DAC1
  pinMode( PWM11, ANALOG );  // PA5 : DAC2
}


/* ----------------------------------------
    pwm out initialize.
---------------------------------------- */
int BOARD_1715_PWM::begin( TIM_TypeDef *TIMx )
{
  STM32F_TIMER::begin( TIMx );
  STM32F_TIMER::stop();
  STM32F_TIMER::frequency( 100 * 1000UL );
  scale = STM32F_TIMER::getAutoReload() / 2;
  offset = scale;
  if( TIMx == TIM5 )
  {
    STM32F_TIMER::pwm1( TIMx_CH1, PWM1, offset );
    STM32F_TIMER::pwm1( TIMx_CH2, PWM2, offset );
    STM32F_TIMER::pwm1( TIMx_CH3, PWM3, offset );
    STM32F_TIMER::pwm1( TIMx_CH4, PWM4, offset );
  }
  else if( TIMx == TIM3 )
  {
    STM32F_TIMER::pwm1( TIMx_CH1, PWM5, offset );
    STM32F_TIMER::pwm1( TIMx_CH2, PWM6, offset );
    STM32F_TIMER::pwm1( TIMx_CH3, PWM7, offset );
    STM32F_TIMER::pwm1( TIMx_CH4, PWM8, offset );
  }
  else return (-1);

  STM32F_TIMER::start();

  return 0;
}

extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
