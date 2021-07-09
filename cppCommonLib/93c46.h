/* ----------------------------------------
  SERIAL EEPROM 93C46 or 93C56 utilities
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

#ifndef __EEPROM_93C46_H__
#define __EEPROM_93C46_H__

#include  <GPIO.h>
#include  <derivertive.h>

extern "C"
{
  #include  <mul_tsk.h>
  #include  <system.h>
}

/* akizuki(TRISTATE) vender code : 00:02:CB */

#define  AT93C46_READ  0x80   /*read data bytes*/
#define  AT93C46_EWEN  0x30   /*Write enable*/
#define  AT93C46_ERASE 0xc0   /*erase*/
#define  AT93C46_WRITE 0x40   /*write*/
#define  AT93C46_ERAL  0x20   /*erase all*/
#define  AT93C46_WRAL  0x10   /*write all memory location*/
#define  AT93C46_EWDS  0x00   /*diable all programing instruction*/

#define  AT93C46_ADDRESS_HEADER       0   /* fixed data : 0x0004,0x0010 */
#define  AT93C46_SIZE_HEADER          4 /* fixed data : 0x0004,0x0010 */
#define  AT93C46_ADDRESS_MAC          4   /* tristate vender code : 0x00,0x02,0xCB,,, */
#define  AT93C46_SIZE_MAC             6  /* tristate vender code : 0x00,0x02,0xCB,,, */
#define  AT93C46_ADDRESS_ZERO_PADDING 10 /* 8 byte 0 padding data : 0x00,0x00,0x00,,, */
#define  AT93C46_SIZE_ZERO_PADDING    8 /* 8 byte 0 padding data : 0x00,0x00,0x00,,, */
/* read and write area. */
#define  AT93C46_ADDRESS_READ_WRITE   \
	(AT93C46_SIZE_HEADER + AT93C46_SIZE_MAC + AT93C46_SIZE_ZERO_PADDING)
#define  AT93C46_SIZE_READ_WRITE      (128 - AT93C46_ADDRESS_READ_WRITE)

class EEPROM_93C46 : public GPIO
{
private:
  int csPin,skPin,diPin,doPin;
  void write( uint8_t data );
  uint16_t read();

public:
  EEPROM_93C46() {}
  EEPROM_93C46( int _cs, int _sk, int _di, int _do ) { begin( _cs, _sk, _di, _do ); }
  ~EEPROM_93C46();

  void begin( int _cs, int _sk, int _di, int _do );
  uint16_t read( uint8_t adr );
};

#endif  /* __EEPROM_93C46_H__ */
