/* ----------------------------------------
  2102 DIO-0808 STM32F401RCT test code
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
#include  <strutil.h>
#include  "derivertive.h"
#include  "priority.h"
#include  <USART_UART.h>
#include  <I2C.h>
#include  <SPI.h>
#include  <Timer.h>
#include  <led.h>
#include  <2102.h>
#include  <rom24aa025.h>
#include  <ST7032i.h>
#include  <wiznet.h>
#include  <ntp.h>

extern "C"
{
  #include  <string.h>
  #include  <stdio.h>
  #include  <stdlib.h>
  #include  <time.h>
  #include  <peripheral.h>
  #include  <mul_tsk.h>
  #include  <wizchip_conf.h>
}

/* ----------------------------------------
  defines.
---------------------------------------- */

/* ----------------------------------------
  prototypes.
---------------------------------------- */
void RCC_Configuration( void );
void NVIC_Configuration( void );

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
SEM_OBJECT sem_obj[ MAX_SEM_NUMBER ];

volatile SYSTIM systim;
volatile time_t unixTime;
volatile time_t startUpTime;
volatile uint32_t millisecond_counter;
volatile uint32_t pps_millisecond_counter;

LED actLed;
LED almLed;
USART_UART usart1;
STM32F_TIMER tim1s;
STM32F_TIMER tim1ms;
STM32F_I2C i2c1;
SPI spi2;
BOARD_2102 bd2102;
ST7032i lcd;
WIZNET wizchip;

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

static char pulseOutputMessage[10];
static char pulseInputMessage[10];

/* ----------------------------------------
  multi task.
---------------------------------------- */
/* ----------------------------------------
  task prototypes.
---------------------------------------- */
void stackMonitor( void );
void pulseOut( void );
void pulseIn( void );
void tsk_ini( void );
static unsigned int RemainStack( void *stk, unsigned int sz );

/* ----------------------------------------
  task stacks.
---------------------------------------- */
uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 4];  // stack for task2
uint8_t tsk3_stk[256 * 4];  // stack for task3

/* ----------------------------------------
  task initialize.
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_pulseOut,(void *)pulseOut, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_pulseIn,(void *)pulseIn, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_pulseOut );
  sta_tsk( ID_pulseIn );
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

  /* board 2102 gpio initialize. */
  bd2102.gpioInit();

  /* initialize leds. */
  actLed.begin( ACT_LED );
  almLed.begin( REDLED );
  almLed.Off();

  /*setup sci */
  usart1.begin( STDIO, 115200UL, STDIO_TX_PIN, STDIO_RX_PIN );
  dly_tsk( 20UL );
  usart1.print( "NUCLEO-64(stm32f401ret) test program from USART?.\r\n    designed by hamayan.\r\n" );

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
  usart1.printf( "  Mac address : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
    xMAC[0],xMAC[1],xMAC[2],xMAC[3],xMAC[4],xMAC[5] );

  /*setup spi2 */
  spi2.begin( SPI2, SEMID_SPI2, SPI2_SCK, SPI2_MOSI, SPI2_MISO );

  /* initialize w5500. */
  bd2102.etherGpioInit();
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
  lcd.begin( &i2c1, LCD_RES );
  lcd.init();
  lcd.clear();
  lcd.printf( "2102 DIO-0808\r\n%d.%d.%d.%d", IPVAL[0],IPVAL[1],IPVAL[2],IPVAL[3] );
  lcd.update();

  tsk_ini();  /* initialize tasks. */
  sta_rdq( ID_stackMonitor );

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

  while(1)
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

#if 0
    /*time report.*/
    if( (systim - timeTim) >= 1 * 1000UL )
    {
      timeTim = systim;
      char buffer[32];
//      lcd.clear();
      lcd.setXY( 0, 1 );
      lcd.printf( "%s %s", localTimeString( buffer, unixTime ), swMsg );
      lcd.update();
    }
