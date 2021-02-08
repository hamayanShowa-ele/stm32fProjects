/* ----------------------------------------
  Board utility header
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
#ifndef  _BOARD_UTILITY_H_
#define  _BOARD_UTILITY_H_

#include  <led.h>
#include  <strutil.h>

extern "C"
{
  #include  <stdlib.h>
  #include  <system.h>
}

/* ----------------------------------------
  defines.
---------------------------------------- */

/* ----------------------------------------
  prototypes.
---------------------------------------- */
void blinkLED( LED *led, RELTIM dly );
void ramCheck( void *ram, size_t size, LED *led );
uint32_t RemainStack( const void *stk, uint32_t sz );

int randomRamCheck( uint16_t *ram, size_t size, uint32_t seed );

#endif  /*_BOARD_UTILITY_H_*/
