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

#ifndef  __BOARD_1715_H__
#define  __BOARD_1715_H__

#include  <derivertive.h>
#include  <gpio.h>
#include  <Timer.h>

extern "C"
{
  #include  <system.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  PWM1  PA0
#define  PWM2  PA1
#define  PWM3  PA2
#define  PWM4  PA3
#define  PWM10 PA4
#define  PWM11 PA5
#define  PWM5  PA6
#define  PWM6  PA7
#define  PWM7  PB0
#define  PWM8  PB1

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BOARD_1715 : public GPIO
{
private:

public:
  BOARD_1715() {}
  ~BOARD_1715() {}

  void begin();
};


class BOARD_1715_PWM : public STM32F_TIMER
{
private:
  uint16_t scale,offset;

public:
  BOARD_1715_PWM() {}
  ~BOARD_1715_PWM() {}

  int begin( TIM_TypeDef *TIMx );
};


#endif  /* __BOARD_1715_H__ */

