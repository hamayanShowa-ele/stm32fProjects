/* ----------------------------------------
 board 1303 utilities.
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

#include  "1303.h"

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
  /*  PA0： analog input*/
  /*  PA1： analog input*/
  /*  PA2： analog input*/
  /*  PA3： analog input*/
  /*  PA4： spi1 nss */
  digitalWrite( PA4, HIGH );
  pinMode( PA4, OUTPUT, GPIO_SPEED_FAST );
  /*  PA5： spi1 sck */
  pinMode( PA5, OUTPUT, GPIO_SPEED_FAST );
  /*  PA6： spi1 miso */
  /*  PA7： spi1 mosi*/
  pinMode( PA7, OUTPUT, GPIO_SPEED_FAST );
  /*  PA8: not connected or MCO output. */
//  pinMode( PA8, ALTERNATE_PP, GPIO_SPEED_FAST );
  /*  PA9: USART1 TXD：Alternate function */
  /*  PA10: USART1 RXD：Alternate function */
  /*  PA11: USB DM */
  /*  PA12: USB DP */
  /*  PA13: SWDIO */
  /*  PA14: SWDCLK */
  /*  PA15： JTAG JTDI but not use. */

  GPIOB->ODR = 0x0000;
  /*  PB0： serial ram chip select. output */
  digitalWrite( PB0, HIGH );
  pinMode( PB0, OUTPUT, GPIO_SPEED_FAST );
  /*  PB1： none*/
  /*  PB2: BOOT1：fixed LOW level.*/
  /*  PB3: JTAG TDO but not use.*/
  /*  PB4: JTAG TRST but not use.*/
  /*  PB5: SD CARD write protect. input */
  /*  PB6: SD CARD detect. input */
  /*  PB7: buzzer TIM4 CH2 output. */
  /*  PB8: I2C1 SCL */
  /*  PB9: I2C1 SDA */
  /*  PB10: I2C2 SCL */
  /*  PB11: I2C2 SDA */
  /*  PB12: SD CARD Slave select output */
  set( SD_SS );
  pinMode( SD_SS, OUTPUT, GPIO_SPEED_FAST );
  /*  PB13:SPI2_SCK output Alternate function*/
  pinMode( PB13, OUTPUT, GPIO_SPEED_FAST );
  /*  PB14:SPI2_MISO input Alternate function*/
  /*  PB15:SPI2_MOSI output Alternate function*/
  pinMode( PB15, OUTPUT, GPIO_SPEED_FAST );

  GPIOC->ODR = 0x0000;
  /*  PC0： analog input*/
  /*  PC1： analog input*/
  /*  PC2： analog input*/
  /*  PC3： analog input*/
  /*  PC4： gpio or axis X or Y. */
  /*  PC5： not connect. */
  /*  PC6: gpio or bright output. */
  /*  PC7: gpio or xbee2 rts. */
  /*  PC8: gpio or xbee2 cts. */
  /*  PC9: gpio or xbee1 rts.  */
  /*  PC10: gpio or UART4 TXD：Alternate function */
  /*  PC11: gpio or UART4 RXD：Alternate function */
  /*  PC12: gpio or UART5 TXD：Alternate function */
  /*  PC13: gpio or tamper */
  /*  PC14、32kclock input  */
  pinMode( PC14, ANALOG, GPIO_SPEED_NORMAL );  /* PC14 is connected to OSC_IN */
  /*  PC15、32Kclock output  */
  pinMode( PC15, ALTERNATE_PP, GPIO_SPEED_NORMAL );  /*PC15 is connected to OSC_OUT */

  GPIOD->ODR = 0x0000;
  /*  PD0： fsmc data bus bit 2 */
  /*  PD1： fsmc data bus bit 3 */
  /*  PD2: gpio or UART5 RXD：Alternate function */
  /*  PD3： TE LCD controller transmit end input */
  /*  PD4： fsmc output enable */
  /*  PD5： fsmc write enable */
  /*  PD6：  xbee cts output. */
  /*  PD7：  fsmc NE1*/
  /*  PD8： fsmc data bus bit 13 */
  /*  PD9： fsmc data bus bit 14 */
  /*  PD10： fsmc data bus bit 15 */
  /*  PD11： fsmc address bus bit 16 */
  /*  PD12： BUZZER pwm1 output.  */
  /*  PD13： not connect.  */
  /*  PD14： fsmc data bus bit 0 */
  /*  PD15： fsmc data bus bit 1 */

  GPIOE->ODR = 0x0000;
  /*  PE0： none*/
  /*  PE1：USBON output. */
  /*  PE2： connect to GND. */
  /*  PE3： connect to GND. */
  /*  PE4： connect to GND. */
  /*  PE5： connect to GND. */
  /*  PE6： connect to GND. */
  /*  PE7： fsmc data bus bit 4*/
  /*  PE8： fsmc data bus bit 5*/
  /*  PE9： fsmc data bus bit 6*/
  /*  PE10： fsmc data bus bit 7*/
  /*  PE11： fsmc data bus bit 8*/
  /*  PE12： fsmc data bus bit 9*/
  /*  PE13： fsmc data bus bit 10*/
  /*  PE14： fsmc data bus bit 11*/
  /*  PE15： fsmc data bus bit 12*/
}

/* ----------------------------------------
    external cpu bus initialize
---------------------------------------- */
void BOARD::busPortInitialize()
{
  pinMode( EXT_BUS_D0, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D1, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D2, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D3, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D4, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D5, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D6, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D7, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D8, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D9, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D10, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D11, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D12, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D13, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D14, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_D15, ALTERNATE_PP, GPIO_SPEED_FAST );

  pinMode( EXT_BUS_A16, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_OE, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_WE, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_NE1, ALTERNATE_PP, GPIO_SPEED_FAST );
}

void BOARD::extBusInit()
{
  busPortInitialize();

  FSMC_BCRx *bcr = (FSMC_BCRx *)(FSMC_Bank1_R_BASE + (8 * 0));
  FSMC_TCRx *tcr = (FSMC_TCRx *)(FSMC_Bank1_R_BASE + 4 + (8 * 0));
  FSMC_BWTRx *bwtr = (FSMC_BWTRx *)(FSMC_Bank1E_R_BASE + (8 * 0));

  /* initialize NE1 for GLCD 0x60000000*/
  bcr->BIT.ASYNCWAIT = 0;  /*not use external wait */
  bcr->BIT.EXTMOD = 1;  /**/
  bcr->BIT.MWID = 1;  /*16bit bus*/
  bcr->BIT.MTYP = 0;  /*sram select*/
  bcr->BIT.MUXEN = 0;  /*non multiplexed bus*/
  bcr->BIT.MBKEN = 1;  /**/

  /* read size configuration */
  tcr->BIT.ACCMOD = 0;  /*access mode a*/
  tcr->BIT.DATAST = 9;  /**/
  tcr->BIT.ADDSET = 1;  /**/

  /* write size configuration */
  bwtr->BIT.ACCMOD = 0;  /**/
  bwtr->BIT.DATAST = 2;  /* 2 */
  bwtr->BIT.ADDSET = 1;  /* 1 */
}



extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
