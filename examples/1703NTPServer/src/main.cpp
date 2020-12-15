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
#include  <EXTI.h>
#include  <led.h>
#include  <1703.h>
#include  <strutil.h>
#include  <pca8574.h>
#include  <eep24aa025.h>
#include  "boardUtility.h"
#include  <wiznet.h>
#include  <gpio.h>
#include  <gnss.h>

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

static void ppsGenerator( void );
static void msecInterrupt( void );
static void secInterrupt( void );
static void gpsPPSInterrupt( void );

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
SEM_OBJECT sem_obj[ MAX_SEM_NUMBER ];

volatile SYSTIM systim;
volatile time_t unixTime;
volatile time_t timeToWork;
volatile uint32_t pps_millisecond_counter,ppsLostCount;
uint16_t reload,reloadBase;
time_t preGpsGmtTime,gpsGmtTime;
volatile bool gpsLock = false;

GPIO gpio;
LED actLed;
Serial Serial1;  /* hardware serial 1 */
Serial Serial5;  /* hardware serial 5 for gps */
SPI spi2;
STM32F_I2C i2c2;
BOARD  chronos;
PCA8574 ic2;
PCA8574 ic3;
PCA8574 ic4;
WIZNET wizchip;
STM32F_TIMER tim1ms;  /* millisecond timer. */
STM32F_TIMER tim1s;  /* second timer. */
//STM32F_TIMER tim8;
GPS_PPS gpsPPS;

uint8_t xMAC[6];   // self hardware address
uint8_t IPVAL[4];  // 192,168,100,69 : self ip address

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void gpsReciever( void );  //  ID_gpsReciever,
void correctingTime( void );
void sntpServer( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 6];  // stack for task2
uint8_t tsk3_stk[256 * 6];  // stack for task3
uint8_t tsk4_stk[256 * 6];  // stack for task4

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_gpsReciever,(void *)gpsReciever, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_correctingTime,(void *)correctingTime, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );
  reg_tsk( ID_sntpServer,(void *)sntpServer, tsk4_stk, sizeof(tsk4_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_gpsReciever );
  sta_tsk( ID_correctingTime );
  sta_tsk( ID_sntpServer );
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

    unixTime = mktime( &localTime ) - TIMEZONE_JST;
    unixTime_Set( unixTime );
  }

  /* initialize serial 1 */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    1703 CHRONOS NTP server\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );
//  serialLoopBack( &Serial1 );

  /* initialize i2c2 */
  i2c2.begin( I2C2, SDA2, SCL2 );
  /* initialize pca8574pw */
  ic2.begin( &i2c2, BOARD_1703_IC2_ADR );
  ic3.begin( &i2c2, BOARD_1703_IC3_ADR );
  ic4.begin( &i2c2, BOARD_1703_IC4_ADR );
  ic2.write( 0xFF );  /* input mode */
  ic4.write( 0xFF );  /* input mode */
  ic3.write( 0x40 );  /* output mode */
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
  wizchip.setMac( (const uint8_t *)xMAC );
  wizchip.setIp( 192,168,100,69 );
  wizchip.setGateway( 192,168,100,1 );
  wizchip.setSubnet( 255,255,255,0 );
  wizchip.setDns( 192,168,100,1 );
  /* The order of the memsize array is sending and receiving. */
  static const uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};  /* txsize,rxsize */
  wizchip.begin( &spi2, (const uint8_t *)memsize, wizchip_select, wizchip_deselect );
  wizchip.getIp( IPVAL );  // ip address

  /* regist gps 1pps interrupt. */
  extiCallBack( PPS_IRQ_NUMBER, gpsPPSInterrupt );
#if  defined(  __GPS_TYPE_FURUNO__ )
  extiConfig( PPS, EXTI_Trigger_Rising, 3, 0 );
#elif  defined(  __GPS_TYPE_MT3339__ )
  extiConfig( PPS, EXTI_Trigger_Falling, 3, 0 );
#endif  /*__GPS_TYPE_FURUNO__ or __GPS_TYPE_MT3339__ */

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
      char buffer[32];
      Serial1.printf( "    %s %lus\r\n", localDateTimeString( buffer, unixTime ), timeToWork );
    }
    if( (unixTime % 10) < 5 ) actLed.On();
    else actLed.Off();

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
    task sntp server.
    ntp server list
      ntp.nict.jp
      133.243.238.243
      61.205.120.130
      133.243.238.163
      133.243.238.244
      133.243.238.164
      time.windows.com
---------------------------------------- */
void sntpServer( void )
{
  uint8_t dip[4];
  uint16_t dport;
  NTP sntp( SNTP_SERVER_SOCKET );
  sntp.refID( (const uint8_t *)IPVAL );

  while( true )
  {
    rcvNtpPacketFormat pkt;
    uint32_t ret = sntp.recieveS( &pkt, dip, &dport );
    if( ret > 0 )
    {
      sntp.transmitS( (sndNtpPacketFormat *)&pkt, (const uint8_t *)dip, dport );
      char buffer[128];
      sprintf( buffer, "ntp recieve from %d.%d.%d.%d:%d\r\n", dip[0],dip[1],dip[2],dip[3],dport );
      Serial1.printf( buffer );
    }
    rot_rdq();
  }
}

