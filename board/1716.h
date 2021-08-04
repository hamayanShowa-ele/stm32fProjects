/* ----------------------------------------
 board 1716 utilities.
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

#ifndef  __BOARD_1716_H__
#define  __BOARD_1716_H__

#include  <gpio.h>
#include  <derivertive.h>

extern "C"
{
  #include  <system.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  WAVE_RIGHT  PA11
#define  WAVE_LEFT   PC9
#define  SHUTDOWN    PC11
#define  OUTLED      PC10
#define  ACT_LED     PA15
#define  SW_CANCEL   0x01  // REALY1
#define  SW_SET      0x02  // REALY2
#define  SW_RSV2     0x04  // REALY3
#define  SW_RSV1     0x08  // REALY4
#define  SW_DOWN     0x40  // LOCK
#define  SW_UP       0x10  // DATE
#define  SW_OUTPUT   0x20  // TIME
#define  SW_OFFSET   0x20  // SCAN5
#define  SW_AMP      0x10  // SCAN4
#define  SW_PHASE    0x08  // SCAN3
#define  SW_FREQ     0x04  // SCAN2
#define  SW_MODE     0x40  // MODE
#define  SW_CHANNEL  0x80  // ALTER
#define  LCD_RS      PB3
#define  LCD_RW      PB4
#define  LCD_E       PB5
#define  LCD_DB4     PB6
#define  LCD_DB5     PB7
#define  LCD_DB6     PB8
#define  LCD_DB7     PB9

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BOARD_1716 : public GPIO
{
private:

public:
  BOARD_1716() {}
  ~BOARD_1716() {}

  void begin();
};


#endif  /* __BOARD_1716_H__ */

