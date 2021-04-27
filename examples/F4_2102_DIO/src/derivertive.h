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
/* SNTP */
#define  SNTP_CLIENT_SOCKET  1
#define  TIME_ZONE           40  /* Korea, East Timor, Russia (Irkutsk Oblast), Japan */

/*
  defines for w5500.
*/
#define  _WIZCHIP_  5500
#define  _WIZCHIP_IO_MODE_  _WIZCHIP_IO_MODE_SPI_VDM_
//#define  _WIZCHIP_CS_    PB12
//#define  W5500_CS        _WIZCHIP_CS_   /* output */
#define  W5500_CS        PB12   /* output */
#define  W5500_INT       PC9   /* input */
#define  W5500_RESET     PA8   /* output */
#define  SPI2_SCK_PORT   GPIOB
#define  SPI2_SCK_PIN    GPIO_Pin_13
#define  SPI2_MISO_PORT  GPIOB
#define  SPI2_MISO_PIN   GPIO_Pin_14
#define  SPI2_MOSI_PORT  GPIOB
#define  SPI2_MOSI_PIN   GPIO_Pin_15
#define  SPI2_SCK        PB13
#define  SPI2_MISO       PB14
#define  SPI2_MOSI       PB15
#define  W5500_CS_PORT   GPIOB
#define  W5500_CS_PIN    GPIO_Pin_12
#define  W5500_CS_IS_0   W5500_CS_PORT->BSRRH = (W5500_CS_PIN << 0)
#define  W5500_CS_IS_1   W5500_CS_PORT->BSRRL = (W5500_CS_PIN << 0)

/* PA5 :ACT LED(LD2) */
#define  ACT_PORT       GPIOA
#define  ACT_PIN        GPIO_Pin_5
#define  ACT_IS_0       ACT_PORT->BSRRH = (ACT_PIN << 0)
#define  ACT_IS_1       ACT_PORT->BSRRL = (ACT_PIN << 0)
#define  ACT_IS         (ACT_PORT->ODR & ACT_PIN)

/* PB8 :SCL1 */
/* PB9 :SDA1 */
#define  I2C1_PORT      GPIOB
#define  I2C1_SCL_PIN   GPIO_Pin_6
#define  I2C1_SDA_PIN   GPIO_Pin_7
#define  I2C1_SCL_IS_0  I2C1_PORT->BSRRH = (I2C1_SCL_PIN << 0)
#define  I2C1_SCL_IS_1  I2C1_PORT->BSRRL = (I2C1_SCL_PIN << 0)
#define  I2C1_SCL_SOURCE GPIO_PinSource6
#define  I2C1_SDA_SOURCE GPIO_PinSource7
#define  SDA1           PB7
#define  SCL1           PB6

/*  PA9: TXD1 */
/*  PA10: RXD1 */
#define  TXD1  PA9
#define  RXD1  PA10

#if 0
/*  PA2: TXD2 */
/*  PA3: RXD2 */
#define  TXD2  PA2
#define  RXD2  PA3
#endif

/*  PC6: TXD6 */
/*  PC7: RXD6 */
#define  TXD6  PC6
#define  RXD6  PC7

/* is it nucleo or 2102? */
#define  STDIO  USART1  // USART2 is nucleo and USART1 is 2102
#define  STDIO_TX_PIN  TXD1  // TXD2 is nucleo and TXD1 is 2102
#define  STDIO_RX_PIN  RXD1  // RXD2 is nucleo and RXD2 is 2102
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
#define  MAX_TASK_NUMBER    3  //
enum GR_SAKURA_TSK_ID
{
  ID_stackMonitor,
  ID_pulseOut,
  ID_pulseIn,
};

#define  MAX_MBX_NUMBER    1
enum GR_SAKURA_MBX_ID
{
  MBXID_dummy,
};

#define  MAX_SEM_NUMBER    1
enum GR_SAKURA_SEM_ID
{
  SEMID_SPI2 = 1,
};

#endif	/*_DERIVERTIVE_H_*/

/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
