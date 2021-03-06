/* ----------------------------------------
 stm32f1xx software i2c utility header
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
#ifndef  __SOFT_WIRE_H__
#define  __SOFT_WIRE_H__

#if  defined(STM32F4XX)
#include  <stm32f4xx.h>
#elif  defined(STM32F1xx)
#include  <stm32f10x.h>
#endif
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

#if  defined( __STM32F4xx_H )
#define  SCL_LOW   sclPort->BSRRH = sclPin << 0  /* scl set 0 */
#define  SCL_HIGH  sclPort->BSRRL = sclPin << 0  /* scl set 1 */
#define  SDA_LOW   sdaPort->BSRRH = sdaPin << 0  /* sda set 0 */
#define  SDA_HIGH  sdaPort->BSRRL = sdaPin << 0  /* sda set 1 */
#else
#define  SCL_LOW   sclPort->BSRR = sclPin << 16  /* scl set 0 */
#define  SCL_HIGH  sclPort->BSRR = sclPin << 0   /* scl set 1 */
#define  SDA_LOW   sdaPort->BSRR = sdaPin << 16  /* sda set 0 */
#define  SDA_HIGH  sdaPort->BSRR = sdaPin << 0   /* sda set 1 */
#endif  /* define( __STM32F4xx_H ) */

#define isSDA     (sdaPort->IDR & sdaPin)
#define isSCL     (sclPort->IDR & sclPin)

#define  TIMEOUT  10UL  /* 10ms */

/* ----------------------------------------
    register bit
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class STM32F_SOFT_I2C : public GPIO
{
public:
  STM32F_SOFT_I2C();
  STM32F_SOFT_I2C( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed = 100 * 1000UL );
  ~STM32F_SOFT_I2C();
  int  begin( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed = 100 * 1000UL );
  void end();
  void setClock( uint32_t speed );

  int beginTransmission( uint8_t adr );
  int addressTransmitOnly( uint8_t adr );
  int requestFrom( uint8_t adr );
  void endTransmission();
  int write( uint8_t c );
  int write( const uint8_t *data, size_t size );
  int read( bool ack = false );
  int read( uint8_t *data, size_t size );
  int read( const uint8_t *snd, size_t sndSize, uint8_t *rcv, size_t rcvSize );

  void generalCallReset();
  void softReset();

private:
  I2C_TypeDef *wire;
  GPIO_TypeDef *sdaPort,*sclPort;
  uint16_t sdaPin,sclPin;

  int  pinsAlternate();
  int clockStrech();
  void start();
  void stop();
  bool writeByte( uint8_t data );
  uint8_t readByte( bool ack );
  int  address( uint8_t adr );
};

#endif  /* __SOFT_WIRE_H__ */

