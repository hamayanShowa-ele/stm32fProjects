/* ----------------------------------------
 board ARIES ARC TEST utilities.
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

#ifndef  __BOARD_ARC_TEST_H__
#define  __BOARD_ARC_TEST_H__

#include  <gpio.h>
#include  <derivertive.h>

extern "C"
{
}

/* ----------------------------------------
    defines
---------------------------------------- */
/* 1206A LED */
#define  LED_1           PE6
#define  LED_2           PE5
#define  LED_3           PE4
#define  LED_4           PE3

#define  X_RST           PE2
#define  E_RST           PE1

#define  MOSI3           PB5

#define  SWITCH_1        PC8
#define  SWITCH_2        PC7
#define  SWITCH_3        PC6

#define  ACT_LED         PB1
#define  LDAC            PB0

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class ARC_TEST : public GPIO
{
public:
  ARC_TEST();
  ~ARC_TEST();

  void gpioInit();

private:
};


#endif  /* __BOARD_ARC_TEST_H__ */

