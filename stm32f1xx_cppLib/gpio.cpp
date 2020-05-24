/* ----------------------------------------
  gpio utilities
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
#include  "gpio.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    response pin's gpio type
---------------------------------------- */
GPIO_TypeDef* GPIO::whatGPIOType( int pin )
{
  if( pin >= PA0 && pin <= PA15 ) return GPIOA;
  else if( pin >= PB0 && pin <= PB15 ) return GPIOB;
  else if( pin >= PC0 && pin <= PC15 ) return GPIOC;
  else if( pin >= PD0 && pin <= PD15 ) return GPIOD;
  else if( pin >= PE0 && pin <= PE15 ) return GPIOE;
  else if( pin >= PF0 && pin <= PF15 ) return GPIOF;
  else if( pin >= PG0 && pin <= PG15 ) return GPIOG;
//  else if( pin >= PH0 && pin <= PH15 ) return GPIOH;
  return 0;
}

/* ----------------------------------------
    response pin's define
---------------------------------------- */
uint16_t GPIO::whatPin( int pin )
{
  return 0x0001 << pin % 16;
}

/* ----------------------------------------
    all pin make input pullup.
---------------------------------------- */
void GPIO::allPullup()
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* the first, all inputs are as input pull up and all outputs are as push/pull.*/
  GPIO_StructInit( &GPIO_InitStruct );
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init( GPIOC, &GPIO_InitStruct );
  GPIO_Init( GPIOD, &GPIO_InitStruct );
  GPIO_Init( GPIOE, &GPIO_InitStruct );
  GPIO_Init( GPIOF, &GPIO_InitStruct );
  GPIO_Init( GPIOG, &GPIO_InitStruct );
  /*GPIOA's 13pin,14pin and 15pin are used as the JTAG. But when using the SWD, only used 13pin and 14pin. */
  /*GPIOB's 3pin and 4pin are used as the JTAG. But when using the SWD, not used.*/
#if 0  /*JTAG*/
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Pin &= ~(GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
  GPIO_Init( GPIOA, &GPIO_InitStruct );
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Pin &= ~(GPIO_Pin_3 | GPIO_Pin_4);
  GPIO_Init( GPIOB, &GPIO_InitStruct );
#else  /*SWD*/
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Pin &= ~(GPIO_Pin_13 | GPIO_Pin_14);
  GPIO_Init( GPIOA, &GPIO_InitStruct );
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init( GPIOB, &GPIO_InitStruct );
#endif
}

/* ----------------------------------------
    pin mode
---------------------------------------- */
void GPIO::pinMode( GPIO_TypeDef *gpiox, uint16_t gpioPin, int type, int speed )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_StructInit( &GPIO_InitStruct );
  GPIO_InitStruct.GPIO_Mode = (GPIOMode_TypeDef)type;  /* GPIO_Mode_AIN or GPIO_Mode_IN_FLOATING or GPIO_Mode_IPD or GPIO_Mode_IPU or GPIO_Mode_Out_OD or GPIO_Mode_Out_PP or GPIO_Mode_AF_OD or GPIO_Mode_AF_PP */
  GPIO_InitStruct.GPIO_Pin = gpioPin;  /* from GPIO_Pin_0 to GPIO_Pin_15 */
  GPIO_InitStruct.GPIO_Speed = (GPIOSpeed_TypeDef)speed;  /* GPIO_Speed_10MHz or GPIO_Speed_2MHz or GPIO_Speed_50MHz */
  GPIO_Init( gpiox, &GPIO_InitStruct );
}

void GPIO::pinMode( int pin, int type, int speed )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  pinMode( gpiox, gpioPin, type, speed );
}



/* ----------------------------------------
    output pin set/reset
---------------------------------------- */
void GPIO::set( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  gpiox->BSRR = gpioPin;
}

void GPIO::reset( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  gpiox->BSRR = gpioPin << 16;
}

/* ----------------------------------------
    digital read
---------------------------------------- */
bool GPIO::digitalRead( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  return (gpiox->IDR & gpioPin) ? true : false;
}

/* ----------------------------------------
    port WORD write
---------------------------------------- */
void GPIO::wordWrite( GPIO_TypeDef *gpiox, uint16_t data, uint16_t mask )
{
  uint16_t tempUS = gpiox->ODR;
  tempUS &= ~mask;
  tempUS |= data;
  gpiox->ODR = tempUS;
}

