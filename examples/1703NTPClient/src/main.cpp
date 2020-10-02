/* ----------------------------------------
  stm32f103ret 1703 chronos examples code
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
#include  <stm32f10x.h>
//#include  "stm32f10x_conf.h"
#include  <time.h>
#include  <math.h>
#include  <HardwareSerial.h>
#include  <Timer.h>
#include  <led.h>
#include  <1703.h>
#include  <strutil.h>
#include  "boardUtility.h"


extern "C"
{
//  #include  <semihosting.h>
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <stm32f_rtc.h>
}


/* ----------------------------------------
    defines
---------------------------------------- */

/* ----------------------------------------
    prototypes
---------------------------------------- */
void RCC_Configuration( void );
void NVIC_Configuration( void );

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
SEM_OBJECT sem_obj[ MAX_SEM_NUMBER ];

volatile SYSTIM systim;
volatile time_t unixTime;
volatile time_t sumTotalTime;  /* Total time since startup */

Serial Serial1;  /* hardware serial 1 */
BOARD  chronos;
LED actLed;

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void ppsGenerator( void );  //ID_ppsGenerator

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 4];  // stack for task2

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_ppsGenerator,(void *)ppsGenerator, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_ppsGenerator );
}

/* ----------------------------------------
    main
---------------------------------------- */
int main(void)
{
  RCC_Configuration();
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  NVIC_Configuration();

  /* Setup SysTick Timer for 1 millisec interrupts, also enables Systick and Systick-Interrupt */
  if( SysTick_Config(SystemCoreClock / 1000UL) )
  {
    /* Capture error */
    while (1);
  }

  /* initialize GPIO and external CPU bus. */
  chronos.gpioInit();
  actLed.begin( ACTLED );

//  if( RTC_Init() != 0 )
  if( true )
  {
    struct tm localTime;
    compileDate( &localTime.tm_year, &localTime.tm_mon, &localTime.tm_mday );
    compileTime( &localTime.tm_hour, &localTime.tm_min, &localTime.tm_sec );
    localTime.tm_year -= 1900;
    localTime.tm_mon -= 1;

    unixTime = mktime( &localTime );
    unixTime_Set( unixTime );
  }

  /* initialize serial */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    1703 CHRONOS\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );
//  serialLoopBack( &Serial1 );

  /* initialize tasks and start dispatch. */
  tsk_ini();  //
  sta_rdq( ID_stackMonitor );  // start round robin

  while(1) {}
}

/* ----------------------------------------
    task stack monitor.
---------------------------------------- */
static unsigned int RemainStack( void *stk, unsigned int sz );

void stackMonitor( void )
{
  SYSTIM baseTim = systim;
  while( 1 )
  {
    /*stack report.*/
    if( (systim - baseTim) >= 1 * 1000UL )
    {
      baseTim = systim;
      Serial1.printf( "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      Serial1.printf( "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      char buffer[32];
      Serial1.printf( "    %s\r\n", localDateTimeString( buffer, unixTime ) );
    }
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
//  if( i == sz ) i = 0;

  return sz - i;
}


/* ----------------------------------------
    task 1PPS generator.
---------------------------------------- */
void tim1Interrupt( void );
void tim8Interrupt( void );
STM32F_TIMER tim1;
STM32F_TIMER tim8;
uint16_t reload;

void ppsGenerator( void )
{
  tim1.begin( TIM1 );
  tim8.begin( TIM8 );
  tim1.frequency( 1000 );
  reload = tim1.getAutoReload();
  tim1.master();
  tim8.slave( SRC_TIM1_DST_TIM8, 1, 1000 );
  tim8.pwm1( TIMx_CH4, PPS, 500 );

  tim1.callBack( TIM1_INT_UP, tim1Interrupt );
  tim8.callBack( TIM8_INT_UP, tim8Interrupt );
  tim1.startInterrupt();
  tim8.startInterrupt();

  tim1.start();
  tim8.start();

  while( 1 )
  {
    rot_rdq();
  }
}

void tim1Interrupt( void )
{
  tim1.setAutoReload( reload );
}

void tim8Interrupt( void )
{
  unixTime++;
  actLed.toggle();
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

  /* Enable GPIO clock */
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );

//  RCC_APB2PeriphResetCmd( RCC_APB2Periph_TIM1, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOA, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOB, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOC, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOD, DISABLE );

  /* DMA1,2 clock enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);

#if 0
  /* Enable spi1 clock enable */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_SPI1, DISABLE );
#endif
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
  NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0 );  // VECT_TAB_OFFSET = 0
}
