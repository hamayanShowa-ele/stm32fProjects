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
    response exti pin's interrupt number.
---------------------------------------- */
int GPIO::whatExtiIntNumber( int pin )
{
  int num;
  switch( pin % 16 )
  {
    case 0:
      num = EXTI0_IRQn;
      break;
    case 1:
      num = EXTI1_IRQn;
      break;
    case 2:
      num = EXTI2_IRQn;
      break;
    case 3:
      num = EXTI3_IRQn;
      break;
    case 4:
      num = EXTI4_IRQn;
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      num = EXTI9_5_IRQn;
      break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      num = EXTI15_10_IRQn;
      break;
    default:
      num = (-1);
      break;
  }
  return num;
}


/* ----------------------------------------
    pin mode
---------------------------------------- */
void GPIO::pinMode( GPIO_TypeDef *gpiox, uint16_t gpioPin, int type, int speed )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_StructInit( &GPIO_InitStruct );

  GPIO_InitStruct.GPIO_Pin = gpioPin;  /* from GPIO_Pin_0 to GPIO_Pin_15 */
  /* GPIO_Speed_10MHz or GPIO_Speed_2MHz or GPIO_Speed_50MHz or GPIO_Speed_100MHz */
  GPIO_InitStruct.GPIO_Speed = (GPIOSpeed_TypeDef)speed;
  /* GPIO_Mode_AIN or GPIO_Mode_IN_FLOATING or GPIO_Mode_IPD or GPIO_Mode_IPU */
  /* or GPIO_Mode_Out_OD or GPIO_Mode_Out_PP */
  /* or GPIO_Mode_AF_OD or GPIO_Mode_AF_PP */
  if( type == INPUT )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  }
  else if( type == OUTPUT )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  }
  else if( type == ALTERNATE_PP )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  }
  else if( type == ANALOG )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  }
  else if( type == INPUT_PULLDOWN )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  }
  else if( type == INPUT_PULLUP )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  }
  else if( type == OUTPUT_OPEN_DRAIN )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  }
  else if( type == ALTERNATE_OD )
  {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  }
  else return;

  GPIO_Init( gpiox, &GPIO_InitStruct );
}

void GPIO::pinMode( int pin, int type, int speed )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  pinMode( gpiox, gpioPin, type, speed );
}

/* ----------------------------------------
    pin alternate function configure.
---------------------------------------- */
void GPIO::pinAFConfig( int pin, uint8_t type )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint8_t gpioSpurce = whatPinSource( pin );
  GPIO_PinAFConfig( gpiox, gpioSpurce, type );
}

/* ----------------------------------------
    read output data register.
---------------------------------------- */
bool GPIO::odr( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  return (gpiox->ODR & gpioPin) ? true : false;
}

/* ----------------------------------------
    read input data register.
---------------------------------------- */
bool GPIO::idr( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  return (gpiox->IDR & gpioPin) ? true : false;
}


/* ----------------------------------------
    output pin set/reset
---------------------------------------- */
void GPIO::set( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  gpiox->BSRRL = gpioPin;
}

void GPIO::reset( int pin )
{
  GPIO_TypeDef *gpiox = whatGPIOType( pin );
  uint16_t gpioPin = whatPin( pin );
  gpiox->BSRRH = gpioPin;
}

