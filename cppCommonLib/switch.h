/* ----------------------------------------
  switch utilities
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

#ifndef  __SWITCH_H__
#define  __SWITCH_H__

#include  <gpio.h>

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class SWITCH : public GPIO
{
private:
  int pinNumber;
  bool level;

public:
  SWITCH(){};
  SWITCH( int pin, bool active = false ) { begin( pin, active ); }
  ~SWITCH(){}

  void begin( int pin, bool active = false )
  {
    pinNumber = pin;
    level = active;
    if( level == true ) pinMode( pinNumber, INPUT_PULLDOWN );
    else pinMode( pinNumber, INPUT_PULLUP );
  }

  bool push();
};


#endif  /* __SWITCH_H__ */

