/* ----------------------------------------
 1405 board utility header for stm32l0xx
  for STMicroelectronics HAL library

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
#ifndef  __1405_STM32L0xx_H__
#define  __1405_STM32L0xx_H__

//#include  <stm32l0xx.h>
#include  <derivertive.h>
#include  <stm32l0xxGPIO.h>
#include  <system.h>

/* ----------------------------------------
    defines
---------------------------------------- */
#define  BOARD_1405_IDSW  (IDSW << 1)

/* ----------------------------------------
    prototypes
---------------------------------------- */
void gpioInit( void );
void fifoWrite( uint16_t data );
void knock1415( void );

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

#endif  /* __1405_STM32L0xx_H__ */

