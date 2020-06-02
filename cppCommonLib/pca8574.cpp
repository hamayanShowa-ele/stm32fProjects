/* ----------------------------------------
  pca8574 utilities
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

#include  "pca8574.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
PCA8574::PCA8574()
{
}

PCA8574::PCA8574( STM32F_I2C *i2c, uint8_t adr )
{
  begin( i2c, adr );
}

PCA8574::~PCA8574()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void PCA8574::begin( STM32F_I2C *i2c, uint8_t adr )
{
  wire = i2c;
  slave = adr;
  byteData = 0;
}

/* ----------------------------------------
    write
---------------------------------------- */
int PCA8574::write( uint8_t data )
{
  byteData = data;
  int ret = wire->beginTransmission( slave );
  if( ret < 0 ) return ret;
  ret = wire->write( byteData );
  wire->endTransmission();
  return ret;
}


/* ----------------------------------------
    read
---------------------------------------- */
int PCA8574::read()
{
  int ret = wire->beginTransmission( slave );
  if( ret < 0 ) return ret;
  ret = wire->read();
  wire->endTransmission();
  return ret;
}

/* ----------------------------------------
    bit set, bit reset.
---------------------------------------- */
int PCA8574::bitSetReset( uint8_t data )
{
  int ret = wire->beginTransmission( slave );
  if( ret < 0 ) return ret;
  ret = wire->write( data );
  wire->endTransmission();
  return ret;
}

int PCA8574::bitSet( int bit )
{
  uint8_t tempUC = 0x01;
  tempUC <<= bit;
  byteData |= tempUC;
  return bitSetReset( byteData );
}

int PCA8574::bitReset( int bit )
{
  uint8_t tempUC = 0x01;
  tempUC <<= bit;
  byteData &= ~tempUC;
  return bitSetReset( byteData );
}
