/* ----------------------------------------
  lt3593 utilities
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

#include  "lt3593.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
LT3593::LT3593()
{
}

LT3593::LT3593( uint8_t pin )
{
  begin( pin );
}

LT3593::~LT3593()
{
  pinMode( gpioPin, INPUT_PULLDOWN );
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void LT3593::begin( uint8_t pin )
{
  gpioPin = pin;
  pinMode( gpioPin, OUTPUT );
}

/* ----------------------------------------
    brightness
---------------------------------------- */
void LT3593::brightness( int luminus )
{
  if( luminus < 0 || luminus >= DARK )
  {
    reset( gpioPin );  // BRIGHT_LO;
    dly100us( 2 );
    return;
  }

  reset( gpioPin );  // BRIGHT_LO;
  dly100us( 2 );
  set( gpioPin );  // BRIGHT_HI;
  int i = 0;
  while( i < luminus )
  {
    reset( gpioPin );  // BRIGHT_LO;
    set( gpioPin );  // BRIGHT_HI;
    i++;
  }
}

