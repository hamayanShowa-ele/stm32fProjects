/* ----------------------------------------
 board 1415 utilities.
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

#ifndef  __BOARD_1415_H__
#define  __BOARD_1415_H__

#include  <gpio.h>
#include  <derivertive.h>
#include  <FSMC.h>
#include  <boardUtility.h>
extern "C"
{
//  #include  <fsmc.h>
  #include  <system.h>
}

enum V50_INT_NUMBERS
{
  INT0_NUMBER = 4,INT1_NUMBER,INT2_NUMBER,INT3_NUMBER,
  INT4_NUMBER,INT5_NUMBER,INT6_NUMBER,
};
#define  EXTI_TO_INT_NUMBER(n)  (n - INT0_NUMBER)
#define  INT0_PIN  PC4
#define  INT1_PIN  PC5
#define  INT2_PIN  PF6
#define  INT3_PIN  PF7
#define  INT4_PIN  PF8
#define  INT5_PIN  PF9
#define  INT6_PIN  PF10

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BOARD_1415 : public GPIO
{
public:
  BOARD_1415() {}
  ~BOARD_1415() {}

  void gpioInit();
  void extBusInit();
  void cbusEnable( bool onOff );
  void cbusReset();
  volatile uint16_t dummyRead();
  void etherGpioInit();

private:
  void busPortInitialize();
};


#endif  /* __BOARD_1415_H__ */

