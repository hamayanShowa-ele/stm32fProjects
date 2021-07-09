/* ----------------------------------------
 stm32l0xx defines for system.
  for STMicroelectronics HAL library

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

#include  <stm32l0xx.h>
#include  <string.h>
#include  <math.h>
#include  <mul_tsk.h>

/*****************************************************/
/* defines                                           */
/*****************************************************/


/*****************************************************/
/* global variables                                  */
/*****************************************************/

/*****************************************************/
/* prototypes                                        */
/*****************************************************/
void wait100ns( void );
void dly1us( void );
void dly2R5us( void );
void dly5us( void );
void dly10us( uint32_t dly_us );
void dly100us( uint32_t dly_us );
void dly1ms( uint32_t dly_ms );
uint32_t millis( void );

uint16_t getDevID( void );
//uint16_t getRevID( void );
//void getUniqueID( uint32_t id[] );
//uint16_t getFlashSize( void );
//void dwt_access_enable( void );

void *dataSectionStart( void );
void *dataSectionEnd( void );
uint32_t dataSectionSize( void );

void *bssSectionStart( void );
void *bssSectionEnd( void );
uint32_t bssSectionSize( void );

void *stackStart( void );
void *stackEnd( void );
uint32_t stackSize( void );

void *heapStart( void );
void *heapEnd( void );
uint32_t heapSize( void );
uint32_t availableHeap( void *base );


#define  SYS_TICK()       (SysTick->VAL)
#define  CPU_CLOCK()      (SystemCoreClock)
//#define  CYCLE_COUNTER()  (DWT->CYCCNT)

extern void Error_Handler( void );
void SystemClockMSI_Config( void );
void SystemClockHSE_Config( void );

#endif  /* __SYSTEM_H__ */
