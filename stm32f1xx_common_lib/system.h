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
#ifndef  __SYSTEM_H__
#define  __SYSTEM_H__

#include  <stm32f10x.h>
#include  <string.h>
#include  <math.h>

/*****************************************************/
/* defines                                           */
/*****************************************************/


/*****************************************************/
/* global variables                                  */
/*****************************************************/

/*****************************************************/
/* prototypes                                        */
/*****************************************************/
void wait140ns( void );
void dly1us( void );
void dly2R5us( void );
void dly5us( void );
void dly10us( uint32_t dly_us );
void dly100us( uint32_t dly_us );
void dly1ms( uint32_t dly_ms );

char *dtostrf(double number, signed char width, unsigned char prec, char *s);

#endif  /* __SYSTEM_H__ */
