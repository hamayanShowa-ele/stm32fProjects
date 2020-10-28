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

#ifndef  __BOARD_1702_H__
#define  __BOARD_1702_H__

//#include  <stm32f10x.h>
#include  <gpio.h>
#include  <derivertive.h>
#include  <pca8574.h>

extern "C"
{
//  #include  <string.h>
//  #include  <ctype.h>
  #include  <stdlib.h>
//  #include  <system.h>
  #include  <mul_tsk.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  SEG_ROW_MASK      0x03F8  /* 0b0000 0011 1111 1000 */

#define  SEG_PORT   GPIOB
#define  SEG_A_PIN  GPIO_Pin_3
#define  SEG_B_PIN  GPIO_Pin_4
#define  SEG_C_PIN  GPIO_Pin_5
#define  SEG_D_PIN  GPIO_Pin_6
#define  SEG_E_PIN  GPIO_Pin_7
#define  SEG_F_PIN  GPIO_Pin_8
#define  SEG_G_PIN  GPIO_Pin_9

#define  LOCK_BIT   0x40
#define  TIME_BIT   0x20
#define  DATE_BIT   0x10
#define  RELAY4_BIT 0x08
#define  RELAY3_BIT 0x04
#define  RELAY2_BIT 0x02
#define  RELAY1_BIT 0x01

#define  MODE_BIT   0x40
#define  ALTER_BIT  0x80

/* ----------------------------------------
    prototypes 
---------------------------------------- */
extern "C"
{
  void segmentHandler( void );
}

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BD1702 : public GPIO
{
public:
  BD1702(){}
  BD1702( PCA8574 *_sw, PCA8574 *_relay, PCA8574 *_scan )
    { begin( _sw, _relay, _scan ); }
  ~BD1702(){}
  void begin( PCA8574 *_sw, PCA8574 *_relay, PCA8574 *_scan );
  void gpioInit();
  void segmentUpdate();
  void lockLED( bool onOff );
  void timeLED( bool onOff );
  void dateLED( bool onOff );
  void setHour( int hour );
  void setMinute( int minute );
  void setSecond( int second );
  void relay1( bool onOff );
  void relay2( bool onOff );
  void relay3( bool onOff );
  void relay4( bool onOff );
  bool modeSwitchIs();
  bool alterSwitchIs();

private:
  PCA8574 *sw;
  PCA8574 *relay;
  PCA8574 *scan;
  uint8_t scanBit,relayBit;
};


#endif  /* __BOARD_1702_H__ */