/* ----------------------------------------
    gps reciever task.
---------------------------------------- */
void gpsReciever( void )
{
  /* initialize serial 5 */
#if  defined(  __GPS_TYPE_FURUNO__ )
  Serial5.begin( SCI_5, 38400UL, SCI_5_SND_BUFFER_SIZE, SCI_5_RCV_BUFFER_SIZE );
#elif  defined(  __GPS_TYPE_MT3339__ )
  Serial5.begin( SCI_5, 9600UL, SCI_5_SND_BUFFER_SIZE, SCI_5_RCV_BUFFER_SIZE );
#endif  /*__GPS_TYPE_FURUNO__ or __GPS_TYPE_MT3339__ */
  GNSS gnss( &Serial5 );
  GPS_GPZDA zda;

  while( 1 )
  {
    char gpsRcvBuf[ 128 ];
    int ret = gnss.line( gpsRcvBuf, sizeof(gpsRcvBuf) );
//    if( ret > 0 ) { Serial1.printf( gpsRcvBuf ); Serial1.printf("\r\n"); }
    ret = gnss.gpzda( (const char *)gpsRcvBuf, &zda );
    if( ret != 0 )
    {
      struct tm gpsT;
      gnss.gpsGMTtime( (const GPS_GPZDA *)&zda, &gpsT );
#if  defined(  __GPS_TYPE_FURUNO__ )
      preGpsGmtTime = mktime( &gpsT );
#elif  defined(  __GPS_TYPE_MT3339__ )
      preGpsGmtTime = mktime( &gpsT ) + 1;
#endif  /*__GPS_TYPE_FURUNO__ or __GPS_TYPE_MT3339__ */
    }
    rot_rdq();
  }
}

/* ----------------------------------------
    correcting the time task.
---------------------------------------- */
void correctingTime( void )
{
  bool run,lock; run = lock = false;
  int ppsRetry = 0;
  ppsLostCount = 0UL;
  uint32_t gpsPPSupdateBase = gpsPPS.update;
  while( 1 )
  {
    SYSTIM systimBase = systim;
    while( gpsPPS.update == gpsPPSupdateBase )
    {
      if( (systim - systimBase) >= 1100UL )
      {
        systimBase = systim;
        ppsLostCount++;
        if( ++ppsRetry >= 10 )
        {
          ppsRetry = 0;
          gpsLock = false;
        }
      }
      rot_rdq();
    }
    gpsPPSupdateBase = gpsPPS.update;
    if( run == false && gpsGmtTime != 0 )
    {
      unixTime = gpsGmtTime;
      run = true;
    }
    else if( run == true )
    {
      /* microsecond */
      double a = ((double)gpsPPS.us / (double)(gpsPPS.ar + 1)) / 1000.0;
      /* millisecond */
      a += (double)gpsPPS.ms / 1000.0;
#if 1
      /* offset calibration. */
      double b = (0.0008 - 0.00015);
      if( a >= b ) a -= b;
      else
      {
        gpsPPS.sec = gpsPPS.sec - 1;
        a = a + 1.0 - b;
      }
#endif
      gpsPPS.fewDigit = a;
      double selfTime = a;
      selfTime += gpsPPS.sec;
      double diff = (double)gpsGmtTime - selfTime;
      #define  STEP  1024
      int gap = 0;
      if( diff <= -1.0 || diff >= 1.0 )
      {
        unixTime = gpsGmtTime;
        gpsLock = false;
      }
      else
      {
#if 0
        if( diff > 0.5 ) gap = (STEP / 1) * (-1);
        else if( diff > 0.2 ) gap = (STEP / 2) * (-1);
        else if( diff > 0.1 ) gap = (STEP / 4) * (-1);
        else if( diff > 0.05 ) gap = (STEP / 8) * (-1);
        else if( diff > 0.02 ) gap = (STEP / 16) * (-1);
        else if( diff > 0.01 ) gap = (STEP / 32) * (-1);
        else if( diff > 0.005 ) gap = (STEP / 64) * (-1);
        else if( diff > 0.002 ) gap = (STEP / 128) * (-1);
        else if( diff > 0.001 ) gap = (STEP / 256) * (-1);
        else if( diff > 0.0005 ) gap = (STEP / 512) * (-1);
        else if( diff > 0.0002 ) gap = (-1);
        else if( diff < -0.5 ) gap = STEP / 1;
        else if( diff < -0.2 ) gap = STEP / 2;
        else if( diff < -0.1 ) gap = STEP / 4;
        else if( diff < -0.05 ) gap = STEP / 8;
        else if( diff < -0.02 ) gap = STEP / 16;
        else if( diff < -0.01 ) gap = STEP / 32;
        else if( diff < -0.005 ) gap = STEP / 64;
        else if( diff < -0.002 ) gap = STEP / 128;
        else if( diff < -0.001 ) gap = STEP / 256;
        else if( diff < -0.0005 ) gap = STEP / 512;
        else if( diff < -0.0002 ) gap = 1;
        else gap = 0;
#else
        int diffI = (int)((diff + 0.00005) * 10000.0);
        if( diffI > 5000 ) gap = (STEP / 1) * (-1);
        else if( diffI > 2000 ) gap = (STEP / 2) * (-1);
        else if( diffI > 1000 ) gap = (STEP / 4) * (-1);
        else if( diffI > 500 ) gap = (STEP / 8) * (-1);
        else if( diffI > 200 ) gap = (STEP / 16) * (-1);
        else if( diffI > 100 ) gap = (STEP / 32) * (-1);
        else if( diffI > 50 ) gap = (STEP / 64) * (-1);
        else if( diffI > 20 ) gap = (STEP / 128) * (-1);
        else if( diffI > 10 ) gap = (STEP / 256) * (-1);
        else if( diffI > 5 ) gap = (STEP / 512) * (-1);
        else if( diffI > 2 ) gap = (-1);
        else if( diffI < -5000 ) gap = STEP / 1;
        else if( diffI < -2000 ) gap = STEP / 2;
        else if( diffI < -1000 ) gap = STEP / 4;
        else if( diffI < -500 ) gap = STEP / 8;
        else if( diffI < -200 ) gap = STEP / 16;
        else if( diffI < -100 ) gap = STEP / 32;
        else if( diffI < -50 ) gap = STEP / 64;
        else if( diffI < -20 ) gap = STEP / 128;
        else if( diffI < -10 ) gap = STEP / 256;
        else if( diffI < -5 ) gap = STEP / 512;
        else if( diffI < -2 ) gap = 1;
        else gap = 0;

        if( gap >= -10 && gap <= 10 ) gpsLock = true;
        else gpsLock = false;
#endif
        reload = reloadBase + gap;
      }
#if 1
      char buffer[128],buffer2[32],buffer3[32];
      dtostrf( diff, 20, 8, buffer2 );
      dtostrf( selfTime, 20, 8, buffer3 );
      sprintf( buffer, "    RELOAD=%d GAP=%d LOST=%d GMT=%d SELF=",
        (int)reload, gap, (int)ppsLostCount, (int)gpsGmtTime );
      Serial1.printf( buffer );
      Serial1.printf( buffer3 );
      Serial1.printf( " DIFF=" );
      Serial1.printf( buffer2 );
      Serial1.printf( "\r\n" );
#endif
    }
    rot_rdq();
  }
}

