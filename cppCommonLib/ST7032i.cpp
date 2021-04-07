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
#include  "ST7032i.h"

extern "C"
{
}

/* ----------------------------------------
    defines
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    ST7032i::write data
---------------------------------------- */
int ST7032i::writeData( uint8_t t_data )
{
  uint8_t dat[2];
  dat[0] = 0x40;  //Co=0,RS=1,another bit=0
  dat[1] = t_data;
  waitSema();
  int ret = ST7032i_Base::write( wire, slave, dat, sizeof(dat) );
  sigSema();
  return ret;
}

/* ----------------------------------------
    ST7032i::write command
---------------------------------------- */
int ST7032i::writeCommand( uint8_t t_command )
{
  uint8_t cmd[2];
  cmd[0] = 0x00;  //Co=0,RS=0,another bit=0
  cmd[1] = t_command;
  waitSema();
  int ret = ST7032i_Base::write( wire, slave, cmd, sizeof(cmd) );
  sigSema();
  return ret;
}

/* ----------------------------------------
    ST7032i::update
---------------------------------------- */
int ST7032i::update()
{
  int ret;
  for( int y = 0; y < height; y++ )
  {
    locate( 0, y );
    waitSema();
    ret = ST7032i_Base::update( wire, slave, (char *)&msg[ y * width ], width );
    sigSema();
  }
  return ret;
}

/* ----------------------------------------
    ST7032i::initialize ST7032i
---------------------------------------- */
void ST7032i::init()
{
  dly_tsk(40);
  writeCommand( 0x38 );  //function set
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x39 );  //function set
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x14 );  //internal OSC frequency
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x70 );  //contrast set
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x56 );  //power/icon/contrast control
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x6c );  //follower control
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x38 );  //function set
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x0c );  //display on/off control
  dly10us( 3 );  // 27 micro seconds.
  writeCommand( 0x01 );  //clear display
  dly_tsk(2);
}

/* ----------------------------------------
    ST7032i::clear display
---------------------------------------- */
void ST7032i::clear()
{
  writeCommand( 0x01 );  //clear display
  dly_tsk(2);
  curX = curY = 0;
  memset( msg, ' ', width * height );
}


/* ----------------------------------------
    ST7032i::locate
---------------------------------------- */
void ST7032i::locate( int _x, int _y )
{
  int temp = _x + (_y * width);

  if(temp >= width) temp = (temp - width) + 0x40;

  writeCommand( (uint8_t)temp | 0x80 );  //set ddram address
  dly_tsk(2);
}


/* ----------------------------------------
    ST7032i::set contrast
---------------------------------------- */
void ST7032i::contrast( uint8_t cnt )
{
  //Contrast adjustments
  writeCommand( 0x2a );  //RE=1
  writeCommand( 0x79 );  //SD=1
  writeCommand( 0x81 );  //contrast set
  writeCommand( cnt );  //contrast max
  writeCommand( 0x78 );  //SD=0
  writeCommand( 0x28 );  //set character size is normal.
  dly_tsk(100);
}


/* ----------------------------------------
    ST7032i::set X,Y
---------------------------------------- */
void ST7032i::setXY( int _x, int _y )
{
  if( _x >= width ) return;
  if( _y >= height ) return;
  curX = _x;
  curY = _y;
}


/* ----------------------------------------
    ST7032i::write
---------------------------------------- */
void ST7032i::write( char c )
{
  if( c == '\r' )
  {
    curX = 0;
  }
  else if( c == '\n' )
  {
    if( ++curY >= height ) curY = 0;
  }
  else if( isprint( c ) )
  {
    if( curX == 0 ) memset( &msg[ curY * width ], ' ', width );
    msg[ (curY * width) + curX ] = c;
    if( ++curX >= width)
    {
      curX = 0;
      if( ++curY >= height ) curY = 0;
    }
  }
}

void ST7032i::write( const char *str ) { while( *str ) write( *str++ ); }

void ST7032i::write( const char *str, int _x, int _y )
{
  setXY( _x, _y );
  while( *str ) write( *str++ );
}

/* ----------------------------------------
    print
---------------------------------------- */
void ST7032i::printf( const char *fmt, ... )
{
  char buf[64];
  va_list args;
  va_start( args, fmt );
  vsnprintf( buf, sizeof(buf), fmt, args );
  va_end( args );
  write( (const char *)buf );
}

/* --------------------------------------------------
    Created 2021 by hamayan (hamayan@showa-ele.jp)
-------------------------------------------------- */
