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
#include  <strutil.h>
#include  <1415.h>
#include  <2007.h>
#include  <1405.h>

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
#if 0
static uint16_t dpRamDividRead( volatile const uint16_t *ram );
static void dpRamRandomRead( volatile const uint16_t *ram, size_t size, uint32_t seed, LED *led );
static void dpRamFixedRead( volatile const uint16_t *ram, size_t size, uint16_t fixed, LED *led );
static void dpRamIncrementRead( volatile const uint16_t *ram, size_t size, LED *led );
static void dpRamSineRead( volatile const uint16_t *ram, int scale, LED *led );
static void dpHayashiCheck( volatile const uint16_t *ram, LED *led );
#endif

void cbGANYMEDE_CALL( int num );
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
//BOARD_1405 bd1405;
LED actLed;

volatile static uint16_t dummy;

volatile uint32_t actLedPeriod;

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void check1405( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 6];  // stack for task2
uint8_t tsk3_stk[256 * 6];  // stack for task3
uint8_t tsk4_stk[256 * 6];  // stack for task4
uint8_t tsk5_stk[256 * 6];  // stack for task5
uint8_t tsk6_stk[256 * 6];  // stack for task6

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_check1405_01,(void *)check1405, tsk2_stk, sizeof(tsk2_stk), BD1405_01_IO_ADR,INT6_PIN,0,0 );
  reg_tsk( ID_check1405_02,(void *)check1405, tsk3_stk, sizeof(tsk3_stk), BD1405_02_IO_ADR,INT5_PIN,0,0 );
  reg_tsk( ID_check1405_03,(void *)check1405, tsk4_stk, sizeof(tsk4_stk), BD1405_03_IO_ADR,INT4_PIN,0,0 );
  reg_tsk( ID_check1405_04,(void *)check1405, tsk5_stk, sizeof(tsk5_stk), BD1405_04_IO_ADR,INT3_PIN,0,0 );
  reg_tsk( ID_check1405_05,(void *)check1405, tsk6_stk, sizeof(tsk6_stk), BD1405_05_IO_ADR,INT2_PIN,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_check1405_01 );
//  sta_tsk( ID_check1405_02 );
//  sta_tsk( ID_check1405_03 );
//  sta_tsk( ID_check1405_04 );
//  sta_tsk( ID_check1405_05 );
}

