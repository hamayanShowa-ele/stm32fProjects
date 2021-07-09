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
#include  <wiznet.h>
#include  <ntp.h>
#include  <echo.h>
#include  <93c46.h>

extern "C"
{
//  #include  <semihosting.h>
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <stm32f_rtc.h>

  #include  <wizchip_conf.h>
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

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
uint8_t xMAC[6] = {0xDE,0xAD,0xBE,0xEF,0x00,0x01};
uint8_t IPVAL[4];  // self ip address

SEM_OBJECT sem_obj[ MAX_SEM_NUMBER ];

volatile SYSTIM systim;
volatile time_t unixTime;

volatile uint32_t pps_millisecond_counter;
STM32F_TIMER tim1ms;
STM32F_TIMER tim1s;

GPIO gpio;
Serial Serial1;  /* hardware serial 1 */
BOARD_1415 cbus;
LED actLed;
WIZNET wizchip;

volatile static uint16_t dummy;
volatile uint32_t actLedPeriod;

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void echoClient( void );
void echoServer( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 6];  // stack for task2
uint8_t tsk3_stk[256 * 6];  // stack for task3

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_echoClient,(void *)echoClient, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_echoServer,(void *)echoServer, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
//  sta_tsk( ID_echoClient );
//  sta_tsk( ID_echoServer );
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

  int tmp_len;
  uint8_t rx_buf0[4] = {0x98,0xBA,0xDC,0xFE,};
  tmp_len = (int)rx_buf0[0] + (int)(rx_buf0[1] << 8) + (int)(rx_buf0[2] << 16) + (int)(rx_buf0[3] << 24);
  tmp_len = (int)rx_buf0[0] + ((int)rx_buf0[1] << 8) + ((int)rx_buf0[2] << 16) + ((int)rx_buf0[3] << 24);
  (void)tmp_len;

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
  Serial1.printf( "    1415 ARM CBUS LONG with W5300\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );
//  serialLoopBack( &Serial1 );

  /* read mac address from eeprom(93C46). */
  EEPROM_93C46 *macRom = new EEPROM_93C46( AT93C46_CS, AT93C46_SK, AT93C46_DI, AT93C46_DO );  /* cs,sk,di,do */
  uint16_t tempUS[ AT93C46_SIZE_MAC / sizeof(uint16_t) ];
  tempUS[0] = macRom->read( AT93C46_ADDRESS_MAC );
  tempUS[1] = macRom->read( AT93C46_ADDRESS_MAC + 2 );
  tempUS[2] = macRom->read( AT93C46_ADDRESS_MAC + 4 );
  xMAC[0] = (uint8_t)(tempUS[0] >> 0);
  xMAC[1] = (uint8_t)(tempUS[0] >> 8);
  xMAC[2] = (uint8_t)(tempUS[1] >> 0);
  xMAC[3] = (uint8_t)(tempUS[1] >> 8);
  xMAC[4] = (uint8_t)(tempUS[2] >> 0);
  xMAC[5] = (uint8_t)(tempUS[2] >> 8);
  (void)tempUS;
  delete macRom;

  /* initialize w5300. */
  cbus.etherGpioInit();  /* reset w5300. */
  wizchip.clearNetworkInfo();
  wizchip.setMac( (const uint8_t *)xMAC );
  wizchip.setIp( 192,168,100,243 );  // already 192,168,100,230 or 231 are used.
  wizchip.setGateway( 192,168,100,1 );
  wizchip.setSubnet( 255,255,255,0 );
  wizchip.setDns( 192,168,100,1 );
  /* The order of the memsize array is sending and receiving. */
  static const uint8_t memsize[2][8] = {{8,8,8,8,8,8,8,8},{8,8,8,8,8,8,8,8}};  /* txsize,rxsize */
  wizchip.begin( (const uint8_t *)memsize );

  Serial1.printf( "\r\nNetwork informations.\r\n" );
  Serial1.printf( "    Mac address %02x:%02x:%02x:%02x:%02x:%02x",
    xMAC[0],xMAC[1],xMAC[2],xMAC[3],xMAC[4],xMAC[5] );
  wizchip.getIp( IPVAL );
  Serial1.printf( " IP address %d.%d.%d.%d\r\n\r\n",
    IPVAL[0],IPVAL[1],IPVAL[2],IPVAL[3] );

  /* initialize tasks and start dispatch. */
  tsk_ini();  //
  sta_rdq( ID_stackMonitor );  // start round robin

  while(1) {}
}


/* ----------------------------------------
    task stack monitor.
---------------------------------------- */
static unsigned int RemainStack( void *stk, unsigned int sz );
static void ntpUpdate();

void stackMonitor( void )
{
  ntpUpdate();
//  while( unixTime % 10 ) rot_rdq();

  /* start another tasks. */
  sta_tsk( ID_echoClient );
  sta_tsk( ID_echoServer );

  actLedPeriod = 500UL;
  SYSTIM baseTim = systim;
  SYSTIM actLedTim = systim;
  uint32_t sntpTim = unixTime;
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
      Serial1.printf( "    %s period:%dms\r\n", localDateTimeString( buffer, unixTime ), actLedPeriod );
    }

    /* sntp update. */
    if( (unixTime - sntpTim) >= (20 * 60UL) )
    {
      sntpTim = unixTime;
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

static void ntpUpdate()
{
  /* configure ntp client. */
  static const uint8_t ntp_server[] = { 133,243,238,164 };
  //  static const uint8_t ntp_server[] = { 192,168,100,124 };
  NTP sntp( SNTP_CLIENT_SOCKET, ntp_server, NTP_PORT );

  /* start ntp client at once. */
  time_t second; uint16_t milli;
  sntp.unixSimple( &second, &milli );
  dly_tsk( 1000UL - milli );
  loc_cpu();
  unixTime = second + 2;
  unixTime_Set( unixTime );
  unl_cpu();
}

/* ----------------------------------------
    task ECHO Client.
---------------------------------------- */
uint8_t txBuffer[16384];
uint8_t rxBuffer[16384];
void echoClient( void )
{
  /* configure echo client. */
  static const uint8_t echo_server[] = { 192,168,100,124 };
  for( int i = 0; i < (int)sizeof(txBuffer); i++ ){ txBuffer[i] = (uint8_t)i; }

  while( 1 )
  {
    TCP_ECHO echo( ECHO_CLIENT_SOCKET, echo_server );
    if( echo.Open( Sn_MR_TCP ) == ECHO_CLIENT_SOCKET && echo.Connect() == SOCK_OK )
    {
      while( 1 )
      {
        int result = echo.client( txBuffer, (int32_t)sizeof(txBuffer), rxBuffer, (int32_t)sizeof(rxBuffer) );
        if( result > 0 )
        {
          for( int i = 0; i < (int)sizeof(txBuffer); i++ )
          {
            if( txBuffer[i] != rxBuffer[i] )
            {
              while( 1 ) rot_rdq();
            }
          }
        }
        else if( result == 0 )
        {
        }
        else  // if( result < 0 )
        {
          break;
        }
      }
    }
    echo.Close();
    dly_tsk( 1000UL );
  }
}

/* ----------------------------------------
    task ECHO Server.
---------------------------------------- */
void echoServer( void )
{
  TCP_ECHO echo( ECHO_SERVER_SOCKET );
  while( 1 )
  {
    int result = echo.server();
    if( result == SOCK_FATAL )
    {
      dly_tsk( 1000UL );
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


