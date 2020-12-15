/* ----------------------------------------
  stm32f103vet 1314 sdf7 main examples code
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
#include  <Wire.h>
//#include  <SPI.h>
#include  <led.h>
#include  <1314.h>
#include  <boardUtility.h>
#include  <eep24aa025.h>
#include  <wiznet.h>
#include  <sntp.h>
#include  <23lc1024.h>

extern "C"
{
//  #include  <semihosting.h>
  #include  <stdlib.h>
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
BOARD  board;
STM32F_I2C i2c1;
SPI spi2;
SPI spi3;
WIZNET wizchip1;
SRAM_23LC1024  sSRAM;

uint8_t xMAC1[6];
uint8_t xMAC2[6];

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void ntpClient( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 6];  // stack for task2

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_ntpClient,(void *)ntpClient, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_ntpClient );
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
  /* initialize act led */
  LED actled( ACTLED );

  /* initialize serial */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    1314 SDF7 Main\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );
//  serialLoopBack( &Serial1 );

  if( RTC_Init() != 0 )
  {
    struct tm localTime =
      {0,3,16,26,8-1,2020-1900,0,0,0};
    unixTime = mktime( &localTime );
    unixTime_Set( unixTime );
  }

  /* initialize I2C1. */
  i2c1.begin( I2C1, SDA1, SCL1 );
  /* initialize eeprom1. */
  uint8_t mac[6];
  EEP24AA025 eeprom1( &i2c1, 0x50 );
  eeprom1.read( MAC_ADDRESS_IN_24AA025E48, mac, sizeof(mac) );
  i2c1.end();
  memcpy( xMAC1, mac, sizeof(xMAC1) );

  /* initialize eeprom2. */
  i2c1.begin( I2C1, SDA1, SCL1 );
  EEP24AA025 eeprom2( &i2c1, 0x51 );
  eeprom2.read( MAC_ADDRESS_IN_24AA025E48, mac, sizeof(mac) );
  i2c1.end();
  memcpy( xMAC2, mac, sizeof(xMAC2) );

  /* initialize ethernet gpio for w5100. */
  board.etherGpioInit();

  /* initialize SPI2 */
  spi2.begin( SPI2, PB13, PB14, PB15, SEMID_SPI2 );  /* SPI?,SCK,MISO,MOSI,SEMAPHORE */

  /* initialize 23LCV1024 */
  sSRAM.begin( &spi2, RAM_CS );
  sSRAM.mode( SRAM_23LCxxx_MODE_RDMR, SRAM_BYTE_MODE );
  sSRAM.mode( SRAM_23LCxxx_MODE_WRMR, SRAM_BYTE_MODE );

  uint32_t sramAddress = 0UL;
  srand( 1234 + 1 );
  for( int i = 0; i < (int)SRAM_23LCxxx_SIZE; i++ )
  {
    uint8_t rcv,r;
    sSRAM.read( sramAddress++, &rcv );
    r = rand();
    if( rcv != r )
    {
   	  srand( 1234 + 1 );
   	  sramAddress = 0UL;
   	  for( int i = 0; i < (int)SRAM_23LCxxx_SIZE; i++ )
   	  {
   	    sSRAM.write( sramAddress++, (uint8_t)rand() );
   	  }
   	  blinkLED( &actled, 50UL );
    }
  }
  blinkLED( &actled, 500UL );

  /* initialize SPI3 */
  spi3.begin( SPI3, PB3, PB4, PB5, SEMID_SPI3 );  /* SPI?,SCK,MISO,MOSI,SEMAPHORE */

  /* initialize wiznet w5500 */
  wizchip1.clearNetworkInfo();
  wizchip1.setMac( (const uint8_t *)xMAC1 );
  wizchip1.setIp( 192,168,100,50 );
  wizchip1.setGateway( 192,168,100,1 );
  wizchip1.setSubnet( 255,255,255,0 );
  wizchip1.setDns( 192,168,100,1 );
  /* The order of the memsize array is sending and receiving. */
  static const uint8_t memsize[2][4] = {{2,2,2,2},{2,2,2,2}};  /* txsize,rxsize */
  wizchip1.begin( &spi3, (const uint8_t *)memsize, wizchip1_select, wizchip1_deselect );

  while( 0 )
  {
    dly_tsk( 100UL );
    actled.toggle();
  }

  /* initialize tasks and start dispatch. */
  tsk_ini();  //
  sta_rdq( ID_stackMonitor );  // start round robin

  while(1) {}
}

/* ----------------------------------------
    task stack monitor.
---------------------------------------- */
static unsigned int RemainStack( void *stk, unsigned int sz );
#define stack(s)  RemainStack(s,sizeof(s)),sizeof(s)

void stackMonitor( void )
{
  SYSTIM baseTim = systim;
  while( 1 )
  {
    /*stack report.*/
    if( (systim - baseTim) >= 10 * 1000UL )
    {
      baseTim = systim;
      Serial1.printf( "  TASK1:%d/%d\r\n", stack( tsk1_stk ) );
      Serial1.printf( "  TASK2:%d/%d\r\n", stack( tsk2_stk ) );
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
    task ntp Client demo.
    ntp server list
      ntp.nict.jp
        133.243.238.243
        61.205.120.130
        133.243.238.163
        133.243.238.244
        133.243.238.164
---------------------------------------- */
void ntpClient( void )
{
#define  SNTP_CLIENT_SOCKET  3
  static const uint8_t ntp_server[] = { 192,168,100,69 };
  SNTP sntp( SNTP_CLIENT_SOCKET, ntp_server, NTP_PORT );
  while( 1 )
  {
    dly_tsk( 10 * 1000UL );
    Serial1.printf( "ntp transmit\r\n" );
    time_t newTime; uint16_t minor;
    if( sntp.unix( &newTime, &minor ) >= 0 )
    {
      dly_tsk( 1000UL - minor );
      unixTime = newTime + 1;
      unixTime_Set( newTime + 1 );
      char buffer[32];
      Serial1.printf( "NTP %s\r\n", localDateTimeString( buffer, unixTime ) );
    }
    rot_rdq();
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
