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


  NOTICE! One second to the minus 26th power of 2 is 14.9 ns.
---------------------------------------- */
#include  <stm32f10x.h>
#include  <time.h>
#include  <math.h>
#include  <HardwareSerial.h>
#include  <Timer.h>
#include  <Wire.h>
#include  <SPI.h>
#include  <led.h>
#include  <strutil.h>
#include  <pca8574.h>
#include  <eep24aa025.h>
#include  "boardUtility.h"
#include  <wiznet.h>
#include  <gpio.h>
#include  <1703.h>
#include  <1702.h>

#include  <ntp.h>

extern "C"
{
//  #include  <semihosting.h>
  #include  <stdio.h>
  #include  <string.h>
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <stm32f_rtc.h>
  #include  <wizchip_conf.h>
  #include  <peripheral.h>
//#include  <sntp.h>
}


/* ----------------------------------------
    defines
---------------------------------------- */

/* ----------------------------------------
    prototypes
---------------------------------------- */
void RCC_Configuration( void );
void NVIC_Configuration( void );

void msecInterrupt( void );
void secInterrupt( void );

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
SEM_OBJECT sem_obj[ MAX_SEM_NUMBER ];

volatile SYSTIM systim;
volatile time_t unixTime;
volatile uint32_t pps_millisecond_counter;

GPIO gpio;
Serial Serial1;  /* hardware serial 1 */
SPI spi2;
STM32F_I2C i2c2;
BOARD  chronos;
LED actLed;
PCA8574 ic2;
PCA8574 ic3;
PCA8574 ic4;
WIZNET wizchip;
STM32F_TIMER tim1ms;
STM32F_TIMER tim1s;
BD1702 bd1702;

uint16_t reload;
#if 0
uint8_t MAC[6];      // self hardware address
//uint8_t IPVAL[4] = {192,168,100,230};  // self ip address
uint8_t IPVAL[4] = {192,168,100,231};  // self ip address
uint8_t GATEWAY[4] = {192,168,100,1};  // default gateway
uint8_t SUBNET[4] = {255,255,255,0};   // sub net mask
uint8_t DNS[4] = {192,168,100,1};      // primary dns server
#else
uint8_t xMAC[6];
#endif

