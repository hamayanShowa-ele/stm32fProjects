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

#ifndef __24AA025_H__
#define __24AA025_H__

#include  <I2C.h>
//#include  <softWire.h>

extern "C"
{
  #include  <mul_tsk.h>
  #include  <system.h>
}

#define  I2C_ADR_24AA025E48         0x50  /* 24AA025E48 mac address i2c eeprom */
#define  MAC_ADDRESS_IN_24AA025E48  0xFA  /* 00:1E:C0 or 80:1F:12 or 00:04:A3 */
                                          /* is Microchip Technology Inc. vender code. */

class EEP24AA025 //: public STM32F_I2C
{
private:
  STM32F_I2C *i2c;
  uint8_t slave;

public:
  EEP24AA025() {}
  EEP24AA025( STM32F_I2C *_i2c, uint8_t adr = I2C_ADR_24AA025E48 );
  ~EEP24AA025() {}

  void begin( STM32F_I2C *_i2c, uint8_t adr = I2C_ADR_24AA025E48 );
  int  read( uint8_t wordAdr, uint8_t *data, size_t sz );
};

#endif  /* __24AA025_H__ */
