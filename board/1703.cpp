/* ----------------------------------------
 board 1703 utilities.
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

#include  "1703.h"

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
  allPulldownOnSWD();

  GPIOA->ODR = 0x0000;
  /*  PA0： gpio */
  /*  PA1： gpio */
  /*  PA2： gpio */
  /*  PA3： gpio */
  /*  PA4： gpio */
  /*  PA5： gpio */
  /*  PA6： gpio */
  /*  PA7： gpio */
  /*  PA8: GCLK RESET output */
  pinMode( PA8, OUTPUT );
  set( PA8 );
  /*  PA9: USART1 TXD：Alternate function */
  /*  PA10: USART1 RXD：Alternate function */
  /*  PA11: DIVIDED CLOCK input/output */
  /*  PA12: GLOBAL CLOCK input/output */
  /*  PA13: SWDIO */
  /*  PA14: SWDCLK */
  /*  PA15： JTAG JTDI but use for RTS4(need remap). */

  GPIOB->ODR = 0x0000;
  /*  PB0： gpio */
  /*  PB1： gpio */
  /*  PB2: BOOT1：fixed LOW level.*/
  /*  PB3: JTAG TDO but use for SEG_A(need remap).*/
  /*  PB4: JTAG TRST but use for SEG_B(need remap).*/
  /*  PB5: SEG_C output */
  /*  PB6: SEG_D output */
  /*  PB7: SEG_E output */
  /*  PB8: SEG_F output */
  /*  PB9: SEG_G output */
  /*  PB10: I2C2 SCL */
  /*  PB11: I2C2 SDA */
  /*  PB12: SD CARD Slave select output */
//  set( SD_SS );
//  pinMode( SD_SS, OUTPUT, GPIO_SPEED_FAST );
  /*  PB13:SPI2_SCK output Alternate function*/
//  pinMode( PB13, OUTPUT, GPIO_SPEED_FAST );
  /*  PB14:SPI2_MISO input Alternate function*/
  /*  PB15:SPI2_MOSI output Alternate function*/
//  pinMode( PB15, OUTPUT, GPIO_SPEED_FAST );

  GPIOC->ODR = 0x0000;
  /*  PC0： gpio */
  /*  PC1： gpio */
  /*  PC2： gpio */
  /*  PC3： gpio */
  /*  PC4： not connect. */
  /*  PC5：  W5500 INT input. */
  /*  PC6: SD card detect input. */
  /*  PC7: W5500 chip select. output */
  /*  PC8: W5500 reset. output */
  pinMode( PC8, OUTPUT );
  reset( PC8 );
  /*  PC9: 1PPS input output.  */
  /*  PC10: gpio or UART4 TXD：Alternate function */
  /*  PC11: gpio or UART4 RXD：Alternate function */
  /*  PC12: gpio or UART5 TXD：Alternate function */
  /*  PC13: ACT LED output */
  /*  PC14: not connected. */
  /*  PC15: not connected. */

  GPIOD->ODR = 0x0000;
  /*  PD0：  not used. */
  /*  PD1：  not used. */
  /*  PD2: gpio or UART5 RXD：Alternate function  */
  /*  PD3：  not used. */
  /*  PD4：  not used. */
  /*  PD5：  not used. */
  /*  PD6：  not used. */
  /*  PD7：  not used. */
  /*  PD8： not used. */
  /*  PD9： not used. */
  /*  PD10： not used. */
  /*  PD11： not used. */
  /*  PD12： not used. */
  /*  PD13： not used. */
  /*  PD14： not used. */
  /*  PD15： not used. */
}


extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