/* ----------------------------------------
    main
---------------------------------------- */
int main(void)
{
  dwt_access_enable();  /* if is there in a debug mode. */
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

#if 0
  extiConfig( PF10, EXTI_Trigger_Falling );  // INT0:PC4 INT6:PF10
  extiCallBack( 10, cbGANYMEDE_CALL );  // INT0:4 INT6:10
//  dpRamRandomRead( (volatile const uint16_t *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, 1234UL, &actLed );
//  dpRamFixedRead( (volatile const uint16_t *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, (uint16_t)0xA5A5, &actLed );
//  dpRamIncrementRead( (volatile const uint16_t *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, &actLed );
//  dpRamSineRead( (volatile const uint16_t *)GANYMEDE_MEM_ADR, 8000, &actLed );
//  ramCheck( (void *)GANYMEDE_MEM_ADR, GANYMEDE_MEM_SIZE, &actLed );
  dpHayashiCheck( (volatile const uint16_t *)GANYMEDE_MEM_ADR, &actLed );

/*
  SRAM_8M sram( 0x00080000, 0x0200 );
  int ret = sram.sram8m();
  if( ret == 0 ) { while( 1 ) {dly_tsk( 5000UL ); actLed.toggle();} }
  else { while( 1 ) {dly_tsk( 250UL ); actLed.toggle();} }
*/
#endif

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
  actLedPeriod = 500UL;
  SYSTIM baseTim = systim;
  SYSTIM actLedTim = systim;
  while( 1 )
  {
    /*stack report.*/
    if( (systim - baseTim) >= 10 * 1000UL )
    {
      baseTim = systim;
      Serial1.printf( "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      Serial1.printf( "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      Serial1.printf( "  TASK3:%d/%d\r\n", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
      Serial1.printf( "  TASK4:%d/%d\r\n", RemainStack( tsk4_stk, sizeof(tsk4_stk) ), sizeof(tsk4_stk) );
      Serial1.printf( "  TASK5:%d/%d\r\n", RemainStack( tsk5_stk, sizeof(tsk5_stk) ), sizeof(tsk5_stk) );
//      Serial1.printf( "  TASK6:%d/%d\r\n", RemainStack( tsk6_stk, sizeof(tsk6_stk) ), sizeof(tsk6_stk) );
      char buffer[32];
      Serial1.printf( "    %s period:%dms\r\n", localDateTimeString( buffer, unixTime ), actLedPeriod );
    }

    /* blink act led. */
    if( (systim - actLedTim) >= actLedPeriod )
    {
      actLedTim = systim;
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


#if 0
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
  dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
  l = *ram;
  dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
  h &= 0x00FF;
  l &= 0x00FF;

  return (h << 8 | l);
}

static uint16_t dpRamDividRead2( volatile const uint16_t *ram )
{
  volatile uint16_t h,l;

  l = *ram++;
  dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
  h = *ram;
  dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
  h &= 0x00FF;
  l &= 0x00FF;

  return (h << 8 | l);
}



#define  ADC_OFFSET  8192
#define  SINE_BUFFER_ELEMENT_SIZE  1024
#define  SINE_BUFFER_BLOCK_SIZE    64
#define  SINE_BUFFER_CHANNELS      16

//uint16_t sineBuffer[ SINE_BUFFER_ELEMENT_SIZE ];
int16_t sineBuffer[ SINE_BUFFER_ELEMENT_SIZE ];

/* ----------------------------------------
  hayashi check.
---------------------------------------- */
#define  HAYASHI_CHANNEL_NUMBER  8
#define  HAYASHI_BLOCK_NUMBER    (2 * 100)

typedef struct
{
  uint16_t Head;
  uint16_t HandShake_L;
  uint16_t HandShake_H;
  uint16_t Data[ HAYASHI_CHANNEL_NUMBER ][ HAYASHI_BLOCK_NUMBER ];
} CBUS_ST1;

static void dpHayashiCheck( volatile const uint16_t *ram, LED *led )
{
  CBUS_ST1 *st1 = (CBUS_ST1 *)ram;
  uint16_t loop = 0;
  uint32_t baseTim = millis();
  uint8_t ganymedeUpdateBase = ganymedeUpdate;

  while( false )
  {
//    dly_tsk( 1 * 1000UL );
    while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();
    ganymedeUpdateBase = ganymedeUpdate;
    *((volatile uint16_t *)DPRAM_INTR_ADDRESS) = loop++;
    dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
    led->toggle();
  }
  while( true )
  {
    while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();
    ganymedeUpdateBase = ganymedeUpdate;
    for( int ch = 0; ch < (HAYASHI_CHANNEL_NUMBER - 1); ch++ )
    {
      uint16_t *ptr = (uint16_t *)st1->Data[ch];
      for( int i = 0; i < (HAYASHI_BLOCK_NUMBER / 2); )
      {
        int16_t rcv = (int16_t)dpRamDividRead2( (const uint16_t *)ptr );
        if( rcv <= -10000 || rcv >= 10000 )
        {
          while( true )
          {
            memcpy( sineBuffer, st1->Data[ch], HAYASHI_BLOCK_NUMBER );
//            dly_tsk( 50UL );
            led->toggle();
            break;
          }
        }
        ptr += 2;
        i++;
      }
    }
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
  sine pattern read and check.
---------------------------------------- */
static void dpRamSineRead( volatile const uint16_t *ram, int scale, LED *led )
{
//  uint16_t *sineBuffer = new uint16_t[ SINE_BUFFER_ELEMENT_SIZE ];
  /* It may be that the HEAP area is lacking. */

  /* Generate sine waveform data.. */
  double phase = 2 * M_PI * 30.0 / 360.0;
  for( int i = 0; i < SINE_BUFFER_ELEMENT_SIZE; i++ )
  {
    double d = scale * sin( (2 * M_PI * i / SINE_BUFFER_ELEMENT_SIZE) + phase );
//    sineBuffer[ i ] = (uint16_t)d + ADC_OFFSET;
    sineBuffer[ i ] = (int16_t)d;
    rot_rdq();
  }

  uint16_t loop = 0;
  uint32_t baseTim = millis();
  uint8_t ganymedeUpdateBase = ganymedeUpdate;

  while( true )
  {
    int index = 0;
    for( int k = 0; k < SINE_BUFFER_ELEMENT_SIZE / SINE_BUFFER_BLOCK_SIZE; k++ )
    {
      while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();
      ganymedeUpdateBase = ganymedeUpdate;

      uint16_t *ptr = (uint16_t *)ram;
//      int16_t *ptr = (int16_t *)ram;
      for( int j = 0; j < SINE_BUFFER_BLOCK_SIZE; j++ )
      {
        uint16_t snd = sineBuffer[ index ];
//        int16_t snd = sineBuffer[ index ];
        for( int i = 0; i < SINE_BUFFER_CHANNELS; )
        {
          rot_rdq();
#if 1
          uint16_t rcv = dpRamDividRead( (const uint16_t *)ptr );
          if( rcv != (snd + i) )
          {
            while( true )
            {
              dly_tsk( 50UL );
              led->toggle();
//              break;
            }
          }
          ptr += 2;
#else
//          uint16_t rcv = *ptr;
          int16_t rcv = *ptr;
          dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
          if( rcv != (snd + i) )
          {
            while( true )
            {
              dly_tsk( 50UL );
              led->toggle();
//              break;
            }
          }
          ptr++;
#endif
          i++;
        }
        index++;
      }

      *((volatile uint16_t *)DPRAM_INTR_ADDRESS) = loop++;
      dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
      if( (millis() - baseTim) >= 500UL )
      {
        baseTim = millis();
        led->toggle();
      }
    }
  }
//  delete [] sineBuffer;
}

/* ----------------------------------------
  random pattern read and check.
---------------------------------------- */
static void dpRamRandomRead( volatile const uint16_t *ram, size_t size, uint32_t seed, LED *led )
{
  uint16_t loop = 0;
  uint32_t baseTim = millis();
  srand( seed );
  uint8_t ganymedeUpdateBase = ganymedeUpdate;
  while( true )
  {
    while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();
    ganymedeUpdateBase = ganymedeUpdate;

    volatile const uint16_t *ptr = ram;
#if 1
    int sz = size / (sizeof(uint16_t) * 2);
    for( int i = 0; i < sz; i++ )
    {
      uint16_t rnd = (uint16_t)rand();
      uint16_t rcv = dpRamDividRead( ptr );
      if( rcv != rnd )
      {
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
    int sz = size / sizeof(uint16_t);
    for( int i = 0; i < sz; i++ )
    {
      uint16_t rnd = (uint16_t)rand();
      uint16_t rcv = *ptr++;
      dummy = *((volatile uint16_t *)CBUS_DUMMY_MEM_ADR);
      if( rcv != rnd )
      {
        while( true )
        {
          dly_tsk( 50UL );
          led->toggle();
//          break;
        }
      }
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
  fixed pattern read and check.
---------------------------------------- */
static void dpRamFixedRead( volatile const uint16_t *ram, size_t size, uint16_t fixed, LED *led )
{
  uint16_t loop = 0;
  uint32_t baseTim = millis();
  size /= sizeof(uint16_t);
  while( true )
  {
    volatile const uint16_t *ptr = ram;
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
static void dpRamIncrementRead( volatile const uint16_t *ram, size_t size, LED *led )
{
  uint16_t loop = 1234;
  uint16_t data = 0;
  uint32_t baseTim = millis();
  uint8_t ganymedeUpdateBase = ganymedeUpdate;
  while( true )
  {
    while( ganymedeUpdate == ganymedeUpdateBase ) rot_rdq();
    ganymedeUpdateBase = ganymedeUpdate;

    volatile const uint16_t *ptr = ram;
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
SYSTIM systimOld = 0UL;
uint32_t cycleCounterOld = 0UL;
void cbGANYMEDE_CALL( int num )
{
  if( (systim - systimOld) >= 3UL )
  {
    ganymedeUpdate++;
    systimOld = systim;
  }
  else
  {
    uint32_t cycleCounterNow = CYCLE_COUNTER();
    uint32_t cycleCounterDiff = cycleCounterNow - cycleCounterOld;
    (void)cycleCounterDiff;
    cycleCounterOld = cycleCounterNow;
  }
}

#endif
