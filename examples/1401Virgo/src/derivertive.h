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
#define  SYS_CLK            (168000000UL)
#define  APB1_CLOCK         (42000000UL)
#define  APB2_CLOCK         (84000000UL)
#define  CK_INT_TIM1        (168000000UL)
#define  CK_INT_TIM2        (84000000UL)
#define  CK_INT_TIM3        (84000000UL)
#define  CK_INT_TIM4        (84000000UL)
#define  CK_INT_TIM5        (84000000UL)
#define  CK_INT_TIM6        (84000000UL)
#define  CK_INT_TIM7        (84000000UL)
#define  CK_INT_TIM8        (168000000UL)
#define  CK_INT_TIM9        (168000000UL)
#define  CK_INT_TIM10       (168000000UL)
#define  CK_INT_TIM11       (168000000UL)
#define  CK_INT_TIM12       (84000000UL)
#define  CK_INT_TIM13       (84000000UL)
#define  CK_INT_TIM14       (84000000UL)

//typedef  unsigned char   bool;
//enum BOOL { FALSE = 0, TRUE = ~FALSE };

#define  SRAM_BASE_ADDRESS  0x60000000
#define  ETH_BASE_ADDRESS   0x64000000
#define  ARC_BASE_ADDRESS   0x68000000
#define  EXT_BUS_BASE_ADDRESS  0x6C000000

#define  MRAM_SIZE          524288UL

/* ----------------------------------------
  gpio port defines.
---------------------------------------- */
/* PG11 :ACT LED */
#define  ACT_LED        PG11
#define  ACT_PORT       GPIOG
#define  ACT_PIN        GPIO_Pin_11
#define  ACT_IS_0       ACT_PORT->BSRRH = (ACT_PIN << 0)
#define  ACT_IS_1       ACT_PORT->BSRRL = (ACT_PIN << 0)
#define  ACT_IS         (ACT_PORT->ODR & ACT_PIN)
#define  ACT_RCC        RCC_AHB1Periph_GPIOG  //for virgo
#define  ACT_LED        PG11

/*  PA1: 1PPS,GPS 1PPS input*/
#define  PPS            PA1
#define  PPS_PORT       GPIOA
#define  PPS_PIN        GPIO_Pin_1
#define  PPS_IS         (PPS_PORT->ODR & PPS_PIN)

/*  PB8: SCL1 */
/*  PB9: SDA1 */
#define  SCL1           PB8
#define  SDA1           PB9
#define  I2C1_PORT      GPIOB
#define  I2C1_SCL_PIN   GPIO_Pin_8
#define  I2C1_SDA_PIN   GPIO_Pin_9
#define  I2C1_SCL_IS_0  I2C1_PORT->BSRRH = (I2C1_SCL_PIN << 0)
#define  I2C1_SCL_IS_1  I2C1_PORT->BSRRL = (I2C1_SCL_PIN << 0)
#define  I2C1_SDA_IS_0  I2C1_PORT->BSRRH = (I2C1_SDA_PIN << 0)
#define  I2C1_SDA_IS_1  I2C1_PORT->BSRRL = (I2C1_SDA_PIN << 0)
#define  I2C1_SCL_SOURCE GPIO_PinSource6
#define  I2C1_SDA_SOURCE GPIO_PinSource7

/*  PC0: USBON, active low output */
#define  USBON          PC0
#define  USBON_PORT     GPIOC
#define  USBON_PIN      GPIO_Pin_0
#define  USBON_IS_0     USBON_PORT->BSRRH = (USBON_PIN << 0)
#define  USBON_IS_1     USBON_PORT->BSRRL = (USBON_PIN << 0)

/*  PC9: EXT_INT1, external interrupt input 1. */
#define  EXT_INT1       PC9
#define  EXT_INT1_PORT  GPIOC
#define  EXT_INT1_PIN   GPIO_Pin_9
#define  EXT_INT1_IS    (EXT_INT1_PORT->ODR & EXT_INT1_PIN)

/*  PC12: UART4 M/S, output for rs485 driver control. */
#define  UART4_MS       PC12
#define  UART4_MS_PORT  GPIOC
#define  UART4_MS_PIN   GPIO_Pin_12
#define  UART4_MS_IS_0  UART4_MS_PORT->BSRRH = (UART4_MS_PIN << 0)
#define  UART4_MS_IS_1  UART4_MS_PORT->BSRRL = (UART4_MS_PIN << 0)

