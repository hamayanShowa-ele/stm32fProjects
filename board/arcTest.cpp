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

#include  "arcTest.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
ARC_TEST::ARC_TEST()
{
}

ARC_TEST::~ARC_TEST()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */

/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void ARC_TEST::gpioInit()
{
  /* 1206A LEDs */
  pinMode( LED_1, OUTPUT );
  pinMode( LED_2, OUTPUT );
  pinMode( LED_3, OUTPUT );
  pinMode( LED_4, OUTPUT );
  digitalWrite( LED_1, LOW );  /* active high. */
  digitalWrite( LED_2, LOW );  /* active high. */
  digitalWrite( LED_3, LOW );  /* active high. */
  digitalWrite( LED_4, LOW );  /* active high. */

  /* board switches */
  pinMode( SWITCH_1, INPUT_PULLUP );
  pinMode( SWITCH_2, INPUT_PULLUP );
  pinMode( SWITCH_3, INPUT_PULLUP );
}


extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