/* ----------------------------------------
    tasks
---------------------------------------- */
void ppsGenerator( void );  //ID_ppsGenerator
void stackMonitor( void );
void sntpClient( void );
void display( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 6];  // stack for task2
uint8_t tsk3_stk[256 * 6];  // stack for task3

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_sntpClient,(void *)sntpClient, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_display,(void *)display, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_sntpClient );
  sta_tsk( ID_display );
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
  chronos.gpioInit();
  actLed.begin( ACTLED );

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
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    1703 CHRONOS ntp client\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );
//  serialLoopBack( &Serial1 );

  /* initialize i2c2 */
  i2c2.begin( I2C2, SDA2, SCL2 );
  /* initialize pca8574pw */
  ic2.begin( &i2c2, BOARD_1703_IC2_ADR );
  ic3.begin( &i2c2, BOARD_1703_IC3_ADR );
  ic4.begin( &i2c2, BOARD_1703_IC4_ADR );
  bd1702.begin( &ic2, &ic3, &ic4 );
  bd1702.gpioInit();
  bd1702.setHour( localTime.tm_hour );
  bd1702.setMinute( localTime.tm_min );
  bd1702.setSecond( localTime.tm_sec );
  /* initialize 24AA025 */
  EEP24AA025 eep( &i2c2, I2C_ADR_24AA025E48 );
  int cnt = eep.read( MAC_ADDRESS_IN_24AA025E48, xMAC, sizeof(xMAC) );  /* 00:1E:C0 is Microchip Technology Inc. vender code. */
  if( cnt != 6 )
  {
    Serial1.printf( "  Mac address read from eeprom, but failed.\r\n" );
    xMAC[0] = 0xDE; xMAC[1] = 0xAD; xMAC[2] = 0xBE; xMAC[3] = 0xEF; xMAC[4] = 0x00; xMAC[5] = 100;
  }

  /* initialize w5500. */
  chronos.etherGpioInit();

  /* initialize SPI2 */
  spi2.begin( SPI2, PB13, PB14, PB15, SEMID_SPI2 );  /* SPI?,SCK,MISO,MOSI,SEMAPHORE */

  /* initialize wiznet w5500 */
  wizchip.clearNetworkInfo();
  wizchip.setMac( (const uint8_t *)xMAC );
  wizchip.setIp( 192,168,100,231 );
  wizchip.setGateway( 192,168,100,1 );
  wizchip.setSubnet( 255,255,255,0 );
  wizchip.setDns( 192,168,100,1 );
  /* The order of the memsize array is sending and receiving. */
  static const uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};  /* txsize,rxsize */
  wizchip.begin( &spi2, (const uint8_t *)memsize, wizchip_select, wizchip_deselect );

  /* start timer1 and timer8 for 1pps generator. */
  ppsGenerator();

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
  time_t unxTimeBase = unixTime;
  while( 1 )
  {
    /*stack report.*/
    if( (systim - baseTim) >= 10 * 1000UL )
    {
      baseTim = systim;
      Serial1.printf( "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      Serial1.printf( "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      Serial1.printf( "  TASK3:%d/%d\r\n", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
      char buffer[32];
      Serial1.printf( "    %s\r\n", localDateTimeString( buffer, unixTime ) );
    }
    if( (unixTime % 10) < 5 ) actLed.On();
    else actLed.Off();
#if 0
    if( unixTime != unxTimeBase )
    {
      unxTimeBase = unixTime;
      time_t ut = unixTime + TIMEZONE_JST;
      struct tm *t = localtime( (const time_t *)&ut );
      bd1702.setHour( t->tm_hour );
      bd1702.setMinute( t->tm_min );
      bd1702.setSecond( t->tm_sec );
    }
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
    task 1PPS generator.
---------------------------------------- */
void ppsGenerator( void )
{
  tim1ms.begin( TIM1 );
  tim1s.begin( TIM8 );
  tim1ms.frequency( 1000 );
  reload = tim1ms.getAutoReload();
  tim1ms.master();
  tim1s.slave( SRC_TIM1_DST_TIM8, 1, 1000 );
  tim1s.pwm1( TIMx_CH4, PPS, 500 );

  tim1ms.callBack( TIM1_INT_UP, msecInterrupt );
  tim1s.callBack( TIM8_INT_UP, secInterrupt );
  tim1ms.startInterrupt();
  tim1s.startInterrupt();

  tim1ms.start();
  tim1s.start();

  while( 0 )
  {
    rot_rdq();
  }
}

/* ----------------------------------------
    fixed size reload value.
---------------------------------------- */

/* ----------------------------------------
    task sntp client.
    ntp server list
      ntp.nict.jp
      133.243.238.243
      61.205.120.130
      133.243.238.163
      133.243.238.244
      133.243.238.164
      time.windows.com
---------------------------------------- */
void sntpClient( void )
{
  #define  SYNC_OUT_LIMIT_COUNT  4
//  static const uint8_t ntp_server[] = { 133,243,238,164 };
  static const uint8_t ntp_server[] = { 192,168,100,69 };
  NTP sntp( SNTP_CLIENT_SOCKET, ntp_server, NTP_PORT );
  uint16_t reloadBase = reload;
  bool run,lock; run = lock = false;
//  int count,limit,preGapValue,unLockCount; count = 0; preGapValue = 0; unLockCount = 0;
  int count,limit,unLockCount; count = 0; unLockCount = 0;
  char buffer[128];
  while( true )
  {
    dly_tsk( 10 * 1000UL );
    Serial1.printf( "ntp transmit\r\n" );
    uint32_t t1H,t1L;
    uint32_t t2H,t2L;
    uint32_t t3H,t3L;
    uint32_t t4H,t4L;
    uint32_t ret = sntp.transmitC( &t1H, &t1L );
    if( ret > 0 )
    {
      SYSTIM baseTim = systim;
      while( true )
      {
        uint8_t dip[4];
        uint16_t port;
        uint8_t stratum;
        ret = sntp.recieveC( dip, &port, &t2H, &t2L, &t3H, &t3L, &t4H, &t4L, &stratum );
        if( ret > 0 )
        {
          if( stratum < NTP_STRATUM_NOT_SYNC ) bd1702.timeLED( false );
          else bd1702.timeLED( true );
        }
        if( ret > 0 && stratum < NTP_STRATUM_NOT_SYNC )
        {
          unixTime = (time_t)(t3H - UTC_EPOCH_DIFF);
          sprintf( buffer, "ntp recieve from %d.%d.%d.%d:%d\r\n", dip[0],dip[1],dip[2],dip[3],port );
          Serial1.printf( buffer );
          int32_t a; int32_t b = 0x40000000;
          a = t1L >> 2;  /* 0x3FFFFFFF */
          double t1D = ((double)a / (double)b) + (double)t1H;
          a = t2L >> 2;  /* 0x3FFFFFFF */
          double t2D = ((double)a / (double)b) + (double)t2H;
          a = t3L >> 2;  /* 0x3FFFFFFF */
          double t3D = ((double)a / (double)b) + (double)t3H;
          a = t4L >> 2;  /* 0x3FFFFFFF */
          double t4D = ((double)a / (double)b) + (double)t4H;

          dtostrf( t1D, 20, 8, buffer );
          Serial1.printf( "    T1=%s\r\n", buffer );

          dtostrf( t2D, 20, 8, buffer );
          Serial1.printf( "    T2=%s\r\n", buffer );

          dtostrf( t3D, 20, 8, buffer );
          Serial1.printf( "    T3=%s\r\n", buffer );

          dtostrf( t4D, 20, 8, buffer );
          Serial1.printf( "    T4=%s\r\n", buffer );

          double tOFFSET = ((t2D - t1D) - (t4D - t3D)) / 2.0;  // ((t3D + t2D) - (t4D + t1D)) / 2.0
          dtostrf( tOFFSET, 20, 8, buffer );
          Serial1.printf( "    OFFSET : %s(s) ", buffer );

          double tRTT = (t2D - t1D) + (t4D - t3D);  // (t4D - t1D) - (t3D - t2D)
          dtostrf( tRTT, 20, 8, buffer );
          Serial1.printf( "ROUND TRIP : %s(s)\r\n", buffer );

          if( run == true )
          {
            int gap = sntp.variableNewReload( tOFFSET );
            if( tOFFSET > -0.001 && tOFFSET < 0.001 )
            {
              lock = true;
              limit = SYNC_OUT_LIMIT_COUNT;
              sntp.movingAverage( gap );
            }
            else
            {
              if( lock == true )
              {
                if( limit > 0 ) { limit--; gap = sntp.movingAverage( gap ); }  // gap = preGapValue;
                else { lock = false; unLockCount++; }
              }
              else { /* limit = 0; gap = gap; */ }
            }
            reload = reloadBase + gap;
            count++;
#if 1
            int tempI = abs( gap );
            bd1702.setHour( (tempI >= 100) ? 99 : tempI );
            bd1702.setMinute( (unLockCount >= 100) ? 99 : unLockCount );
            bd1702.setSecond( limit );
#endif
            bd1702.lockLED( lock );
            sprintf( buffer, "    RELOAD=%d GAP=%d COUNT=%d UNLOCK=%d",
              (int)reload, gap, count, unLockCount );
            Serial1.printf( buffer );
            dly_tsk( 2UL );
            Serial1.printf( (lock) ? " LOCKED\r\n" : " NOT LOCKED\r\n" );
          }

          run = true;
          break;
        }
        if( (systim - baseTim) >= 100UL ) break;
        rot_rdq();
      }
    }
  }
}


/* ----------------------------------------
    task display.
---------------------------------------- */
void display( void )
{
  while( 1 )
  {
    bd1702.segmentUpdate();
    dly_tsk( 3UL );
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

/* ----------------------------------------
    timer interrupt handlers.
---------------------------------------- */
void msecInterrupt( void )
{
  tim1ms.setAutoReload( reload );
  if( ++pps_millisecond_counter >= 1000 ) pps_millisecond_counter = 0;
}

void secInterrupt( void )
{
  unixTime++;
}

