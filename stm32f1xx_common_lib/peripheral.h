/* ----------------------------------------
 stm32f1xx defines for peripheral.
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
#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#include  <stm32f10x.h>
#include  <derivertive.h>
#include  <mul_tsk.h>
#include  "usart.h"

/* ----------------------------------------
    prototypes
---------------------------------------- */
void Dly100us( int delay );
void Dly50us( int delay );
void Dly10us( int delay );
void WaitMs( int delay );
void WaitUs( unsigned long delay );

void GPIO_Terminal_Init( GPIO_TypeDef* GPIOx, GPIOSpeed_TypeDef Speed, GPIOMode_TypeDef Mode, u16 Pins );

#define  GPIO_Output_Init(GPIOx,Pins)     GPIO_Terminal_Init(GPIOx,GPIO_Speed_2MHz, GPIO_Mode_Out_PP,Pins)
#define  GPIO_Output10M_Init(GPIOx,Pins)  GPIO_Terminal_Init(GPIOx,GPIO_Speed_10MHz,GPIO_Mode_Out_PP,Pins)
#define  GPIO_Output50M_Init(GPIOx,Pins)  GPIO_Terminal_Init(GPIOx,GPIO_Speed_50MHz,GPIO_Mode_Out_PP,Pins)
#define  GPIO_Input_Init(GPIOx,Pins)      GPIO_Terminal_Init(GPIOx,GPIO_Speed_2MHz, GPIO_Mode_IPU,   Pins)
#define  GPIO_InputFloat_Init(GPIOx,Pins) GPIO_Terminal_Init(GPIOx,GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING,Pins)
#define  GPIO_Analog_Init(GPIOx,Pins)     GPIO_Terminal_Init(GPIOx,GPIO_Speed_2MHz, GPIO_Mode_AIN,   Pins)
#define  GPIO_Alternate_Init(GPIOx,Pins)  GPIO_Terminal_Init(GPIOx,GPIO_Speed_50MHz,GPIO_Mode_AF_PP, Pins)
#define  GPIO_AlternateOD_Init(GPIOx,Pins)  GPIO_Terminal_Init(GPIOx,GPIO_Speed_50MHz,GPIO_Mode_AF_OD, Pins)
#define  GPIO_OutputOD_Init(GPIOx,Pins)  GPIO_Terminal_Init(GPIOx,GPIO_Speed_10MHz,GPIO_Mode_Out_OD,Pins)


void gpioInit( void );
void beginSCI( int ch, unsigned long brr );
void terminateSCI( int ch );


#endif /*_PERIPHERAL_H_*/
