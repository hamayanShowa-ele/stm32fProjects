/* ----------------------------------------
 board aitendo mb-stm32f103 utilities.
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

#include  "mbStm32f103.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
BOARD::BOARD()
{
}

BOARD::~BOARD()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */

/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void BOARD::gpioInit()
{
  swdEnable();
  allPullupOnSWD();

  GPIOA->ODR = 0x0000;
  /*  PA0： none*/
  /*  PA1： none*/
  /*  PA2： USART2_TX (RS232C)*/
  /*  PA3： USART2_RX (RS232C)*/
  /*  PA4： M25Pxx SS or dac output*/
  /*  PA5： M25Pxx CMD or dac output*/
  /*  PA6： M25Pxx Q*/
  /*  PA7： M25Pxx D*/
  /*  PA8: none*/
  /*  PA9: USART1 TXD：Alternate function (USB/SERIAL)*/
  /*  PA10: USART1 RXD：Alternate function (USB/SERIAL)*/
  /*  PA11: USB DM*/
  /*  PA12: USB DP*/
  /*  PA13: SWDIO*/
  /*  PA14: SWDCLK*/
  /*  PA15： JTAG JTDI but not use.*/

  GPIOB->ODR = 0x0000;
  /*  PB0： none*/
  /*  PB1： none*/
  /*  PB2: BOOT1：fixed LOW level.*/
  /*  PB3: JTAG TDO but not use.*/
  /*  PB4: JTAG TRST but not use.*/
  /*  PB5: none*/
  /*  PB6: none*/
  /*  PB7: none*/
  /*  PB8: none*/
  /*  PB9: none*/
  /*  PB10: none*/
  /*  PB11: none*/
  /*  PB12: none*/
  /*  PB13: none*/
  /*  PB14: none*/
  /*  PB15: none*/

  GPIOC->ODR = 0x0000;
  /*  PC0： none*/
  /*  PC1： none*/
  /*  PC2： none*/
  /*  PC3： none*/
  /*  PC4： none*/
  /*  PC5： none*/
  /*  PC6: LED D1*/
  /*  PC7: LED D2*/
  /*  PC8: SD MISO：Alternate function*/
  /*  PC9: none*/
  /*  PC10: none*/
  /*  PC11: SD SS：Alternate function*/
  /*  PC12: SD SCLK：Alternate function*/
  /*  PC13: USB ON (active low)*/
  /*  PC14、32kclock input  */
  pinMode( PC14, ANALOG, GPIO_SPEED_NORMAL );  /* PC14 is connected to OSC_IN */
  /*  PC15、32Kclock output  */
  pinMode( PC15, ALTERNATE_PP, GPIO_SPEED_NORMAL );  /*PC15 is connected to OSC_OUT */

  GPIOD->ODR = 0x0000;
  /*  PD0： none*/
  /*  PD1： none*/
  /*  PD2: SD MOSI：Alternate function*/
  /*  PD3： none*/
  /*  PD4： none*/
  /*  PD5： none*/
  /*  PD6： LED D4*/
  /*  PD7：  none*/
  /*  PD8： none*/
  /*  PD9： none*/
  /*  PD10： none*/
  /*  PD11： none*/
  /*  PD12： none*/
  /*  PD13： LED D3*/
  /*  PD14： none*/
  /*  PD15： none*/

  GPIOE->ODR = 0x0000;
  /*  PE0： BUZZER*/
  /*  PE1： none*/
  /*  PE2： KEY S4*/
  /*  PE3： KEY S3*/
  /*  PE4： KEY S2*/
  /*  PE5： KEY S1*/
  /*  PE6： none*/
  /*  PE7： none*/
  /*  PE8： none*/
  /*  PE9： none*/
  /*  PE10： none*/
  /*  PE11： none*/
  /*  PE12： none*/
  /*  PE13： none*/
  /*  PE14： none*/
  /*  PE15： none*/
}


extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