#else
    /*input pulse report.*/
    if( (systim - timeTim) >= 100UL )
    {
      timeTim = systim;
      lcd.setXY( 0, 0 );
      lcd.printf( "OUT:%s\r\nIN :%s", pulseOutputMessage, pulseInputMessage );
      lcd.update();
    }
#endif

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
    pulseOutputMessage[8] = '\0';

    out00.On();
    strcpy( pulseOutputMessage, "00000001" );
    dly_tsk( 1 * 1000UL );
    out00.Off();
    strcpy( pulseOutputMessage, "00000000" );

    out01.On();
    strcpy( pulseOutputMessage, "00000010" );
    dly_tsk( 1 * 1000UL );
    out01.Off();
    strcpy( pulseOutputMessage, "00000000" );

    out02.On();
    strcpy( pulseOutputMessage, "00000100" );
    dly_tsk( 1 * 1000UL );
    out02.Off();
    strcpy( pulseOutputMessage, "00000000" );

    out03.On();
    strcpy( pulseOutputMessage, "00001000" );
    dly_tsk( 1 * 1000UL );
    out03.Off();
    strcpy( pulseOutputMessage, "00000000" );

    out04.On();
    strcpy( pulseOutputMessage, "00010000" );
    dly_tsk( 1 * 1000UL );
    out04.Off();
    strcpy( pulseOutputMessage, "00000000" );

    out05.On();
    strcpy( pulseOutputMessage, "00100000" );
    dly_tsk( 1 * 1000UL );
    out05.Off();
    strcpy( pulseOutputMessage, "00000000" );

    out06.On();
    strcpy( pulseOutputMessage, "01000000" );
    dly_tsk( 1 * 1000UL );
    out06.Off();
    strcpy( pulseOutputMessage, "00000000" );

    out07.On();
    strcpy( pulseOutputMessage, "10000000" );
    dly_tsk( 1 * 1000UL );
    out07.Off();
    strcpy( pulseOutputMessage, "00000000" );
  }
#endif
}

/* ----------------------------------------
    pulse input demo.
---------------------------------------- */
void pulseIn( void )
{
  GPIO gpio;
  while( 1 )
  {
    dly_tsk( 50UL );
    pulseInputMessage[8] = '\0';

    if( gpio.digitalRead( IN00 ) == LOW ) pulseInputMessage[7] = '0';
    else pulseInputMessage[7] = '1';

    if( gpio.digitalRead( IN01 ) == LOW ) pulseInputMessage[6] = '0';
    else pulseInputMessage[6] = '1';

    if( gpio.digitalRead( IN02 ) == LOW ) pulseInputMessage[5] = '0';
    else pulseInputMessage[5] = '1';

    if( gpio.digitalRead( IN03 ) == LOW ) pulseInputMessage[4] = '0';
    else pulseInputMessage[4] = '1';

    if( gpio.digitalRead( IN04 ) == LOW ) pulseInputMessage[3] = '0';
    else pulseInputMessage[3] = '1';

    if( gpio.digitalRead( IN05 ) == LOW ) pulseInputMessage[2] = '0';
    else pulseInputMessage[2] = '1';

    if( gpio.digitalRead( IN06 ) == LOW ) pulseInputMessage[1] = '0';
    else pulseInputMessage[1] = '1';

    if( gpio.digitalRead( IN07 ) == LOW ) pulseInputMessage[0] = '0';
    else pulseInputMessage[0] = '1';

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
  SystemCoreClockUpdate();

  /* clock enable for gpio. */
  GPIO gpio;
  gpio.rccClockEnable( RCC_GPIOA );
  gpio.rccClockEnable( RCC_GPIOB );
  gpio.rccClockEnable( RCC_GPIOC );
  gpio.rccClockEnable( RCC_GPIOH );

  /* Enable clock for SYSCFG. When using an EXTI interrupt. */
  gpio.rccClockEnable( RCC_SYSCFG );
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
