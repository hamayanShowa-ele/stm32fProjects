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

#if defined (STM32F10X_LD) || defined (STM32F10X_LD_VL) || defined (STM32F10X_MD) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_XL) || defined (STM32F10X_CL)
#include  <stm32f10x.h>
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
  RCC_AFIO = 0,  /* RCC_APB2Periph_AFIO ((uint32_t)0x00000001) */
  RCC_RESERVE01, /*                     ((uint32_t)0x00000002) */
  RCC_GPIOA,   /* RCC_APB2Periph_GPIOA  ((uint32_t)0x00000004) */
  RCC_GPIOB,   /* RCC_APB2Periph_GPIOB  ((uint32_t)0x00000008) */
  RCC_GPIOC,   /* RCC_APB2Periph_GPIOC  ((uint32_t)0x00000010) */
  RCC_GPIOD,   /* RCC_APB2Periph_GPIOD  ((uint32_t)0x00000020) */
  RCC_GPIOE,   /* RCC_APB2Periph_GPIOE  ((uint32_t)0x00000040) */
  RCC_GPIOF,   /* RCC_APB2Periph_GPIOF  ((uint32_t)0x00000080) */
  RCC_GPIOG,   /* RCC_APB2Periph_GPIOG  ((uint32_t)0x00000100) */
  RCC_ADC1,    /* RCC_APB2Periph_ADC1   ((uint32_t)0x00000200) */
  RCC_ADC2,    /* RCC_APB2Periph_ADC2   ((uint32_t)0x00000400) */
  RCC_TIM1,    /* RCC_APB2Periph_TIM1   ((uint32_t)0x00000800) */
  RCC_SPI1,    /* RCC_APB2Periph_SPI1   ((uint32_t)0x00001000) */
  RCC_TIM8,    /* RCC_APB2Periph_TIM8   ((uint32_t)0x00002000) */
  RCC_USART1,  /* RCC_APB2Periph_USART1 ((uint32_t)0x00004000) */
  RCC_ADC3,    /* RCC_APB2Periph_ADC3   ((uint32_t)0x00008000) */
  RCC_TIM15,   /* RCC_APB2Periph_TIM15  ((uint32_t)0x00010000) */
  RCC_TIM16,   /* RCC_APB2Periph_TIM16  ((uint32_t)0x00020000) */
  RCC_TIM17,   /* RCC_APB2Periph_TIM17  ((uint32_t)0x00040000) */
  RCC_TIM9,    /* RCC_APB2Periph_TIM9   ((uint32_t)0x00080000) */
  RCC_TIM10,   /* RCC_APB2Periph_TIM10  ((uint32_t)0x00100000) */
  RCC_TIM11,   /* RCC_APB2Periph_TIM11  ((uint32_t)0x00200000) */

  RCC_TIM2 = 32, /* RCC_APB1Periph_TIM2 ((uint32_t)0x00000001) */
  RCC_TIM3,    /* RCC_APB1Periph_TIM3   ((uint32_t)0x00000002) */
  RCC_TIM4,    /* RCC_APB1Periph_TIM4   ((uint32_t)0x00000004) */
  RCC_TIM5,    /* RCC_APB1Periph_TIM5   ((uint32_t)0x00000008) */
  RCC_TIM6,    /* RCC_APB1Periph_TIM6   ((uint32_t)0x00000010) */
  RCC_TIM7,    /* RCC_APB1Periph_TIM7   ((uint32_t)0x00000020) */
  RCC_TIM12,   /* RCC_APB1Periph_TIM12  ((uint32_t)0x00000040) */
  RCC_TIM13,   /* RCC_APB1Periph_TIM13  ((uint32_t)0x00000080) */
  RCC_TIM14,   /* RCC_APB1Periph_TIM14  ((uint32_t)0x00000100) */
  RCC_RESERVE02, /*                     ((uint32_t)0x00000200) */
  RCC_RESERVE03, /*                     ((uint32_t)0x00000400) */
  RCC_WWDG,    /* RCC_APB1Periph_WWDG   ((uint32_t)0x00000800) */
  RCC_RESERVE04, /*                     ((uint32_t)0x00001000) */
  RCC_RESERVE05, /*                     ((uint32_t)0x00002000) */
  RCC_SPI2,    /* RCC_APB1Periph_SPI2   ((uint32_t)0x00004000) */
  RCC_SPI3,    /* RCC_APB1Periph_SPI3   ((uint32_t)0x00008000) */
  RCC_RESERVE06, /*                     ((uint32_t)0x00010000) */
  RCC_USART2,  /* RCC_APB1Periph_USART2 ((uint32_t)0x00020000) */
  RCC_USART3,  /* RCC_APB1Periph_USART3 ((uint32_t)0x00040000) */
  RCC_UART4,   /* RCC_APB1Periph_UART4  ((uint32_t)0x00080000) */
  RCC_UART5,   /* RCC_APB1Periph_UART5  ((uint32_t)0x00100000) */
  RCC_I2C1,    /* RCC_APB1Periph_I2C1   ((uint32_t)0x00200000) */
  RCC_I2C2,    /* RCC_APB1Periph_I2C2   ((uint32_t)0x00400000) */
  RCC_USB,     /* RCC_APB1Periph_USB    ((uint32_t)0x00800000) */
  RCC_RESERVE07, /*                     ((uint32_t)0x01000000) */
  RCC_CAN1,    /* RCC_APB1Periph_CAN1   ((uint32_t)0x02000000) */
  RCC_CAN2,    /* RCC_APB1Periph_CAN2   ((uint32_t)0x04000000) */
  RCC_BKP,     /* RCC_APB1Periph_BKP    ((uint32_t)0x08000000) */
  RCC_PWR,     /* RCC_APB1Periph_PWR    ((uint32_t)0x10000000) */
  RCC_DAC,     /* RCC_APB1Periph_DAC    ((uint32_t)0x20000000) */
  RCC_CEC,     /* RCC_APB1Periph_CEC    ((uint32_t)0x40000000) */
};

