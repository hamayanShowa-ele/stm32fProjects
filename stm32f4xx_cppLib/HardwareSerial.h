/* ----------------------------------------
 hardware serial header
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
#ifndef  __HARDWARE_SERIAL_H__
#define  __HARDWARE_SERIAL_H__

#include  <stdarg.h>
#include  <stdio.h>
#include  <string.h>
#include  <gpio.h>

extern "C"
{
  #include  <usart.h>
}

/* ----------------------------------------
    prototypes 
---------------------------------------- */



/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    class
---------------------------------------- */
class Serial : public GPIO
{
public:
  Serial();
  Serial( int sci, uint32_t baud = BAUD_115200, uint16_t txSize = 256, uint16_t rxSize = 256 );
  ~Serial();

  int begin( int sci, uint32_t baud = BAUD_115200, uint16_t txSize = 256, uint16_t rxSize = 256 );
  int begin( int sci, uint32_t baud, uint8_t *txBuf, uint8_t *rxBuf, size_t txSize, size_t rxSize );
  void end();

  int printf( const char *fmt, ... );
  int puts( const char *str );
  uint16_t available();
  uint16_t txAvailable();
  void purge();
  int write( uint8_t c );
  int write( const void *src, uint16_t size );
  int read();
  char *read( char *buf, int BufSz );
  int read( char *buf, int BufSz, uint32_t tmout );
  char *line( char *buf, int BufSz );
  void infoErrors( uint32_t *fe, uint32_t *pe, uint32_t *ne, uint32_t *ore );


private:
  int sciNumber;
#if 0
  int txdPinNumber;
  int rxdPinNumber;
  GPIO_TypeDef* txdGpiox;
  GPIO_TypeDef* rxdGpiox;
  uint16_t txdGpioPin;
  uint16_t rxdGpioPin;
#endif

  uint8_t *sndBuffer;
  uint8_t *rcvBuffer;
  SCI_BUFFER txBuffer, rxBuffer;
  SCI_ERROR_COUNT errors;

  int putc( char c );
  int getc();
  char *gets( char *buf, int BufSz );
};


#endif  /* __HARDWARE_SERIAL_H__ */

