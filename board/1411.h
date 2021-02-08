/* ----------------------------------------
 board 1411 for 1408 jigu utilities.
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

#ifndef  __BOARD_1411_H__
#define  __BOARD_1411_H__

#include  <gpio.h>
#include  <derivertive.h>

extern "C"
{
  #include  <mul_tsk.h>
}


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BOARD_1411 : public GPIO
{
private:
  uint8_t si,sclk,oe,latch;
  void bitShift( bool bit );
  void dwordShift( uint32_t data );

public:
  BOARD_1411();
  ~BOARD_1411();

  void begin( uint8_t _si, uint8_t _sclk, uint8_t _oe, uint8_t _latch );
  void clear();
};


#endif  /* __BOARD_1411_H__ */

