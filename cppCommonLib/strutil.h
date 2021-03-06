/* ----------------------------------------
  string utilities header

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
#ifndef  __STRUTIL_H__
#define  __STRUTIL_H__

//#include  <HardwareSerial.h>
#include  <USART_UART.h>

extern "C" 
{
  #include  <stdio.h>
  #include  <stdlib.h>
  #include  <string.h>
  #include  <ctype.h>
  #include  <time.h>
  #include  <math.h>
};

/* ----------------------------------------
    defines
---------------------------------------- */
enum THE_WEEK_DAY {SUNDAY,MONDAY,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY,SATURDAY};

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */
char *uint2Ascii( char *dest, unsigned int num );
char *int2Ascii( char *dest, int num );
char *int2Hex( char *dest, unsigned int num );
char *int2Hex08( char *dest, unsigned long num );
char *int2Hex04( char *dest, unsigned short num );
char *int2Hex02( char *dest, unsigned char num );
char *longLong2Ascii( char *dest, long long num );
int  htoi( const char *s );

int  split( char *str , char *argv[], int sz );
int  split( char comp, char *str , char *argv[], int sz );
int  isDigitString( const char *digit );
char *small2large( char *str );

char *fixed_little_number( char *dst, long num, int point );
char *dtostrf(double number, signed char width, unsigned char prec, char *s);
void int2byteBig( char *dst, int d );
void int2byteLittle( char *dst, int d );
void double2byteLittle( char *dst, double d );
void double2byteBig( char *dst, double d );

unsigned short utf8to16( const unsigned char utf8[], int sz );

void compileDate(int *year, int *month, int *day);
void compileTime(int *hour, int *minute, int *second);
void utcDateTime( time_t ut,
  uint16_t *year, uint8_t *month, uint8_t *day,
  uint8_t *hour, uint8_t *minute, uint8_t *second );
void localDateTime( time_t ut,
  uint16_t *year, uint8_t *month, uint8_t *day,
  uint8_t *hour, uint8_t *minute, uint8_t *second );
char *utcDateString( char *dst, time_t ut );
char *localDateString( char *dst, time_t ut );
char *utcTimeString( char *dst, time_t ut );
char *localTimeString( char *dst, time_t ut );
char *utcDateTimeString( char *dst, time_t ut );
char *localDateTimeString( char *dst, time_t ut );
time_t utcDateTimeToUnixtime(
  uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s );
time_t localDateTimeToUnixtime(
  uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s );
int weekDay( time_t ut );
int weekDayFromYMD( uint16_t y, uint8_t mo, uint8_t d );

time_t iso8601ToUnixtime( const char *iso8601 );

int patternMatch( const char *compA, const char *compB );

uint8_t swap( uint8_t c );
uint16_t swap( uint16_t w );
uint32_t swap( uint32_t dw );

#if 0
void dump( const uint8_t *data, uint32_t sz, Serial *s );
void dump( const uint16_t *data, uint32_t sz, Serial *s );
void dump( const uint32_t *data, uint32_t sz, Serial *s );
#else
void dump( const uint8_t *data, uint32_t sz, USART_UART *s );
void dump( const uint16_t *data, uint32_t sz, USART_UART *s );
void dump( const uint32_t *data, uint32_t sz, USART_UART *s );
#endif
float char2Float( const char *str );

#endif  /*__STRUTIL_H__*/
