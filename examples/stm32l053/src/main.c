/**
  ******************************************************************************
  * @file    system_stm32l0xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M0+ Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l0xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l0xx_system
  * @{
  */

/** @addtogroup STM32L0xx_System_Private_Includes
  * @{
  */

#include  <string.h>
#include  <time.h>
#include  <stm32l0xx.h>
#include  "derivertive.h"
#include  <mul_tsk.h>
#include  <system.h>
#include  <stm32l0xxGPIO.h>
#include  <stm32l0xxUSART.h>
#include  <1405_STM32L0xx.h>

/* ----------------------------------------
    defines
---------------------------------------- */
//#define  loc_cpu()  __asm__("\tcpsid i\n")
//#define  unl_cpu()  __asm__("\tcpsie i\n")

/* ----------------------------------------
    prototypes
---------------------------------------- */
static void RCC_configuration( void );
void task1( void );
void task2( void );

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
//volatile uint32_t uwTick;
uint32_t sysClockFreq;
volatile SYSTIM systim;
volatile time_t unixTime;
int sci1Handle;  /* sci handle for usart1. */

uint8_t tsk1_stk[256];
uint8_t tsk2_stk[256];

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_task1,(void *)task1, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_task2,(void *)task2, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );

  sta_tsk( ID_task1 );
  sta_tsk( ID_task2 );
}

int main(void)
{
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();
  RCC_configuration();
  loc_cpu();

  /* configure swd */
  GPIO_InitTypeDef GPIO_Init;
  GPIO_Init.Pin = SWDIO_PIN;
  GPIO_Init.Mode = GPIO_MODE_AF_PP;
  GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init.Alternate = SWDIO_ALTERNATE_FUNCTION;
  HAL_GPIO_Init( SWDIO_GPIOx, &GPIO_Init );
  GPIO_Init.Pin = SWCLK_PIN;
  GPIO_Init.Alternate = SWCLK_ALTERNATE_FUNCTION;
  HAL_GPIO_Init( SWCLK_GPIOx, &GPIO_Init );

  /* initialize gpio ports. */
  gpioInit();
//  HAL_GPIO_DeInit( GPIOA, GPIO_PIN_5 );
#if 0
  GPIO_Init.Pin = ACTLED_PIN;
  GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( GPIOA, &GPIO_Init );
#else
//  pinMode( ACTLED, OUTPUT );
#endif

  /* Configure the system clock */
//  SystemClockMSI_Config();
  SystemClockHSE_Config();
  sysClockFreq = HAL_RCC_GetSysClockFreq();
  unl_cpu();

  /* initialize usart1 and start usart1. */
  static const char msg[] = "  1405 isolation board driven by stm32l053.\r\n";
  sci1Handle = usartBegin( USART1, 115200UL, PA9, PA10 );
  usartReadStart( sci1Handle );
  usartPrint( sci1Handle, msg );
  dly_tsk( 10UL );
  usartPrintf( sci1Handle, "    system clock:%uHz board id:%02x\r\n",
    sysClockFreq, BOARD_1405_IDSW );

  /* initialize tasks and start dispatch. */
  tsk_ini();  //
  sta_rdq( ID_task1 );  // start round robin
  while( 1 ){}
}

void task1( void )
{
  while( 1 )
  {
    dly_tsk( 250UL );
    ACTLED_IS_0;
    dly_tsk( 250UL );
    ACTLED_IS_1;
//    pinToggle( ACTLED );
//    HAL_GPIO_TogglePin( GPIOA, ACTLED_PIN );
  }
}

void task2( void )
{
  while( 1 )
  {
    int c = usartGetc( 0 );
    if( c >= 0 )
    {
      usartPutc( 0, c );
    }
    rot_rdq();
  }
}

/*
  RCC_configuration
*/
static void RCC_configuration( void )
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler( void )
{
  volatile int i = 0;
  while(1)
  {
    i++;
//  return;
  }
}

/**
  * @brief  This function handles called from SysTick Handler.
  * @param  None
  * @retval None
  */
void HAL_IncTick( void )
{
  systim++;
  if( (systim % 1000UL) == 999UL ) unixTime++;
}

