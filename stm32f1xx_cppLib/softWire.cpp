/* ----------------------------------------
  stm32f1xx software i2c utilities code
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
#include  "softWire.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern SYSTIM systim;

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
STM32F_SOFT_I2C::STM32F_SOFT_I2C()
{
}

STM32F_SOFT_I2C::STM32F_SOFT_I2C( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed )
{
  (void)begin( i2c, sda, scl, speed );
}

STM32F_SOFT_I2C::~STM32F_SOFT_I2C()
{
  end();
}

/* ----------------------------------------
    pins alternate
---------------------------------------- */
int STM32F_SOFT_I2C::pinsAlternate()
{
  /* gpio set. */
  pinMode( sdaPort, sdaPin, OUTPUT, GPIO_SPEED_FAST );
  pinMode( sclPort, sclPin, OUTPUT, GPIO_SPEED_FAST );

  /* start condition. */
  SDA_HIGH;  /* sda set 1 */
  SCL_HIGH;  /* scl set 1 */
  dly5us();
  SDA_LOW;   /* sda set 0 */
  dly5us();
  for( int i = 0; i < 10; i++ )
  {
    SCL_LOW;   /* scl set 0 */
    dly5us();
    SCL_HIGH;  /* scl set 1 */
    dly5us();
  }
  /* stop condition. */
  SDA_HIGH;  /* sda set 1 */

  /* check bus busy. */
  pinMode( sdaPort, sdaPin, OUTPUT_OPEN_DRAIN, GPIO_SPEED_FAST );
  pinMode( sclPort, sclPin, OUTPUT_OPEN_DRAIN, GPIO_SPEED_FAST );
  dly10us(5UL);
  if( !isSDA || !isSCL ) return I2C_ERROR_WIRE_OTHER_ERROR;


  return I2C_SUCCESS;
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
int STM32F_SOFT_I2C::begin( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed )
{
  /* save scl and sda pin numbers. */
  wire = i2c;
  sdaPin = whatPin( sda );
  sclPin =  whatPin( scl );
  sdaPort = whatGPIOType( sda );
  sclPort = whatGPIOType( scl );

  /* clear i2c bus. */
  if( pinsAlternate() < 0 ) return I2C_ERROR_WIRE_OTHER_ERROR;

  return I2C_SUCCESS;
}

void STM32F_SOFT_I2C::end()
{
  pinMode( sdaPort, sdaPin, INPUT_PULLUP );
  pinMode( sclPort, sclPin, INPUT_PULLUP );
}


/* ----------------------------------------
    set clock
---------------------------------------- */
void STM32F_SOFT_I2C::setClock( uint32_t speed )
{
}

/* ----------------------------------------
    start and stop condition.
---------------------------------------- */
void STM32F_SOFT_I2C::start()
{
  SDA_HIGH;
  SCL_HIGH;
  dly5us();
  SDA_LOW;
  dly5us();
  SCL_LOW;
  dly5us();
}

void STM32F_SOFT_I2C::stop()
{
  SCL_LOW;
  SDA_LOW;
  dly5us();
  SCL_HIGH;
  dly5us();
  SDA_HIGH;
}

/* ----------------------------------------
    byte write.
---------------------------------------- */
bool STM32F_SOFT_I2C::writeByte( uint8_t data )
{
  for( int i = 0; i < 8; i++ )
  {
    SCL_LOW;
    if( data & (0x80 >> i) ) SDA_HIGH;
    else SDA_LOW;
    dly5us();
    SCL_HIGH;
    dly5us();
  }
  SCL_LOW;
  SDA_HIGH;
  dly5us();
  SCL_HIGH;
  dly5us();
  bool ack = ( isSDA ) ? true : false;
  SCL_LOW;

  return ack;
}


/* ----------------------------------------
    byte read.
---------------------------------------- */
uint8_t STM32F_SOFT_I2C::readByte( bool ack )
{
  uint8_t data = 0;

  SDA_HIGH;
  for( int i = 0; i < 8; i++ )
  {
    SCL_LOW;
    dly5us();
    SCL_HIGH;
    dly5us();
    data <<= 1;
    if( isSDA ) data |= 0x01;
  }
  SCL_LOW;
  dly5us();
  if( ack ) SDA_HIGH;
  else SDA_LOW;
  dly5us();
  SCL_HIGH;
  dly5us();
  SCL_LOW;
  SDA_HIGH;

  return data;
}

/* ----------------------------------------
    address.
---------------------------------------- */
int STM32F_SOFT_I2C::clockStrech()
{
  /* While the bus is busy */
  SYSTIM baseTim = systim;
  while( !isSCL )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_BUS_BUSY;
    }
    rot_rdq();
  }
  return I2C_SUCCESS;
}

