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
#include  <USART_UART.h>
#include  <EXTI.h>
#include  <strutil.h>
#include  <boardUtility.h>
#include  <led.h>

#include  <1804.h>
#include  <1411.h>

extern "C"
{
  #include  <peripheral.h>
  #include  <mul_tsk.h>
//  #include  <strutil.h>
}

/* ----------------------------------------
  defines.
---------------------------------------- */

/* ----------------------------------------
  prototypes.
---------------------------------------- */
void RCC_Configuration( void );
void NVIC_Configuration( void );
static unsigned int RemainStack( void *stk, unsigned int sz );

void cbINTDP( void );
void cbINTEOLC1( void );
void cbINTEOLC2( void );

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
SYSTIM systim;
time_t unixTime;
time_t startUpTime;
LED actled;
USART_UART Serial1;
BOARD_1804 bd1804;
BOARD_1411 bd1411;

ADC_1804 adc1;
ADC_1804 adc2;
volatile uint16_t adc1Buffer[8],adc2Buffer[8];
volatile int adc1Update,adc2Update;

volatile uint8_t intdpUpdate;
volatile static uint16_t v50Ack;
volatile static uint16_t dummy;

/* ----------------------------------------
  multi task.
---------------------------------------- */
/* ----------------------------------------
  task prototypes.
---------------------------------------- */
void stackMonitor( void );
void adcTask( void );
void tsk_ini( void );

/* ----------------------------------------
  task stacks.
---------------------------------------- */
char tsk1_stk[256 * 8];
char tsk2_stk[256 * 8];

