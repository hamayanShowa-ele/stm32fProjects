/* ----------------------------------------
 GPIO utility header
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
#ifndef  __STM32L0xxGPIO_H__
#define  __STM32L0xxGPIO_H__

#include  <stm32l0xx.h>
#include  <stm32l0xx_hal_gpio.h>
#include  <stm32fPeripheral.h>
#include  <derivertive.h>
#include  <mul_tsk.h>

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
  ANALOG = GPIO_MODE_ANALOG,
  INPUT = GPIO_MODE_INPUT | (GPIO_NOPULL << 8),
  INPUT_PULLDOWN = GPIO_MODE_INPUT | (GPIO_PULLDOWN << 8),
  INPUT_PULLUP = GPIO_MODE_INPUT | (GPIO_PULLUP << 8),
  OUTPUT_OPEN_DRAIN = GPIO_MODE_OUTPUT_OD,
  OUTPUT = GPIO_MODE_OUTPUT_PP,
  ALTERNATE_OD = GPIO_MODE_AF_OD,
  ALTERNATE_PP = GPIO_MODE_AF_PP,
};
//#define  GPIO_MODE_INPUT     (0x00000000U)   /*!< Input Floating Mode                   */
//#define  GPIO_MODE_OUTPUT_PP (0x00000001U)   /*!< Output Push Pull Mode                 */
//#define  GPIO_MODE_OUTPUT_OD (0x00000011U)   /*!< Output Open Drain Mode                */
//#define  GPIO_MODE_AF_PP     (0x00000002U)   /*!< Alternate Function Push Pull Mode     */
//#define  GPIO_MODE_AF_OD     (0x00000012U)   /*!< Alternate Function Open Drain Mode    */
//#define  GPIO_MODE_ANALOG    (0x00000003U)   /*!< Analog Mode  */
//#define  GPIO_NOPULL         (0x00000000U)   /*!< No Pull-up or Pull-down activation  */
//#define  GPIO_PULLUP         (0x00000001U)   /*!< Pull-up activation                  */
//#define  GPIO_PULLDOWN       (0x00000002U)   /*!< Pull-down activation                */

/* ----------------------------------------
  speed
  GPIO_Speed_10MHz or GPIO_Speed_2MHz
  or GPIO_Speed_50MHz
---------------------------------------- */
#if 0
enum GPIO_PIN_SPEED
{
  GPIO_SPEED_SLOW = GPIO_SPEED_FREQ_LOW,  /*!< range up to 0.4 MHz, please refer to the product datasheet */
//  GPIO_SPEED_MEDIUM = GPIO_SPEED_FREQ_MEDIUM,  /*!< range 0.4 MHz to 2 MHz, please refer to the product datasheet */
  GPIO_SPEED_NORMAL = GPIO_SPEED_FREQ_HIGH,  /*!< range   2 MHz to 10 MHz, please refer to the product datasheet */
//  GPIO_SPEED_FAST = GPIO_SPEED_FREQ_VERY_HIGH,  /*!< range  10 MHz to 35 MHz, please refer to the product datasheet */
};
#endif

/*  */
/* ----------------------------------------
    instances or global variables
---------------------------------------- */
GPIO_TypeDef* whatGPIOType( int pin );
#define  whatPin(pin)       (0x0001 << pin % 16)  /* response pin's define */
#define  whatPinSource(pin) (pin % 16)  /* response pin's source */
#define  whatPinLine(pin)   (0x0001 << whatPinSource(pin))  /* response pin's line */

//  void pinMode( GPIO_TypeDef *gpiox, uint16_t gpioPin, int type, int speed = GPIO_SPEED_HIGH );
  void pinMode( int pin, int type );
  void pinAlternateMode( int pin, uint32_t mode, uint8_t alt );
  void set( int pin );
  void reset( int pin );
  void pinToggle( int pin );
  void digitalWrite( int pin, bool highOrLow );
  bool digitalRead( int pin );
//  void wordWrite( GPIO_TypeDef *gpiox, uint16_t data, uint16_t mask );

//  void allPullupOnSWD();
//  void allPulldownOnSWD();
//  void swdEnable();

//  void rccClockEnable( int rcc );
//  void rccClockDisable( int rcc );
//  void remap( uint32_t map ) { GPIO_PinRemapConfig( map, ENABLE ); }


#endif  /* __STM32L0xxGPIO_H__ */

