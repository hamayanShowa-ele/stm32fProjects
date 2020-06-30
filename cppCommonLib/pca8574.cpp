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

PCA8574::PCA8574( STM32F_I2C *_i2c, uint8_t adr )
{
  begin( _i2c, adr );
}

PCA8574::~PCA8574()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void PCA8574::begin( STM32F_I2C *_i2c, uint8_t adr )
{
  i2c = _i2c;
  slave = adr;
  byteData = 0;
}

/* ----------------------------------------
    write
---------------------------------------- */
int PCA8574::write( uint8_t data )
{
  byteData = data;
  int ret = i2c->beginTransmission( slave );
  if( ret < 0 )
  {
    i2c->endTransmission();
    return ret;
  }
  ret = i2c->write( byteData );
  i2c->endTransmission();
  return ret;
}


/* ----------------------------------------
    read
---------------------------------------- */
int PCA8574::read()
{
  int ret = i2c->requestFrom( slave );
  if( ret < 0 )
  {
    i2c->endTransmission();
    return ret;
  }
  ret = i2c->read( true );
  i2c->endTransmission();
  return ret;
}

/* ----------------------------------------
    bit set, bit reset.
---------------------------------------- */
int PCA8574::bitSetReset( uint8_t data )
{
  int ret = i2c->beginTransmission( slave );
  if( ret < 0 )
  {
    i2c->endTransmission();
    return ret;
  }
  ret = i2c->write( data );
  i2c->endTransmission();
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

int PCA8574::toggle( int bit )
{
  uint8_t tempUC = 0x01;
  tempUC <<= bit;
  byteData ^= tempUC;
  return bitSetReset( byteData );
}



/* ----------------------------------------
    software i2c use
---------------------------------------- */
/* ----------------------------------------
    constructor destructor
---------------------------------------- */
SOFT_PCA8574::SOFT_PCA8574()
{
}

SOFT_PCA8574::SOFT_PCA8574( STM32F_SOFT_I2C *_i2c, uint8_t adr )
{
  begin( _i2c, adr );
}

SOFT_PCA8574::~SOFT_PCA8574()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void SOFT_PCA8574::begin( STM32F_SOFT_I2C *_i2c, uint8_t adr )
{
  i2c = _i2c;
  slave = adr;
  byteData = 0;
}

/* ----------------------------------------
    write
---------------------------------------- */
int SOFT_PCA8574::write( uint8_t data )
{
  byteData = data;
  int ret = i2c->beginTransmission( slave );
  if( ret < 0 )
  {
    i2c->endTransmission();
    return ret;
  }
  ret = i2c->write( byteData );
  i2c->endTransmission();
  return ret;
}


/* ----------------------------------------
    read
---------------------------------------- */
int SOFT_PCA8574::read()
{
  int ret = i2c->requestFrom( slave );
  if( ret < 0 )
  {
    i2c->endTransmission();
    return ret;
  }
  ret = i2c->read( true );
  i2c->endTransmission();
  return ret;
}

/* ----------------------------------------
    bit set, bit reset.
---------------------------------------- */
int SOFT_PCA8574::bitSetReset( uint8_t data )
{
  int ret = i2c->beginTransmission( slave );
  if( ret < 0 )
  {
    i2c->endTransmission();
    return ret;
  }
  ret = i2c->write( data );
  i2c->endTransmission();
  return ret;
}

int SOFT_PCA8574::bitSet( int bit )
{
  uint8_t tempUC = 0x01;
  tempUC <<= bit;
  byteData |= tempUC;
  return bitSetReset( byteData );
}

int SOFT_PCA8574::bitReset( int bit )
{
  uint8_t tempUC = 0x01;
  tempUC <<= bit;
  byteData &= ~tempUC;
  return bitSetReset( byteData );
}

int SOFT_PCA8574::toggle( int bit )
{
  uint8_t tempUC = 0x01;
  tempUC <<= bit;
  byteData ^= tempUC;
  return bitSetReset( byteData );
}
