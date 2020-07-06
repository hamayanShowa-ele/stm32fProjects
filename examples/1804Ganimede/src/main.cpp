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
#include  <stm32f4xx.h>
#include  "derivertive.h"
#include  "priority.h"
#include  <string.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <time.h>
#include  <gpioInit.h>
#include  <HardwareSerial.h>

extern "C"
{
  #include  <peripheral.h>
  #include  <mul_tsk.h>
  #include  <strutil.h>
}

/* ----------------------------------------
  defines.
---------------------------------------- */

/* ----------------------------------------
  prototypes.
---------------------------------------- */
void gpioInit( void );

void RCC_Configuration( void );
void NVIC_Configuration( void );
static unsigned int RemainStack( void *stk, unsigned int sz );

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
SYSTIM systim;
time_t unixTime;
time_t startUpTime;

/* ----------------------------------------
  multi task.
---------------------------------------- */
/* ----------------------------------------
  task prototypes.
---------------------------------------- */
void stackMonitor( void );
void tsk_ini( void );

/* ----------------------------------------
  task stacks.
---------------------------------------- */
char tsk1_stk[256 * 8];

/* ----------------------------------------
  task initialize.
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
}

/* ----------------------------------------
  main.
---------------------------------------- */
int main(void)
{
//  struct tm localTime =
//    {0,23,14,25,8-1,2014-1900,0,0,0};
  struct tm localTime;
  int year,month,day;
  compileDate( &year, &month, &day );
  int hour,minute,second;
  compileTime( &hour, &minute, &second );
  localTime.tm_year = year - 1900;
  localTime.tm_mon = month - 1;
  localTime.tm_mday = day;
  localTime.tm_hour = hour;
  localTime.tm_min = minute;
  localTime.tm_sec = second;
  unixTime = mktime( &localTime );
  startUpTime = unixTime;

  RCC_Configuration();
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  NVIC_Configuration();

  if( SysTick_Config(SystemCoreClock / 1000) )
  {
    /* Capture error */
    while (1);
  }

  /* initialize board gpio */
  GPIO_INIT board;
  board.extBus();
  board.exti( EOLC1, EXTI_Trigger_Falling );
  board.exti( EOLC2, EXTI_Trigger_Falling );

  /*setup sci 1*/
  Serial Serial1( SCI_1, 115200UL, 64, 64 );
  Serial1.puts( "1804 GANYMEDE test program from SCI_1.\r\n    designed by hamayan.\r\n" );
  /*setup sci 4*/
  Serial Serial4( SCI_4, 115200UL, 64, 64 );
  /*setup sci 5*/
  Serial Serial5( SCI_5, 115200UL, 64, 64 );

  while( 1 )
  {
    if( Serial1.available() > 0 )
    {
      char buffer[10 + 1];
      char c = Serial1.read();
      for( int i = 0; i < 10; i++ )
      {
        buffer[i] = c;
      }
      buffer[10] = '\0';
      Serial4.printf( buffer );
    }
    if( Serial4.available() > 0 )
    {
      char buffer[2];
      buffer[0] = Serial4.read();
      buffer[1] = '\0';
      Serial5.printf( buffer );
    }
    if( Serial5.available() > 0 )
    {
      char buffer[2];
      buffer[0] = Serial5.read();
      buffer[1] = '\0';
      Serial1.printf( buffer );
    }
  }


  tsk_ini();  /* initialize tasks. */
  sta_rdq( ID_stackMonitor );

  while(1){}

  return 0;
}

/*******************************************************************************
* Function Name  : stackMonitor
* Description    : task stack size report
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void stackMonitor( void )
{
  while(1)
  {
    rot_rdq();
  }
}

static unsigned int RemainStack( void *stk, unsigned int sz )
{
  unsigned int i;
  char *ptr = (char *)stk;

  for( i = 0; i < sz; i++ )
  {
    if( *ptr++ != 0 ) break;
  }

  return sz - i;
}



/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration( void )
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();

  /*stm32f407zetなので、ポートの最大はPGまでとなる*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

  /* DMA1,DMA2 clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  /* FSMC clock enable */
  RCC_AHB3PeriphResetCmd( RCC_AHB3Periph_FSMC, DISABLE );
  RCC_AHB3PeriphClockCmd( RCC_AHB3Periph_FSMC, ENABLE );

  /* Enable clock for SYSCFG. When using an EXTI interrupt. */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE );
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//extern uint32_t _isr_vectorsflash_offs;
void NVIC_Configuration( void )
{
  /* Set the Vector Table base location at 0x08000000+_isr_vectorsflash_offs */
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t)&_isr_vectorsflash_offs);
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
}

extern "C"
{
/* ----------------------------------------
    SysTick_Handler.
---------------------------------------- */
void SysTick_Handler(void)
{
  systim++;
  if( !(systim % 1000UL) )  /* 1sec update */
  {
    unixTime++;
  }
}

/* ----------------------------------------
    exti4 interrupt handler.
---------------------------------------- */
void EXTI4_IRQHandler( void )
{
  if( EXTI_GetITStatus( EXTI_Line4 ) == SET )  /*XOVR*/
  {
    EXTI_ClearITPendingBit( EXTI_Line4 );  /*割込み要求要因の解除*/
  }
}

/* ----------------------------------------
    exti5,6,7,8,9 interrupt handler.
---------------------------------------- */
void EXTI9_5_IRQHandler( void )
{
  if( EXTI_GetITStatus( EXTI_Line5 ) == SET )  /*XOVR*/
  {
    EXTI_ClearITPendingBit( EXTI_Line5 );  /*割込み要求要因の解除*/
  }
}

}
/* ----------------------------------------
  end of file
  designed by hamayan since 2008/11/06
---------------------------------------- */
