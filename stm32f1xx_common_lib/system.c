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
#if 0
  for( ; dly_ms > 0; dly_ms-- )
  {
    dly100us( 10UL );
  }
#else
  dly_tsk( dly_ms );
#endif
}

/*****************************************************/
/* millis                                            */
/*****************************************************/
extern SYSTIM systim;

uint32_t millis( void )
{
  return (uint32_t)systim;
}

/*****************************************************/
/* get revision ID.  */
/*****************************************************/
uint16_t getRevID( void )
{
  DBGMCU_TypeDef dbg = *((DBGMCU_TypeDef *)DBGMCU);
  return (uint16_t)(dbg.IDCODE >> 16);
}

/*****************************************************/
/* get device ID.  */
/*****************************************************/
uint16_t getDevID( void )
{
  DBGMCU_TypeDef dbg = *((DBGMCU_TypeDef *)DBGMCU);
  return (uint16_t)(dbg.IDCODE & 0x0FFF);
}

/*****************************************************/
/* get unique ID.  */
/*****************************************************/
void getUniqueID( uint32_t id[] )
{
#if  defined(STM32F10X_HD)
  #define  UNIQUE_ID_BASE_ADDRESS  0x1FFFF7E8
#endif  /* defined(STM32F10X_HD) */
  id[0] =  *((uint32_t *)UNIQUE_ID_BASE_ADDRESS + 0);
  id[1] |= *((uint32_t *)UNIQUE_ID_BASE_ADDRESS + 4);
  id[2] |= *((uint32_t *)UNIQUE_ID_BASE_ADDRESS + 8);
}
