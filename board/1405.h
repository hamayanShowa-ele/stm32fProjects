/* ----------------------------------------
 board 1405 utilities.
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

#ifndef  __BOARD_1405_H__
#define  __BOARD_1405_H__

#include  <led.h>
#include  <derivertive.h>
#include  <EXTI.h>
#if defined( ARM_CBUS_LONG )
#include  <1415.h>
#include  <HardwareSerial.h>
#endif  /* defined( ARM_CBUS_LONG ) */
extern "C"
{
  #include  <system.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  ALVC7804_WORD_SIZE  (512 - 1)  /* 512 * 18bits. */

#if defined( ARM_CBUS_LONG )
  #define  IDSW  0
  #define  BD1405_01_IO_ADR  (CBUS_IO_ADR + 0x0200)
  #define  BD1405_02_IO_ADR  (CBUS_IO_ADR + 0x0202)
  #define  BD1405_03_IO_ADR  (CBUS_IO_ADR + 0x0204)
  #define  BD1405_04_IO_ADR  (CBUS_IO_ADR + 0x0206)
  #define  BD1405_05_IO_ADR  (CBUS_IO_ADR + 0x0208)
  #define  BD1405_06_IO_ADR  (CBUS_IO_ADR + 0x020A)
  #define  BD1405_07_IO_ADR  (CBUS_IO_ADR + 0x020C)
#endif  /* defined( ARM_CBUS_LONG ) */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class BOARD_1405 : public LED
{
private:
  volatile uint16_t *ioAddress;

public:
  BOARD_1405() {}
  BOARD_1405( uint16_t *adr ) { begin( adr ); }
  ~BOARD_1405() {}

#if defined( STM32F103RB )
  void gpioInit();
  void fifoWrite( uint16_t data );
  void fifoWrite( uint16_t *data, size_t size );
  uint8_t idSW() { return (IDSW << 1); }
  volatile uint16_t *ioAdr() { return ioAddress; }

  void fifoIncrementWrite( LED *led );
  void knock1415();
#endif /* STM32F103RB */

  void begin( volatile uint16_t *adr ) { ioAddress = adr; }
  void fifoIncrementRead( int portNum, LED *led );
  uint16_t fifoIncrementRead( uint16_t count, int size );
  void fifoDummyRead( int size );

  void knock1405();
};

#endif  /* __BOARD_1405_H__ */

