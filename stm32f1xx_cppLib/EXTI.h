/* ----------------------------------------
 stm32f1xx EXTI utility header
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
#ifndef  __EXTI_H__
#define  __EXTI_H__

#include  <gpio.h>

extern "C"
{
  #include  <nvic.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  EXTI_IRQ_NUMBERS  16

/* ----------------------------------------
    prototypes
---------------------------------------- */
void extiConfig(
  uint8_t pin, EXTITrigger_TypeDef trigger, int active = LOW,
  uint8_t pri = BASE_PRIORITY, uint8_t sub = BASE_SUB_PRIORITY );
void extiCallBack( int num, void(*cb)(void) );

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

#endif  /* __EXTI_H__ */

