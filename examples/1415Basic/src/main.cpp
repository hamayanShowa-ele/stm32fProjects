/* ----------------------------------------
  stm32f103vet 1415 arm cbus long examples code
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
#include  <gpio.h>
#include  <HardwareSerial.h>
#include  <Timer.h>
#include  <EXTI.h>
//#include  <FSMC.h>
#include  <led.h>
#include  <1415.h>
#include  <2007.h>
#include  <strutil.h>

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

void tim1Interrupt( void );
void tim8Interrupt( void );

static uint16_t dpRamDividRead( const uint16_t *ram );
static void dpRamRead( const void *ram, size_t size, uint32_t seed, LED *led );
static void dpRamRead( const void *ram, size_t size, uint16_t fixed, LED *led );
static void dpRamRead( const void *ram, size_t size, LED *led );

void cbGANYMEDE_CALL( void );
volatile uint8_t ganymedeUpdate;

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
SEM_OBJECT sem_obj[ MAX_SEM_NUMBER ];

volatile SYSTIM systim;
volatile time_t unixTime;

GPIO gpio;
Serial Serial1;  /* hardware serial 1 */
BOARD_1415 cbus;
LED actLed;

volatile static uint16_t dummy;

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

  /* Setup SysTick Timer for 1 millisec interrupts, also enables Systick and Systick-Interrupt */
  if( SysTick_Config(SystemCoreClock / 1000UL) )
  {
    /* Capture error */
    while (1);
  }

  /* initialize GPIO and external CPU bus. */
  cbus.gpioInit();
  cbus.extBusInit();
  actLed.begin( ACTLED );
  cbus.cbusEnable( true );
  cbus.cbusReset();
  dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
//  while( 1 ) rot_rdq();

  if( RTC_Init() != 0 )
  {
    struct tm localTime;
    compileDate( &localTime.tm_year, &localTime.tm_mon, &localTime.tm_mday );
    compileTime( &localTime.tm_hour, &localTime.tm_min, &localTime.tm_sec );
    localTime.tm_year -= 1900;
    localTime.tm_mon -= 1;

    unixTime = mktime( &localTime ) - TIMEZONE_JST;
    unixTime_Set( unixTime );
  }

  /* initialize serial */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    1415 ARM CBUS LONG\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );
//  serialLoopBack( &Serial1 );

  extiConfig( PF10, EXTI_Trigger_Falling );
  extiCallBack( 10, cbGANYMEDE_CALL );
