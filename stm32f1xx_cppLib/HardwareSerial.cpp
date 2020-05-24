/* ----------------------------------------
  hardware serial utilities
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
#include  <HardwareSerial.h>

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
Serial::Serial()
{
}

Serial::Serial( int sci, uint32_t baud, uint16_t txSize, uint16_t rxSize )
{
  (void)begin( sci, baud, txSize, rxSize );
}

Serial::~Serial()
{
  end();
}


/* ----------------------------------------
    begin and end
---------------------------------------- */
int Serial::begin( int sci, uint32_t baud, uint16_t txSize, uint16_t rxSize )
{
  sciNumber = sci;

#if 0
  txdPinNumber = txdPin;
  rxdPinNumber = rxdPin;
  txdGpiox = whatGPIOType( txdPin );
  rxdGpiox = whatGPIOType( rxdPin );

  txdGpioPin = whatPin( txdPin );
  rxdGpioPin = whatPin( rxdPin );
#endif

//  pinMode( txdPin, OUTPUT, ALTERNATE_PP );
//  pinMode( rxdPin, INPUT, GPIO_SPEED_FAST );

  sndBuffer = new uint8_t[ txSize ];
  txBuffer.buf = sndBuffer;
  txBuffer.rptr = txBuffer.wptr = 0;
  txBuffer.size = txSize;

  rcvBuffer = new uint8_t[ rxSize ];
  rxBuffer.buf = rcvBuffer;
  rxBuffer.rptr = rxBuffer.wptr = 0;
  rxBuffer.size = rxSize;

  errors.FE_cnt = errors.NE_cnt = errors.ORE_cnt = errors.PE_cnt = 0UL;

  return SCI_Init( sciNumber, &rxBuffer, &txBuffer, &errors, baud );
}

void Serial::end()
{
  SCI_Deinit( sciNumber );
  #if 0
  pinMode( txdPinNumber, INPUT_PULLUP );
  pinMode( rxdPinNumber, INPUT_PULLUP );
  #endif

  delete [] sndBuffer;
  delete [] rcvBuffer;
}

/* ----------------------------------------
    print
---------------------------------------- */
int Serial::printf( const char *fmt, ... )
{
  char buf[256];
  va_list args;
  va_start( args, fmt );
  vsnprintf( buf, 256, fmt, args );
  va_end( args );
  int ret = SCI_Puts( sciNumber, (const char *)buf );

  return ret;
}

/* ----------------------------------------
    putc
---------------------------------------- */
int Serial::putc( char c )
{
  return SCI_Putc( sciNumber, c );
}

/* ----------------------------------------
    getc
---------------------------------------- */
int Serial::getc()
{
  return SCI_Getc( sciNumber );
}

/* ----------------------------------------
    puts
---------------------------------------- */
int Serial::puts( const char *str )
{
  return SCI_Puts( sciNumber, str );
}

/* ----------------------------------------
    gets
---------------------------------------- */
char *Serial::gets( char *buf, int BufSz )
{
  return SCI_Gets( buf, sciNumber, BufSz );
}

/* ----------------------------------------
    available for rx
---------------------------------------- */
uint16_t Serial::available()
{
  return RcvSizeOfBuf( sciNumber );
}

/* ----------------------------------------
    available for tx
---------------------------------------- */
uint16_t Serial::txAvailable()
{
  return SndSizeOfBuf( sciNumber );
}

/* ----------------------------------------
    tx buffer purge
---------------------------------------- */
void Serial::purge()
{
  SCI_Purge( sciNumber );
}

/* ----------------------------------------
    write
---------------------------------------- */
int Serial::write( uint8_t c )
{
  return SCI_Putc( sciNumber, c );
}

int Serial::write( const void *src, uint16_t size )
{
  return SCI_Write( sciNumber, src, size );
}

/* ----------------------------------------
    read
---------------------------------------- */
int Serial::read()
{
  return getc();
}

char *Serial::read( char *buf, int BufSz )
{
  return gets( buf, BufSz );
}

int Serial::read( char *buf, int BufSz, uint32_t tmout )
{
  return SCI_GetsWithTmout( buf, sciNumber, BufSz, tmout );
}

/* ----------------------------------------
    read line
---------------------------------------- */
char *Serial::line( char *buf, int BufSz )
{
  return GetLine( buf, sciNumber, BufSz );
}


/* ----------------------------------------
    information for errors
---------------------------------------- */
void Serial::infoErrors( uint32_t *fe, uint32_t *pe, uint32_t *ne, uint32_t *ore )
{
  GetErrorInfo( sciNumber, fe, pe, ne, ore );
}
