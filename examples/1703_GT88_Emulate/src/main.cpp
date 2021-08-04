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
#include  <Timer.h>
#include  <USART_UART.h>
#include  <I2C.h>
#include  <SPI.h>
#include  <led.h>
#include  <strutil.h>
#include  <pca8574.h>
#include  <rom24aa025.h>
#include  <boardUtility.h>
#include  <wiznet.h>
#include  <gpio.h>
#include  <1703.h>
#include  <1702.h>

#include  <ntp.h>
#include  <gnss.h>

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
USART_UART usart1;
USART_UART gpsSerial;
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
void display( void );
void gt88( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 6];  // stack for task2
uint8_t tsk3_stk[256 * 6];  // stack for task3

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_display,(void *)display, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_gt88,(void *)gt88, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_display );
  sta_tsk( ID_gt88 );
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
  usart1.begin( USART1, 115200UL, USART1_TXD, USART1_RXD );
  usart1.print( "    1703 CHRONOS GT88 Emulate.\r\n" );
  usart1.print( "    designed by hamayan.\r\n" );
  usart1.printf( "    DEVICE ID:0x%04X REVISION:0x%04X UNIQUE ID:%02X:02X:02X.\r\n",
    dev, rev, uid[0], uid[1], uid[2] );
//  usart1.echo();  /* echo back is on. */

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
    usart1.printf( "  Mac address read from eeprom, but failed.\r\n" );
    xMAC[0] = 0xDE; xMAC[1] = 0xAD; xMAC[2] = 0xBE; xMAC[3] = 0xEF; xMAC[4] = 0x00; xMAC[5] = 100;
  }
  usart1.printf( "  Mac address was set next. " );
  usart1.printf( "%02X:%02X:%02X:%02X:%02X:%02X:\r\n", xMAC[0],xMAC[1],xMAC[2],xMAC[3],xMAC[4],xMAC[5] );

  /* initialize w5500. */
  chronos.etherGpioInit();

  /* initialize SPI2 */
  spi2.begin( SPI2, SEMID_SPI2, SPI2_SCK, SPI2_MOSI, SPI2_MISO );  /* SPI?,SEMAPHORE,SCK,MOSI,MISO */

  /* initialize wiznet w5500 */
  wizchip.clearNetworkInfo();
  wizchip.setMac( (const uint8_t *)xMAC );
  wizchip.setIp( 192,168,100,231 );  // already 192,168,100,230 is used.
  wizchip.setGateway( 192,168,100,1 );
  wizchip.setSubnet( 255,255,255,0 );
  wizchip.setDns( 192,168,100,1 );
  /* The order of the memsize array is sending and receiving. */
  static const uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};  /* txsize,rxsize */
  wizchip.begin( &spi2, (const uint8_t *)memsize, wizchip_select, wizchip_deselect );

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
      usart1.printf( "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      usart1.printf( "  TASK3:%d/%d\r\n", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
      char buffer[32];
      usart1.printf( "    %s\r\n", localDateTimeString( buffer, unixTime ) );
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

/* ----------------------------------------
    task gt88.
    ntp server list
      ntp.nict.jp
      133.243.238.243
      61.205.120.130
      133.243.238.163
      133.243.238.244
      133.243.238.164
      time.windows.com
---------------------------------------- */
void gt88( void )
{
  /* initialize gps serial. */
#if  defined(  __GPS_TYPE_FURUNO__ )
  gpsSerial.begin( UART5, 38400UL, UART5_TXD, UART5_RXD );
#elif  defined(  __GPS_TYPE_MT3339__ )
  gpsSerial.begin( UART5, 9600UL, UART5_TXD, UART5_RXD );
#endif  /*__GPS_TYPE_FURUNO__ or __GPS_TYPE_MT3339__ */

  /* generate a gnss instance. */
  GNSS gnss( &gpsSerial );

  /* configure ntp client. */
  static const uint8_t ntp_server[] = { 133,243,238,164 };
//  static const uint8_t ntp_server[] = { 192,168,100,69 };
  NTP sntp( SNTP_CLIENT_SOCKET, ntp_server, NTP_PORT );

  /* start ntp client at once. */
  dly_tsk( 5 * 1000UL );
  time_t second; uint16_t milli;
  sntp.unix( &second, &milli );
  unixTime = second + 1;
  dly_tsk( 1000UL - milli );

  /* start timer1 and timer8 for 1pps generator. */
  time_t unixTimeBase = unixTime;
  ppsGenerator();

  while( 1 )
  {
    while( unixTime == unixTimeBase ) rot_rdq();
    unixTimeBase = unixTime;

    uint16_t year;
    uint8_t month,day,hour,minute,second;
    localDateTime( unixTimeBase,
      &year, &month, &day, &hour, &minute, &second );
    bd1702.setHour( hour );
    bd1702.setMinute( minute );
    bd1702.setSecond( second );

    utcDateTime( unixTimeBase,
      &year, &month, &day, &hour, &minute, &second );
    GPS_GPZDA zda;
    zda.hhmmss = (hour * 3600UL) + (minute * 60UL) + second + 0.0F;
    zda.day = day;
    zda.month = month;
    zda.year = year;
    zda.tzHour = 9;
    zda.tzMinute = 0;
    char gnssBuffer[100];
    gnss.nmeaZDA( &zda, TYPE_GPS, gnssBuffer, sizeof(gnssBuffer) );
    gnss.output( gnssBuffer );
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
void msecInterrupt( void )
{
  tim1ms.setAutoReload( reload );
  if( ++pps_millisecond_counter >= 1000 ) pps_millisecond_counter = 0;
}

void secInterrupt( void )
{
  unixTime++;
}

