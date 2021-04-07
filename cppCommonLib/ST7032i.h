/* ----------------------------------------
  ST7032i lcd display utilities
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

#ifndef __ST7032i_h__
#define __ST7032i_h__

#include  <stdarg.h>
#include  <stdio.h>
#include  <string.h>
#include  <I2C.h>
#include  <gpio.h>

extern "C"
{
  #include  <ctype.h>
  #include  <mul_tsk.h>
  #include  <system.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
//#define  ST7032i_ADR          0x7C
#define  ST7032i_ADR          0x3e
#define  AQM1602A_LINES       2
#define  AQM1602A_COLUMN      16
#define  AQM0802A_LINES       2
#define  AQM0802A_COLUMN      8
#define  AQM0802A_DISPLAY_ON  0x04
#define  AQM0802A_DISPLAY_OFF 0x00
#define  AQM0802A_CURSOR_ON   0x02
#define  AQM0802A_CURSOR_OFF  0x00
#define  AQM0802A_BLINK_ON    0x01
#define  AQM0802A_BLINK_OFF   0x00


/* ----------------------------------------
    prototypes 
---------------------------------------- */
#define  waitSema()
#define  sigSema()


/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class ST7032i_Base
{
private:
public:
  int write( STM32F_I2C *wire, uint8_t addr, const uint8_t *dat, int len )
  {
    int ret;
    if( (ret = wire->beginTransmission( addr )) < I2C_SUCCESS ) return ret;
    ret = wire->write( dat, len );
    wire->endTransmission();
    return ret;
  }

  int update( STM32F_I2C *wire, uint8_t addr, const char *str, int len )
  {
    int ret;
    if( (ret = wire->beginTransmission( addr )) < I2C_SUCCESS ) return ret;
    if( (ret = wire->write( 0x40 )) < I2C_SUCCESS ) return ret;
    if( (ret = wire->write( (const uint8_t *)str, len )) < I2C_SUCCESS ) return ret;
    wire->endTransmission();
    return ret;
  }
};

class ST7032i : public ST7032i_Base,GPIO
{
private:
  STM32F_I2C *wire;
  uint8_t slave;
  char *msg;
  int width,height;
  int curX,curY;
  int rstPin;

//  int row,col;
//  int x,y;
//  uint8_t addr;

  int writeCommand( uint8_t t_command );
  int writeData( uint8_t t_data );
  void locate( int _x, int _y );

public:
  /*
    constructor and destructor.
  */
  ST7032i() {}
  ST7032i(
    STM32F_I2C *i2c,
    int rst,
    uint8_t adr = ST7032i_ADR, int w = AQM1602A_COLUMN, int h = AQM1602A_LINES )
  {
    begin( i2c, rst, adr, w, h );
  }

  ~ST7032i() { end(); }

  void begin(
    STM32F_I2C *i2c,
    int rst,
    uint8_t adr = ST7032i_ADR, int w = AQM1602A_COLUMN, int h = AQM1602A_LINES )
  {
    wire = i2c;
    rstPin = rst;
    slave = adr;
    width = w;
    height = h;
    msg = new char[ width * height ];
    pinMode( rstPin, OUTPUT );
    reset();
  }
  void end() { if( msg != nullptr ) delete [] msg; }
  void reset() { digitalWrite( rstPin, LOW ); dly_tsk( 2UL ); digitalWrite( rstPin, HIGH ); }

  void init();
  void clear();
  void OnOff( uint8_t onoff ) { writeCommand( onoff | 0x08 ); dly_tsk(2); }  //display on/off,cursor on/off,blink on/off
  void contrast( uint8_t cnt );
  int  update();
  void write( char c );
  void write( const char *str );
  void write( const char *str, int _x, int _y );
  void printf( const char *fmt, ... );

  void setXY( int _x, int _y );
};

#endif  /* __ST7032i_H__ */

/* --------------------------------------------------
    Created 2021 by hamayan (hamayan@showa-ele.jp)
-------------------------------------------------- */
