/* ----------------------------------------
 stm32f1xx defines for system.
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
#include  "system.h"

/*****************************************************/
/* defines                                           */
/*****************************************************/
#define  false  0
#define  true   (~false)
#define  bool  int

/*****************************************************/
/* global variables                                  */
/*****************************************************/


/*****************************************************/
/* prototypes                                        */
/*****************************************************/


/*****************************************************/
/* delay 140ns                                       */
/*****************************************************/
void wait140ns( void )
{
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
  __NOP();  /*1cpu clock時間は約14ns*/
}


/*****************************************************/
/* delay 1us                                        */
/*****************************************************/
void dly1us( void )
{
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();
}

/*****************************************************/
/* delay 2.5us                                        */
/*****************************************************/
void dly2R5us( void )
{
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();

  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();

  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();
  wait140ns();

  wait140ns();
  wait140ns();
}

/*****************************************************/
/* delay 5us                                        */
/*****************************************************/
void dly5us( void )
{
#if 0
  uint32_t dly_us = SystemCoreClock / (1000000UL / 5UL);  /* 5us count. */
  volatile uint32_t now = SysTick->VAL;
  volatile uint32_t end = (now - dly_us) % (SystemCoreClock / 1000UL);  /* SysTick->VAL is down counter. */
  if( now > end )
  {
    while( SysTick->VAL > end ) {}
  }
  else
  {
    while( SysTick->VAL <= end ) {}
    while( SysTick->VAL > end ) {}
  }
#else
  dly1us();
  dly1us();
  dly1us();
  dly1us();
  dly1us();
#endif
}

/*****************************************************/
/* delay 10us                                        */
/*****************************************************/
void dly10us( uint32_t dly_us )
{
  dly_us *= SystemCoreClock / (1000000UL / 10UL);  /* 10us count. */
  volatile uint32_t now = SysTick->VAL;
  volatile uint32_t end = (now - dly_us) % (SystemCoreClock / 1000UL);  /* SysTick->VAL is down counter. */
  if( now > end )
  {
    while( SysTick->VAL > end ) {}
  }
  else
  {
    while( SysTick->VAL <= end ) {}
    while( SysTick->VAL > end ) {}
  }
}


/*****************************************************/
/* delay 100us                                       */
/*****************************************************/
void dly100us( uint32_t dly_us )
{
  dly_us *= SystemCoreClock / (1000000UL / 100UL);  /* 100us count. */
  volatile uint32_t now = SysTick->VAL;
  volatile uint32_t end = (now - dly_us) % (SystemCoreClock / 1000UL);  /* SysTick->VAL is down counter. */
  if( now > end )
  {
    while( SysTick->VAL > end ) {}
  }
  else
  {
    while( SysTick->VAL <= end ) {}
    while( SysTick->VAL > end ) {}
  }
}


/*****************************************************/
/* delay 1ms                                         */
/*****************************************************/
void dly1ms( uint32_t dly_ms )
{
  for( ; dly_ms > 0; dly_ms-- )
  {
    dly100us( 10UL );
  }
}

/*****************************************************/
/* dtostrf                                           */
/*****************************************************/
char *dtostrf(double number, signed char width, unsigned char prec, char *s)
{
  bool negative = false;
  if( isnan(number) )
  {
    strcpy(s, "nan");
    return s;
  }
  if( isinf(number) )
  {
    strcpy(s, "inf");
    return s;
  }

  char *out = s;
  int fillme = width; // how many cells to fill for the integer part
  if( prec > 0 ) fillme -= (prec+1);

  // Handle negative numbers
  if( number < 0.0 )
  {
    negative = true;
    fillme--;
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  // I optimized out most of the divisions
  double rounding = 2.0;
  for( uint8_t i = 0; i < prec; ++i )
  {
    rounding *= 10.0;
  }
  rounding = 1.0 / rounding;
  number += rounding;

  // Figure out how big our number really is
  double tenpow = 1.0;
  int digitcount = 1;
  while (number >= 10.0 * tenpow)
  {
    tenpow *= 10.0;
    digitcount++;
  }

  number /= tenpow;
  fillme -= digitcount;

  // Pad unused cells with spaces
  while( fillme-- > 0 ) *out++ = ' ';

  // Handle negative sign
  if( negative ) *out++ = '-';

  // Print the digits, and if necessary, the decimal point
  digitcount += prec;
  int8_t digit = 0;
  while( digitcount-- > 0 )
  {
    digit = (int8_t)number;
    if (digit > 9) digit = 9; // insurance
    *out++ = (char)('0' | digit);
    if( (digitcount == prec) && (prec > 0) )
    {
      *out++ = '.';
    }
    number -= digit;
    number *= 10.0;
  }

  // make sure the string is terminated
  *out = 0;
  return s;
}
