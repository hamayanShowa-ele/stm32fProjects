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

#ifndef __23LC1024_H__
#define __23LC1024_H__

#include  <SPI.h>

extern "C"
{
  #include  <mul_tsk.h>
  #include  <system.h>
}

#define  SRAM_23LCxxx_SIZE  131072UL
//#define  SRAM_23LCxxx_SIZE  (131072UL / 2)
#define  SRAM_23LCxxx_MODE_READ   0x03
#define  SRAM_23LCxxx_MODE_WRITE  0x02
#define  SRAM_23LCxxx_MODE_EDIO   0x3B
#define  SRAM_23LCxxx_MODE_RSTIO  0xFF
#define  SRAM_23LCxxx_MODE_RDMR   0x05
#define  SRAM_23LCxxx_MODE_WRMR   0x01

#define  SRAM_BYTE_MODE   0x00  /* bit7 = 0, bit6 = 0 */
#define  SRAM_PAGE_MODE   0x80  /* bit7 = 1, bit6 = 0 */
#define  SRAM_SEQUENTIAL_MODE 0x40  /* bit7 = 0, bit6 = 1 */

#define  CS_IS_LOW_0  GPIOx->BSRR = (csPin << 16)
#define  CS_IS_LOW_1  GPIOx->BSRR = (csPin << 0)

class SRAM_23LC1024 : public GPIO
{
private:
  SPI *spi;
  GPIO_TypeDef *GPIOx;
  uint16_t csPin;
  uint32_t byteSize;

  int  write( const uint8_t *data, int size );

public:
  SRAM_23LC1024();
  ~SRAM_23LC1024();

  void begin( SPI *_spi, uint8_t cs, uint32_t sz = SRAM_23LCxxx_SIZE );
  int  mode( uint8_t mode, uint8_t reg );
  int  write( uint32_t addr, uint8_t data );
  int  write( uint32_t addr, const uint8_t *data, int size );
  int  read( uint32_t addr, uint8_t *data );
  int  read( uint32_t addr, uint8_t *data, int size );
  int  fill( uint32_t addr, uint8_t data, int size );
};

#endif  /* __23LC1024_H__ */
