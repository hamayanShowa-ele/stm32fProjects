/* ----------------------------------------
  stm32f103 am2320 examples code
  for STMicroelectronics SPL library

  THE SOURCE CODE OF THE FOLLOWING url WAS MODIFIED FOR STM32F.
  https://github.com/asukiaaa/AM2320_asukiaaa

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
#include  <Wire.h>
#include  <Timer.h>
#include  <led.h>
#include  <am2320.h>
#include  <pca8574.h>
#include  <mbStm32f103.h>

extern "C"
{
//  #include  <semihosting.h>
  #include  <system.h>
  #include  <mul_tsk.h>
}


/* ----------------------------------------
    defines
---------------------------------------- */
#define  ACT_LED  PD6

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
STM32F_I2C i2c2;
BOARD board;

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );

uint8_t tsk1_stk[256 * 4];  // stack for task1

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

  /* Setup SysTick Timer for 1 millisec interrupts, also enables Systick and Systick-Interrupt */
  if( SysTick_Config(SystemCoreClock / 1000UL) )
  {
    /* Capture error */
    while (1);
  }

  /* initialize GPIO and external CPU bus. */
  board.gpioInit();

  /* initialize i2c */
  if( i2c2.begin( I2C2, SDA2, SCL2 ) != I2C_SUCCESS )
  {
    while( 1 ) {}
  }
  i2c2.generalCallReset();

  PCA8574 pca( &i2c2, 0x3F );
  PCA8574 pca2( &i2c2, 0x3E );
  uint8_t pcaCount = 0;
  pca2.write( 0xFF );
  while( 1 )
  {
      pca.write( pcaCount );
      int ret = pca2.read();
      if( ret >= 0 )
      {
        pcaCount = (uint8_t)ret;
        pcaCount++;
      }
      dly_tsk( 500 );
  }
  AM2320 am2320( &i2c2 );
  LED led( ACT_LED );
  led.Off();
  while( 1 )
  {
    int result;
    result = am2320.wakeUp();
    float temperature,humidity;
    result = am2320.update( &temperature, &humidity );
    if( result == AM2320_SUCCESS )
    {
#if defined( __SIMIHOSTTING_IMPL )
      char buffer[128];
      sprintf( buffer, "temperature = %f humidity = %f\r\n", temperature, humidity );
      SH_SendString( (const char *)buffer );
#else
      led.toggle();
      pca.write( pcaCount++ );
      dly_tsk( 1 );
      uint8_t data = pca2.read();
      dly_tsk( 1 );
#endif
    }
    else
    {
#if 0
      SH_SendString( "am2320 read error.\r\n" );
#endif
    }
//    pca.toggle( 0 );
//    i2c2.generalCallReset();
//    i2c2.pinsAlternate();
//    i2c2.softReset();

    dly_tsk( 2 * 1000UL );
  }

  /* initialize serial */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    AITENDO MB-STM32F103 rev.2\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );

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
#if defined( __SIMIHOSTTING_IMPL )
      char buffer[128];
      sprintf( buffer, "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      SH_SendString( (const char *)buffer );
      SH_SendString( "\r\n" );
#endif /* __SIMIHOSTTING_IMPL */
    }
#if 0
    struct tm localTime;
    localtime_r( &unixTime, &localTime );
    Serial1.printf( "  %d/%d/%d %d:%d:%d\r\n",
      localTime.tm_year + 1900,
      localTime.tm_mon + 1,
      localTime.tm_mday,
      localTime.tm_hour,
      localTime.tm_min,
      localTime.tm_sec
    );
#endif
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
    temperature and humidity
---------------------------------------- */
void temperature( void )
{
  while( 1 )
  {
    dly_tsk( 1 * 1000UL );
  }
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
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE );

//  RCC_APB2PeriphResetCmd( RCC_APB2Periph_TIM1, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOA, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOB, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOC, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOD, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOE, DISABLE );

  /* DMA1,2 clock enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);

  /* FSMC clock enable */
//  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_FSMC, ENABLE );

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
