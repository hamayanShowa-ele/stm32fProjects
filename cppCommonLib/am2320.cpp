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
#include "AM2320.h"

extern volatile SYSTIM systim;

/* ----------------------------------------
   CRC16
---------------------------------------- */
static uint16_t CRC16(uint8_t *ptr, uint8_t length)
{
  uint16_t crc = 0xFFFF;
  uint8_t s = 0x00;

  while( length-- )
  {
    crc ^= *ptr++;
    for( s = 0; s < 8; s++ )
    {
      if( (crc & 0x01) != 0 )
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else crc >>= 1;
    }
  }
  return crc;
}


/* ----------------------------------------
   AM2320_asukiaaa
---------------------------------------- */
AM2320::AM2320()
{
}

AM2320::AM2320( STM32F_I2C *_i2c, uint8_t adr )
{
  begin( _i2c, adr );
}

AM2320::~AM2320()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void AM2320::begin( STM32F_I2C *_i2c, uint8_t adr )
{
  i2c = _i2c;
  slave = adr;
}

/* ----------------------------------------
   wakeUp
   result = 0 : success.
   result = 1 : The size of the send data has exceeded the size of the send buffer.
   result = 2 : Slave address sent, but NACK received.
   result = 3 : Data was sent, but a NACK was received.
   result = 4 : Other errors.
---------------------------------------- */
int AM2320::wakeUp()
{
  // Wakeup sensor
  int result = i2c->addressTransmitOnly( slave );
  dly_tsk(1);
  i2c->endTransmission(); // Don't use value of endTransmission on this time because it returns error when the sensor slept.
  // am2320 issue
  // https://github.com/Tamakichi/ttbasic_arduino_stm32/issues/8

  return result;
}

/* ----------------------------------------
    update
---------------------------------------- */
int AM2320::update( float *t, float *h )
{
  int result = i2c->beginTransmission( slave );
  if( result < 0 )
  {
    i2c->endTransmission();
    return result;
  }
  static const uint8_t code[] = { 0x03, 0x00, 0x04 };
//  result = i2c->write( code, sizeof(code) );
  result = i2c->write( code[0] );
  result = i2c->write( code[1] );
  result = i2c->write( code[2] );
  i2c->endTransmission();

  dly_tsk(2); // >1.5ms

  #define  BUFFER_SIZE  (2 + 2 + 2 + 2)
  uint8_t buf[BUFFER_SIZE];
  result = i2c->requestFrom( slave );
  if( result < 0 )
  {
    i2c->endTransmission();
    return result;
  }
  result = i2c->read( buf, BUFFER_SIZE );
  i2c->endTransmission();

  // Fusion Code check
  if( buf[0] != 0x03 )
  {
    return AM2320_ERROR_FUSION_CODE_UNMATCH;
  }

  // CRC check
  uint16_t Rcrc = buf[7] << 8;
  Rcrc += buf[6];
  if( Rcrc != CRC16(buf, 6) ) return AM2320_ERROR_CRC_UNMATCH;

  int tempI = (int)(buf[4] & 0x7F) << 8;
  tempI |= buf[5];
  float tempF = tempI / 10.0f;
  if( buf[4] & 0x80 ) tempF *= (-1);
  *t = tempF;

  tempI = (int)buf[2] << 8;
  tempI |= buf[3];
  *h = tempI / 10.0f;

  return AM2320_SUCCESS;
}


