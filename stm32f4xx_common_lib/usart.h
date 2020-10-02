/* ----------------------------------------
  STM32F407ZET6 utilities header
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
#ifndef	_USART_H_
#define	_USART_H_

#include  <stm32f4xx.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <stdarg.h>
#include  <ctype.h>
#include  <priority.h>
#include  <derivertive.h>
//#include  <peripheral.h>
#include  <mul_tsk.h>

/* ----------------------------------------
  defines.
---------------------------------------- */
/* ----------------------------------------
  baud rate.
---------------------------------------- */
#define  BAUD_921600  (0x00000270UL / 8)
#define  BAUD_460800  (0x00000270UL / 4)
#define  BAUD_230400  (0x00000270UL / 2)
#define  BAUD_115200  (0x00000270UL)
#define  BAUD_57600   (0x000004E2UL)
#define  BAUD_38400   (0x00000753UL)
#define  BAUD_19200   (0x00000EA6UL)
#define  BAUD_9600    (0x00001D4CUL)

typedef enum
{
  SCI_1 = 0, SCI_2, SCI_3, SCI_4, SCI_5, SCI_6,
} SciNum_t;

typedef struct
{
  unsigned long sr;
  unsigned long dr;
  unsigned long brr;
  unsigned long cr1;
  unsigned long cr2;
  unsigned long cr3;
  unsigned long gtpr;
} SCI_REG;

typedef struct
{
  void *buf; 
  unsigned short wptr,rptr; 
  unsigned short size;
} SCI_BUFFER;

typedef struct
{
  unsigned long FE_cnt;  /* framing error accumulated error counter */
  unsigned long PE_cnt;  /* parity error accumulated error counter */
  unsigned long NE_cnt;  /* noise error accumulated error counter */
  unsigned long ORE_cnt;  /* over run error accumulated error counter */
} SCI_ERROR_COUNT;

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */


/* ----------------------------------------
  prototypes.
---------------------------------------- */
//int SCI_Init( int ch, SCI_BUFFER *rcv, SCI_BUFFER *snd, unsigned long brr );
int  SCI_Init( int ch, SCI_BUFFER *rcv, SCI_BUFFER *snd, SCI_ERROR_COUNT *err, unsigned long brr );
int  SCI_Deinit( int ch );
int  SCI_Putc( int ch, char c );
int  SCI_Puts( int ch, const char *str );
int  SCI_Getc( int ch );
unsigned short RcvSizeOfBuf( int ch );
unsigned short SndSizeOfBuf( int ch );
void SCI_ISR( int ch );
unsigned long SCI_Status( int ch );
char SCI_DirectRead( int ch );
void SCI_Purge( int ch );
int  SCI_printf( int ch, const char *fmt, ... );

int  SCI_Write( int ch, const void *src, size_t size );
char *GetLine( char *buf, int ch, int BufSz );
int  Rcv1Char( int handle );

char *SCI_Gets( char *buf, int ch, int BufSz );
int  SCI_GetsWithTmout( char *buf, int ch, int BufSz, unsigned long tmout );

void GetErrorInfo( int ch, unsigned long *fe, unsigned long *pe, unsigned long *ne, unsigned long *ore );

#endif	/* _USART_H_ */

/* ----------------------------------------
  end of file
  designed by hamayan since 2008/11/06
---------------------------------------- */
