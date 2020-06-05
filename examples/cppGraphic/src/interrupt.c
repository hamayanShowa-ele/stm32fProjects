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

/* ----------------------------------------
    DMA1 channel 1 interrupt handler.
---------------------------------------- */
extern volatile uint8_t dma1ch1HT_Update;  /* half transmission complete */
extern volatile uint8_t dma1ch1TC_Update;  /* full transmission complete */
extern volatile uint8_t dma1ch1TE_Update;  /* transmission error */

void DMA1_Channel1_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA1_IT_GL1 ) == SET )
  {
    if( DMA_GetITStatus( DMA1_IT_HT1 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA1_IT_HT1 );
      dma1ch1HT_Update++;
    }
    if( DMA_GetITStatus( DMA1_IT_TC1 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA1_IT_TC1 );
      dma1ch1TC_Update++;
    }
    if( DMA_GetITStatus( DMA1_IT_TE1 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA1_IT_TE1 );
      dma1ch1TE_Update++;
    }
    DMA_ClearITPendingBit( DMA1_IT_GL1 );
  }
}

/* ----------------------------------------
    DMA2 channel 3 interrupt handler.
---------------------------------------- */
extern volatile uint8_t dma2ch3HT_Update;  /* half transmission complete */
extern volatile uint8_t dma2ch3TC_Update;  /* full transmission complete */
extern volatile uint8_t dma2ch3TE_Update;  /* transmission error */

void DMA2_Channel3_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA2_IT_GL3 ) == SET )
  {
    if( DMA_GetITStatus( DMA2_IT_HT3 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_HT3 );
      dma2ch3HT_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TC3 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TC3 );
      dma2ch3TC_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TE3 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TE3 );
      dma2ch3TE_Update++;
    }
    DMA_ClearITPendingBit( DMA2_IT_GL3 );
  }
}


/* ----------------------------------------
    DMA2 channel 4 and channel 5 interrupt handler.
---------------------------------------- */
extern volatile uint8_t dma2ch4HT_Update;  /* half transmission complete */
extern volatile uint8_t dma2ch4TC_Update;  /* full transmission complete */
extern volatile uint8_t dma2ch4TE_Update;  /* transmission error */
extern volatile uint8_t dma2ch5HT_Update;  /* half transmission complete */
extern volatile uint8_t dma2ch5TC_Update;  /* full transmission complete */
extern volatile uint8_t dma2ch5TE_Update;  /* transmission error */
void DMA2_Channel4_5_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA2_IT_GL4 ) == SET )
  {
    if( DMA_GetITStatus( DMA2_IT_HT4 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_HT4 );
      dma2ch4HT_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TC4 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TC4 );
      dma2ch4TC_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TE4 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TE4 );
      dma2ch4TE_Update++;
    }
    DMA_ClearITPendingBit( DMA2_IT_GL4 );
  }

  if( DMA_GetITStatus( DMA2_IT_GL5 ) == SET )
  {
    if( DMA_GetITStatus( DMA2_IT_HT5 ) == SET )  /*half transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_HT5 );
      dma2ch5HT_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TC5 ) == SET )  /*full transmission complete*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TC5 );
      dma2ch5TC_Update++;
    }
    if( DMA_GetITStatus( DMA2_IT_TE5 ) == SET )  /*transmission error*/
    {
      DMA_ClearITPendingBit( DMA2_IT_TE5 );
      dma2ch5TE_Update++;
    }
    DMA_ClearITPendingBit( DMA2_IT_GL4 );
  }
}
