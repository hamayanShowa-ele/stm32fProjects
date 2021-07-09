/* ----------------------------------------
 USART utility header
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
#ifndef  __STM32L0xxUSART_H__
#define  __STM32L0xxUSART_H__

#include  <stdarg.h>
#include  <stdio.h>
#include  <string.h>

#include  <stm32l0xx.h>
//#include  <stm32l0xx_hal_gpio.h>
#include  <stm32l0xx_hal_dma.h>
#include  <stm32l0xx_hal_uart.h>
#include  <stm32l0xx_hal_usart.h>
#include  <stm32fPeripheral.h>
#include  <derivertive.h>
#include  <mul_tsk.h>
#include  <stm32l0xxGPIO.h>
#include  <stm32l0xxNVIC.h>

/* ----------------------------------------
    defines
---------------------------------------- */
#define  USART_OK   0
#define  USART_ERR  (-1)

/* ----------------------------------------
    prototypes
---------------------------------------- */
int usartBegin( USART_TypeDef *USARTx, uint32_t brr, uint8_t txPin, uint8_t rxPin );
int usartBlockWrite( int handle, const uint8_t *data, int size );
int usartBlockRead( int handle, uint8_t *data, int size );
int usartReadStart( int handle );
int usartAvailable( int handle );
int usartByteRead( int handle );
int usartByteWrite( int handle, uint8_t data );
int usartPrint( int handle, const char *str );
int usartPrintf( int handle, const char *fmt, ... );

#define  usartPutc(h,c)  usartByteWrite(h,c)
#define  usartGetc(h)    usartByteRead(h)

/* ----------------------------------------
    instances or global variables
---------------------------------------- */


#endif  /* __STM32L0xxUSART_H__ */

