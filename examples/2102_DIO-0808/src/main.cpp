/* ----------------------------------------
  stm32f103ret 2102 DIO-0808 examples code
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
#include  <I2C.h>
#include  <SPI.h>
#include  <led.h>
#include  <switch.h>
#include  <strutil.h>
#include  <rom24aa025.h>
#include  <boardUtility.h>
#include  <wiznet.h>
#include  <2102.h>
#include  <ST7032i.h>
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
volatile uint32_t millisecond_counter;
volatile uint32_t pps_millisecond_counter;

USART_UART usart1;
SPI spi2;
STM32F_I2C i2c1;
STM32F_TIMER tim1s;
STM32F_TIMER tim1ms;
WIZNET wizchip;

LED actLed;
LED almLed;

BOARD_2102 bd2102;
ST7032i lcd;

#if 0
uint8_t MAC[6];      // self hardware address
//uint8_t IPVAL[4] = {192,168,100,230};  // self ip address
uint8_t IPVAL[4] = {192,168,100,231};  // self ip address
uint8_t GATEWAY[4] = {192,168,100,1};  // default gateway
uint8_t SUBNET[4] = {255,255,255,0};   // sub net mask
uint8_t DNS[4] = {192,168,100,1};      // primary dns server
#else
uint8_t xMAC[6];
uint8_t IPVAL[4];  // self ip address
#endif

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void pulseOut( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 4];  // stack for task2

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_pulseOut,(void *)pulseOut, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_pulseOut );
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
  bd2102.gpioInit();
  actLed.begin( ACTLED );
  almLed.begin( REDLED );
  almLed.Off();

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
  usart1.print( "    2102 DIO-0808.\r\n" );
  usart1.print( "    designed by hamayan.\r\n" );
  usart1.printf( "    DEVICE ID:0x%04X REVISION:0x%04X UNIQUE ID:%02X:02X:02X.\r\n",
    dev, rev, uid[0], uid[1], uid[2] );
//  usart1.echo();  /* echo back is on. */

  /* initialize i2c1 */
  i2c1.begin( I2C1, SDA1, SCL1 );
  /* initialize 24AA025 */
  uint8_t tempUC[6];
  EEP24AA025 eep( &i2c1, I2C_ADR_24AA025E48 );
  int cnt = eep.read( MAC_ADDRESS_IN_24AA025E48, tempUC, sizeof(tempUC) );  /* 00:1E:C0 is Microchip Technology Inc. vender code. */
  if( cnt != 6 )
  {
    usart1.printf( "  Mac address read from eeprom, but failed.\r\n" );
    tempUC[0] = 0xDE; tempUC[1] = 0xAD; tempUC[2] = 0xBE;
    tempUC[3] = 0xEF; tempUC[4] = 0x00; tempUC[5] = 100;
  }
  memcpy( xMAC, tempUC, sizeof(xMAC) );

  /* initialize w5500. */
  bd2102.etherGpioInit();

  /* initialize SPI2 */
  spi2.begin( SPI2, SEMID_SPI2, SPI2_SCK, SPI2_MOSI, SPI2_MISO );  /* SPI?,SEMAPHORE,SCK,MOSI,MISO */

  /* initialize wiznet w5500 */
  wizchip.clearNetworkInfo();
  wizchip.setMac( (const uint8_t *)xMAC );
  wizchip.setIp( 192,168,100,233 );  // already 192,168,100,230 or 231 are used.
  wizchip.setGateway( 192,168,100,1 );
  wizchip.setSubnet( 255,255,255,0 );
  wizchip.setDns( 192,168,100,1 );
  /* The order of the memsize array is sending and receiving. */
  static const uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};  /* txsize,rxsize */
  wizchip.begin( &spi2, (const uint8_t *)memsize, wizchip_select, wizchip_deselect );

  wizchip.getIp( IPVAL );
  usart1.print( "\r\nNetwork informations.\r\n" );
  usart1.printf( "    Mac address %02x:%02x:%02x:%02x:%02x:%02x",
    xMAC[0],xMAC[1],xMAC[2],xMAC[3],xMAC[4],xMAC[5] );
  usart1.printf( " IP address %d.%d.%d.%d\r\n\r\n",
    IPVAL[0],IPVAL[1],IPVAL[2],IPVAL[3] );

  /* initialize AQM1602A. */
