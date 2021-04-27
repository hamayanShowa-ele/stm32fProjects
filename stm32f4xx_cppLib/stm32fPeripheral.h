/* ----------------------------------------
 stm32f peripheral header
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
#ifndef  __STM32F_PERIPHERAL_H__
#define  __STM32F_PERIPHERAL_H__

#if defined (STM32F4XX)
#include  <stm32f4xx.h>
#endif  /* STM32F10X_xx */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    defines
---------------------------------------- */
enum STM32F_PIN_NAME
{
  PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
  PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
  PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
  PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
  PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
  PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
  PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
  PH0,PH1,PH2,PH3,PH4,PH5,PH6,PH7,PH8,PH9,PH10,PH11,PH12,PH13,PH14,PH15,
  PI0,PI1,PI2,PI3,PI4,PI5,PI6,PI7,PI8,PI9,PI10,PI11,PI12,PI13,PI14,PI15,
  PORT_END,
};


enum STM32F_ADC_NAME
{
//  ADC123_IN0,ADC123_IN1,ADC123_IN2,ADC123_IN3,
//  ADC12_IN4,ADC12_IN5,ADC12_IN6,ADC12_IN7,
//  ADC12_IN8,ADC12_IN9,
//  ADC123_IN10,ADC123_IN11,ADC123_IN12,ADC123_IN13,
//  ADC12_IN14,ADC12_IN15,

  ADC1_IN0,ADC1_IN1,ADC1_IN2,ADC1_IN3,
  ADC1_IN4,ADC1_IN5,ADC1_IN6,ADC1_IN7,
  ADC1_IN8,ADC1_IN9,ADC1_IN10,ADC1_IN11,
  ADC1_IN12,ADC1_IN13,ADC1_IN14,ADC1_IN15,

  ADC2_IN0,ADC2_IN1,ADC2_IN2,ADC2_IN3,
  ADC2_IN4,ADC2_IN5,ADC2_IN6,ADC2_IN7,
  ADC2_IN8,ADC2_IN9,ADC2_IN10,ADC2_IN11,
  ADC2_IN12,ADC2_IN13,ADC2_IN14,ADC2_IN15,

  ADC3_IN0,ADC3_IN1,ADC3_IN2,ADC3_IN3,
  ADC3_IN10,ADC3_IN11,ADC3_IN12,ADC3_IN13,
};


enum STM32F_ADC_PIN
{
  ADC1_IN0_PIN = PA0,
  ADC1_IN1_PIN = PA1,
  ADC1_IN2_PIN = PA2,
  ADC1_IN3_PIN = PA3,
  ADC1_IN4_PIN = PA4,
  ADC1_IN5_PIN = PA5,
  ADC1_IN6_PIN = PA6,
  ADC1_IN7_PIN = PA7,
  ADC1_IN8_PIN = PB0,
  ADC1_IN9_PIN = PB1,
  ADC1_IN10_PIN = PC0,
  ADC1_IN11_PIN = PC1,
  ADC1_IN12_PIN = PC2,
  ADC1_IN13_PIN = PC3,
  ADC1_IN14_PIN = PC4,
  ADC1_IN15_PIN = PC5,

  ADC2_IN0_PIN = PA0,
  ADC2_IN1_PIN = PA1,
  ADC2_IN2_PIN = PA2,
  ADC2_IN3_PIN = PA3,
  ADC2_IN4_PIN = PA4,
  ADC2_IN5_PIN = PA5,
  ADC2_IN6_PIN = PA6,
  ADC2_IN7_PIN = PA7,
  ADC2_IN8_PIN = PB0,
  ADC2_IN9_PIN = PB1,
  ADC2_IN10_PIN = PC0,
  ADC2_IN11_PIN = PC1,
  ADC2_IN12_PIN = PC2,
  ADC2_IN13_PIN = PC3,
  ADC2_IN14_PIN = PC4,
  ADC2_IN15_PIN = PC5,

  ADC3_IN0_PIN = PA0,
  ADC3_IN1_PIN = PA1,
  ADC3_IN2_PIN = PA2,
  ADC3_IN3_PIN = PA3,
  ADC3_IN10_PIN = PC0,
  ADC3_IN11_PIN = PC1,
  ADC3_IN12_PIN = PC2,
  ADC3_IN13_PIN = PC3,
};

