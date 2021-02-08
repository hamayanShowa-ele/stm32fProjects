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

#include  "1411.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
BOARD_1411::BOARD_1411()
{
}

BOARD_1411::~BOARD_1411()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void BOARD_1411::begin( uint8_t _si, uint8_t _sclk, uint8_t _oe, uint8_t _latch )
{
  si = _si;
  sclk = _sclk;
  oe = _oe;
  latch = _latch;

  digitalWrite( si, LOW );
  digitalWrite( sclk, LOW );
  digitalWrite( oe, HIGH );
  digitalWrite( latch, LOW );

  pinMode( si, OUTPUT );
  pinMode( sclk, OUTPUT );
  pinMode( oe, OUTPUT );
  pinMode( latch, OUTPUT );

  clear();

  digitalWrite( oe, LOW );
}

/* ----------------------------------------
    clear
---------------------------------------- */
void BOARD_1411::clear()
{
  for( int i = 0; i < 32; i++ )
  {
    bitShift( false );
  }
}

/* ----------------------------------------
    bit shift
---------------------------------------- */
void BOARD_1411::bitShift( bool bit )
{
  digitalWrite( sclk, LOW );

  if( bit ) digitalWrite( si, HIGH );
  else digitalWrite( si, LOW );

  digitalWrite( sclk, HIGH );
}

/* ----------------------------------------
    double word shift
---------------------------------------- */
void BOARD_1411::dwordShift( uint32_t data )
{
  uint16_t upper,lower;
  lower = (uint16_t)(data >> 0);
  upper = (uint16_t)(data >> 16);
  for( int i = 0; i < 16; i++ )
  {
    bitShift( lower & 0x01 );
    bitShift( upper & 0x01 );
    lower >>= 1;
    upper >>= 1;
    dly_tsk( 1UL );
  }
  digitalWrite( latch, HIGH );
  dly_tsk( 1UL );
  digitalWrite( latch, LOW );
}

extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
