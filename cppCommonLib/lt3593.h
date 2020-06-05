/* ----------------------------------------
  lt3593 utilities
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

#ifndef  __LT3593_H__
#define  __LT3593_H__

#include  <gpio.h>
extern "C"
{
  #include  <system.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
enum BACKLIGHT_BRIGHTNESS {
  BRIGHTEST = 0, BRIGH_LV1, BRIGH_LV2, BRIGH_LV3, BRIGH_LV4,
  BRIGH_LV5, BRIGH_LV6, BRIGH_LV7, BRIGH_LV8, BRIGH_LV9,
  BRIGH_LV10, BRIGH_LV11, BRIGHTER, BRIGH_LV13, BRIGH_LV14,
  BRIGH_LV15, BRIGH_LV16, BRIGH_LV17, BRIGH_LV18, BRIGH_LV19,
  BRIGH_LV20, BRIGH_LV21, BRIGH_LV22, BRIGH_LV23, UMBER,
  BRIGH_LV25, BRIGH_LV26, BRIGH_LV27, BRIGH_LV28, BRIGH_LV29,
  BRIGH_LV30, DARK };


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class LT3593 : public GPIO
{
public:
  LT3593();
  LT3593( uint8_t pin );
  ~LT3593();
  void begin( uint8_t pin );
  void brightness( int luminus );

private:
  uint8_t gpioPin;
};


#endif  /* __LT3593_H__ */

