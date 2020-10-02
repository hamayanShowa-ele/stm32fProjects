/* ----------------------------------------
 board 1303 utilities.
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

#include  "1426.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
static int colIndex;
static uint8_t segData[5];

static const uint8_t segDigit[] =
{
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F ,  /*0*/
  SEG_B | SEG_C,  /*1*/
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,  /*2*/
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,  /*3*/
  SEG_F | SEG_G | SEG_B | SEG_C,  /*4*/
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,  /*5*/
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D | SEG_E ,  /*6*/
  SEG_A | SEG_B | SEG_C,  /*7*/
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,  /*8*/
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,  /*9*/
};

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
BD1426::BD1426()
{
}

BD1426::~BD1426()
{
}

/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void BD1426::gpioInit()
{
  pinMode( PD7, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW7 */
  pinMode( PD6, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW6 */
  pinMode( PD5, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW5 */
  pinMode( PD4, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW4 */
  pinMode( PD3, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW3 */
  pinMode( PD2, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW2 */
  pinMode( PD1, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW1 */
  pinMode( PD0, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW0 */

  pinMode( PC12, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG COL2 */
  pinMode( PC11, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG COL1 */
  pinMode( PC10, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG COL0 */

  colIndex = 0;
  memset( segData, 0 , sizeof(segData) / sizeof(segData[0]) );
}

/* ----------------------------------------
    numerical data set
---------------------------------------- */
void BD1426::numerical( float value )
{
  if( value == 0.0F )
  {
    segData[0] = 0;
    segData[1] = 0;
    segData[2] = segDigit[0] | SEG_DP;
    segData[3] = segDigit[0];
    return;
  }

  if( value >= 2000.0F || value <= -2000.0F )
  {
    memset( segData, 0 , sizeof(segData) / sizeof(segData[0]) );
    return;
  }
  else if( value >= 1000.0F || value <= -1000.0F )  /* from 1000.0 to 1999.9 */
  {
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = SEG_ONE;
    segData[1] = segDigit[ (tempI % 1000) / 100 ];
    segData[2] = segDigit[ (tempI % 100) / 10 ];
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
  else if( value >= 200.0F || value <= -200.0F )  /* from 200.0 to 999.9 */
  {
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = 0;
    segData[1] = segDigit[ tempI / 100 ];
    segData[2] = segDigit[ (tempI % 100) / 10 ];
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
  else if( value >= 100.0F || value <= -100.0F )  /* from 100.0 to 199.9 */
  {
    value *= 10.0F;
//    value += ( value > 0.0F ) ? 0.5F : -0.5F;
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = SEG_ONE;
    segData[1] = segDigit[ (tempI % 1000) / 100 ];
    segData[2] = segDigit[ (tempI % 100) / 10 ] | SEG_DP;
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
  else if( value >= 20.0F || value <= -20.0F )  /* from 20.0 to 99.9 */
  {
    value *= 10.0F;
//    value += ( value > 0.0F ) ? 0.5F : -0.5F;
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = 0;
    segData[1] = segDigit[ tempI / 100 ];
    segData[2] = segDigit[ (tempI % 100) / 10 ] | SEG_DP;
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
  else if( value >= 10.0F || value <= -10.0F )  /* from 10.00 to 19.99 */
  {
    value *= 100.0F;
//    value += ( value > 0.0F ) ? 0.5F : -0.5F;
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = SEG_ONE;
    segData[1] = segDigit[ (tempI % 1000) / 100 ] | SEG_DP;
    segData[2] = segDigit[ (tempI % 100) / 10 ];
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
  else if( value >= 2.0F || value <= -2.0F )  /* from 2.00 to 9.99 */
  {
    value *= 100.0F;
//    value += ( value > 0.0F ) ? 0.5F : -0.5F;
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = 0;
    segData[1] = segDigit[ tempI / 100 ] | SEG_DP;
    segData[2] = segDigit[ (tempI % 100) / 10 ];
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
  else if( value >= 1.0F || value <= -1.0F )  /* from 1.000 to 1.999 */
  {
    value *= 1000.0F;
//    value += ( value > 0.0F ) ? 0.5F : -0.5F;
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = SEG_ONE | SEG_DP;
    segData[1] = segDigit[ (tempI % 1000) / 100 ];
    segData[2] = segDigit[ (tempI % 100) / 10 ];
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
  else  /* from 0.01 to 0.99 */
  {
    value *= 100.0F;
//    value += ( value > 0.0F ) ? 0.5F : -0.5F;
    int tempI = (int)value;
    tempI = abs( tempI );
    segData[0] = 0;
    segData[1] = segDigit[ 0 ] | SEG_DP;
    segData[2] = segDigit[ tempI / 10 ];
    segData[3] = segDigit[ (tempI % 10) ];
    segData[0] |= ( value > 0.0F ) ? SEG_PULS : SEG_MINUS;
  }
}

/* ----------------------------------------
    status led set
---------------------------------------- */
void BD1426::status( uint8_t bit, bool onOff )
{
  if( bit > 4 ) return;
  uint8_t tempUC = 0x01;
  tempUC <<= bit;
  if( onOff ) segData[4] |= tempUC;
  else segData[4] &= ~tempUC;
}



extern "C"
{

/* ----------------------------------------
    row write
---------------------------------------- */
static void row( uint8_t data )
{
  uint16_t rowODR = (data & 0xFF) << SEG_ROW_SHIFT;
  loc_cpu();  /* enter the critical section. */
  uint16_t tempUS = SEG_ROW_PORT->ODR;
  tempUS &= ~SEG_ROW_MASK;
  tempUS |= rowODR;
  SEG_ROW_PORT->ODR = tempUS;
  unl_cpu();  /* exit the critical section. */
}

/* ----------------------------------------
    col write
---------------------------------------- */
static void col( uint8_t data )
{
  uint16_t colODR = (data & 0x07) << SEG_COL_SHIFT;
  loc_cpu();  /* enter the critical section. */
  uint16_t tempUS = SEG_COL_PORT->ODR;
  tempUS &= ~SEG_COL_MASK;
  tempUS |= colODR;
  SEG_COL_PORT->ODR = tempUS;
  unl_cpu();  /* exit the critical section. */
}

/* ----------------------------------------
    7segment led driver handler
---------------------------------------- */
void segmentHandler( void )
{
  row( 0 );
  col( colIndex );
  row( segData[ colIndex ] );
  if( ++colIndex >= (int)(sizeof(segData) / sizeof(segData[0])) ) colIndex = 0;
}

}  /* extern "C" */
