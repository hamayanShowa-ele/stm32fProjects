/* ----------------------------------------
  gpio utilities
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
#include  "spi.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

SPI::SPI()
{
}

SPI::SPI( SPI_TypeDef *spi, uint8_t cs )
{
  begin( spi, cs );
}

SPI::~SPI()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void SPI::begin( SPI_TypeDef *spi, uint8_t cs )
{
  SPIx = spi;
  csPin = cs;
  pinMode( csPin, OUTPUT );
}

void SPI::end()
{
}


/* ----------------------------------------
    transfer
---------------------------------------- */
int SPI::transfer( uint8_t data )
{
  digitalWrite( csPin, LOW );
  digitalWrite( csPin, HIGH );
  return 1;
}

extern "C"
{
/* ----------------------------------------
    SPI1 interrupt handler
---------------------------------------- */
void SPI1_IRQHandler( void )
{
  ITStatus flag;

  /* Examine the interruption factors. */
  flag = SPI_I2S_GetITStatus( SPI1, SPI_I2S_IT_TXE );
  if(flag == SET)  // Transmission Empty
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI1, SPI_I2S_IT_RXNE );
  if(flag == SET)  // full reception
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI1, SPI_I2S_IT_OVR | SPI_IT_MODF );
  if(flag == SET)  // error
  {
    ;
  }
}

/* ----------------------------------------
    SPI2 interrupt handler
---------------------------------------- */
void SPI2_IRQHandler( void )
{
  ITStatus flag;

  /* Examine the interruption factors. */
  flag = SPI_I2S_GetITStatus( SPI2, SPI_I2S_IT_TXE );
  if(flag == SET)  // Transmission Empty
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI2, SPI_I2S_IT_RXNE );
  if(flag == SET)  // full reception
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI2, SPI_I2S_IT_OVR | SPI_IT_MODF );
  if(flag == SET)  // error
  {
    ;
  }
}

}  /* extern "C" */
