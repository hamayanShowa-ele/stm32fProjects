/* ----------------------------------------
 board 9405 digital switches utilities.
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

#ifndef  __BOARD_9405_H__
#define  __BOARD_9405_H__

#include  <gpio.h>

extern "C"
{
}  /* extern "C" */

/* ----------------------------------------
    defines
---------------------------------------- */
#define  SWITCH_SEL0  PD11
#define  SWITCH_SEL1  PD12
#define  SWITCH_SEL2  PD13

#define  SWITCH_ROW0  PD14
#define  SWITCH_ROW1  PD15
#define  SWITCH_ROW2  PC8
#define  SWITCH_ROW3  PC9

enum SWITCH_GROUP { TIMER_SWITCH, VOLTAGE_SWITCH ,PHASE_SWITCH };

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class BD9405 : public GPIO
{
public:
  BD9405();
  ~BD9405();

  void gpioInit();
  uint8_t switchRead( int sw );

private:
  uint8_t read( int sel );
};

#endif  /* __BOARD_9405_H__ */

