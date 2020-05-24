/* ----------------------------------------
  US2066 oled display utilities
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

#ifndef  __US2066_H__
#define  __US2066_H__

#include  <Wire.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <string.h>

extern "C"
{
  #include  <system.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  SO1602_LINES     2
#define  SO1602_COLUMN    16
#define  SO2002A_LINES    2
#define  SO2002A_COLUMN   20

#define  SO1602_I2C_ADR       0x3C
#define  SO2002A_DISPLAY_ON   0x04
#define  SO2002A_DISPLAY_OFF  0x00
#define  SO2002A_CURSOR_ON    0x02
#define  SO2002A_CURSOR_OFF   0x00
#define  SO2002A_BLINK_ON     0x01
#define  SO2002A_BLINK_OFF    0x00

#define  SO2002A_ENTRY_MODE_INCREMENT 0x02
#define  SO2002A_ENTRY_MODE_DECREMENT 0x00

#define  SO2002A_FUNCTION_2LINES 0x00
#define  SO2002A_FUNCTION_1LINE  0x08
#define  SO2002A_FUNCTION_SINGLE_HEIGHT  0x00
#define  SO2002A_FUNCTION_DOUBLE_HEIGHT  0x04
#define  SO2002A_FUNCTION_REGISTER_RE    0x02
#define  SO2002A_FUNCTION_REGISTER_IS    0x01
#define  SO2002A_CURSOR_SHIFT            0x08
#define  SO2002A_DISPLAY_SHIFT           0x00
#define  SO2002A_CURSOR_RIGHT            0x04
#define  SO2002A_CURSOR_LEFT             0x00

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class US2066 // : public STM32F_I2C
{
public:
  US2066( STM32F_I2C *i2c, uint8_t adr = SO1602_I2C_ADR, int w = SO1602_COLUMN, int h = SO1602_LINES );
  ~US2066();

  void init();
  int clear();
  int returnHome();
  int entryMode( uint8_t mode );
  int OnOff( uint8_t onoff );
  int functionSet( uint8_t func );
  int shift( uint8_t shft );
  int setCGRam( uint8_t adr );
  int setDDRam( uint8_t adr );
  int locate( int x, int y );
  int contrast( uint8_t cnt );

  int write( char c );
  int write( const uint8_t *str, uint32_t len );
  int write( const char *str );
  int printf( const char *fmt, ... );

  int isBusy();

private:
  STM32F_I2C *wire;
  uint8_t slave;
  int width,height;
  int curX,curY;

  int blkWrite( const uint8_t *data, size_t size );
  int ramRead( uint8_t *data, size_t size );
  int command( uint8_t t_command );
  int data( uint8_t t_data );
};


#endif  /* __US2066_H__ */

