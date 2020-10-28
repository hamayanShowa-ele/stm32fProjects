/* ----------------------------------------
 SPI utility header
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
#ifndef  __SPI_H__
#define  __SPI_H__

#include  <stm32fPeripheral.h>
#include  <stdio.h>
#include  <gpio.h>

extern "C"
{
  #include <mul_tsk.h>
  uint8_t spi_rw_SPI1( uint8_t out );
  uint8_t spi_rw_SPI2( uint8_t out );
  uint8_t spi_rw_SPI3( uint8_t out );
  void waiSema_SPI1( void );
  void waiSema_SPI2( void );
  void waiSema_SPI3( void );
  void sigSema_SPI1( void );
  void sigSema_SPI2( void );
  void sigSema_SPI3( void );
}

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    defines
---------------------------------------- */
#define  SPI_SUCCESS         (0)
#define  SPI_BUS_BUSY        (-1)
#define  SPI_SEMAPHORE_BUSY  (-2)
#define  SPI_RECIEVE_TIMEOUT (-3)

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class SPI : public GPIO
{
public:
  SPI();
  SPI( SPI_TypeDef *spi, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, ID id = 0 );
  ~SPI();
  void begin( SPI_TypeDef *spi, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, ID id = 0 );
  void end();
  int  waiSema();
  void sigSema();
  int  isBusy();
  int  isEmpty();
  int  transmit( uint8_t data );
  int  transmit( const uint8_t *data, size_t sz );
  int  recieve( uint8_t *data );
  int  recieve( uint8_t *data, size_t sz );

  int  write( uint8_t csPin, uint8_t data );
  int  write( uint8_t csPin, const uint8_t *data, size_t sz );
  int  write( uint8_t csPin, uint16_t data );
  int  read( uint8_t csPin );
  int  read( uint8_t csPin, uint8_t *data, size_t sz );
  uint16_t readUS( uint8_t csPin );

  SPI_TypeDef *whatTypeDef() { return SPIx; }

private:
  SPI_TypeDef *SPIx;
  uint8_t sck,miso,mosi;
  ID semaID;
  uint32_t rcc;
};


#endif  /* __SPI_H__ */

