/* ----------------------------------------
 board 2102 utilities.
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

#include  "2102.h"

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
void BOARD_2102::gpioInit()
{
  swdEnable();
//  allPulldownOnSWD();

  GPIOA->ODR = 0x0000;
  /*  PA0： pulse input 4 */
  /*  PA1： pulse input 5 */
  /*  PA2： pulse input 6 */
  /*  PA3： pulse input 7 */
  /*  PA4： pulse output 0 */
  /*  PA5： pulse output 1 */
  /*  PA6： pulse output 2 */
  /*  PA7： pulse output 3 */
  /*  PA8: USB ON */
  /*  PA9: USART1 TXD：Alternate function */
  /*  PA10: USART1 RXD：Alternate function */
  /*  PA11: USBDM input/output */
  /*  PA12: USBDP input/output */
  /*  PA13: SWDIO */
  /*  PA14: SWDCLK */
  /*  PA15： JTAG JTDI but use for RTS4(need remap). */

  GPIOB->ODR = 0x0000;
  /*  PB0： pulse output 6 */
  /*  PB1： pulse output 7 */
  /*  PB2: BOOT1：fixed LOW level.*/
  /*  PB3: JTAG TDO but use for RED LED(need remap).*/
  /*  PB4: JTAG TRST but use for ACT LED(BLUE)(need remap).*/
  /*  PB5: LCD RESET output */
  /*  PB6: SCL1 */
  /*  PB7: SDA1 */
  /*  PB8: not connection. */
  /*  PB9: gpio */
  /*  PB10: not connection. */
  /*  PB11: not connection. */
  /*  PB12: W5500 Slave select output */
  /*  PB13:SPI2_SCK output Alternate function*/
  /*  PB14:SPI2_MISO input Alternate function*/
  /*  PB15:SPI2_MOSI output Alternate function*/

  GPIOC->ODR = 0x0000;
  /*  PC0： pulse input 0 */
  /*  PC1： pulse input 1 */
  /*  PC2： pulse input 2 */
  /*  PC3： pulse input 3 */
  /*  PC4： pulse output 4 */
  /*  PC5： pulse output 5 */
  /*  PC6: W5500 interrupt input. */
  /*  PC7: W5500 reset. output */
  /*  PC8: not connection. */
  /*  PC9: not connection. */
  /*  PC10: UART4 TXD：Alternate function */
  /*  PC11: UART4 RXD：Alternate function */
  /*  PC12: not connection. */
  /*  PC13: gpio */
  /*  PC14: gpio */
  /*  PC15: gpio */

  GPIOD->ODR = 0x0000;
  /*  PD0：  oscillator input. */
  /*  PD1：  oscillator output. */
  /*  PD2: not connection. */
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

  pinMode( W5500_CS, OUTPUT ); set( W5500_CS );
  pinMode( W5500_INT, INPUT_PULLUP );
  pinMode( W5500_RESET, OUTPUT ); reset( W5500_RESET );

  pinMode( IN00, INPUT_PULLUP );
  pinMode( IN01, INPUT_PULLUP );
  pinMode( IN02, INPUT_PULLUP );
  pinMode( IN03, INPUT_PULLUP );
  pinMode( IN04, INPUT_PULLUP );
  pinMode( IN05, INPUT_PULLUP );
  pinMode( IN06, INPUT_PULLUP );
  pinMode( IN07, INPUT_PULLUP );

  pinMode( OUT00, OUTPUT ); reset( OUT00 );
  pinMode( OUT01, OUTPUT ); reset( OUT01 );
  pinMode( OUT02, OUTPUT ); reset( OUT02 );
  pinMode( OUT03, OUTPUT ); reset( OUT03 );
  pinMode( OUT04, OUTPUT ); reset( OUT04 );
  pinMode( OUT05, OUTPUT ); reset( OUT05 );
  pinMode( OUT06, OUTPUT ); reset( OUT06 );
  pinMode( OUT07, OUTPUT ); reset( OUT07 );
}

/* ----------------------------------------
    ethernet initialize.
---------------------------------------- */
void BOARD_2102::etherGpioInit()
{
  pinMode( W5500_INT, INPUT );
  pinMode( W5500_CS, OUTPUT );
  pinMode( W5500_RESET, OUTPUT );
  etherReset();
}

/* ----------------------------------------
    ethernet reset.
---------------------------------------- */
void BOARD_2102::etherReset()
{
  digitalWrite( W5500_CS, HIGH );
  digitalWrite( W5500_RESET, LOW );  /* reset wizchip */
  dly_tsk( 2UL );
  digitalWrite( W5500_RESET, HIGH );
}

extern "C"
{
  /* If necessary, write the C code here. */
/* ----------------------------------------
    ethernet chip select and deselect.
---------------------------------------- */
void wizchip_select( void ) { W5500_CS_IS_0; }
void wizchip_deselect( void ) { W5500_CS_IS_1; }
}  /* extern "C" */
