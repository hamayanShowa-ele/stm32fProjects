/* ----------------------------------------
  pca8574 utilities
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

#ifndef  __PCA8574_H__
#define  __PCA8574_H__

#include  <Wire.h>
extern "C"
{
}

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class PCA8574 //  : public GPIO
{
public:
  PCA8574();
  PCA8574( STM32F_I2C *i2c, uint8_t adr );
  ~PCA8574();
  void begin( STM32F_I2C *i2c, uint8_t adr );

  int write( uint8_t data );
  int read();

private:
  STM32F_I2C *wire;
  uint8_t slave;
};


#endif  /* __PCA8574_H__ */

