/* ----------------------------------------
 board 1804 utilities.
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

#ifndef  __BOARD_1804_H__
#define  __BOARD_1804_H__

//#include  <HardwareSerial.h>
#include  <strutil.h>
#include  <led.h>
#include  <FSMC.h>
#include  <Timer.h>
#include  <math.h>

extern "C"
{
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <stdlib.h>
}


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    defines
---------------------------------------- */
#define  DPRAM_BASE_ADDRESS  (0x60000000UL)
#define  DPRAM_SIZE          (8192)  // 4k * 16bit
#define  DPRAM_INTR_ADDRESS  (DPRAM_BASE_ADDRESS + (0x0FFF * 2))
#define  DPRAM_INTL_ADDRESS  (DPRAM_BASE_ADDRESS + (0x0FFE * 2))
#define  GANYMEDE_DUMMY_ADDRESS  (0x6C000000UL)

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class BOARD_1804 : public GPIO
{
private:
  void busPort();
  void dpRamDividWrite( uint16_t *ram, uint16_t data );
  uint16_t dpRamDividRead( const uint16_t *ram );

public:
  BOARD_1804(){}
  ~BOARD_1804(){}

  void gpioInit();
  void extBus();
  void v50INT();
  void dpRamRandomWrite( void *ram, size_t size, uint32_t seed, LED *led );  /* random */
  void dpRamFixedWrite( void *ram, size_t size, uint16_t fixed, LED *led );  /* fixed */
  void dpRamIncrementWrite( void *ram, size_t size, LED *led );  /* increment */
  void dpRamSineWrite( void *ram, int scale, uint32_t ms, LED *led );  /* sine */
  void dpRamRead( const void *ram, size_t size, uint32_t seed, LED *led );
  void fclk( uint32_t freq );
  void convert( uint32_t freq );
};


class ADC_1804
{
private:
  volatile uint16_t *adcAddress;

public:
  ADC_1804() {}
  ADC_1804( volatile uint16_t *a, uint16_t mask = 0x00FF ) { begin( a, mask ); }
  ~ADC_1804() {}

  void begin( volatile uint16_t *a, uint16_t mask = 0x00FF );
  uint16_t read();
};


#endif  /* __BOARD_1804_H__ */