//  dpRamRead( (const void *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, 1234UL, &actLed );
//  dpRamRead( (const void *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, (uint16_t)0xA5A5, &actLed );
  dpRamRead( (const void *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, &actLed );
//  ramCheck( (void *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, &actLed );
/*
  SRAM_8M sram( 0x00080000, 0x0200 );
  int ret = sram.sram8m();
  if( ret == 0 ) { while( 1 ) {dly_tsk( 5000UL ); actLed.toggle();} }
  else { while( 1 ) {dly_tsk( 250UL ); actLed.toggle();} }
*/

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
    if( (systim - baseTim) >= 10 * 1000UL )
    {
      baseTim = systim;
      Serial1.printf( "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      char buffer[32];
      Serial1.printf( "    %s\r\n", localDateTimeString( buffer, unixTime ) );
      actLed.toggle();
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
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG, ENABLE );

//  RCC_APB2PeriphResetCmd( RCC_APB2Periph_TIM1, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOA, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOB, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOC, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOD, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOE, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOF, DISABLE );
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOG, DISABLE );

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
  dpram read and check.
---------------------------------------- */
/* ----------------------------------------
  dpram divid read.
---------------------------------------- */
static uint16_t dpRamDividRead( volatile const uint16_t *ram )
{
  volatile uint16_t h,l;

  h = *ram++;
  l = *ram;
  dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
  h &= 0x00FF;
  l &= 0x00FF;

  return (h << 8 | l);
}

/* ----------------------------------------
  random pattern read and check.
---------------------------------------- */
static void dpRamRead( const void *ram, size_t size, uint32_t seed, LED *led )
{
  uint16_t loop = 0;
  uint32_t baseTim = millis();
  srand( seed );
  size /= sizeof(uint16_t);
  while( true )
  {
    const volatile uint16_t *ptr = (const volatile uint16_t *)ram;

    volatile uint8_t ganymedeUpdateBase = ganymedeUpdate;
    while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();

    for( int i = 0; i < (int)size; i++ )
    {
      uint16_t rnd = (uint16_t)rand() & 0x00FF;
      uint16_t dat = *ptr++ & 0x00FF;
      if( i < 4096 && dat != rnd )
      {
        while( true )
        {
          dly_tsk( 50UL );
          led->toggle();
          break;
        }
      }
    }
    volatile uint16_t *intr = (volatile uint16_t *)DPRAM_INTR_ADDRESS;
    *intr = loop++;
    if( (millis() - baseTim) >= 500UL )
    {
      baseTim = millis();
      led->toggle();
    }
  }
}

/* ----------------------------------------
  fixed pattern read and check.
---------------------------------------- */
static void dpRamRead( const void *ram, size_t size, uint16_t fixed, LED *led )
{
  uint16_t loop = 0;
  uint32_t baseTim = millis();
  size /= sizeof(uint16_t);
  while( true )
  {
    const volatile uint16_t *ptr = (const volatile uint16_t *)ram;

    volatile uint8_t ganymedeUpdateBase = ganymedeUpdate;
    while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();

    for( int i = 0; i < (int)size; i++ )
    {
      uint16_t rnd = fixed & 0x00FF;
      uint16_t dat = *ptr++ & 0x00FF;
      if( i < 4096 && dat != rnd )
      {
        while( true )
        {
          dly_tsk( 50UL );
          led->toggle();
          break;
        }
      }
    }
    volatile uint16_t *intr = (volatile uint16_t *)DPRAM_INTR_ADDRESS;
    *intr = loop++;
    if( (millis() - baseTim) >= 500UL )
    {
      baseTim = millis();
      led->toggle();
    }
  }
}

/* ----------------------------------------
  increment pattern read and check.
---------------------------------------- */
static void dpRamRead( const void *ram, size_t size, LED *led )
{
  uint16_t loop = 1234;
  uint16_t data = 0;
  uint32_t baseTim = millis();
  uint8_t ganymedeUpdateBase = ganymedeUpdate;
  while( true )
  {
    while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();
    ganymedeUpdateBase = ganymedeUpdate;

    const volatile uint16_t *ptr = (const volatile uint16_t *)ram;
#if 0
    int sz = size / (sizeof(uint16_t) * 2);
    for( int i = 0; i < sz; i++ )
    {
      uint16_t rnd;
      if( i & 1 ) rnd = data ^ 0xFFFF;
      else rnd = data;
      data++;
      uint16_t rcv = dpRamDividRead( ptr );
      if( i < 2048 && rcv != rnd )
      {
        dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
        while( true )
        {
          dly_tsk( 50UL );
          led->toggle();
//          break;
        }
      }
      ptr += 2;
    }
#else
    int sz = size / (sizeof(uint16_t) * 1);
    for( int i = 0; i < sz; i++ )
    {
      uint16_t rnd;
      if( i & 1 ) rnd = data ^ 0xFFFF;
      else rnd = data;
      uint16_t rcv = *ptr;
      if( i < 4096 && rcv != rnd )
      {
        dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
        while( true )
        {
          dly_tsk( 50UL );
          led->toggle();
//          break;
        }
      }
      data++;
      ptr++;
    }
#endif
    *((volatile uint16_t *)DPRAM_INTR_ADDRESS) = loop++;
    dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);

    if( (millis() - baseTim) >= 500UL )
    {
      baseTim = millis();
      led->toggle();
    }
  }
}



/* ----------------------------------------
  dpram int call back routine.
---------------------------------------- */
void cbGANYMEDE_CALL( void )
{
  ganymedeUpdate++;
}

