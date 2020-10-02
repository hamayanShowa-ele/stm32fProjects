/* ----------------------------------------
 board 9405 digital switches utilities.
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

#include  "9405.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
BD9405::BD9405()
{
  gpioInit();
}

BD9405::~BD9405()
{
}

/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void BD9405::gpioInit()
{
  pinMode( SWITCH_ROW0, INPUT );  /* SWITCH ROW0 */
  pinMode( SWITCH_ROW1, INPUT );  /* SWITCH ROW1 */
  pinMode( SWITCH_ROW2, INPUT );  /* SWITCH ROW2 */
  pinMode( SWITCH_ROW3, INPUT );  /* SWITCH ROW3 */

  pinMode( SWITCH_SEL0, OUTPUT );  /* SWITCH SEL0 */
  pinMode( SWITCH_SEL1, OUTPUT );  /* SWITCH SEL1 */
  pinMode( SWITCH_SEL2, OUTPUT );  /* SWITCH SEL2 */
  reset( SWITCH_SEL0 );
  reset( SWITCH_SEL1 );
  reset( SWITCH_SEL2 );
}

/* ----------------------------------------
    switch read
---------------------------------------- */
uint8_t BD9405::read( int sel )
{
  digitalWrite( SWITCH_SEL0, sel & 0x01 ? HIGH : LOW );
  digitalWrite( SWITCH_SEL1, sel & 0x02 ? HIGH : LOW );
  digitalWrite( SWITCH_SEL2, sel & 0x04 ? HIGH : LOW );
  uint8_t tempUC = 0;
  tempUC |= digitalRead( SWITCH_ROW0 ) == HIGH ? 0x01 : 0x00;
  tempUC |= digitalRead( SWITCH_ROW1 ) == HIGH ? 0x02 : 0x00;
  tempUC |= digitalRead( SWITCH_ROW2 ) == HIGH ? 0x04 : 0x00;
  tempUC |= digitalRead( SWITCH_ROW3 ) == HIGH ? 0x08 : 0x00;

  return tempUC;
}

/* ----------------------------------------
    switch group read
---------------------------------------- */
uint8_t BD9405::switchRead( int sw )
{
  uint8_t tempUC = 0;
  switch ( sw )
  {
    case TIMER_SWITCH :
      tempUC = read( 4 ) << 4;
      tempUC |= read( 5 );
      break;

    case VOLTAGE_SWITCH :
      tempUC = read( 0 ) << 4;
      tempUC |= read( 1 );
      break;

    case PHASE_SWITCH :
      tempUC = read( 2 ) << 4;
      tempUC |= read( 3 );
      break;

    default :
      break;
  }
  return tempUC ^ 0xFF;
}


extern "C"
{
}  /* extern "C" */