/* ----------------------------------------
    RCC defines.
---------------------------------------- */
enum RCC_PERIPHERALS
{
  /* ahb1 */
  RCC_GPIOA = 0,  // #define RCC_AHB1Periph_GPIOA ((uint32_t)0x00000001)
  RCC_GPIOB,  // #define RCC_AHB1Periph_GPIOB     ((uint32_t)0x00000002)
  RCC_GPIOC,  // #define RCC_AHB1Periph_GPIOC     ((uint32_t)0x00000004)
  RCC_GPIOD,  // #define RCC_AHB1Periph_GPIOD     ((uint32_t)0x00000008)
  RCC_GPIOE,  // #define RCC_AHB1Periph_GPIOE ((uint32_t)0x00000010)
  RCC_GPIOF,  // #define RCC_AHB1Periph_GPIOF ((uint32_t)0x00000020)
  RCC_GPIOG,  // #define RCC_AHB1Periph_GPIOG ((uint32_t)0x00000040)
  RCC_GPIOH,  // #define RCC_AHB1Periph_GPIOH ((uint32_t)0x00000080)
  RCC_GPIOI,  // #define RCC_AHB1Periph_GPIOI ((uint32_t)0x00000100)
  RCC_RESERVE01, /*                           ((uint32_t)0x00000200) */
  RCC_RESERVE02, /*                           ((uint32_t)0x00000400) */
  RCC_RESERVE03, /*                           ((uint32_t)0x00000800) */
  RCC_CRC,  // #define RCC_AHB1Periph_CRC     ((uint32_t)0x00001000)
  RCC_RESERVE04, /*                           ((uint32_t)0x00002000) */
  RCC_RESERVE05, /*                           ((uint32_t)0x00004000) */
  RCC_FLITF,  // #define RCC_AHB1Periph_FLITF ((uint32_t)0x00008000)
  RCC_SRAM1,  // #define RCC_AHB1Periph_SRAM1     ((uint32_t)0x00010000)
  RCC_SRAM2,  // #define RCC_AHB1Periph_SRAM2     ((uint32_t)0x00020000)
  RCC_BKPSRAM,  // #define RCC_AHB1Periph_BKPSRAM ((uint32_t)0x00040000)
  RCC_RESERVE06, /*                               ((uint32_t)0x00080000) */
  RCC_CCMDATARAMEN,  // #define RCC_AHB1Periph_CCMDATARAMEN ((uint32_t)0x00100000)
  RCC_DMA1,  // #define RCC_AHB1Periph_DMA1                 ((uint32_t)0x00200000)
  RCC_DMA2,  // #define RCC_AHB1Periph_DMA2                 ((uint32_t)0x00400000)
  RCC_RESERVE07, /*                                         ((uint32_t)0x00800000) */
  RCC_RESERVE08, /*                                     ((uint32_t)0x01000000) */
  RCC_ETH_MAC,  // #define RCC_AHB1Periph_ETH_MAC       ((uint32_t)0x02000000)
  RCC_ETH_MAC_Tx,  // #define RCC_AHB1Periph_ETH_MAC_Tx ((uint32_t)0x04000000)
  RCC_ETH_MAC_Rx,  // #define RCC_AHB1Periph_ETH_MAC_Rx ((uint32_t)0x08000000)
  RCC_ETH_MAC_PTP,  // #define RCC_AHB1Periph_ETH_MAC_PTP ((uint32_t)0x10000000)
  RCC_OTG_HS,  // #define RCC_AHB1Periph_OTG_HS           ((uint32_t)0x20000000)
  RCC_OTG_HS_ULPI,  // #define RCC_AHB1Periph_OTG_HS_ULPI ((uint32_t)0x40000000)
  RCC_RESERVE09, /*                                       ((uint32_t)0x80000000) */

  /* ahb2 */
  RCC_DCMI = 32,  // #define RCC_AHB2Periph_DCMI ((uint32_t)0x00000001)
  RCC_RESERVE10, /*                              ((uint32_t)0x00000002) */
  RCC_RESERVE11, /*                              ((uint32_t)0x00000004) */
  RCC_RESERVE12, /*                              ((uint32_t)0x00000008) */
  RCC_CRYP,  // #define RCC_AHB2Periph_CRYP     ((uint32_t)0x00000010)
  RCC_HASH,  // #define RCC_AHB2Periph_HASH     ((uint32_t)0x00000020)
  RCC_RNG,  // #define RCC_AHB2Periph_RNG       ((uint32_t)0x00000040)
  RCC_OTG_FS,  // #define RCC_AHB2Periph_OTG_FS ((uint32_t)0x00000080)

  /* ahb3 */
  RCC_FSMC = 64,  // #define RCC_AHB3Periph_FSMC ((uint32_t)0x00000001)
  RCC_RESERVE13, /*                              ((uint32_t)0x00000002) */
  RCC_RESERVE14, /*                              ((uint32_t)0x00000004) */
  RCC_RESERVE15, /*                              ((uint32_t)0x00000008) */