/* ----------------------------------------
    REMAP defines.
---------------------------------------- */
enum GPIO_REMAP
{
  REMAP_SPI1 = GPIO_Remap_SPI1,
  REMAP_I2C1 = GPIO_Remap_I2C1,
  REMAP_USART1 = GPIO_Remap_USART1,
  REMAP_USART2 = GPIO_Remap_USART2,
  REMAP_Partial_USART3 = GPIO_PartialRemap_USART3,
  REMAP_Full_USART3 = GPIO_FullRemap_USART3,
  REMAP_Partial_TIM1 = GPIO_PartialRemap_TIM1,
  REMAP_Full_TIM1 = GPIO_FullRemap_TIM1,
  REMAP_Partial1_TIM2 = GPIO_PartialRemap1_TIM2,
  REMAP_Partial2_TIM2 = GPIO_PartialRemap2_TIM2,
  REMAP_Full_TIM2 = GPIO_FullRemap_TIM2,
  REMAP_Partial_TIM3 = GPIO_PartialRemap_TIM3,
  REMAP_Full_TIM3 = GPIO_FullRemap_TIM3,
  REMAP_TIM4 = GPIO_Remap_TIM4,
  REMAP_Remap1_CAN1 = GPIO_Remap1_CAN1,
  REMAP_Remap2_CAN1 = GPIO_Remap2_CAN1,
  REMAP_PD01 = GPIO_Remap_PD01,
  REMAP_TIM5CH4_LSI = GPIO_Remap_TIM5CH4_LSI,
  REMAP_ADC1_ETRGINJ = GPIO_Remap_ADC1_ETRGINJ,
  REMAP_ADC1_ETRGREG = GPIO_Remap_ADC1_ETRGREG,
  REMAP_ADC2_ETRGINJ = GPIO_Remap_ADC2_ETRGINJ,
  REMAP_ADC2_ETRGREG = GPIO_Remap_ADC2_ETRGREG,
  REMAP_ETH = GPIO_Remap_ETH,
  REMAP_CAN2 = GPIO_Remap_CAN2,
  REMAP_SWJ_NoJTRST = GPIO_Remap_SWJ_NoJTRST,
  REMAP_SWJ_JTAGDisable = GPIO_Remap_SWJ_JTAGDisable,
  REMAP_SWJ_Disable = GPIO_Remap_SWJ_Disable,
  REMAP_SPI3 = GPIO_Remap_SPI3,
  REMAP_TIM2ITR1_PTP_SOF = GPIO_Remap_TIM2ITR1_PTP_SOF,
  REMAP_PTP_PPS = GPIO_Remap_PTP_PPS,
  REMAP_TIM15 = GPIO_Remap_TIM15,
  REMAP_TIM16 = GPIO_Remap_TIM16,
  REMAP_TIM17 = GPIO_Remap_TIM17,
  REMAP_CEC = GPIO_Remap_CEC,
  REMAP_TIM1_DMA = GPIO_Remap_TIM1_DMA,
  REMAP_TIM9 = GPIO_Remap_TIM9,
  REMAP_TIM10 = GPIO_Remap_TIM10,
  REMAP_TIM11 = GPIO_Remap_TIM11,
  REMAP_TIM13 = GPIO_Remap_TIM13,
  REMAP_TIM14 = GPIO_Remap_TIM14,
  REMAP_FSMC_NADV = GPIO_Remap_FSMC_NADV,
  REMAP_TIM67_DAC_DMA = GPIO_Remap_TIM67_DAC_DMA,
  REMAP_TIM12 = GPIO_Remap_TIM12,
  REMAP_MISC = GPIO_Remap_MISC,
};

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

#endif  /* __STM32F_PERIPHERAL_H__ */

