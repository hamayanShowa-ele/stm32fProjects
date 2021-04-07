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

/* ----------------------------------------
    defines
---------------------------------------- */
#define  false  0
#define  true   (~false)
#define  bool  int

/* ----------------------------------------
    global variables
---------------------------------------- */


/* ----------------------------------------
    prototypes
---------------------------------------- */


/* ----------------------------------------
    enable DWT->LAR
---------------------------------------- */

void dwt_access_enable( void )
{
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
//  volatile uint32_t cycle = CYCLE_COUNTER();
}

/* ----------------------------------------
    delay 140ns
---------------------------------------- */
void wait140ns( void )
{
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
  __NOP();  /* 1cpu clock time is about 14ns. */
}

/* ----------------------------------------
    delay 1us
---------------------------------------- */
void dly1us( void )
{
  uint32_t limit = CPU_CLOCK() / 1000000UL;
  uint32_t old = SYS_TICK();
  uint32_t mod = CPU_CLOCK() / 1000UL;
  while( ((old - SYS_TICK()) % mod) < limit ) {}
}

/* ----------------------------------------
    delay 2.5us
---------------------------------------- */
void dly2R5us( void )
{
  uint32_t limit = CPU_CLOCK() / (1000000UL * 10 / 25);
  uint32_t old = SYS_TICK();
  uint32_t mod = CPU_CLOCK() / 1000UL;
  while( ((old - SYS_TICK()) % mod) < limit ) {}
}

/* ----------------------------------------
    delay 5us
---------------------------------------- */
void dly5us( void )
{
  uint32_t limit = CPU_CLOCK() / (1000000UL / 5);
  uint32_t old = SYS_TICK();
  uint32_t mod = CPU_CLOCK() / 1000UL;
  while( ((old - SYS_TICK()) % mod) < limit ) {}
}

/* ----------------------------------------
    delay 10us
---------------------------------------- */
void dly10us( uint32_t dly_us )
{
  if( dly_us == 0 ) return;
  uint32_t limit = CPU_CLOCK() / (1000000UL / (10 * dly_us));
  uint32_t old = SYS_TICK();
  uint32_t mod = CPU_CLOCK() / 1000UL;
  while( ((old - SYS_TICK()) % mod) < limit ) {}
}

/* ----------------------------------------
    delay 100us
---------------------------------------- */
void dly100us( uint32_t dly_us )
{
  if( dly_us == 0 ) return;
  else if( dly_us < 10UL ) dly10us( dly_us * 10UL );
  else { for( ; dly_us > 0; dly_us-- ) dly10us( 10UL ); }
}

/* ----------------------------------------
    delay 1ms
---------------------------------------- */
void dly1ms( uint32_t dly_ms )
{
  if( dly_ms == 0 ) return;
  else if( dly_ms == 1UL ) dly100us( 10UL );
  else dly_tsk( dly_ms );
}

/* ----------------------------------------
    millis
---------------------------------------- */
extern SYSTIM systim;

uint32_t millis( void )
{
  return (uint32_t)systim;
}

/* ----------------------------------------
    get revision ID.
---------------------------------------- */
uint16_t getRevID( void )
{
  DBGMCU_TypeDef dbg = *((DBGMCU_TypeDef *)DBGMCU);
  return (uint16_t)(dbg.IDCODE >> 16);
}

/* ----------------------------------------
    get device ID.
---------------------------------------- */
uint16_t getDevID( void )
{
  DBGMCU_TypeDef dbg = *((DBGMCU_TypeDef *)DBGMCU);
  return (uint16_t)(dbg.IDCODE & 0x0FFF);
}

/* ----------------------------------------
    get unique ID.
---------------------------------------- */
void getUniqueID( uint32_t id[] )
{
#if  defined(STM32F10X_HD) || defined(STM32F10X_MD)
  #define  UNIQUE_ID_BASE_ADDRESS  0x1FFFF7E8
#endif  /* defined(STM32F10X_HD) */
  id[0] =  *((uint32_t *)UNIQUE_ID_BASE_ADDRESS + 0);
  id[1] |= *((uint32_t *)UNIQUE_ID_BASE_ADDRESS + 4);
  id[2] |= *((uint32_t *)UNIQUE_ID_BASE_ADDRESS + 8);
}

/* ----------------------------------------
    get flash size.
    unit is k byte.
---------------------------------------- */
uint16_t getFlashSize( void )
{
#if  defined(STM32F10X_HD) || defined(STM32F10X_MD)
  #define  FLASH_SIZE_BASE_ADDRESS  0x1FFFF7E0
#endif  /* defined(STM32F10X_HD) */
  return *((uint16_t *)FLASH_SIZE_BASE_ADDRESS);
}

/* ----------------------------------------
    get data section start and end and size.
---------------------------------------- */
extern uint32_t __data_start__;
extern uint32_t _edata;

void *dataSectionStart( void )
{
  return &__data_start__;
}

void *dataSectionEnd( void )
{
  return &_edata;
}

uint32_t dataSectionSize( void )
{
  return (uint32_t)dataSectionEnd() - (uint32_t)dataSectionStart();
}

/* ----------------------------------------
    get bss section start and end and size.
---------------------------------------- */
extern uint32_t __bss_start__;
extern uint32_t _ebss;

void *bssSectionStart( void )
{
  return &__bss_start__;
}

void *bssSectionEnd( void )
{
  return &_ebss;
}

uint32_t bssSectionSize( void )
{
  return (uint32_t)bssSectionEnd() - (uint32_t)bssSectionStart();
}

/* ----------------------------------------
    get stack start and end and size.
---------------------------------------- */
extern uint32_t __StackTop;
extern uint32_t __StackLimit;

void *stackStart( void )
{
  return &__StackLimit;
}

void *stackEnd( void )
{
  return &__StackTop;
}

uint32_t stackSize( void )
{
  return (uint32_t)stackEnd() - (uint32_t)stackStart();
}

/* ----------------------------------------
    get heap start and end and size.
---------------------------------------- */
extern uint32_t __HeapLimit;

void *heapStart( void )
{
  return &__HeapLimit;
}

void *heapEnd( void )
{
  return &__StackLimit;
}

uint32_t heapSize( void )
{
  return (uint32_t)heapEnd() - (uint32_t)heapStart();
}

/* ----------------------------------------
    get available heap size.
---------------------------------------- */
uint32_t availableHeap( void *base )
{
  return (uint32_t)heapEnd() - (uint32_t)base;
}
