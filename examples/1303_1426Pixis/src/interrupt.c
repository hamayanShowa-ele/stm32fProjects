/* ----------------------------------------
 board interrupt handlers.
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
#include  <stm32f10x.h>

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    graphic controller TE interrupt handler
    60Hz(17ms period)
---------------------------------------- */
#if 0
void EXTI3_IRQHandler( void )
{
//  EXTI->PR = 0x00000008;  /* release pending interrupt request. */
  EXTI_ClearITPendingBit( EXTI_Line3 );
#if 0
  unsigned short x,y;
  static int wptr;
  reftim++;  /*リフレッシュカウンター*/

  x = TouchPanel_X_Read();
  y = TouchPanel_Y_Read();

  ave_x_tbl[ wptr ] = x;
  ave_y_tbl[ wptr ] = y;

  if( ++wptr >= ave_xy_tbl_size ) wptr = 0;
#endif
}
#endif