int STM32F_SOFT_I2C::address( uint8_t adr )
{
  /* While the bus is busy */
  if( clockStrech() != I2C_SUCCESS ) return I2C_BUS_BUSY;
  /* initiate start sequence */
  start();
  //Send address for write
  bool ack = writeByte( adr );

  return ( ack ) ? I2C_ERROR_WIRE_RECEIVED_NACK_OF_ADDRESS : I2C_SUCCESS;
}

/* ----------------------------------------
    begin transmission
---------------------------------------- */
int STM32F_SOFT_I2C::beginTransmission( uint8_t adr )
{
  adr <<= 1;
  adr &= 0xFE;  /* write mode */

  return address( adr );
}

int STM32F_SOFT_I2C::addressTransmitOnly( uint8_t adr )
{
  return beginTransmission( adr );
}

/* ----------------------------------------
    end transmission
---------------------------------------- */
void STM32F_SOFT_I2C::endTransmission()
{
  stop();
}

/* ----------------------------------------
    request from
---------------------------------------- */
int STM32F_SOFT_I2C::requestFrom( uint8_t adr )
{
  adr <<= 1;
  adr |= 0x01;  /* read mode */

  return address( adr );
}

/* ----------------------------------------
    write
---------------------------------------- */
int STM32F_SOFT_I2C::write( uint8_t c )
{
  bool ack = writeByte( c );
  if( ack == true ) return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;

  return 1;
}

int STM32F_SOFT_I2C::write( const uint8_t *data, size_t size )
{
  int count;
  for( count = 0; count < (int)size; count++ )
  {
    bool ack = writeByte( *data++ );
    if( ack == true ) return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;
  }

  return count;
}

/* ----------------------------------------
    read
---------------------------------------- */
int STM32F_SOFT_I2C::read( bool ack )
{
  int c = readByte( ack );
  return c;
}

int STM32F_SOFT_I2C::read( uint8_t *data, size_t size )
{
  int c;
  int sz = size;
  sz--;
  if( sz < 0 ) return 0;
  else if( sz == 0 )
  {
    c = readByte( true );
    if( c < 0 ) return c;
    *data = (uint8_t)c;
    return 1;
  }

  int count;
  for( count = 1; count <= sz; count++ )
  {
    c = readByte( false );
    if( c < 0 ) return c;
    *data++ = (uint8_t)c;
  }
  c = readByte( true );
  if( c < 0 ) return c;
  *data = (uint8_t)c;

  return count;
}

int STM32F_SOFT_I2C::read( const uint8_t *snd, size_t sndSize, uint8_t *rcv, size_t rcvSize )
{
  return 0;
}

/* ----------------------------------------
    i2c general call reset.
---------------------------------------- */
void STM32F_SOFT_I2C::generalCallReset()
{
  /* general call reset. */
  beginTransmission( 0x00 );
  write( 0x06 );
  endTransmission();
}

/* ----------------------------------------
    i2c software reset.
---------------------------------------- */
void STM32F_SOFT_I2C::softReset()
{
}