/* ----------------------------------------
  task initialize.
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_adcTask,(void *)adcTask, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );

//  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_adcTask );
}

/* ----------------------------------------
  main.
---------------------------------------- */
int main(void)
{
//  struct tm localTime =
//    {0,23,14,25,8-1,2014-1900,0,0,0};
  int year,month,day;
  compileDate( &year, &month, &day );
  int hour,minute,second;
  compileTime( &hour, &minute, &second );
  unixTime = localDateTimeToUnixtime( year, month, day, hour, minute, second );
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
//  GPIO_INIT board;
  bd1804.gpioInit();
  bd1804.extBus();
  /* initialize leds. */
  actled.begin( ACT_LED );

  /*setup sci 1*/
  Serial1.begin( USART1, 115200UL, TXD1_PIN, RXD1_PIN );
  Serial1.print( "1804 GANYMEDE test program from USART1.\r\n    designed by hamayan.\r\n" );

  /* configure 1411 board. */
//  bd1411.begin( PF11, PF10, PF9, PF8 );

  tsk_ini();  /* initialize tasks. */
//  sta_rdq( ID_stackMonitor );
  sta_rdq( ID_adcTask );

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
  dly_tsk( 5 *1000UL );
  sta_tsk( ID_adcTask );

  while(1)
  {
//  ramCheck( (void *)DPRAM_BASE_ADDRESS, DPRAM_SIZE, &actled );
//  bd1804.dpRamRandomWrite( (void *)DPRAM_BASE_ADDRESS, DPRAM_SIZE, 1234UL, &actled );  /* random */
//  bd1804.dpRamFixedWrite( (void *)DPRAM_BASE_ADDRESS, DPRAM_SIZE, (uint16_t)0xA5A5, &actled ); /* fixed */
//  bd1804.dpRamIncrementWrite( (void *)DPRAM_BASE_ADDRESS, DPRAM_SIZE, &actled );  /* incrment */
//  bd1804.dpRamSineWrite( (void *)DPRAM_BASE_ADDRESS, 2000, 10UL, &actled );  /* sine pattern. */

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


/* ----------------------------------------
    adc task.
---------------------------------------- */
#define  ADC_OFFSET  8192
#define  SINE_BUFFER_ELEMENT_SIZE  1024
#define  SINE_BUFFER_BLOCK_SIZE    64
#define  SINE_BUFFER_CHANNELS      16

//uint16_t sineBuffer[ SINE_BUFFER_ELEMENT_SIZE ];
int16_t sineBuffer[ SINE_BUFFER_ELEMENT_SIZE ];

void adcTask( void )
{
  //  uint16_t *sineBuffer = new uint16_t[ SINE_BUFFER_ELEMENT_SIZE ];
   /* It may be that the HEAP area is lacking. */
  /* Generate sine waveform data.. */
  const int scale = 8000;
  double phase = 2 * M_PI * 30.0 / 360.0;
  for( int i = 0; i < SINE_BUFFER_ELEMENT_SIZE; i++ )
  {
    double d = scale * sin( (2 * M_PI * i / SINE_BUFFER_ELEMENT_SIZE) + phase );
//    sineBuffer[ i ] = (uint16_t)d + ADC_OFFSET;
    sineBuffer[ i ] = (int16_t)d;
    rot_rdq();
  }

  /* configure filter clock. */
  bd1804.fclk( 500 * 1000UL );
  /* configure exti. */
  extiConfig( INTDP, EXTI_Trigger_Falling );
  extiConfig( EOLC1, EXTI_Trigger_Falling );
  extiConfig( EOLC2, EXTI_Trigger_Falling );
  extiCallBack( 3, cbINTDP );
  extiCallBack( 5, cbINTEOLC1 );
  extiCallBack( 4, cbINTEOLC2 );
  /* initialize adc1,adc2. */
  adc1.begin( (volatile uint16_t *)ADC1_BASE_ADDRESS );
  adc2.begin( (volatile uint16_t *)ADC2_BASE_ADDRESS );
  /* configure convert clock. */
  bd1804.convert( 10 * 1000UL );

  while( true )
  {
    int index = 0;
    for( int k = 0; k < SINE_BUFFER_ELEMENT_SIZE / SINE_BUFFER_BLOCK_SIZE; k++ )
    {
      while( (adc1Update % 100) != 99 ) { rot_rdq(); }

      uint16_t *ptr = (uint16_t *)DPRAM_BASE_ADDRESS;
//      int16_t *ptr = (int16_t *)DPRAM_BASE_ADDRESS;
      for( int j = 0; j < SINE_BUFFER_BLOCK_SIZE; j++ )
      {
        uint16_t snd = sineBuffer[ index ];
//        int16_t snd = sineBuffer[ index ];
        for( int i = 0; i < SINE_BUFFER_CHANNELS; )
        {
          rot_rdq();
#if 1
          bd1804.dpRamDividWrite( ptr, snd + i );
          uint16_t rcv = bd1804.dpRamDividRead( (const uint16_t *)ptr );
          if( rcv != (snd + i) )
          {
            actled.toggle();
            dly_tsk( 50UL );
            continue;
          }
          ptr += 2;
#else
          *ptr = snd + i;
//          uint16_t rcv = *ptr;
          int16_t rcv = *ptr;
          if( rcv != (snd + i) )
          {
            actled.toggle();
            dly_tsk( 50UL );
            continue;
          }
          ptr++;
#endif
          i++;
        }
        index++;
      }

      dummy = *((volatile uint16_t *)GANYMEDE_DUMMY_ADDRESS);
      uint8_t intdpUpdateBase = intdpUpdate;
      bd1804.v50INT();
      while( intdpUpdate == intdpUpdateBase ) rot_rdq();
    }
  }
  //  delete [] sineBuffer;
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

  /* clock enable for gpio. */
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
//  RCC_AHB3PeriphResetCmd( RCC_AHB3Periph_FSMC, DISABLE );
//  RCC_AHB3PeriphClockCmd( RCC_AHB3Periph_FSMC, ENABLE );

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

/* ----------------------------------------
    interrupt call back routine.
---------------------------------------- */
void cbINTDP( void )
{
  v50Ack = *((volatile uint16_t *)DPRAM_INTR_ADDRESS);
  dummy = *((volatile uint16_t *)GANYMEDE_DUMMY_ADDRESS);
  intdpUpdate++;
}

void cbINTEOLC1( void )
{
  for( int i = 0; i < (int)(sizeof(adc1Buffer) / sizeof(adc1Buffer[0])); i++ )
  {
    adc1Buffer[i] = adc1.read();
  }
  adc1Update++;
}

void cbINTEOLC2( void )
{
  for( int i = 0; i < (int)(sizeof(adc2Buffer) / sizeof(adc2Buffer[0])); i++ )
  {
    adc2Buffer[i] = adc2.read();
  }
  adc2Update++;
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

}
/* ----------------------------------------
  end of file
  designed by hamayan since 2008/11/06
---------------------------------------- */
