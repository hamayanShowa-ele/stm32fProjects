/* ----------------------------------------
  GPIO utilities
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
#include  "stm32l0xxGPIO.h"

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
    whatGPIOType
---------------------------------------- */
GPIO_TypeDef* whatGPIOType( int pin )
{
  if( pin >= PA0 && pin <= PA15 ) return GPIOA;
  else if( pin >= PB0 && pin <= PB15 ) return GPIOB;
  else if( pin >= PC0 && pin <= PC15 ) return GPIOC;
  else if( pin >= PD0 && pin <= PD15 ) return GPIOD;
//  else if( pin >= PE0 && pin <= PE15 ) return GPIOE;
//  else if( pin >= PF0 && pin <= PF15 ) return GPIOF;
//  else if( pin >= PG0 && pin <= PG15 ) return GPIOG;
  else if( pin >= PH0 && pin <= PH15 ) return GPIOH;
  return 0;
}

/* ----------------------------------------
    whatPin
---------------------------------------- */


/* ----------------------------------------
    pin mode
---------------------------------------- */
static void pinMode_Base( GPIO_TypeDef *gpiox, uint16_t gpioPin, int type, int speed )
{
  GPIO_InitTypeDef GPIO_Init;
#if 0
  GPIO_StructInit( &GPIO_InitStruct );
  GPIO_InitStruct.GPIO_Mode = (GPIOMode_TypeDef)type;  /* GPIO_Mode_AIN or GPIO_Mode_IN_FLOATING or GPIO_Mode_IPD or GPIO_Mode_IPU or GPIO_Mode_Out_OD or GPIO_Mode_Out_PP or GPIO_Mode_AF_OD or GPIO_Mode_AF_PP */
  GPIO_InitStruct.GPIO_Pin = gpioPin;  /* from GPIO_Pin_0 to GPIO_Pin_15 */
  GPIO_InitStruct.GPIO_Speed = (GPIOSpeed_TypeDef)speed;  /* GPIO_Speed_10MHz or GPIO_Speed_2MHz or GPIO_Speed_50MHz */
  GPIO_Init( gpiox, &GPIO_InitStruct );
#endif
  GPIO_Init.Pin = gpioPin;
  GPIO_Init.Mode = type & 0x000000FF;
  GPIO_Init.Speed = speed;
  GPIO_Init.Pull = (type & 0x00000300) >> 8;
//  GPIO_Init.Alternate = 0;
  HAL_GPIO_Init( gpiox, &GPIO_Init );
}

void pinMode( int pin, int type )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  pinMode_Base( gpiox, gpioPin, type, GPIO_SPEED_HIGH );
}

/* ----------------------------------------
    output pin set/reset
---------------------------------------- */
void pinSet( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  gpiox->BSRR = gpioPin;
}

void pinReset( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  gpiox->BSRR = gpioPin << 16;
}

void pinToggle( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  loc_cpu();
  uint32_t odr = gpiox->ODR;
  odr ^= gpioPin;
  gpiox->ODR = odr;
  unl_cpu();
}

/* ----------------------------------------
    digital write
---------------------------------------- */
void digitalWrite( int pin, bool highOrLow )
{
  if( highOrLow ) pinSet( pin );
  else pinReset( pin );
}

/* ----------------------------------------
    digital read
---------------------------------------- */
bool digitalRead( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  return (gpiox->IDR & gpioPin) ? true : false;
}

/* ----------------------------------------
    alternate mode
---------------------------------------- */
void pinAlternateMode( int pin, uint32_t mode, uint8_t alt )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );

  GPIO_InitTypeDef GPIO_Init;
  GPIO_Init.Pin = gpioPin;
  GPIO_Init.Mode = mode;
  GPIO_Init.Pull = GPIO_NOPULL;
  GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init.Alternate = alt;
  HAL_GPIO_Init( gpiox, &GPIO_Init );
}

#if 0

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

