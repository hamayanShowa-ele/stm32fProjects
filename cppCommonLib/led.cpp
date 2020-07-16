/* ----------------------------------------
  led utilities
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

#include  "led.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
LED::LED()
{
}

LED::LED( int pin, bool active )
{
  begin( pin, active );
}

LED::~LED()
{
  GPIO::pinMode( pinNumber, INPUT_PULLUP );
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void LED::begin( int pin, bool active )
{
  pinNumber = pin;
  activeHigh = active;
  GPIO::pinMode( pin, OUTPUT, GPIO_SPEED_SLOW );
}

void LED::end()
{
  GPIO::pinMode( pinNumber, INPUT_PULLUP );
}


/* ----------------------------------------
    led turn on / off
---------------------------------------- */
void LED::On()
{
  if( activeHigh ) GPIO::set( pinNumber );
  else GPIO::reset( pinNumber );
}

void LED::Off()
{
  if( activeHigh ) GPIO::reset( pinNumber );
  else GPIO::set( pinNumber );
}

void LED::OnOff( bool bit )
{
  if( bit ) On();
  else Off();
}

void LED::toggle()
{
  GPIO::digitalWrite( pinNumber, !GPIO::digitalRead(pinNumber) );
}