//  ST7032i lcd( &i2c1, LCD_RES );
  lcd.begin( &i2c1, LCD_RES );
  lcd.init();
  lcd.clear();
  lcd.printf( "2102 DIO-0808\r\n%d.%d.%d.%d", IPVAL[0],IPVAL[1],IPVAL[2],IPVAL[3] );
  lcd.update();

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
  /* configure ntp client. */
  static const uint8_t ntp_server[] = { 133,243,238,164 };
  //  static const uint8_t ntp_server[] = { 192,168,100,69 };
  NTP sntp( SNTP_CLIENT_SOCKET, ntp_server, NTP_PORT );

  /* start ntp client at once. */
  dly_tsk( 5 * 1000UL );
  time_t second; uint16_t milli;
  sntp.unixSimple( &second, &milli );
  dly_tsk( 1000UL - milli );
  loc_cpu();
  millisecond_counter = 0UL;
  unixTime = second + 1;
  unl_cpu();

  SYSTIM baseTim = systim;
  SYSTIM timeTim = systim;
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

    /*time report.*/
    if( (systim - timeTim) >= 1 * 1000UL )
    {
      timeTim = systim;
      char buffer[32];
//      lcd.clear();
      lcd.setXY( 0, 1 );
      lcd.printf( "%s", localTimeString( buffer, unixTime ) );
      lcd.update();
    }

    if( (unixTime % 10) < 5 ){ actLed.On(); almLed.Off(); }
    else{ actLed.Off(); almLed.On(); }

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
    pulse output demo.
---------------------------------------- */
void pulseOut( void )
{
#if 0
  LED out00( OUT00 );
  LED out01( OUT01 );
  LED out02( OUT02 );
  LED out03( OUT03 );
  SWITCH sw01( SW1 );
  SWITCH sw02( SW2 );
  SWITCH sw03( SW3 );
  SWITCH sw04( SW4 );
  while( true )
  {
    dly_tsk( 100UL );
    if( sw01.push() ) out00.On();
    else out00.Off();

    if( sw02.push() ) out01.On();
    else out01.Off();

    if( sw03.push() ) out02.On();
    else out02.Off();

    if( sw04.push() ) out03.On();
    else out03.Off();
  }
#else
  LED out00( OUT00 );
  LED out01( OUT01 );
  LED out02( OUT02 );
  LED out03( OUT03 );
  LED out04( OUT04 );
  LED out05( OUT05 );
  LED out06( OUT06 );
  LED out07( OUT07 );
  while( true )
  {
    out00.On();
    dly_tsk( 1 * 1000UL );
    out00.Off();

    out01.On();
    dly_tsk( 1 * 1000UL );
    out01.Off();

    out02.On();
    dly_tsk( 1 * 1000UL );
    out02.Off();

    out03.On();
    dly_tsk( 1 * 1000UL );
    out03.Off();

    out04.On();
    dly_tsk( 1 * 1000UL );
    out04.Off();

    out05.On();
    dly_tsk( 1 * 1000UL );
    out05.Off();

    out06.On();
    dly_tsk( 1 * 1000UL );
    out06.Off();

    out07.On();
    dly_tsk( 1 * 1000UL );
    out07.Off();
  }
#endif
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
#if 0
void msecInterrupt( void )
{
  tim1ms.setAutoReload( reload );
  if( ++pps_millisecond_counter >= 1000 ) pps_millisecond_counter = 0;
}

void secInterrupt( void )
{
  unixTime++;
}
#endif

