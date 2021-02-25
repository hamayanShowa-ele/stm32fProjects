/* ----------------------------------------
 board 1415 utilities.
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

#include  "1415.h"

extern "C"
{
}

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
    begin and end
---------------------------------------- */

/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void BOARD_1415::gpioInit()
{
  swdEnable();
  allPulldownOnSWD();

  GPIOA->ODR = 0x0000;
  /*  PA0： usart2 cts */
  /*  PA1： usart2 rts */
  /*  PA2： usart2 txd */
  /*  PA3： usart2 rxd */
  /*  PA4： spi1 nss */
  digitalWrite( PA4, HIGH );
  pinMode( PA4, OUTPUT, GPIO_SPEED_FAST );
  /*  PA5： spi1 sck */
  pinMode( PA5, OUTPUT, GPIO_SPEED_FAST );
  /*  PA6： spi1 miso */
  /*  PA7： spi1 mosi*/
  pinMode( PA7, OUTPUT, GPIO_SPEED_FAST );
  /*  PA8: usb on. */
//  pinMode( PA8, OUTPUT );
  /*  PA9: USART1 TXD：Alternate function */
  /*  PA10: USART1 RXD：Alternate function */
  /*  PA11: USB DM */
  /*  PA12: USB DP */
  /*  PA13: SWDIO */
  /*  PA14: SWDCLK */
  /*  PA15： JTAG JTDI but not use. */

  GPIOB->ODR = 0x0000;
  /*  PB0： serial eeprom chip select. output */
  digitalWrite( PB0, HIGH );
  pinMode( PB0, OUTPUT, GPIO_SPEED_FAST );
  /*  PB1： none*/
  /*  PB2: BOOT1：fixed LOW level.*/
  /*  PB3: JTAG TDO but not use.*/
  /*  PB4: JTAG TRST but not use.*/
  /*  PB5: exi2c semaphore. */
  /*  PB6: exi2c1 scl1. */
  /*  PB7: exi2c1 sda1. */
  /*  PB8: dfu */
  /*  PB9: act led */
  /*  PB10: usart3 txd */
  /*  PB11: usart3 rxd */
  /*  PB12: usart3 cd */
  /*  PB13: usart3 cts */
  /*  PB14: usart3 rts */
  /*  PB15: usart3 dsr */

  GPIOC->ODR = 0x0000;
  /*  PC0：  ETN_INT ethernet interrup. */
  /*  PC1：  ARC_INT arcnet interrup. */
  /*  PC2：  ETN_RST ethernet reset. */
  digitalWrite( PC2, HIGH );
  pinMode( PC2, OUTPUT );
  /*  PC3：  ARC_RST arcnet reset. */
  digitalWrite( PC3, HIGH );
  pinMode( PC3, OUTPUT );
  /*  PC4： INT0 (external interrupt). */
  /*  PC5： INT1 (external interrupt). */
  /*  PC6: usart3 ri. */
  /*  PC7: not connected. */
  /*  PC8: not connected. */
  /*  PC9: not connected. */
  /*  PC10: uart4 txd. */
  /*  PC11: uart4 rxd. */
  /*  PC12: not connected. */
  /*  PC13: nmi. */
  /*  PC14、32kclock input  */
  pinMode( PC14, ANALOG, GPIO_SPEED_NORMAL );  /* PC14 is connected to OSC_IN */
  /*  PC15、32Kclock output  */
  pinMode( PC15, ALTERNATE_PP, GPIO_SPEED_NORMAL );  /*PC15 is connected to OSC_OUT */

  GPIOD->ODR = 0x0000;
  /*  PD0： fsmc data bus bit 2(D2) */
  /*  PD1： fsmc data bus bit 3(D3) */
  /*  PD2: not connected. */
  /*  PD3： cbus reset(C_RST). */
  digitalWrite( PD3, HIGH );
  pinMode( PD3, OUTPUT );
  /*  PD4： fsmc output enable(RD). */
  /*  PD5： fsmc write enable(WR). */
  /*  PD6：  fsmc wait(WAIT). */
  /*  PD7：  fsmc NE1(SRAM CS). */
  /*  PD8： fsmc data bus bit 13(D13) */
  /*  PD9： fsmc data bus bit 14(D14) */
  /*  PD10： fsmc data bus bit 15(D15) */
  /*  PD11： fsmc address bus bit 16(A16) */
  /*  PD12： fsmc address bus bit 17(A17) */
  /*  PD13： fsmc address bus bit 18(A18) */
  /*  PD14： fsmc data bus bit 0(D0) */
  /*  PD15： fsmc data bus bit 1(D1) */

  GPIOE->ODR = 0x0000;
  /*  PE0： fsmc bus low enable(BLE). */
  /*  PE1： fsmc bus high enable(BHE). */
  /*  PE2： not connected. */
  /*  PE3： not connected. */
  /*  PE4： not connected. */
  /*  PE5： not connected. */
  /*  PE6： fsmc address bus bit 21(A21) */
  /*  PE7： fsmc data bus bit 4(D4) */
  /*  PE8： fsmc data bus bit 5(D5) */
  /*  PE9： fsmc data bus bit 6(D6) */
  /*  PE10： fsmc data bus bit 7(D7) */
  /*  PE11： fsmc data bus bit 8(D8) */
  /*  PE12： fsmc data bus bit 9(D9) */
  /*  PE13： fsmc data bus bit 10(D10) */
  /*  PE14： fsmc data bus bit 11(D11) */
  /*  PE15： fsmc data bus bit 12(D12) */

  GPIOF->ODR = 0x0000;
  /*  PF0： fsmc address bus bit 0(A0) */
  /*  PF1： fsmc address bus bit 1(A1) */
  /*  PF2： fsmc address bus bit 2(A2) */
  /*  PF3： fsmc address bus bit 3(A3) */
  /*  PF4： fsmc address bus bit 4(A4) */
  /*  PF5： fsmc address bus bit 5(A5) */
  /*  PF6： INT2 (external interrupt). */
  /*  PF7： INT3 (external interrupt). */
  /*  PF8： INT4 (external interrupt). */
  /*  PF9： INT5 (external interrupt). */
  /*  PF10： INT6 (external interrupt). */
  /*  PF11： cbus enable(BUS_ENB). */
  digitalWrite( PF11, LOW );  // active high
  pinMode( PF11, OUTPUT );
  /*  PF12： fsmc address bus bit 6(A6) */
  /*  PF13： fsmc address bus bit 7(A7) */
  /*  PF14： fsmc address bus bit 8(A8) */
  /*  PF15： fsmc address bus bit 9(A9) */

  GPIOG->ODR = 0x0000;
  /*  PG0： fsmc address bus bit 10(A10) */
  /*  PG1： fsmc address bus bit 11(A11) */
  /*  PG2： fsmc address bus bit 12(A12) */
  /*  PG3： fsmc address bus bit 13(A13) */
  /*  PG4： fsmc address bus bit 14(A14) */
  /*  PG5： fsmc address bus bit 15(A15) */
  /*  PG6： not connected. */
  /*  PG7： not connected. */
  /*  PG8: usart3 dtr */
  /*  PG9：  fsmc NE2(ETH_CS). */
  /*  PG10：  fsmc NE3(ARC_CS). */
  /*  PG11： not connected. */
  /*  PG12：  fsmc NE4(CBUS_CS). */
  /*  PG13： not connected. */
  /*  PG14： not connected. */
  /*  PG15: exi2c interrupt. */
}

