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

#include  "us2066.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
US2066::US2066( STM32F_I2C *i2c, uint8_t adr, int w, int h )
{
  wire = i2c;
  slave = adr;
  width = w;
  height = h;
  curX = curY = 0;

  dly_tsk( 100UL );
  command( 0x01 );  //clear display
  dly_tsk( 20UL );
  command( 0x02 );  //return home
  dly_tsk( 2UL );
  command( 0x0f );  //send display on command
  dly_tsk( 2UL );
  command( 0x01 );  //clear display
  dly_tsk( 20UL );
}

US2066::~US2066()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */


/* ----------------------------------------
    oled initialize
---------------------------------------- */
void US2066::init()
{
  contrast( 0xff );
  clear();
  OnOff( SO2002A_DISPLAY_ON | SO2002A_CURSOR_OFF | SO2002A_BLINK_OFF );
  write( "US2066 designed by hamayan." );
}


/* ----------------------------------------
    block write
---------------------------------------- */
int US2066::blkWrite( const uint8_t *data, size_t size )
{
  int ret;
  if( (ret = wire->beginTransmission( slave )) < I2C_SUCCESS ) return ret;
  ret = wire->write( data, size );
  wire->endTransmission();
  return ret;
}

/* ----------------------------------------
    block ram read
---------------------------------------- */
int US2066::ramRead( uint8_t *data, size_t size )
{
  int ret;
  if( (ret = wire->beginTransmission( slave )) < I2C_SUCCESS ) return ret;
  ret = wire->write( 0x40 );
  wire->endTransmission();

  if( (ret = wire->beginTransmission( slave )) < I2C_SUCCESS ) return ret;
  ret = wire->read( data, size );
  wire->endTransmission();

  return ret;
}

/* ----------------------------------------
    isBusy ?
---------------------------------------- */
int US2066::isBusy()
{
#if 0
  int ret;
  if( (ret = wire->beginTransmission( slave )) < I2C_SUCCESS ) return ret;
  const uint8_t cmd = 0x00;
  uint8_t rcv;
  ret = wire->read( &cmd, sizeof(cmd), &rcv, sizeof(rcv) );
  wire->endTransmission();
  return rcv;
#else
  int ret;
  if( (ret = wire->beginTransmission( slave )) < I2C_SUCCESS ) return ret;
  ret = wire->write( 0x00 );
  wire->endTransmission();

  uint8_t flg;
  if( (ret = wire->beginTransmission( slave )) < I2C_SUCCESS ) return ret;
  ret = wire->read( &flg, sizeof(flg) );
  wire->endTransmission();

  return flg;
#endif
}

/* ----------------------------------------
    command
---------------------------------------- */
int US2066::command( uint8_t t_command )
{
  uint8_t cmd[2];
  cmd[0] = 0x00;
  cmd[1] = t_command;
  return blkWrite( (const uint8_t *)cmd, sizeof(cmd) );
}

/* ----------------------------------------
    data
---------------------------------------- */
int US2066::data( uint8_t t_data )
{
  uint8_t dat[2];
  dat[0] = 0x40;
  dat[1] = t_data;
  return blkWrite( (const uint8_t *)dat, sizeof(dat) );
}


/* ----------------------------------------
    clear
---------------------------------------- */
int US2066::clear()
{
//  dly_tsk( 100UL );
  int ret = command( 0x01 );  //clear display
  curX = curY = 0;
  dly_tsk( 2UL );
  return ret;
}

/* ----------------------------------------
    return home
---------------------------------------- */
int US2066::returnHome()
{
  int ret = command( 0x02 );  //return home
  curX = curY = 0;
  dly_tsk( 2UL );
  return ret;
}

/* ----------------------------------------
    entry mode
---------------------------------------- */
int US2066::entryMode( uint8_t mode )
{
  int ret = command( mode | 0x04 );  //cursor increment / decrement.
  dly10us( 4UL );
  return ret;
}


/* ----------------------------------------
    display on/off
---------------------------------------- */
int US2066::OnOff( uint8_t onoff )
{
  int ret = command( onoff | 0x08 );  //display on/off,cursor on/off,blink on/off
  dly10us( 4UL );
  return ret;
}