/* ----------------------------------------
    clock print test task.
---------------------------------------- */
void clockPrint( void )
{
  uint32_t sec = gpsPPS.update;
  while( 1 )
  {
    while( gpsPPS.update == sec ) rot_rdq();
    sec = gpsPPS.update;
    char buffer[32];
    Serial1.printf( "    GPS time %s\r\n", localDateTimeString( buffer, gpsGmtTime ) );
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

  /* configure adc1,2,3 clock. */
  RCC_ADCCLKConfig( RCC_PCLK2_Div8 );
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
    1PPS generator.
---------------------------------------- */
static void ppsGenerator( void )
{
  tim1ms.begin( TIM5 );  /* 1 millisecond timer. */
  tim1s.begin( TIM1 );  /* second timer. */
  tim1ms.frequency( 1000 );
  reload = tim1ms.getAutoReload();
  reloadBase = reload;
  tim1ms.master();
  tim1s.slave( SRC_TIM5_DST_TIM1, 1, 1000 );
  tim1s.pwm1( TIMx_CH4, DIVCLK, 500 );

  tim1ms.callBack( TIM5_INT_UP, msecInterrupt );
  tim1s.callBack( TIM1_INT_UP, secInterrupt );
  tim1ms.startInterrupt();
  tim1s.startInterrupt();

  tim1ms.start();
  tim1s.start();
}

/* ----------------------------------------
    timer millisecond interrupt handlers.
---------------------------------------- */
static void msecInterrupt( void )
{
  tim1ms.setAutoReload( reload );
  if( ++pps_millisecond_counter >= 1000 )
  {
    pps_millisecond_counter = 0;
    unixTime++;
  }
}

/* ----------------------------------------
    timer second interrupt handlers.
---------------------------------------- */
static void secInterrupt( void )
{
  timeToWork++;
}

/* ----------------------------------------
    gps 1pps interrupt handlers.
---------------------------------------- */
static void gpsPPSInterrupt( void )
{
  gpsPPS.us = tim1ms.getCounter();
  gpsPPS.ar = tim1ms.getAutoReload();
//  gpsPPS.ms = tim1s.getCounter();
  gpsPPS.ms = pps_millisecond_counter;
  gpsPPS.sec = (uint32_t)unixTime;
  gpsGmtTime = preGpsGmtTime;
  gpsPPS.update++;
}

