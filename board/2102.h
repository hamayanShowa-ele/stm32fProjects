/* ----------------------------------------
 board 2102 utilities.
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

#ifndef  __BOARD_2102_H__
#define  __BOARD_2102_H__

#include  <gpio.h>
#include  <derivertive.h>
extern "C"
{
  #include  <system.h>
  void wizchip_select( void );
  void wizchip_deselect( void );
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  IN00  PC0
#define  IN01  PC1
#define  IN02  PC2
#define  IN03  PC3
#define  IN04  PA0
#define  IN05  PA1
#define  IN06  PA2
#define  IN07  PA3

#define  OUT00 PA4
#define  OUT01 PA5
#define  OUT02 PA6
#define  OUT03 PA7
#define  OUT04 PC4
#define  OUT05 PC5
#define  OUT06 PB0
#define  OUT07 PB1

#define  DIO0  PB9
#define  DIO1  PC13
#define  DIO2  PC14
#define  DIO3  PC15

#define  SW1   DIO0
#define  SW2   DIO1
#define  SW3   DIO2
#define  SW4   DIO3

#define  REDLED   PB3   /* output */
#define  LCD_RES  PB5   /* LCD reset output. */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BOARD_2102 : public GPIO
{
private:

public:
  BOARD_2102() {}
  ~BOARD_2102() {}

  void gpioInit();
  void etherGpioInit();
  void etherReset();
};


#endif  /* __BOARD_2102_H__ */