/* ----------------------------------------
    function set
---------------------------------------- */
int US2066::functionSet( uint8_t func )
{
  int ret = command( func | 0x20 );  //number of lines,double height font,extension register RE,extension register IS
  dly10us( 4UL );
  return ret;
}


/* ----------------------------------------
    cursor or display shift
---------------------------------------- */
int US2066::shift( uint8_t shft )
{
  int ret = command( shft | 0x10 );  //
  dly10us( 4UL );
  return ret;
}

/* ----------------------------------------
    set CG ram address
---------------------------------------- */
int US2066::setCGRam( uint8_t adr )
{
  adr &= 0x3F;
  int ret = command( adr | 0x40 );  //
  dly10us( 4UL );
  return ret;
}

/* ----------------------------------------
    set DD ram address
---------------------------------------- */
int US2066::setDDRam( uint8_t adr )
{
  adr &= 0x7F;
  int ret = command( adr | 0x80 );  //
  dly10us( 4UL );
  return ret;
}


/* ----------------------------------------
    locate
---------------------------------------- */
int US2066::locate( int x, int y )
{
  int temp = x + (y * width);
  if(temp >= width) temp = (temp - width) + 0x20;
  int ret = command( (unsigned char)temp | 0x80 );  //set ddram address
  curX = x; curY = y;
  dly10us( 4UL );
  return ret;
}

/* ----------------------------------------
    contrast
---------------------------------------- */
int US2066::contrast( uint8_t cnt )
{
  //コントラスト調整
  int ret = command( 0x2a );  //RE=1
  command( 0x79 );  //SD=1
  command( 0x81 );  //contrast set
  command( cnt );  //contrast max
  command( 0x78 );  //SD=0
  command( 0x28 );  //set character size is normal.
  dly_tsk( 100UL );
  return ret;
}

/* ----------------------------------------
    write strings
---------------------------------------- */
int US2066::write( char c )
{
  uint8_t dat[2];

  dat[0] = 0x40;
  dat[1] = c;
  return blkWrite( (const uint8_t *)dat, 2 );
}

int US2066::write( const uint8_t *str, uint32_t len )
{
  uint8_t dat[1 * width + 1];

  dat[0] = 0x40;
  len = (len > sizeof(dat) - 1) ? sizeof(dat) - 1 : len;
  memcpy( &dat[1], str, len );

  return blkWrite( (const uint8_t *)dat, len + 1 );
}

int US2066::write( const char *str )
{
#if 0
  char buf[ height ][ width ];
  int len = strlen( str );
  len = ( len > (width * height) ) ? width * height : len;

  locate( 0, 0 );
  int ret;
  if( len <= width )
  {
    memset( buf[0], ' ', width  );
    memcpy( buf[0], str, len  );
    ret = write( (const uint8_t *)buf[0], width );
  }
  else if( len > width )
  {
    memcpy( buf[0], str, width  );
    ret = write( (const uint8_t *)buf[0], width );

    locate( 0, 1 );
    len -= width;
    memset( buf[1], ' ', width  );
    memcpy( buf[1], str + width, len );
    ret = write( (const uint8_t *)buf[1], width );
  }

  return ret;
#else
  int ret;
  while( *str )
  {
    if( *str == '\r' )
    {
      locate( 0, curY );
    }
    else if( *str == '\n' )
    {
      if( ++curY >= height ) curY = 0;
      locate( curX, curY );
    }
    else
    {
      ret = write( *str );
      if( ++curX >= width )
      {
        curX = 0;
        if( ++curY >= height ) curY = 0;
        locate( curX, curY );
      }
    }
    str++;
  }
  locate( curX, curY );
  return ret;
#endif
}

/* ----------------------------------------
    print
---------------------------------------- */
int US2066::printf( const char *fmt, ... )
{
  char buf[64];
  va_list args;
  va_start( args, fmt );
  vsnprintf( buf, 256, fmt, args );
  va_end( args );
  int ret = write( (const char *)buf );

  return ret;
}