/* ----------------------------------------
    digital write
---------------------------------------- */
void GPIO::digitalWrite( int pin, bool highOrLow )
{
  if( highOrLow ) set( pin );
  else reset( pin );
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

/* ----------------------------------------
    all pin make input pullup or pulldown.
---------------------------------------- */
//void GPIO::allPullupPulldown( GPIOMode_TypeDef mode, uint16_t pa, uint16_t pb )
void GPIO::allPullupPulldown( int type )
{
  if( type != INPUT_PULLDOWN && type != INPUT_PULLUP ) return;
  for( int pin = PA0; pin <= PG15; pin++ )
  {
    if( pin != PA13 && pin != PA14 && pin != PA15 && pin != PB3 && pin != PB4 )
    {
      pinMode( pin, type );
    }
  }
}

void GPIO::allPullupOnSWD()
{
  for( int pin = PA0; pin <= PG15; pin++ )
  {
    if( pin != PA13 && pin != PA14 )
    {
      pinMode( pin, INPUT_PULLUP );
    }
  }
}

void GPIO::allPulldownOnSWD()
{
  for( int pin = PA0; pin <= PG15; pin++ )
  {
    if( pin != PA13 && pin != PA14 )
    {
      pinMode( pin, INPUT_PULLDOWN );
    }
  }
}

void GPIO::allPullupOnJTAG()
{
  for( int pin = PA0; pin <= PG15; pin++ )
  {
    if( pin != PA13 && pin != PA14 && pin != PA15 && pin != PB3 && pin != PB4 )
    {
      pinMode( pin, INPUT_PULLUP );
    }
  }
}

void GPIO::allPulldownOnJTAG()
{
  for( int pin = PA0; pin <= PG15; pin++ )
  {
    if( pin != PA13 && pin != PA14 && pin != PA15 && pin != PB3 && pin != PB4 )
    {
      pinMode( pin, INPUT_PULLDOWN );
    }
  }
}

/* ----------------------------------------
    SWD enable.
---------------------------------------- */
void GPIO::swdEnable()
{
  /* The GPIOA pins 13, 14, and 15 are used in JTAG. However, if it is used in SWD, only 13 and 14 pins are applicable. */
  /* The GPIOB pins 3 and 4 are used in JTAG. However, in the case of SWD, these pins are not used. */
  pinAFConfig( PA13, GPIO_AF_SWJ );
  pinAFConfig( PA14, GPIO_AF_SWJ );
}

void GPIO::jtagEnable()
{
  /* The GPIOA pins 13, 14, and 15 are used in JTAG. However, if it is used in SWD, only 13 and 14 pins are applicable. */
  /* The GPIOB pins 3 and 4 are used in JTAG. However, in the case of SWD, these pins are not used. */
  pinAFConfig( PA13, GPIO_AF_SWJ );
  pinAFConfig( PA14, GPIO_AF_SWJ );
  pinAFConfig( PA15, GPIO_AF_SWJ );
  pinAFConfig( PB3, GPIO_AF_SWJ );
  pinAFConfig( PB4, GPIO_AF_SWJ );
}

/* ----------------------------------------
    configure EXTI interrupt.
---------------------------------------- */
void GPIO::exti( int pin, EXTITrigger_TypeDef trig, uint8_t pri, uint8_t sub )
{
  /* Enable clock for SYSCFG */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE );

  uint8_t portSource = whatExtiPortSource( pin );
  uint8_t pinSource = whatExtiPinSource( pin );
  /* Configure EXTI line */
//  EXTI_DeInit();
  EXTI_InitTypeDef EXTI_InitStruct;
  EXTI_StructInit( &EXTI_InitStruct );
  SYSCFG_EXTILineConfig( portSource, pinSource );

  uint32_t pinLine = 0x00000001 << ( pin % 16 );
  EXTI_InitStruct.EXTI_Line = pinLine;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = trig;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init( &EXTI_InitStruct );

  /* initialize nvic*/
  int irqNum = whatExtiIntNumber( pin );
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = irqNum;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pri;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
}

/* ----------------------------------------
    RCC clock enable and disable
---------------------------------------- */
void GPIO::rccClockEnable( int rcc )
{
  uint32_t shift;
  if( rcc < 32 )  /* RCC_AHB1Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_AHB1PeriphClockCmd( shift, ENABLE );
    RCC_AHB1PeriphResetCmd( shift, DISABLE );
  }
  else if( rcc < 64 )  /* RCC_AHB2Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_AHB2PeriphClockCmd( shift, ENABLE );
    RCC_AHB2PeriphResetCmd( shift, DISABLE );
  }
  else if( rcc < 96 )  /* RCC_AHB3Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_AHB3PeriphClockCmd( shift, ENABLE );
    RCC_AHB3PeriphResetCmd( shift, DISABLE );
  }
  else if( rcc < 128 )  /* RCC_APB1Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB1PeriphClockCmd( shift, ENABLE );
    RCC_APB1PeriphResetCmd( shift, DISABLE );
  }
  else if( rcc < 160 )  /* RCC_APB2Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB2PeriphClockCmd( shift, ENABLE );
    RCC_APB2PeriphResetCmd( shift, DISABLE );
  }
  else {}
}

void GPIO::rccClockDisable( int rcc )
{
  uint32_t shift;
  if( rcc < 32 )  /* RCC_AHB1Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_AHB1PeriphClockCmd( shift, DISABLE );
    RCC_AHB1PeriphResetCmd( shift, ENABLE );
  }
  else if( rcc < 64 )  /* RCC_AHB2Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_AHB2PeriphClockCmd( shift, DISABLE );
    RCC_AHB2PeriphResetCmd( shift, ENABLE );
  }
  else if( rcc < 96 )  /* RCC_AHB3Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_AHB3PeriphClockCmd( shift, DISABLE );
    RCC_AHB3PeriphResetCmd( shift, ENABLE );
  }
  else if( rcc < 128 )  /* RCC_APB1Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB1PeriphClockCmd( shift, DISABLE );
    RCC_APB1PeriphResetCmd( shift, ENABLE );
  }
  else if( rcc < 160 )  /* RCC_APB2Periph */
  {
    shift = 0x00000001 << (rcc % 32);
    RCC_APB2PeriphClockCmd( shift, DISABLE );
    RCC_APB2PeriphResetCmd( shift, ENABLE );
  }
  else {}
}


