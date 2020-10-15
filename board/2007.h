/* ----------------------------------------
 board 2007 sram 8m utilities.
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

#ifndef  __BOARD_2007_H__
#define  __BOARD_2007_H__

#include  <derivertive.h>
//#include  <boardUtility.h>
#include  <HardwareSerial.h>
#include  <led.h>

extern "C"
{
  #include  <stdio.h>
  #include  <stdlib.h>
  #include  <mul_tsk.h>
}

//#define  SRAM_8M_MEM_ADR  ((volatile void *)(CBUS_MEM_ADR + 0x00080000))
//#define  SRAM_8M_IO_ADR   ((volatile uint16_t *)(CBUS_IO_ADR + 0x0200))
#define  BYTE_SIZE_AT_ONE_BANK  (65536 * 2UL)
#define  BANK_COUNT  64

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class SRAM_8M // : public GPIO
{
public:
  SRAM_8M( uint32_t m, uint16_t b )
  {
    memBaseAdr = (volatile void *)(m + CBUS_MEM_ADR);
    bankIOAdr = (volatile uint16_t *)(b + CBUS_IO_ADR);
  }
  ~SRAM_8M() {}

  int sram8m();

private:
  volatile void *memBaseAdr;
  volatile uint16_t *bankIOAdr;

  void bank( uint16_t b ) { *bankIOAdr = b; };
  void randomWrite( uint8_t *adr, int loop, uint32_t seed );
  void randomWrite( uint16_t *adr, int loop, uint32_t seed );
  void randomWrite( uint32_t *adr, int loop, uint32_t seed );
  int  randomCheck( const uint8_t *adr, int loop, uint32_t seed, bool escape = false );
  int  randomCheck( const uint16_t *adr, int loop, uint32_t seed, bool escape = false );
  int  randomCheck( const uint32_t *adr, int loop, uint32_t seed, bool escape = false );
  void ledBusy();
};


#endif  /* __BOARD_2007_H__ */