/* ----------------------------------------
    version
---------------------------------------- */
int AM2320::version( uint8_t *ver )
{
  int result = i2c->beginTransmission( slave );
  if( result < 0 )
  {
    i2c->endTransmission();
    return result;
  }
  static const uint8_t code[] = { 0x03, 0x0A, 1 };
  result = i2c->write( code, sizeof(code) );
  i2c->endTransmission();

  dly_tsk(2); // >1.5ms

  #undef   BUFFER_SIZE
  #define  BUFFER_SIZE  (2 + 1 + 2)
  uint8_t buf[BUFFER_SIZE];
  result = i2c->requestFrom( slave );
  if( result < 0 )
  {
    i2c->endTransmission();
    return result;
  }
  result = i2c->read( buf, BUFFER_SIZE );
  i2c->endTransmission();

  // Fusion Code check
  if( buf[0] != 0x03 )
  {
    return AM2320_ERROR_FUSION_CODE_UNMATCH;
  }

  // CRC check
  uint16_t Rcrc = buf[4] << 8;
  Rcrc += buf[3];
  if( Rcrc != CRC16(buf, 6) ) return AM2320_ERROR_CRC_UNMATCH;

  // version number.
  *ver = buf[2];

  return AM2320_SUCCESS;
}



/* ----------------------------------------
    software i2c use
---------------------------------------- */
/* ----------------------------------------
   AM2320_asukiaaa
---------------------------------------- */
SOFT_AM2320::SOFT_AM2320()
{
}

SOFT_AM2320::SOFT_AM2320( STM32F_SOFT_I2C *_i2c, uint8_t adr )
{
  begin( _i2c, adr );
}

SOFT_AM2320::~SOFT_AM2320()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void SOFT_AM2320::begin( STM32F_SOFT_I2C *_i2c, uint8_t adr )
{
  i2c = _i2c;
  slave = adr;
}

/* ----------------------------------------
   wakeUp
   result = 0 : success.
   result = 1 : The size of the send data has exceeded the size of the send buffer.
   result = 2 : Slave address sent, but NACK received.
   result = 3 : Data was sent, but a NACK was received.
   result = 4 : Other errors.
---------------------------------------- */
int SOFT_AM2320::wakeUp()
{
  // Wakeup sensor
  int result = i2c->addressTransmitOnly( slave );
  dly_tsk(1);
  i2c->endTransmission(); // Don't use value of endTransmission on this time because it returns error when the sensor slept.
  // am2320 issue
  // https://github.com/Tamakichi/ttbasic_arduino_stm32/issues/8

  return result;
}

/* ----------------------------------------
    update
---------------------------------------- */
int SOFT_AM2320::update( float *t, float *h )
{
  int result = i2c->beginTransmission( slave );
  if( result < 0 )
  {
    i2c->endTransmission();
    return result;
  }
  static const uint8_t code[] = { 0x03, 0x00, 0x04 };
  result = i2c->write( code, sizeof(code) );
//  result = 0;
//  result |= i2c->write( code[0] );
//  result |= i2c->write( code[1] );
//  result |= i2c->write( code[2] );
  i2c->endTransmission();
  if( result < 0 ) return result;

  dly_tsk(2); // >1.5ms

  #undef  BUFFER_SIZE
  #define  BUFFER_SIZE  (2 + 2 + 2 + 2)
  uint8_t buf[BUFFER_SIZE];
  result = i2c->requestFrom( slave );
  if( result < 0 )
  {
    i2c->endTransmission();
    return result;
  }
  result = i2c->read( buf, BUFFER_SIZE );
  i2c->endTransmission();

  // Fusion Code check
  if( buf[0] != 0x03 )
  {
    return AM2320_ERROR_FUSION_CODE_UNMATCH;
  }

  // CRC check
  uint16_t Rcrc = buf[7] << 8;
  Rcrc += buf[6];
  if( Rcrc != CRC16(buf, 6) ) return AM2320_ERROR_CRC_UNMATCH;

  int tempI = (int)(buf[4] & 0x7F) << 8;
  tempI |= buf[5];
  float tempF = tempI / 10.0f;
  if( buf[4] & 0x80 ) tempF *= (-1);
  *t = tempF;

  tempI = (int)buf[2] << 8;
  tempI |= buf[3];
  *h = tempI / 10.0f;

  return AM2320_SUCCESS;
}