/*  PC13: TAMPER, input. */
#define  TAMPER         PC13
#define  TAMPER_PORT    GPIOC
#define  TAMPER_PIN     GPIO_Pin_13
#define  TAMPER_IS      (TAMPER_PORT->ODR & TAMPER_PIN)

/*  PD2: TRIGGER2, external trigger input 2. */
#define  TRIGGER2       PD2
#define  TRIGGER2_PORT  GPIOD
#define  TRIGGER2_PIN   GPIO_Pin_2
#define  TRIGGER2_IS    (TRIGGER2_PORT->ODR & TRIGGER2_PIN)

/*  PD3: TRIGGER1, external trigger input 1. */
#define  TRIGGER1       PD3
#define  TRIGGER1_PORT  GPIOD
#define  TRIGGER1_PIN   GPIO_Pin_3
#define  TRIGGER1_IS    (TRIGGER1_PORT->ODR & TRIGGER1_PIN)

/*  PF8: LED RED, output */
#define  LED_RED        PF8
#define  LED_RED_PORT   GPIOF
#define  LED_RED_PIN    GPIO_Pin_8
#define  LED_RED_IS_0   LED_RED_PORT->BSRRH = (LED_RED_PIN << 0)
#define  LED_RED_IS_1   LED_RED_PORT->BSRRL = (LED_RED_PIN << 0)

/*  PF9: LED BLUE, output */
#define  LED_BLUE       PF9
#define  LED_BLUE_PORT   GPIOF
#define  LED_BLUE_PIN    GPIO_Pin_9
#define  LED_BLUE_IS_0   LED_BLUE_PORT->BSRRH = (LED_BLUE_PIN << 0)
#define  LED_BLUE_IS_1   LED_BLUE_PORT->BSRRL = (LED_BLUE_PIN << 0)

/*  PF10: LED GREEN, output */
#define  LED_GREEN      PF10
#define  LED_GREEN_PORT   GPIOF
#define  LED_GREEN_PIN    GPIO_Pin_10
#define  LED_GREEN_IS_0   LED_GREEN_PORT->BSRRH = (LED_GREEN_PIN << 0)
#define  LED_GREEN_IS_1   LED_GREEN_PORT->BSRRL = (LED_GREEN_PIN << 0)

/*  PG6: ETH_INT, ethernet interrupt input. */
#define  ETH_INT_PORT    GPIOG
#define  ETH_INT_PIN     GPIO_Pin_6
#define  ETH_INT_IS      (ETH_INT_PORT->ODR & ETH_INT_PIN)

/*  PG7: ARC_INT, arcnet interrupt input. */
#define  ARC_INT_PORT    GPIOG
#define  ARC_INT_PIN     GPIO_Pin_7
#define  ARC_INT_IS      (ARC_INT_PORT->ODR & ARC_INT_PIN)

/*  PG8: EXT_INT0, external interrupt input 0. */
#define  EXT_INT0_PORT   GPIOG
#define  EXT_INT0_PIN    GPIO_Pin_8
#define  EXT_INT0_IS     (EXT_INT0_PORT->ODR & EXT_INT0_PIN)

/*  PG13: IO_RST, external bus reset output. */
#define  IO_RST        PG13
#define  IO_RST_PORT   GPIOG
#define  IO_RST_PIN    GPIO_Pin_13
#define  IO_RST_IS_0   IO_RST_PORT->BSRRH = (IO_RST_PIN << 0)
#define  IO_RST_IS_1   IO_RST_PORT->BSRRL = (IO_RST_PIN << 0)

/*  PG15: CONVERT_MS, converter master/slave input. high = master low = slave. */
#define  CONVERT_MS       PG15
#define  CONVERT_MS_PORT  GPIOG
#define  CONVERT_MS_PIN   GPIO_Pin_15
#define  CONVERT_MS_IS    (CONVERT_MS_PORT->ODR & CONVERT_MS_PIN)

/*  PA9: TXD1 */
/*  PA10: RXD1 */
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


/* ----------------------------------------
  i2c.
---------------------------------------- */
#define  I2C_BOARD_ADDRESS  0x20

/* ----------------------------------------
  wiznet w5300.
---------------------------------------- */
#define  _WIZCHIP_  5300  // W5300
#define  _WIZCHIP_IO_BUS_WIDTH_  16  // 8bit bus or 16bit bus
#define  IDM_AR  ((_WIZCHIP_IO_BASE_ + (0x0002 * 2)))  ///< Indirect mode address register
#define  IDM_DR  ((_WIZCHIP_IO_BASE_ + (0x0004 * 2)))  ///< Indirect mode data register
#define  DHCP_SOCKET_NUMBER  7

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
