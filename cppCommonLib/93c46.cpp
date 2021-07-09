/* ----------------------------------------
  SERIAL EEPROM 93C46 or 93C56 utilities
  for STMicroelectronics SPL library

  THE SOURCE CODE OF THE FOLLOWING url WAS MODIFIED FOR STM32F.
  https://github.com/asukiaaa/AM2320_asukiaaa

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
#include  "93c46.h"


/* ----------------------------------------
    constructor destructor
---------------------------------------- */
EEPROM_93C46::~EEPROM_93C46()
{
  pinMode( csPin, OUTPUT, GPIO_SPEED_FAST );
  pinMode( skPin, OUTPUT, GPIO_SPEED_FAST );
  pinMode( diPin, OUTPUT, GPIO_SPEED_FAST );
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void EEPROM_93C46::begin( int _cs, int _sk, int _di, int _do )
{
  csPin = _cs; skPin = _sk; diPin = _di; doPin = _do;
  pinMode( csPin, OUTPUT, GPIO_SPEED_NORMAL );
  pinMode( skPin, OUTPUT, GPIO_SPEED_NORMAL );
  pinMode( diPin, OUTPUT, GPIO_SPEED_NORMAL );
  pinMode( doPin, INPUT );
  AT93C46_CS_IS_0;
  AT93C46_SK_IS_0;
  AT93C46_DI_IS_0;
}

/* ----------------------------------------
    write
    1byte write and msb first.
---------------------------------------- */
void EEPROM_93C46::write( uint8_t data )
{
  /* START BIT */
  AT93C46_DI_IS_1;
  AT93C46_SK_IS_0;  /* SCK down */
  AT93C46_SK_IS_1;  /* SCK up */
  AT93C46_SK_IS_1;  /* SCK up */
  AT93C46_SK_IS_1;  /* SCK up */
  AT93C46_SK_IS_1;  /* SCK up */
  AT93C46_SK_IS_0;  /* SCK down */

  /* ope code 1,ope code 2, A5~A0 */
  for( int i = 0; i < 8; i++ )
  {
    /* di bit set. */
    if( data & 0x80 ) AT93C46_DI_IS_1;
    else AT93C46_DI_IS_0;
    data <<= 1;

    AT93C46_SK_IS_0;  /* SCK down */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_0;  /* SCK down */
  }
}

/* ----------------------------------------
    read
    1word read and msb first.
---------------------------------------- */
uint16_t EEPROM_93C46::read()
{
  uint16_t tempUS = 0;
  for( int i = 0; i < 16; i++ )
  {
    tempUS <<= 1;
    AT93C46_SK_IS_0;  /* SCK down */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_1;  /* SCK up */
    AT93C46_SK_IS_0;  /* SCK down */

    tempUS |= (AT93C46_DO_IS) ? 0x0001 : 0x0000;
  }

  return tempUS;
}

/* ----------------------------------------
    read
    1word read and msb first.
---------------------------------------- */
uint16_t EEPROM_93C46::read( uint8_t adr )
{
  adr >>= 1;
  adr |= AT93C46_READ;
  AT93C46_CS_IS_1;  /* chip select asset. */
  write( adr );
  uint16_t data = read();
  AT93C46_DI_IS_0;
  AT93C46_CS_IS_0;  /* chip select negate. */

  return data;
}
