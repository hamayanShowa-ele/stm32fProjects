/* ----------------------------------------
 stm32f1xx and stm32f4xx i2c utility header
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
#ifndef  __WIRE_H__
#define  __WIRE_H__

#if  defined( STM32F4XX )
#include  <stm32f4xx.h>
#elif  defined(STM32F1xx)
#include  <stm32f10x.h>
#endif  /* defined( STM32F4XX ) */

#include  <stdio.h>
#include  <gpio.h>
extern "C"
{
  #include  <mul_tsk.h>
  #include  <system.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define I2C_SUCCESS                             (0)
#define I2C_ERROR_WIRE_DATA_TOO_LONG            (-1)
#define I2C_ERROR_WIRE_RECEIVED_NACK_OF_ADDRESS (-2)
#define I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA    (-3)
#define I2C_ERROR_WIRE_OTHER_ERROR              (-4)
#define I2C_BUS_BUSY                            (-5)

/* ----------------------------------------
    register bit
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class STM32F_I2C : public GPIO
{
public:
  STM32F_I2C();
  STM32F_I2C( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed = 100 * 1000UL );
  ~STM32F_I2C();
  int  begin( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed = 100 * 1000UL );
  void end();
  void setClock( uint32_t speed );

  int beginTransmission( uint8_t adr );
  int addressTransmitOnly( uint8_t adr );
  int requestFrom( uint8_t adr );
  void endTransmission();
  int write( uint8_t c );
  int write( const uint8_t *data, size_t size );
  int read( bool ack );
  int read( uint8_t *data, size_t size );
  int read( const uint8_t *snd, size_t sndSize, uint8_t *rcv, size_t rcvSize );

  void generalCallReset();
  void softReset();

private:
  I2C_TypeDef *wire;
  GPIO_TypeDef *sdaPort,*sclPort;
  uint16_t sdaPin,sclPin;

  int pinsAlternate();
};


#endif  /* __WIRE_H__ */

