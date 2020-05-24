/* ----------------------------------------
 board 0900 utilities.
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

#ifndef  __BOARD_0900_H__
#define  __BOARD_0900_H__

#include  <Timer.h>
#include  <gpio.h>
extern "C"
{
  #include  <mul_tsk.h>
  #include  <ctype.h>
  #include  <fsmc.h>
  #include  <system.h>
}


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BOARD_0900 : public GPIO
{
public:
  BOARD_0900();
  ~BOARD_0900();

  void gpioInit();
  void extBusInit();
  void glcdClockInit();
  void glcdInterruptInit();
  void glcdReset();
  void glcdSleep( bool onOff );

private:
  void busPortInitialize();
};


#endif  /* __BOARD_0900_H__ */

