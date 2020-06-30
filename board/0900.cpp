/* ----------------------------------------
 board 0900 utilities.
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

#include  "0900.h"

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
  /*  PA4： analog input or dac output*/
  /*  PA5： analog input or dac output*/
  /*  PA6： analog input*/
  /*  PA7： USBON output*/
  /*  PA8: MCO output*/
  pinMode( PA8, ALTERNATE_PP, GPIO_SPEED_FAST );
  /*  PA9: USART1 TXD：Alternate function*/
  /*  PA10: USART1 RXD：Alternate function*/
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
  /*  PB8: BRIGHT output*/
  /*  PB9: BUZZER output*/
  /*  PB10: SD CARD DETECT input*/
  /*  PB11: SD CARD write protect input*/
  /*  PB12: SD CARD Slave select output*/
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
  /*  PC4： analog input*/
  /*  PC5： analog input*/
  /*  PC6: none*/
  /*  PC7: none*/
  /*  PC8: none*/
  /*  PC9: none*/
  /*  PC10: UART4 TXD：Alternate function*/
  /*  PC11: UART4 RXD：Alternate function*/
  /*  PC12: UART5 TXD：Alternate function*/
  /*  PC13: none*/
  /*  PC14、32kclock input  */
  pinMode( PC14, ANALOG, GPIO_SPEED_NORMAL );  /* PC14 is connected to OSC_IN */
  /*  PC15、32Kclock output  */
  pinMode( PC15, ALTERNATE_PP, GPIO_SPEED_NORMAL );  /*PC15 is connected to OSC_OUT */

  GPIOD->ODR = 0x0000;
  /*  PD0： fsmc data bus bit 2*/
  /*  PD1： fsmc data bus bit 3*/
  /*  PD2: UART5 RXD：Alternate function*/
  /*  PD3： TE LCD controller transmit end input*/
  /*  PD4： fsmc output enable*/
  /*  PD5： fsmc write enable*/
  /*  PD6： LCD reset output*/
  reset( LCD_RST );
  pinMode( LCD_RST, OUTPUT, GPIO_SPEED_NORMAL );
  /*  PD7：  fsmc NE1*/
  /*  PD8： fsmc data bus bit 13*/
  /*  PD9： fsmc data bus bit 14*/
  /*  PD10： fsmc data bus bit 15*/
  /*  PD11： fsmc address bus bit 16*/
  /*  PD12： LCD interrupt input*/
  /*  PD13： LCD PWRSVE output*/
  set( PWRSVE );  /* if PWRSVE is high then lcd controller will be enter sleep mode. */
  pinMode( PWRSVE, OUTPUT, GPIO_SPEED_NORMAL );
  /*  PD14： fsmc data bus bit 0*/
  /*  PD15： fsmc data bus bit 1*/

  GPIOE->ODR = 0x0000;
  /*  PE0： none*/
  /*  PE1： none*/
  /*  PE2： none*/
  /*  PE3： none*/
  /*  PE4： none*/
  /*  PE5： none*/
  /*  PE6： none*/
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

/* ----------------------------------------
    graphic lcd clock initialize
---------------------------------------- */
void BOARD::glcdClockInit()
{
//  RCC_MCOConfig( RCC_MCO_SYSCLK );  /* System clock source */
//  RCC_MCOConfig( RCC_MCO_HSI );  /* HSI oscillator clock source */
//  RCC_MCOConfig( RCC_MCO_PLLCLK_Div2 );  /* LCD controller CKLI = 8MHz : PLL clock divided by 2 clock source */
  RCC_MCOConfig( RCC_MCO_HSE );  /* LCD controller CKLI = 8MHz : HSE oscillator clock source */
}

/* ----------------------------------------
    graphic controller ic reset
---------------------------------------- */
void BOARD::glcdReset()
{
  reset( LCD_RST );
  dly100us( 20UL );
  set( LCD_RST );
}

/* ----------------------------------------
    graphic controller sleep on/off
---------------------------------------- */
void BOARD::glcdSleep( bool onOff )
{
  if( onOff ) set( PWRSVE );
  else reset( PWRSVE );
}

/* ----------------------------------------
    graphic data dma interrupt initialize
---------------------------------------- */
void BOARD::glcdInterruptInit()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStruct;

  /* DMA interrupt setting */
  NVIC_InitStructure.NVIC_IRQChannel = LCD_DMA_IREQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  /* Since TE is connected to PD3, it is used as an interrupt. */
  NVIC_InitStructure.NVIC_IRQChannel = LCD_TE_IREQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  /* Since TE is connected to PD3, it is used as an interrupt. */
  GPIO_EXTILineConfig( LCD_TE_PORT_SOURCE, LCD_TE_PIN_SOURCE );
  EXTI_InitStruct.EXTI_Line = LCD_TE_ETXI_LINE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init( &EXTI_InitStruct );
}


extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