/* ----------------------------------------
    external cpu bus initialize
---------------------------------------- */
void BOARD_1415::busPortInitialize()
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

  pinMode( EXT_BUS_A0, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A1, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A2, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A3, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A4, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A5, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A6, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A7, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A8, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A9, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A10, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A11, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A12, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A13, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A14, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A15, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A16, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A17, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A18, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_A21, ALTERNATE_PP, GPIO_SPEED_FAST );

  pinMode( EXT_BUS_OE, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_WE, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_NE1, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_NE2, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_NE3, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_NE4, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_BLE, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_BHE, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( EXT_BUS_WAIT, ALTERNATE_PP, GPIO_SPEED_FAST );  // wait input ,if you need.
}

void BOARD_1415::extBusInit()
{
  busPortInitialize();

#if 1
  STM32F1_FSMC *fsmc = new STM32F1_FSMC;
  fsmc->sram( FSMC_Bank1_NORSRAM1,FSMC_MemoryDataWidth_16b, 1,4, 1,4 );  /* NE1 0x60000000 sram bank1 */
  fsmc->sram( FSMC_Bank1_NORSRAM2,FSMC_MemoryDataWidth_16b, 1,4, 1,4 );  /* NE2 0x64000000 w5300 */
  fsmc->sram( FSMC_Bank1_NORSRAM3,FSMC_MemoryDataWidth_16b, 1,2, 4,2 );  /* NE3 0x68000000 arcnet */
  fsmc->sram( FSMC_Bank1_NORSRAM4,FSMC_MemoryDataWidth_16b, 10,13, 10,15, true );  /* NE4 0x6C000000 c-bus */
//  fsmc->sram( FSMC_Bank1_NORSRAM4,FSMC_MemoryDataWidth_16b, 5,5, 5,5, true );  /* NE4 0x6C000000 c-bus */
  delete fsmc;
#else
  /* FSMC clock enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_FSMC, ENABLE );

  /* Enable the sram area of sram bank1. */
  FSMC_BCRx *bcr = (FSMC_BCRx *)(FSMC_Bank1_R_BASE + (8 * 0));
  FSMC_TCRx *tcr = (FSMC_TCRx *)(FSMC_Bank1_R_BASE + 4 + (8 * 0));
  FSMC_BWTRx *bwtr = (FSMC_BWTRx *)(FSMC_Bank1E_R_BASE + (8 * 0));

  /* Enable the sram area of sram bank1. : 0x60000000 */
  bcr->BIT.ASYNCWAIT = 0;  /* 1: Use an external wait */
  bcr->BIT.EXTMOD = 1;  /**/
  bcr->BIT.MWID = 1;  /*16bit bus*/
  bcr->BIT.MTYP = 0;  /*sram select*/
  bcr->BIT.MUXEN = 0;  /*non multiplexed bus*/
  bcr->BIT.MBKEN = 1;  /**/

  /* Read side settings */
  tcr->BIT.ACCMOD = 0;  /*access mode a*/
  tcr->BIT.ADDSET = 1;  /**/
//  tcr->BIT.DATAST = 2;  /*for sram*/
  tcr->BIT.DATAST = 4;  /*for mram*/

  /* Write side settings */
  bwtr->BIT.ACCMOD = 0;  /**/
  bwtr->BIT.ADDSET = 1;  /**/
//  bwtr->BIT.DATAST = 2;  /*???ns for sram*/
  bwtr->BIT.DATAST = 4;  /*35ns for mram*/

  /* Enable the w5300 area of sram bank2. : 0x64000000*/
  bcr = (FSMC_BCRx *)(FSMC_Bank1_R_BASE + (8 * 1));
  tcr = (FSMC_TCRx *)(FSMC_Bank1_R_BASE + 4 + (8 * 1));
  bwtr = (FSMC_BWTRx *)(FSMC_Bank1E_R_BASE + (8 * 1));

  bcr->BIT.ASYNCWAIT = 0;  /* 1: Use an external wait */
  bcr->BIT.EXTMOD = 1;  /**/
  bcr->BIT.MWID = 1;  /*16bit bus*/
  bcr->BIT.MTYP = 0;  /*sram select*/
  bcr->BIT.MUXEN = 0;  /*non multiplexed bus*/
  bcr->BIT.MBKEN = 1;  /**/
  bcr->BIT.WREN = 1;  /**/

  /* Read side settings */
  tcr->BIT.ACCMOD = 0;  /*access mode a*/
  tcr->BIT.ADDSET = 1;  /*1*/
  tcr->BIT.DATAST = 4;  /*4*/

  /* Write side settings */
  bwtr->BIT.ACCMOD = 0;  /**/
  bwtr->BIT.ADDSET = 1;  /*1*/
  bwtr->BIT.DATAST = 4;  /*4*/

  /* Enable the arcnet area of sram bank3. : 0x68000000 */
  bcr = (FSMC_BCRx *)(FSMC_Bank1_R_BASE + (8 * 2));
  tcr = (FSMC_TCRx *)(FSMC_Bank1_R_BASE + 4 + (8 * 2));
  bwtr = (FSMC_BWTRx *)(FSMC_Bank1E_R_BASE + (8 * 2));

  bcr->BIT.ASYNCWAIT = 0;  /* 1: Use an external wait */
  bcr->BIT.EXTMOD = 1;  /**/
  bcr->BIT.MWID = 1;  /*16bit bus*/
  bcr->BIT.MTYP = 0;  /*sram select*/
  bcr->BIT.MUXEN = 0;  /*non multiplexed bus*/
  bcr->BIT.MBKEN = 1;  /**/
  bcr->BIT.WREN = 1;  /**/

  /* Read side settings */
  tcr->BIT.ACCMOD = 0;  /*access mode a*/
  tcr->BIT.ADDSET = 1;  /**/
  tcr->BIT.DATAST = 2;  /**/

  /* Write side settings */
  bwtr->BIT.ACCMOD = 0;  /**/
  bwtr->BIT.ADDSET = 4;  /**/
  bwtr->BIT.DATAST = 2;  /**/

  /*Enable the c-bus area of sram bank4. : 0x6C000000 */
  bcr = (FSMC_BCRx *)(FSMC_Bank1_R_BASE + (8 * 3));
  tcr = (FSMC_TCRx *)(FSMC_Bank1_R_BASE + 4 + (8 * 3));
  bwtr = (FSMC_BWTRx *)(FSMC_Bank1E_R_BASE + (8 * 3));

  bcr->BIT.ASYNCWAIT = 0;  /* 1: Use an external wait */
  bcr->BIT.EXTMOD = 1;  /**/
  bcr->BIT.MWID = 1;  /*16bit bus*/
  bcr->BIT.MTYP = 0;  /*sram select*/
  bcr->BIT.MUXEN = 0;  /*non multiplexed bus*/
  bcr->BIT.MBKEN = 1;  /**/
  bcr->BIT.WREN = 1;  /**/

  /* Read side settings */
  tcr->DWORD = 0x0FFFFFFF;
  tcr->BIT.ACCMOD = 0;  /* 2bit long : access mode A */
//  tcr->BIT.DATLAT = 0;  /* 4bit long */
//  tcr->BIT.CLKDIV = 0;  /* 4bit long */
//  tcr->BIT.BUSTURN = 10;  /* 4bit long */
//  tcr->BIT.ADDHLD = 15;  /* 4bit long */
  tcr->BIT.ADDSET = 10;  /* 4bit long : original 10 ->  */
  tcr->BIT.DATAST = 10;  /* 8bit long : original 13 -> */

  /* Write side settings */
  bwtr->DWORD = 0x0FFFFFFF;
  bwtr->BIT.ACCMOD = 0;  /* 2bit long : access mode A */
//  bwtr->BIT.DATLAT = 0;  /* 4bit long */
//  bwtr->BIT.CLKDIV = 0;  /* 4bit long */
//  bwtr->BIT.BUSTURN = 0;  /* 4bit long */
//  bwtr->BIT.ADDHLD = 15;  /* 4bit long */
  bwtr->BIT.ADDSET = 10;  /* 4bit long : original 10 -> */
  bwtr->BIT.DATAST = 15;  /* 8bit long : original 15 -> */
#endif
}

/* ----------------------------------------
    cbus enable
---------------------------------------- */
void BOARD_1415::cbusEnable( bool onOff )
{
  pinMode( PF11, OUTPUT );
  digitalWrite( PF11, (onOff == true) ? HIGH : LOW );
}

/* ----------------------------------------
    cbus reset
---------------------------------------- */
void BOARD_1415::cbusReset()
{
  pinMode( PD3, OUTPUT );
  digitalWrite( PD3, LOW );
  dly_tsk( 10UL );
  digitalWrite( PD3, HIGH );
  dly_tsk( 100UL );
}

/* ----------------------------------------
    cbus dummy read.
---------------------------------------- */
volatile uint16_t BOARD_1415::dummyRead()
{
  volatile uint16_t *dummy = (volatile uint16_t *)CBUS_DUMMY_MEM_ADR;
  uint16_t dum = *dummy;
  return dum;
}



extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
