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
#include  <gpio.h>

/* ----------------------------------------
    prototypes 
---------------------------------------- */


/* ----------------------------------------
    defines
---------------------------------------- */


/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class SPI  : public GPIO
{
public:
  SPI();
  SPI( SPI_TypeDef *spi, uint8_t cs );
  ~SPI();
  void begin( SPI_TypeDef *spi, uint8_t cs );
  void end();
  int transfer( uint8_t data );

private:
  SPI_TypeDef *SPIx;
  uint8_t csPin;
};


#endif  /* __SPI_H__ */

