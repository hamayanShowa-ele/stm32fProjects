/* ----------------------------------------
 board 1314 sdf7 main utilities.
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

#include  "1314.h"

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
  /*  PB0： gpio */
  /*  PB1： gpio */
  /*  PB2: BOOT1：fixed LOW level. */
  /*  PB3: spi1 sck (remap) */
  /*  PB4: spi1 miso (remap) */
  /*  PB5: spi1 mosi (remap) */
  /*  PB6: not connected. */
  /*  PB7: not connected. */
  /*  PB8: I2C1 SCL (remap) */
  /*  PB9: I2C1 SDA (remap) */
  /*  PB10: gpio */
  /*  PB11: gpio */
  /*  PB12: serial sram Slave Select output */
  set( PB12 );
  pinMode( PB12, OUTPUT, GPIO_SPEED_FAST );
  /*  PB13:SPI2_SCK output Alternate function*/
  pinMode( PB13, OUTPUT, GPIO_SPEED_FAST );
  /*  PB14:SPI2_MISO input Alternate function*/
  /*  PB15:SPI2_MOSI output Alternate function*/
  pinMode( PB15, OUTPUT, GPIO_SPEED_FAST );

  GPIOC->ODR = 0x0000;
  /*  PC0： not connected. */
  /*  PC1： not connected. */
  /*  PC2： not connected. */
  /*  PC3： not connected. */
  /*  PC4： gpio */
  /*  PC5： gpio */
  /*  PC6: connected to ground. */
  /*  PC7: connected to ground. */
  /*  PC8: connected to ground. */
  /*  PC9: connected to ground. */
  /*  PC10: gpio or UART3 TXD：Alternate function (remap) */
  /*  PC11: gpio or UART3 RXD：Alternate function (remap) */
  /*  PC12: not connected. */
  /*  PC13: tamper */
  /*  PC14: 32kclock input  */
  pinMode( PC14, ANALOG, GPIO_SPEED_NORMAL );  /* PC14 is connected to OSC_IN */
  /*  PC15: 32Kclock output  */
  pinMode( PC15, ALTERNATE_PP, GPIO_SPEED_NORMAL );  /*PC15 is connected to OSC_OUT */

  GPIOD->ODR = 0x0000;
  /*  PD0： not connected. */
  /*  PD1： not connected. */
  /*  PD2: eth2 interrupt. */
  /*  PD3：  eth1 interrupt. */
  /*  PD4：  ethernet reset. */
  /*  PD5：  ethernet reset. */
  /*  PD6：  eth2 chip select. */
  /*  PD7：  eth1 chip select. */
  /*  PD8：  pulse0. */
  /*  PD9：  pulse1. */
  /*  PD10：  pulse2. */
  /*  PD11：  pulse3. */
  /*  PD12：  pulse4. */
  /*  PD13：  pulse5. */
  /*  PD14：  pulse6. */
  /*  PD15：  pulse7. */

  GPIOE->ODR = 0x0000;
  /*  PE0： act led */
  /*  PE1： USBON output. */
  /*  PE2： connected to ground. */
  /*  PE3： connected to ground. */
  /*  PE4： connected to ground. */
  /*  PE5： connected to ground. */
  /*  PE6： connected to ground. */
  /*  PE7： gpio. */
  /*  PE8： gpio. */
  /*  PE9： gpio. */
  /*  PE10： gpio. */
  /*  PE11： gpio. */
  /*  PE12： gpio. */
  /*  PE13： gpio. */
  /*  PE14： gpio. */
  /*  PE15： gpio. */
}

/* ----------------------------------------
    ethernet initialize.
---------------------------------------- */
void BOARD::etherGpioInit()
{
  pinMode( ETH1_INT, INPUT );
  pinMode( ETH2_INT, INPUT );
  pinMode( ETH1_CS, OUTPUT );
  pinMode( ETH2_CS, OUTPUT );
  pinMode( ETH1_RST, OUTPUT );
  pinMode( ETH2_RST, OUTPUT );
  etherReset();
}

/* ----------------------------------------
    ethernet reset.
---------------------------------------- */
void BOARD::etherReset()
{
  digitalWrite( ETH1_CS, HIGH );
  digitalWrite( ETH2_CS, HIGH );
  digitalWrite( ETH1_RST, LOW );  /* reset wizchip */
  digitalWrite( ETH2_RST, LOW );  /* reset wizchip */
  dly_tsk( 2UL );
  digitalWrite( ETH1_RST, HIGH );
  digitalWrite( ETH2_RST, HIGH );
}



extern "C"
{
  /* If necessary, write the C code here. */

/* ----------------------------------------
    ethernet chip select and deselect.
---------------------------------------- */
void wizchip1_select( void ) { ETH1_CS_IS_0; }
void wizchip1_deselect( void ) { ETH1_CS_IS_1; }

void wizchip2_select( void ) { ETH2_CS_IS_0; }
void wizchip2_deselect( void ) { ETH2_CS_IS_1; }
}  /* extern "C" */