/* ----------------------------------------
    all pin make input pullup or pulldown.
---------------------------------------- */
void GPIO::allPullupPulldown( GPIOMode_TypeDef mode, uint16_t pa, uint16_t pb )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* the first, all inputs are as input pull up and all outputs are as push/pull.*/
  GPIO_StructInit( &GPIO_InitStruct );
  GPIO_InitStruct.GPIO_Mode = mode;
  GPIO_Init( GPIOC, &GPIO_InitStruct );
  GPIO_Init( GPIOD, &GPIO_InitStruct );
  GPIO_Init( GPIOE, &GPIO_InitStruct );
  GPIO_Init( GPIOF, &GPIO_InitStruct );
  GPIO_Init( GPIOG, &GPIO_InitStruct );
  /*GPIOA's 13pin,14pin and 15pin are used as the JTAG. But when using the SWD, only used 13pin and 14pin. */
  /*GPIOB's 3pin and 4pin are used as the JTAG. But when using the SWD, not used.*/
  GPIO_InitStruct.GPIO_Mode = mode;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
//  GPIO_InitStruct.GPIO_Pin &= ~(GPIO_Pin_13 | GPIO_Pin_14);
  GPIO_InitStruct.GPIO_Pin = pa;
  GPIO_Init( GPIOA, &GPIO_InitStruct );
//  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Pin = pb;
  GPIO_Init( GPIOB, &GPIO_InitStruct );
}

void GPIO::allPullupOnSWD()
{
  /*GPIOA's 13pin,14pin and 15pin are used as the JTAG. But when using the SWD, only used 13pin and 14pin. */
  /*GPIOB's 3pin and 4pin are used as the JTAG. But when using the SWD, not used.*/
  allPullupPulldown( GPIO_Mode_IPU, GPIO_Pin_All & ~(GPIO_Pin_13 | GPIO_Pin_14), GPIO_Pin_All );
}

void GPIO::allPulldownOnSWD()
{
  /*GPIOA's 13pin,14pin and 15pin are used as the JTAG. But when using the SWD, only used 13pin and 14pin. */
  /*GPIOB's 3pin and 4pin are used as the JTAG. But when using the SWD, not used.*/
  allPullupPulldown( GPIO_Mode_IPD, GPIO_Pin_All & ~(GPIO_Pin_13 | GPIO_Pin_14), GPIO_Pin_All  );
}

/* ----------------------------------------
    SWD enable.
---------------------------------------- */
void GPIO::swdEnable()
{
  /* The GPIOA pins 13, 14, and 15 are used in JTAG. However, if it is used in SWD, only 13 and 14 pins are applicable. */
  /* The GPIOB pins 3 and 4 are used in JTAG. However, in the case of SWD, these pins are not used. */
  GPIO_PinRemapConfig( GPIO_Remap_SWJ_JTAGDisable, ENABLE );
}


/* ----------------------------------------
    RCC clock enable and disable
---------------------------------------- */
void GPIO::rccClockEnable( int rcc )
{
  uint32_t shift;
  if( rcc < 32 )  /* APB2_peripheral */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB2PeriphClockCmd( shift, ENABLE );
    RCC_APB2PeriphResetCmd( shift, DISABLE );
  }
  else if( rcc < 64 )  /* APB1_peripheral */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB1PeriphClockCmd( shift, ENABLE );
    RCC_APB1PeriphResetCmd( shift, DISABLE );
  }
  else {}
}

void GPIO::rccClockDisable( int rcc )
{
  uint32_t shift;
  if( rcc < 32 )  /* APB2_peripheral */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB2PeriphClockCmd( shift, DISABLE );
    RCC_APB2PeriphResetCmd( shift, ENABLE );
  }
  else if( rcc < 64 )  /* APB1_peripheral */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB1PeriphClockCmd( shift, DISABLE );
    RCC_APB1PeriphResetCmd( shift, ENABLE );
  }
  else {}
}

#endif
