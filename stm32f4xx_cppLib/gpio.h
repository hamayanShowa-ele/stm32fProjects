/* ----------------------------------------
 gpio utility header
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
#ifndef  __GPIO_H__
#define  __GPIO_H__

#include  <stm32f4xx.h>
#include  <stm32fPeripheral.h>

extern "C"
{
  #include  <priority.h>
}

/* ----------------------------------------
    prototypes 
---------------------------------------- */


/* ----------------------------------------
    defines
---------------------------------------- */
#define  LOW   0
#define  HIGH  !LOW

/* ----------------------------------------
  type
  GPIO_Mode_AIN or GPIO_Mode_IN_FLOATING
  or GPIO_Mode_IPD or GPIO_Mode_IPU
  or GPIO_Mode_Out_OD or GPIO_Mode_Out_PP
  or GPIO_Mode_AF_OD or GPIO_Mode_AF_PP
---------------------------------------- */
enum GPIO_PIN_MODE
{
  INPUT = GPIO_Mode_IN,   /*!< GPIO Input Mode */
  OUTPUT = GPIO_Mode_OUT,   /*!< GPIO Output Mode */
  ALTERNATE_PP = GPIO_Mode_AF,   /*!< GPIO Alternate function Mode */
  ANALOG = GPIO_Mode_AN,    /*!< GPIO Analog Mode */
  INPUT_PULLDOWN,  /* 0x04 */
  INPUT_PULLUP,  /* 0x05 */
  OUTPUT_OPEN_DRAIN,  /* 0x06 */
  ALTERNATE_OD,  /* 0x07 */
};

/* ----------------------------------------
  speed
  GPIO_Speed_10MHz or GPIO_Speed_2MHz
  or GPIO_Speed_50MHz
---------------------------------------- */
enum GPIO_PIN_SPEED
{
  GPIO_SPEED_NORMAL = GPIO_Speed_25MHz,  /*!< Medium speed */
  GPIO_SPEED_SLOW = GPIO_Speed_2MHz,  /*!< Low speed */
  GPIO_SPEED_FAST = GPIO_Speed_50MHz,  /*!< Fast speed */
  GPIO_SPEED_MAX = GPIO_Speed_100MHz,  /*!< High speed on 30 pF (80 MHz Output max speed on 15 pF) */
};

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class GPIO  // : public hoge_class
{
private:
//  void allPullupPulldown( GPIOMode_TypeDef mode, uint16_t pa, uint16_t pb );
  void allPullupPulldown( int type = INPUT_PULLDOWN );

public:
  void rccClockEnable( int rcc );
  void rccClockDisable( int rcc );

  GPIO_TypeDef* whatGPIOType( int pin )
  {
    if( pin >= PA0 && pin <= PA15 ) return GPIOA;
    else if( pin >= PB0 && pin <= PB15 ) return GPIOB;
    else if( pin >= PC0 && pin <= PC15 ) return GPIOC;
    else if( pin >= PD0 && pin <= PD15 ) return GPIOD;
    else if( pin >= PE0 && pin <= PE15 ) return GPIOE;
    else if( pin >= PF0 && pin <= PF15 ) return GPIOF;
    else if( pin >= PG0 && pin <= PG15 ) return GPIOG;
    else if( pin >= PH0 && pin <= PH15 ) return GPIOH;
    else if( pin >= PI0 && pin <= PI15 ) return GPIOI;
    return 0;
  }

  uint16_t whatPin( int pin ) {return 0x0001 << pin % 16;}
  uint8_t whatPinSource( int pin ) {return pin % 16;}
  uint8_t whatExtiPortSource( int pin )
  {
    if( pin >= PA0 && pin <= PA15 ) return EXTI_PortSourceGPIOA;
    else if( pin >= PB0 && pin <= PB15 ) return EXTI_PortSourceGPIOB;
    else if( pin >= PC0 && pin <= PC15 ) return EXTI_PortSourceGPIOC;
    else if( pin >= PD0 && pin <= PD15 ) return EXTI_PortSourceGPIOD;
    else if( pin >= PE0 && pin <= PE15 ) return EXTI_PortSourceGPIOE;
    else if( pin >= PF0 && pin <= PF15 ) return EXTI_PortSourceGPIOF;
    else if( pin >= PG0 && pin <= PG15 ) return EXTI_PortSourceGPIOG;
    else if( pin >= PH0 && pin <= PH15 ) return EXTI_PortSourceGPIOH;
    else if( pin >= PI0 && pin <= PI15 ) return EXTI_PortSourceGPIOI;
    return 0xFF;
  }

  uint8_t whatExtiPinSource( int pin ) {return pin % 16;}
  int  whatExtiIntNumber( int pin );

  void pinMode( GPIO_TypeDef *gpiox, uint16_t gpioPin, int type, int speed = GPIO_SPEED_NORMAL );
  void pinMode( int pin, int type, int speed = GPIO_SPEED_NORMAL );
  void pinAFConfig( int pin, uint8_t type );
  bool odr( GPIO_TypeDef *gpiox, uint16_t gpioPin ) { return (gpiox->ODR & gpioPin) ? true : false; }
  bool odr( int pin );
  bool idr( GPIO_TypeDef *gpiox, uint16_t gpioPin ) { return (gpiox->IDR & gpioPin) ? true : false; }
  bool idr( int pin );

  void set( GPIO_TypeDef *gpiox, uint16_t gpioPin ) {gpiox->BSRRL = gpioPin;}
  void set( int pin );
  void reset( GPIO_TypeDef *gpiox, uint16_t gpioPin ) {gpiox->BSRRH = gpioPin;}
  void reset( int pin );
  void digitalWrite( int pin, bool highOrLow );
  bool digitalRead( int pin );
  void wordWrite( GPIO_TypeDef *gpiox, uint16_t data, uint16_t mask );

  void allPullupOnSWD();
  void allPulldownOnSWD();
  void allPullupOnJTAG();
  void allPulldownOnJTAG();
  void swdEnable();
  void jtagEnable();
  void exti( int pin, EXTITrigger_TypeDef trig = EXTI_Trigger_Falling,
    uint8_t pri = BASE_PRIORITY, uint8_t sub = BASE_SUB_PRIORITY );
};


#endif  /* __GPIO_H__ */

