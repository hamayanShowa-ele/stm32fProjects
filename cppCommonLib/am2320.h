/* ----------------------------------------
  am2320 utilities
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

#ifndef AM2320_H
#define AM2320_H

#include  <Wire.h>
#include  <softWire.h>

extern "C"
{
  #include  <mul_tsk.h>
  #include  <system.h>
}

#define AM2320_DEVICE_ADDRESS (0xB8 >> 1)  // 0xB8

#define AM2320_SUCCESS                             I2C_SUCCESS
#define AM2320_ERROR_WIRE_DATA_TOO_LONG            I2C_ERROR_WIRE_DATA_TOO_LONG
#define AM2320_ERROR_WIRE_RECEIVED_NACK_OF_ADDRESS I2C_ERROR_WIRE_RECEIVED_NACK_OF_ADDRESS
#define AM2320_ERROR_WIRE_RECEIVED_NACK_OF_DATA    I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA
#define AM2320_ERROR_WIRE_OTHER_ERROR              I2C_ERROR_WIRE_OTHER_ERROR
#define AM2320_ERROR_FUSION_CODE_UNMATCH           (-10)
#define AM2320_ERROR_CRC_UNMATCH                   (-11)


class AM2320 // : public hoge
{
public:
  AM2320();
  AM2320( STM32F_I2C *_i2c, uint8_t adr = AM2320_DEVICE_ADDRESS );
  ~AM2320();

  void begin( STM32F_I2C *_i2c, uint8_t adr );
  int  wakeUp();
  int  update( float *t, float *h );
  int  version( uint8_t *ver );

private:
  STM32F_I2C *i2c;
  uint8_t slave;
};


class SOFT_AM2320
{
public:
  SOFT_AM2320();
  SOFT_AM2320( STM32F_SOFT_I2C *_i2c, uint8_t adr = AM2320_DEVICE_ADDRESS );
  ~SOFT_AM2320();

  void begin( STM32F_SOFT_I2C *_i2c, uint8_t adr );
  int  wakeUp();
  int  update( float *t, float *h );

private:
  STM32F_SOFT_I2C *i2c;
  uint8_t slave;
};

#endif  /* AM2320_H */
