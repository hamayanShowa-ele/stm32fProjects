/* ----------------------------------------
  stm32f103rbt 1405 ISOLATE AD 10ch examples code
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


  NOTICE! One second to the minus 26th power of 2 is 14.9 ns.
---------------------------------------- */
#include  <stm32f10x.h>
#include  <time.h>
#include  <Timer.h>
#include  <USART_UART.h>
#include  <led.h>
#include  <strutil.h>
#include  <boardUtility.h>
#include  <1405.h>

extern "C"
{
//  #include  <semihosting.h>
  #include  <stdio.h>
  #include  <string.h>
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <stm32f_rtc.h>
  #include  <peripheral.h>
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
volatile uint32_t millisecond_counter;
volatile uint32_t pps_millisecond_counter;

USART_UART usart1;
LED actLed;

BOARD_1405 bd1405;

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
}

/* ----------------------------------------
    main
---------------------------------------- */
int main(void)
{
  RCC_Configuration();
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  NVIC_Configuration();

  uint16_t rev = getRevID();
  uint16_t dev = getDevID();
  uint32_t uid[3];
  getUniqueID( uid );

  /* Setup SysTick Timer for 1 millisec interrupts, also enables Systick and Systick-Interrupt */
  if( SysTick_Config(SystemCoreClock / 1000UL) )
  {
    /* Capture error */
    while (1);
  }

  /* initialize GPIO and external CPU bus. */
  bd1405.gpioInit();
  actLed.begin( BD1405_ACTLED );

//  if( RTC_Init() != 0 )
  struct tm localTime;
  if( true )
  {
//    struct tm localTime;
    compileDate( &localTime.tm_year, &localTime.tm_mon, &localTime.tm_mday );
    compileTime( &localTime.tm_hour, &localTime.tm_min, &localTime.tm_sec );
    localTime.tm_year -= 1900;
    localTime.tm_mon -= 1;

    unixTime = mktime( &localTime ) - TIMEZONE_JST;
    unixTime_Set( unixTime );
  }

  /* initialize serial */
  usart1.begin( USART1, 115200UL, USART1_TXD, USART1_RXD );
  usart1.print( "    1405 ISOLATE AD 10ch.\r\n" );
  usart1.print( "    designed by hamayan.\r\n" );
  usart1.printf( "    DEVICE ID:0x%04X REVISION:0x%04X UNIQUE ID:%02X:&02X:%02X.\r\n",
    dev, rev, uid[0], uid[1], uid[2] );
  usart1.printf( "    BOARD ID:0x%02X.\r\n", bd1405.idSW() );
//  usart1.echo();  /* echo back is on. */

  dly_tsk( 5 * 1000UL );

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
//  time_t unxTimeBase = unixTime;
  while( 1 )
  {
    /*stack report.*/
    if( (systim - baseTim) >= 10 * 1000UL )
    {
      baseTim = systim;
      usart1.printf( "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      char buffer[32];
      usart1.printf( "    %s\r\n", localDateTimeString( buffer, unixTime ) );
    }

    if( (unixTime % 10) < 5 ){ actLed.On(); }
    else{ actLed.Off(); }

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
  GPIO gpio;
  gpio.rccClockEnable( RCC_AFIO );
  gpio.rccClockEnable( RCC_GPIOA );
  gpio.rccClockEnable( RCC_GPIOB );
  gpio.rccClockEnable( RCC_GPIOC );
  gpio.rccClockEnable( RCC_GPIOD );

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

/* ----------------------------------------
    timer interrupt handlers.
---------------------------------------- */

