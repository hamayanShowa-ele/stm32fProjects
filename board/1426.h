/* ----------------------------------------
 board 1426 s25n 7segment led display utilities.
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

#ifndef  __BOARD_1426_H__
#define  __BOARD_1426_H__

#include  <gpio.h>
#include  <derivertive.h>

extern "C"
{
  #include  <string.h>
  #include  <ctype.h>
  #include  <stdlib.h>
//  #include  <system.h>
  #include  <mul_tsk.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  SEG_ROW_PORT      GPIOD
#define  SEG_COL_PORT      GPIOC
                                   /*   5432 1098 7654 3210 */
#define  SEG_ROW_MASK      0x00FF  /* 0b0000 0000 1111 1111 */
#define  SEG_COL_MASK      0x1C00  /* 0b0001 1100 0000 0000 */

#define  SEG_ROW_SHIFT     0
#define  SEG_COL_SHIFT     10

#define  SEGMENT_STRING_SIZE 10

#define  SEG_A      0x01
#define  SEG_B      0x02
#define  SEG_C      0x04
#define  SEG_D      0x08
#define  SEG_E      0x10
#define  SEG_F      0x20
#define  SEG_G      0x40
#define  SEG_DP     0x80

#define  SEG_ONE    (SEG_A | SEG_B)
#define  SEG_MINUS  SEG_C
#define  SEG_PULS   SEG_D

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

class BD1426 : public GPIO
{
public:
  BD1426();
  ~BD1426();

  void gpioInit();
  void numerical( float value );
  void status( uint8_t bit, bool onOff );

private:
};


#endif  /* __BOARD_1426_H__ */

