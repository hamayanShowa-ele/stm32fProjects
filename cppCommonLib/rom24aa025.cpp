/* ----------------------------------------
  EEPROM 24aa025 utilities
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
#include  "rom24aa025.h"


/* ----------------------------------------
    constructor destructor
---------------------------------------- */
EEP24AA025::EEP24AA025( STM32F_I2C *_i2c, uint8_t adr )
{
  begin( _i2c, adr );
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void EEP24AA025::begin( STM32F_I2C *_i2c, uint8_t adr )
{
  i2c = _i2c;
  slave = adr;
}

/* ----------------------------------------
   eeprom read
---------------------------------------- */
int EEP24AA025::read( uint8_t wordAdr, uint8_t *data, size_t sz )
{
  int ret;
  /* While the bus is busy */
  if( (ret = i2c->isBusy()) != I2C_SUCCESS )
    return ret;

  /* start condition */
  if( (ret = i2c->startCondition()) != I2C_SUCCESS )
    return ret;

  /* Send module address and behave as receiver */
  if( (ret = i2c->sendAddressAndBehaveAsReceiver( slave )) != I2C_SUCCESS )
    return ret;

  /* Send word address for 24aa025e */
  if( (ret = i2c->sendWordAddress( wordAdr )) != I2C_SUCCESS )
    return ret;

  /* restart condition */
  if( (ret = i2c->restartCondition()) != I2C_SUCCESS )
    return ret;

  /* Send control byte for 24aa025e */
  if( (ret = i2c->sendAddressForRecieve( slave )) != I2C_SUCCESS )
    return ret;

  /* Get data from 24aa025e until data size */
  if( (ret = i2c->getDataUntilSize( data, sz )) < 0 )
    return ret;

  /* Send STOP Condition */
  i2c->stopCondition();

  return ret;
}