  /* apb1 */
  RCC_TIM2 = 96,  // #define RCC_APB1Periph_TIM2 ((uint32_t)0x00000001)
  RCC_TIM3,  // #define RCC_APB1Periph_TIM3     ((uint32_t)0x00000002)
  RCC_TIM4,  // #define RCC_APB1Periph_TIM4     ((uint32_t)0x00000004)
  RCC_TIM5,  // #define RCC_APB1Periph_TIM5     ((uint32_t)0x00000008)
  RCC_TIM6,  // #define RCC_APB1Periph_TIM6   ((uint32_t)0x00000010)
  RCC_TIM7,  // #define RCC_APB1Periph_TIM7   ((uint32_t)0x00000020)
  RCC_TIM12,  // #define RCC_APB1Periph_TIM12 ((uint32_t)0x00000040)
  RCC_TIM13,  // #define RCC_APB1Periph_TIM13 ((uint32_t)0x00000080)
  RCC_TIM14,  // #define RCC_APB1Periph_TIM14 ((uint32_t)0x00000100)
  RCC_RESERVE16, /*                           ((uint32_t)0x00000200) */
  RCC_RESERVE17, /*                           ((uint32_t)0x00000400) */
  RCC_WWDG,  // #define RCC_APB1Periph_WWDG   ((uint32_t)0x00000800)
  RCC_RESERVE18, /*                         ((uint32_t)0x00001000) */
  RCC_RESERVE19, /*                         ((uint32_t)0x00002000) */
  RCC_SPI2,  // #define RCC_APB1Periph_SPI2 ((uint32_t)0x00004000)
  RCC_SPI3,  // #define RCC_APB1Periph_SPI3 ((uint32_t)0x00008000)
  RCC_RESERVE20, /*                             ((uint32_t)0x00010000) */
  RCC_USART2,  // #define RCC_APB1Periph_USART2 ((uint32_t)0x00020000)
  RCC_USART3,  // #define RCC_APB1Periph_USART3 ((uint32_t)0x00040000)
  RCC_UART4,  // #define RCC_APB1Periph_UART4   ((uint32_t)0x00080000)
  RCC_UART5,  // #define RCC_APB1Periph_UART5 ((uint32_t)0x00100000)
  RCC_I2C1,  // #define RCC_APB1Periph_I2C1   ((uint32_t)0x00200000)
  RCC_I2C2,  // #define RCC_APB1Periph_I2C2   ((uint32_t)0x00400000)
  RCC_I2C3,  // #define RCC_APB1Periph_I2C3   ((uint32_t)0x00800000)
  RCC_RESERVE21, /*                         ((uint32_t)0x01000000) */
  RCC_CAN1,  // #define RCC_APB1Periph_CAN1 ((uint32_t)0x02000000)
  RCC_CAN2,  // #define RCC_APB1Periph_CAN2 ((uint32_t)0x04000000)
  RCC_RESERVE22, /*                         ((uint32_t)0x08000000) */
  RCC_PWR,  // #define RCC_APB1Periph_PWR ((uint32_t)0x10000000)
  RCC_DAC,  // #define RCC_APB1Periph_DAC ((uint32_t)0x20000000)
  RCC_RESERVE23, /*                       ((uint32_t)0x40000000) */
  RCC_RESERVE24, /*                       ((uint32_t)0x80000000) */

  /* apb2 */
  RCC_TIM1 = 128,  // #define RCC_APB2Periph_TIM1 ((uint32_t)0x00000001)
  RCC_TIM8,  // #define RCC_APB2Periph_TIM8       ((uint32_t)0x00000002)
  RCC_RESERVE25, /*                               ((uint32_t)0x00000004) */
  RCC_RESERVE26, /*                               ((uint32_t)0x00000008) */
  RCC_USART1,  // #define RCC_APB2Periph_USART1 ((uint32_t)0x00000010)
  RCC_USART6,  // #define RCC_APB2Periph_USART6 ((uint32_t)0x00000020)
  RCC_RESERVE27, /*                             ((uint32_t)0x00000040) */
  RCC_RESERVE28, /*                             ((uint32_t)0x00000080) */
//  RCC_ADC,  // #define RCC_APB2Periph_ADC   ((uint32_t)0x00000100)
  RCC_ADC1,  // #define RCC_APB2Periph_ADC1 ((uint32_t)0x00000100)
  RCC_ADC2,  // #define RCC_APB2Periph_ADC2 ((uint32_t)0x00000200)
  RCC_ADC3,  // #define RCC_APB2Periph_ADC3 ((uint32_t)0x00000400)
  RCC_SDIO,  // #define RCC_APB2Periph_SDIO ((uint32_t)0x00000800)
  RCC_SPI1,  // #define RCC_APB2Periph_SPI1     ((uint32_t)0x00001000)
  RCC_RESERVE29, /*                             ((uint32_t)0x00002000) */
  RCC_SYSCFG,  // #define RCC_APB2Periph_SYSCFG ((uint32_t)0x00004000)
  RCC_RESERVE30, /*                             ((uint32_t)0x00008000) */
  RCC_TIM9,  // #define RCC_APB2Periph_TIM9   ((uint32_t)0x00010000)
  RCC_TIM10,  // #define RCC_APB2Periph_TIM10 ((uint32_t)0x00020000)
  RCC_TIM11,  // #define RCC_APB2Periph_TIM11 ((uint32_t)0x00040000)
  RCC_RESERVE31, /*                           ((uint32_t)0x00080000) */
};

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

#endif  /* __STM32F_PERIPHERAL_H__ */

