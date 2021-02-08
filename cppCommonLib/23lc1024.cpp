/* ----------------------------------------
  ERIAL SRAM 23LC1024 or 23LC512 utilities
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
#include  "23lc1024.h"


/* ----------------------------------------
    constructor destructor
---------------------------------------- */
SRAM_23LC1024::SRAM_23LC1024()
{
}


SRAM_23LC1024::~SRAM_23LC1024()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void SRAM_23LC1024::begin( SPI *_spi, uint8_t _cs, uint32_t sz )
{
  spi = _spi;
  csPin = _cs;
  byteSize = sz;
  GPIOx = whatGPIOType( csPin );
  csPin = whatPin( csPin );
//  digitalWrite( csPin, HIGH );
  pinMode( _cs, OUTPUT, GPIO_SPEED_FAST );
  CS_IS_LOW_1;
}

/* ----------------------------------------
   sram write
---------------------------------------- */
int SRAM_23LC1024::write( const uint8_t *data, int size )
{
  int ret;
  CS_IS_LOW_0;
  for( int i = 0; i < size; i++ )
  {
    ret = spi->readWrite( *data++ );
    if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
  }
  CS_IS_LOW_1;
  return SPI_SUCCESS;
}

int SRAM_23LC1024::write( uint32_t addr, uint8_t data )
{
  uint8_t snd[5];
  snd[0] = SRAM_23LCxxx_MODE_WRITE;
  snd[1] = (uint8_t)(addr >> 16);
  snd[2] = (uint8_t)(addr >> 8);
  snd[3] = (uint8_t)(addr >> 0);
  snd[4] = data;
  return write( (const uint8_t *)snd, sizeof(snd) );
}

int SRAM_23LC1024::write( uint32_t addr, const uint8_t *data, int size )
{
  int ret;
  for( int i = 0; i < size; i++ )
  {
    ret = write( addr, *data );
    if( ret < SPI_SUCCESS ) return ret;
    addr++;
    data++;
  }
  return ret;
}

int SRAM_23LC1024::fill( uint32_t addr, uint8_t data, int size )
{
  int ret;
  for( int i = 0; i < size; i++ )
  {
    ret = write( addr, data );
    if( ret < SPI_SUCCESS ) return ret;
    addr++;
  }
  return ret;
}

/* ----------------------------------------
   sram read
---------------------------------------- */
int SRAM_23LC1024::byteRead( uint32_t addr, uint8_t *data )
{
  int ret;
  uint8_t snd[4];
  snd[0] = SRAM_23LCxxx_MODE_READ;
  snd[1] = (uint8_t)(addr >> 16);
  snd[2] = (uint8_t)(addr >> 8);
  snd[3] = (uint8_t)(addr >> 0);

  CS_IS_LOW_0;
  for( int i = 0; i < (int)sizeof(snd); i++ )
  {
    ret = spi->readWrite( snd[i] );
    if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
  }
  ret = spi->readWrite( 0xFF );
  if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
  *data = (uint8_t)ret;
  CS_IS_LOW_1;

  return SPI_SUCCESS;
}

int SRAM_23LC1024::byteRead( uint32_t addr, uint8_t *data, int size )
{
  int ret;
  for( int i = 0; i < size; i++ )
  {
    ret = byteRead( addr, data );
    if( ret < SPI_SUCCESS ) return ret;
    addr++;
    data++;
  }
  return ret;
}

/* ----------------------------------------
   sram  sequential read
---------------------------------------- */
int SRAM_23LC1024::sequRead( uint32_t addr, uint8_t *data, int size )
{
  int ret;
  uint8_t snd[4];
  snd[0] = SRAM_23LCxxx_MODE_READ;
  snd[1] = (uint8_t)(addr >> 16);
  snd[2] = (uint8_t)(addr >> 8);
  snd[3] = (uint8_t)(addr >> 0);

  CS_IS_LOW_0;
  for( int i = 0; i < (int)sizeof(snd); i++ )
  {
    ret = spi->readWrite( snd[i] );
    if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
  }
  for( int i = 0; i < size; i++ )
  {
    ret = spi->readWrite( 0xFF );
    if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
    *data++ = (uint8_t)ret;
  }
  CS_IS_LOW_1;
  return size;
}

/* ----------------------------------------
   sram  sequential write
---------------------------------------- */
int SRAM_23LC1024::sequWrite( uint32_t addr, const uint8_t *data, int size )
{
  int ret;
  uint8_t snd[4];
  snd[0] = SRAM_23LCxxx_MODE_WRITE;
  snd[1] = (uint8_t)(addr >> 16);
  snd[2] = (uint8_t)(addr >> 8);
  snd[3] = (uint8_t)(addr >> 0);

  CS_IS_LOW_0;
  for( int i = 0; i < (int)sizeof(snd); i++ )
  {
    ret = spi->readWrite( snd[i] );
    if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
  }
  for( int i = 0; i < size; i++ )
  {
    ret = spi->readWrite( *data++ );
    if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
  }
  CS_IS_LOW_1;
  return size;
}

/* ----------------------------------------
   sram instruction mode
---------------------------------------- */
int SRAM_23LC1024::mode( uint8_t mode, uint8_t reg )
{
  int ret;
  uint8_t snd[2];
  snd[0] = mode;
  snd[1] = reg;
  CS_IS_LOW_0;
  for( int i = 0; i < (int)sizeof(snd); i++ )
  {
    ret = spi->readWrite( snd[i] );
    if( ret < SPI_SUCCESS ) { CS_IS_LOW_1; return ret; }
  }
  CS_IS_LOW_1;
  return SPI_SUCCESS;
}

uint8_t SRAM_23LC1024::mode()
{
  uint8_t reg;
  CS_IS_LOW_0;
  spi->readWrite( SRAM_23LCxxx_MODE_RDMR );
  reg = spi->readWrite( 0xFF );
  CS_IS_LOW_1;
  return reg;
}

