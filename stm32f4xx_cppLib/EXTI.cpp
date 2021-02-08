/* ----------------------------------------
  stm32f1xx Timer utilities
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
#include  "EXTI.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */
extern "C"
{
//  static void defaultHandler( void ) { while(1) ; }
}

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
static void (*cbList[ EXTI_IRQ_NUMBERS ])( void );


/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
---------------------------------------- */

/* ----------------------------------------
    configure interrupt handler
---------------------------------------- */
#if 0
static int extiIRQn( int num )
{
  switch ( num )
  {
    case 0: return EXTI0_IRQn;
    case 1: return EXTI1_IRQn;
    case 2: return EXTI2_IRQn;
    case 3: return EXTI3_IRQn;
    case 4: return EXTI4_IRQn;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9: return EXTI9_5_IRQn;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15: return EXTI15_10_IRQn;
  }
  return (-1);
}
#endif

/* ----------------------------------------
    configure interrupt handler
---------------------------------------- */
void extiConfig(
  uint8_t pin, EXTITrigger_TypeDef trigger, uint8_t pri, uint8_t sub )
{
  if( pin < PA0 || pin >= PORT_END ) return;
  GPIO gpio;
  gpio.pinMode( pin, INPUT );
  gpio.exti( pin, trigger, pri, sub );
}

/* ----------------------------------------
    register call back routine
    and unregister call back routine.
---------------------------------------- */
void extiCallBack( int num, void(*cb)(void) )
{
  if( num >= EXTI_IRQ_NUMBERS ) return;
  cbList[ num ] = cb;
}

void extiRejectCallBack( int name )
{
  if( name >= EXTI_IRQ_NUMBERS ) return;
  cbList[ name ] = nullptr;
}


/* ----------------------------------------
    interrupt handlers
---------------------------------------- */
extern "C"
{
void EXTI0_IRQHandler( void ){ EXTI_ClearITPendingBit( EXTI_Line0 ); if( cbList[0] != 0 ) (*cbList[0])(); }
void EXTI1_IRQHandler( void ){ EXTI_ClearITPendingBit( EXTI_Line1 ); if( cbList[1] != 0 ) (*cbList[1])(); }
void EXTI2_IRQHandler( void ){ EXTI_ClearITPendingBit( EXTI_Line2 ); if( cbList[2] != 0 ) (*cbList[2])(); }
void EXTI3_IRQHandler( void ){ EXTI_ClearITPendingBit( EXTI_Line3 ); if( cbList[3] != 0 ) (*cbList[3])(); }
void EXTI4_IRQHandler( void ){ EXTI_ClearITPendingBit( EXTI_Line4 ); if( cbList[4] != 0 ) (*cbList[4])(); }

void EXTI9_5_IRQHandler( void )
{
  if( EXTI_GetITStatus( EXTI_Line5 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line5 );  /* Release of interrupt request factors. */
    if( cbList[5] != 0 ) (*cbList[5])();
  }
  if( EXTI_GetITStatus( EXTI_Line6 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line6 );  /* Release of interrupt request factors. */
    if( cbList[6] != 0 ) (*cbList[6])();
  }
  if( EXTI_GetITStatus( EXTI_Line7 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line7 );  /* Release of interrupt request factors. */
    if( cbList[7] != 0 ) (*cbList[7])();
  }
  if( EXTI_GetITStatus( EXTI_Line8 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line8 );  /* Release of interrupt request factors. */
    if( cbList[8] != 0 ) (*cbList[8])();
  }
  if( EXTI_GetITStatus( EXTI_Line9 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line9 );  /* Release of interrupt request factors. */
    if( cbList[9] != 0 ) (*cbList[9])();
  }
}

void EXTI15_10_IRQHandler( void )
{
  if( EXTI_GetITStatus( EXTI_Line10 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line10 );  /* Release of interrupt request factors. */
    if( cbList[10] != 0 ) (*cbList[10])();
  }
  if( EXTI_GetITStatus( EXTI_Line11 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line11 );  /* Release of interrupt request factors. */
    if( cbList[11] != 0 ) (*cbList[11])();
  }
  if( EXTI_GetITStatus( EXTI_Line12 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line12 );  /* Release of interrupt request factors. */
    if( cbList[12] != 0 ) (*cbList[12])();
  }
  if( EXTI_GetITStatus( EXTI_Line13 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line13 );  /* Release of interrupt request factors. */
    if( cbList[13] != 0 ) (*cbList[13])();
  }
  if( EXTI_GetITStatus( EXTI_Line14 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line14 );  /* Release of interrupt request factors. */
    if( cbList[14] != 0 ) (*cbList[14])();
  }
  if( EXTI_GetITStatus( EXTI_Line15 ) == SET )  /* XOVR */
  {
    EXTI_ClearITPendingBit( EXTI_Line15 );  /* Release of interrupt request factors. */
    if( cbList[15] != 0 ) (*cbList[15])();
  }
}

}  /* extern "C" */


