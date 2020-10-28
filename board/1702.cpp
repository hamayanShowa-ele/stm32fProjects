/* ----------------------------------------
 board 1702 chronos driver utilities.
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

#include  "1702.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
static int colIndex;
static uint8_t segData[6];

static const uint16_t segDigit[] =
{
  SEG_A_PIN | SEG_B_PIN | SEG_C_PIN | SEG_D_PIN | SEG_E_PIN | SEG_F_PIN ,  /*0*/
  SEG_B_PIN | SEG_C_PIN,  /*1*/
  SEG_A_PIN | SEG_B_PIN | SEG_D_PIN | SEG_E_PIN | SEG_G_PIN,  /*2*/
  SEG_A_PIN | SEG_B_PIN | SEG_C_PIN | SEG_D_PIN | SEG_G_PIN,  /*3*/
  SEG_F_PIN | SEG_G_PIN | SEG_B_PIN | SEG_C_PIN,  /*4*/
  SEG_A_PIN | SEG_F_PIN | SEG_G_PIN | SEG_C_PIN | SEG_D_PIN,  /*5*/
  SEG_A_PIN | SEG_F_PIN | SEG_G_PIN | SEG_C_PIN | SEG_D_PIN | SEG_E_PIN ,  /*6*/
  SEG_A_PIN | SEG_B_PIN | SEG_C_PIN,  /*7*/
  SEG_A_PIN | SEG_B_PIN | SEG_C_PIN | SEG_D_PIN | SEG_E_PIN | SEG_F_PIN | SEG_G_PIN,  /*8*/
  SEG_A_PIN | SEG_B_PIN | SEG_C_PIN | SEG_D_PIN | SEG_F_PIN | SEG_G_PIN,  /*9*/
};

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
---------------------------------------- */
void BD1702::begin( PCA8574 *_sw, PCA8574 *_relay, PCA8574 *_scan )
{
  sw = _sw;
  relay = _relay;
  scan = _scan;
}

/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void BD1702::gpioInit()
{
  pinMode( SEG_G, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW6 */
  pinMode( SEG_F, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW5 */
  pinMode( SEG_E, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW4 */
  pinMode( SEG_D, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW3 */
  pinMode( SEG_C, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW2 */
  pinMode( SEG_B, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW1 */
  pinMode( SEG_A, OUTPUT, GPIO_SPEED_NORMAL );  /* SEG ROW0 */

  sw->write( 0xFF );  /* all input mode. */
  relayBit = 0x30;  /* 0b0011 0000 NONE,LOCK:active high,TIME low,DATE low, RELAY1~4 high */
  relay->write( relayBit );
  scanBit = 0xFF;  /* all active low. */
  scan->write( scanBit );

  colIndex = 0;
  memset( segData, 0 , sizeof(segData) / sizeof(segData[0]) );
}


/* ----------------------------------------
    segment update
---------------------------------------- */
void BD1702::segmentUpdate()
{
  scan->write( 0xFF );  /* disable segment display. */

  uint16_t tempUS = segDigit[ segData[ colIndex ] ];
  loc_cpu();
  volatile uint16_t odr = SEG_PORT->ODR;
  odr &= ~SEG_ROW_MASK;
  odr |= tempUS;
  SEG_PORT->ODR = odr;
  unl_cpu();

  uint8_t tempUC = 0x01 << colIndex;
  tempUC ^= 0xFF;
  scan->write( tempUC );  /* all active low. */
  if( ++colIndex >= (int)(sizeof(segData) / sizeof(segData[0])) ) colIndex = 0;
}

/* ----------------------------------------
    lock led is active high.
---------------------------------------- */
void BD1702::lockLED( bool onOff )
{
  if( onOff ) relayBit |= LOCK_BIT;
  else relayBit &= ~LOCK_BIT;
  relay->write( relayBit );
}


/* ----------------------------------------
    time led is active low.
---------------------------------------- */
void BD1702::timeLED( bool onOff )
{
  if( onOff ) relayBit &= ~TIME_BIT;
  else relayBit |= TIME_BIT;
  relay->write( relayBit );
}


/* ----------------------------------------
    date led is active low.
---------------------------------------- */
void BD1702::dateLED( bool onOff )
{
  if( onOff ) relayBit &= ~DATE_BIT;
  else relayBit |= DATE_BIT;
  relay->write( relayBit );
}

/* ----------------------------------------
    set hour string.
---------------------------------------- */
void BD1702::setHour( int hour )
{
//  if( hour < 0 || hour >= 24 ) return;
  segData[0] = (uint8_t)(hour / 10);
  segData[1] = (uint8_t)(hour % 10);
}

/* ----------------------------------------
    set minute string.
---------------------------------------- */
void BD1702::setMinute( int minute )
{
//  if( minute < 0 || minute >= 60 ) return;
  segData[2] = (uint8_t)(minute / 10);
  segData[3] = (uint8_t)(minute % 10);
}

/* ----------------------------------------
    set second string.
---------------------------------------- */
void BD1702::setSecond( int second )
{
//  if( second < 0 || second >= 60 ) return;
  segData[4] = (uint8_t)(second / 10);
  segData[5] = (uint8_t)(second % 10);
}

/* ----------------------------------------
    relay1 is active high.
---------------------------------------- */
void BD1702::relay1( bool onOff )
{
  if( onOff ) relayBit |= RELAY1_BIT;
  else relayBit &= ~RELAY1_BIT;
  relay->write( relayBit );
}

/* ----------------------------------------
    relay2 is active high.
---------------------------------------- */
void BD1702::relay2( bool onOff )
{
  if( onOff ) relayBit |= RELAY2_BIT;
  else relayBit &= ~RELAY2_BIT;
  relay->write( relayBit );
}

/* ----------------------------------------
    relay3 is active high.
---------------------------------------- */
void BD1702::relay3( bool onOff )
{
  if( onOff ) relayBit |= RELAY3_BIT;
  else relayBit &= ~RELAY3_BIT;
  relay->write( relayBit );
}

/* ----------------------------------------
    relay4 is active high.
---------------------------------------- */
void BD1702::relay4( bool onOff )
{
  if( onOff ) relayBit |= RELAY4_BIT;
  else relayBit &= ~RELAY4_BIT;
  relay->write( relayBit );
}

/* ----------------------------------------
    what status mode switch.
---------------------------------------- */
bool BD1702::modeSwitchIs()
{
  uint8_t tempUC = sw->read();
  return (tempUC & MODE_BIT) ? false : true;
}

/* ----------------------------------------
    what status alter switch.
---------------------------------------- */
bool BD1702::alterSwitchIs()
{
  uint8_t tempUC = sw->read();
  return (tempUC & ALTER_BIT) ? false : true;
}

