/* ----------------------------------------
 stm32f1xx defines for system.
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
#include  "system.h"

/*****************************************************/
/* defines                                           */
/*****************************************************/

/*****************************************************/
/* global variables                                  */
/*****************************************************/


/*****************************************************/
/* prototypes                                        */
/*****************************************************/

/*****************************************************/
/* delay 10us                                        */
/*****************************************************/
void dly10us( uint32_t dly_us )
{
  dly_us *= SystemCoreClock / (1000000UL / 10UL);  /* 100us count. */
  uint32_t now = SysTick->VAL;
  uint32_t end = (now - dly_us) % (SystemCoreClock / 1000UL);  /* SysTick->VAL is down counter. */
  if( now > end )
  {
    while( SysTick->VAL > end ) {}
    while( SysTick->VAL < end ) {}
  }
  else
  {
    while( SysTick->VAL <= end ) {}
    while( SysTick->VAL > end ) {}
  }
}


/*****************************************************/
/* delay 100us                                       */
/*****************************************************/
void dly100us( uint32_t dly_us )
{
  dly_us *= SystemCoreClock / (1000000UL / 100UL);  /* 100us count. */
  uint32_t now = SysTick->VAL;
  uint32_t end = (now - dly_us) % (SystemCoreClock / 1000UL);  /* SysTick->VAL is down counter. */
  if( now > end )
  {
    while( SysTick->VAL > end ) {}
    while( SysTick->VAL < end ) {}
  }
  else
  {
    while( SysTick->VAL <= end ) {}
    while( SysTick->VAL > end ) {}
  }
}


/*****************************************************/
/* delay 1ms                                         */
/*****************************************************/
void dly1ms( uint32_t dly_ms )
{
  for( ; dly_ms > 0; dly_ms-- )
  {
    dly100us( 10UL );
  }
}
