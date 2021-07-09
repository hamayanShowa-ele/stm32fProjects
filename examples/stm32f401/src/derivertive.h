/* ----------------------------------------
  STM32F401RCT( nucleo-64 ) template code
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

#ifndef	_DERIVERTIVE_H_
#define	_DERIVERTIVE_H_

/* ----------------------------------------
  defines.
---------------------------------------- */

//typedef  unsigned char   bool;
//enum BOOL { FALSE = 0, TRUE = ~FALSE };


/* ----------------------------------------
  gpio port defines.
---------------------------------------- */
/* PA5 :ACT LED(LD2) */
#define  ACT_PORT       GPIOA
#define  ACT_PIN        GPIO_Pin_5
#define  ACT_IS_0       ACT_PORT->BSRRH = (ACT_PIN << 0)
#define  ACT_IS_1       ACT_PORT->BSRRL = (ACT_PIN << 0)
#define  ACT_IS         (ACT_PORT->ODR & ACT_PIN)
#define  ACT_RCC        RCC_AHB1Periph_GPIOG  //for virgo

/* PB8 :SCL1 */
/* PB9 :SDA1 */
#define  I2C1_PORT      GPIOB
#define  I2C1_SCL_PIN   GPIO_Pin_8
#define  I2C1_SDA_PIN   GPIO_Pin_9
#define  I2C1_SCL_IS_0  I2C1_PORT->BSRRH = (I2C1_SCL_PIN << 0)
#define  I2C1_SCL_IS_1  I2C1_PORT->BSRRL = (I2C1_SCL_PIN << 0)
#define  I2C1_SCL_SOURCE GPIO_PinSource6
#define  I2C1_SDA_SOURCE GPIO_PinSource7

/*  PA9: TXD1 */
/*  PA10: RXD1 */
#define  TXD1_PIN  PA9
#define  RXD1_PIN  PA10

/*  PA2: TXD2 */
/*  PA3: RXD2 */
#define  TXD2_PIN  PA2
#define  RXD2_PIN  PA3
#define  USART2_TXD_PORT GPIOA
#define  USART2_RXD_PORT GPIOA
#define  USART2_TXD_PIN  GPIO_Pin_2
#define  USART2_RXD_PIN  GPIO_Pin_3
#define  USART2_TXD_SOURCE  GPIO_PinSource2
#define  USART2_RXD_SOURCE  GPIO_PinSource3

/*  PB10: TXD3 */
/*  PB11: RXD3 */
#define  USART3_TXD_PORT GPIOB
#define  USART3_RXD_PORT GPIOB
#define  USART3_TXD_PIN  GPIO_Pin_10
#define  USART3_RXD_PIN  GPIO_Pin_11
#define  USART3_TXD_SOURCE  GPIO_PinSource10
#define  USART3_RXD_SOURCE  GPIO_PinSource11

/*  PC10: TXD4 */
/*  PC11: RXD4 */
#define  UART4_TXD_PORT GPIOC
#define  UART4_RXD_PORT GPIOC
#define  UART4_TXD_PIN  GPIO_Pin_10
#define  UART4_RXD_PIN  GPIO_Pin_11
#define  UART4_TXD_SOURCE  GPIO_PinSource10
#define  UART4_RXD_SOURCE  GPIO_PinSource11

/*  PC12: TXD5 */
/*  PD2: RXD5 */
#define  UART5_TXD_PORT  GPIOC
#define  UART5_RXD_PORT  GPIOD
#define  UART5_TXD_PIN   GPIO_Pin_12
#define  UART5_RXD_PIN   GPIO_Pin_2
#define  UART5_TXD_SOURCE  GPIO_PinSource12
#define  UART5_RXD_SOURCE  GPIO_PinSource2

/* is it nucleo or 2102? */
#define  STDIO  USART1  // USART2 is nucleo and USART1 is 2102
#define  STDIO_TX_PIN  TXD1_PIN  // TXD2_PIN is nucleo and TXD1_PIN is 2102
#define  STDIO_RX_PIN  RXD1_PIN  // RXD2_PIN is nucleo and RXD2_PIN is 2102
#define  ACT_LED  PB4  //PA5 is nucleo and PB4 is 2102

/* ----------------------------------------
 defines for usart and uart.
---------------------------------------- */
#define  SCI_1_SND_BUFFER_SIZE  256
#define  SCI_1_RCV_BUFFER_SIZE  256
#define  SCI_2_SND_BUFFER_SIZE  0
#define  SCI_2_RCV_BUFFER_SIZE  0
#define  SCI_3_SND_BUFFER_SIZE  0
#define  SCI_3_RCV_BUFFER_SIZE  0
#define  SCI_4_SND_BUFFER_SIZE  0
#define  SCI_4_RCV_BUFFER_SIZE  0
#define  SCI_5_SND_BUFFER_SIZE  0
#define  SCI_5_RCV_BUFFER_SIZE  0
#define  SCI_6_SND_BUFFER_SIZE  0
#define  SCI_6_RCV_BUFFER_SIZE  0
/*
#define  SCI_7_SND_BUFFER_SIZE  0
#define  SCI_7_RCV_BUFFER_SIZE  0
#define  SCI_8_SND_BUFFER_SIZE  0
#define  SCI_8_RCV_BUFFER_SIZE  0
*/

/* ----------------------------------------
  multi tasks defines.
---------------------------------------- */
#define  MAX_TASK_NUMBER    1  //
enum GR_SAKURA_TSK_ID
{
  ID_stackMonitor,
};

#define  MAX_MBX_NUMBER    1
enum GR_SAKURA_MBX_ID
{
  MBXID_dummy,
};


#endif	/*_DERIVERTIVE_H_*/

/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
