/* ----------------------------------------
  STM32F407ZET template code
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

#define  DPRAM_BASE_ADDRESS  (0x60000000UL)
#define  DPRAM_SIZE          (8192)  // 4k * 16bit
#define  DPRAM_INTR_ADDRESS  (DPRAM_BASE_ADDRESS + (0x0FFF * 2))

#define  ADC1_BASE_ADDRESS  (0x64000000UL)
#define  ADC2_BASE_ADDRESS  (0x68000000UL)

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
  gpio port defines.
---------------------------------------- */
/* PG11 :ACT LED */
#define  ACT_PORT       GPIOG
#define  ACT_PIN        GPIO_Pin_11
#define  ACT_IS_0       ACT_PORT->BSRRH = (ACT_PIN << 0)
#define  ACT_IS_1       ACT_PORT->BSRRL = (ACT_PIN << 0)
#define  ACT_IS         (ACT_PORT->ODR & ACT_PIN)
#define  ACT_RCC        RCC_AHB1Periph_GPIOG  //for virgo
#define  ACT_LED        PG11

/* PB6 :SCL1 */
/* PB7 :SDA1 */
#define  I2C1_PORT      GPIOB
#define  I2C1_SCL_PIN   GPIO_Pin_6
#define  I2C1_SDA_PIN   GPIO_Pin_7
#define  I2C1_SCL_IS_0  I2C1_PORT->BSRRH = (I2C1_SCL_PIN << 0)
#define  I2C1_SCL_IS_1  I2C1_PORT->BSRRL = (I2C1_SCL_PIN << 0)
#define  I2C1_SCL_SOURCE GPIO_PinSource6
#define  I2C1_SDA_SOURCE GPIO_PinSource7

/*  PA9: TXD1 */
#define  TXD1_PIN  PA9
/*  PA10: RXD1 */
#define  RXD1_PIN  PA10
#define  USART1_TXD_PORT GPIOA
#define  USART1_RXD_PORT GPIOA
#define  USART1_TXD_PIN  GPIO_Pin_9
#define  USART1_RXD_PIN  GPIO_Pin_10
#define  USART1_TXD_SOURCE  GPIO_PinSource9
#define  USART1_RXD_SOURCE  GPIO_PinSource10

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

#define  INTDP  PG3
#define  EOLC1  PG5
#define  EOLC2  PG4

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
