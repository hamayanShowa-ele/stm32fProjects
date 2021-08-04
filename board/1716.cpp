/* ----------------------------------------
 board 1716 utilities.
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

#include  "1716.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
---------------------------------------- */
void BOARD_1716::begin()
{
  pinMode( WAVE_RIGHT, OUTPUT );
  pinMode( WAVE_LEFT, OUTPUT );
  pinMode( SHUTDOWN, OUTPUT );
  pinMode( OUTLED, OUTPUT );  // active high
  pinMode( ACT_LED, OUTPUT );  // active high

  pinMode( LCD_RS, OUTPUT );
  pinMode( LCD_RW, OUTPUT );
  pinMode( LCD_E, OUTPUT );
  pinMode( LCD_DB4, OUTPUT );
  pinMode( LCD_DB5, OUTPUT );
  pinMode( LCD_DB6, OUTPUT );
  pinMode( LCD_DB7, OUTPUT );
}

extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
