/* ----------------------------------------
  1401 Virgo for STM32F407ZET
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
#include  <Timer.h>
#include  <USART_UART.h>
#include  <led.h>
#include  <I2C.h>
#include  <FSMC.h>
#include  <EXTI.h>
#include  <pca8574.h>
#include  <rom24aa025.h>
#include  <Ethernet.h>

/* board check functions. */
#include  <boardUtility.h>
#include  <1401.h>

extern "C"
{
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <socket.h>
}

/* ----------------------------------------
  defines.
---------------------------------------- */

/* ----------------------------------------
  prototypes.
---------------------------------------- */
void RCC_Configuration( void );
void NVIC_Configuration( void );

void cb1PPS_INT( void );
void cbEXTI_INT0( void );
void cbEXTI_INT1( void );
void cbTRIGGER1_INT( void );
void cbTRIGGER2_INT( void );
void cbETHERNET_INT( void );
void cbARCNET_INT( void );

void blink( void );

/* ----------------------------------------
  class libraries.
---------------------------------------- */

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
SYSTIM systim;
time_t unixTime;
time_t startUpTime;

BOARD_1401 bd1401;

LED actLED;
LED redLED;
LED blueLED;
LED greenLED;

STM32F_I2C i2c1;
PCA8574 brdAddress;

STM32F_TIMER fclkTimer;
STM32F_TIMER convertTimer;
STM32F_TIMER blinkTimer;

USART_UART Serial1;
USART_UART gps;

Ethernet eth1;
//static const uint8_t mac[] = {0xDE,0xAD,0xBE,0xEF,0x00,0x01};
//static const uint8_t mac[] = {0x00, 0x08, 0xDC, 0x00, 0x00, 0x01 };  // wiznet bender code
//static const uint8_t sip[] = {192,168,100,254};
uint8_t xMAC[6];
uint8_t xSIP[4] = {192,168,100,222};
static uint8_t boardID;


/* ----------------------------------------
  multi task.
---------------------------------------- */
/* ----------------------------------------
  task prototypes.
---------------------------------------- */
void stackMonitor( void );
void tsk_ini( void );

/* ----------------------------------------
  task stacks.
---------------------------------------- */
uint8_t tsk1_stk[256 * 8];


/* ----------------------------------------
  task initialize.
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
}

/* ----------------------------------------
  main.
---------------------------------------- */
int main(void)
{
  RCC_Configuration();
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  NVIC_Configuration();

  if( SysTick_Config(SystemCoreClock / 1000) )
  {
    /* Capture error */
    while (1);
  }
  //  struct tm localTime =
  //    {0,23,14,25,8-1,2014-1900,0,0,0};
  int year,month,day;
  compileDate( &year, &month, &day );
  int hour,minute,second;
  compileTime( &hour, &minute, &second );
  unixTime = localDateTimeToUnixtime( year, month, day, hour, minute, second );
  startUpTime = unixTime;

  /* initialize board gpio. */
  bd1401.gpioInit();
  bd1401.extBus();
  /* execute io reset. */
  bd1401.ioReset();  // io reset.
  /* configure any leds. */
  actLED.begin( ACT_LED );
  redLED.begin( LED_RED );
  blueLED.begin( LED_BLUE );
  greenLED.begin( LED_GREEN );

  /* filter clock start. */
  fclkTimer.begin( TIM3 );  /* tim3 ch3 */
  bd1401.fclk( &fclkTimer, 100 * 1000UL );

  /* convert clock start. */
  convertTimer.begin( TIM2 );  /* tim2 ch2 */
  bd1401.convert( &convertTimer, 10 * 1000UL );

  /* initialize external cpu bus. */
  FSMC *fsmc = new FSMC;
  /* sram area,16bit */
  /* is62wv51216ebll Read Cycle Time is min 45ns or min 55ns. */
  /* is62wv51216ebll Address Access Time is max 45ns or max 55ns. */
  /* mr2a16ays35 Read Cycle Time is min 35ns. */
  /* mr2a16ays35 Address Access Time is max 35ns. */
  /* is62wv51216ebll Write Cycle Time is min 45ns or min 55ns. */
  /* is62wv51216ebll Address Setup Time to Write End is min 35ns or min 40ns. */
  /* is62wv51216ebll Data Setup to Write End is min 28ns or min 28ns. */
  /* is62wv51216ebll Data Hold from Write End is min 0ns or min 0ns. */
  /* is62wv51216ebll Write pulse width is min 35ns or min 40ns. */
  /* mr2a16ays35 Write Cycle Time is min 35ns. */
  /* mr2a16ays35 Address Setup Time to Write End is min 18ns. */
  /* mr2a16ays35 Data Setup to Write End is min 10ns. */
  /* mr2a16ays35 Data Hold from Write End is min 0ns. */
  /* mr2a16ays35 Write pulse width is min 15ns. */
  fsmc->sram( FSMC_Bank1_NORSRAM1,FSMC_MemoryDataWidth_16b, 4,7, 4,7 );
  /* w5300 area,16bit */
  fsmc->sram( FSMC_Bank1_NORSRAM2,FSMC_MemoryDataWidth_16b, 0,12, 0,9 );
  /* arcnet area,16bit */
  fsmc->sram( FSMC_Bank1_NORSRAM3,FSMC_MemoryDataWidth_16b, 3,11, 3,11 );
  /* external bus area,16bit */
  fsmc->sram( FSMC_Bank1_NORSRAM4,FSMC_MemoryDataWidth_16b, 1,12, 1,9 );
  delete fsmc;
  /* mram or sram checker. */
  ramCheck( (void *)SRAM_BASE_ADDRESS, MRAM_SIZE, &actLED );

  /*setup sci1*/
  Serial1.begin( USART1, 115200UL, TXD1_PIN, RXD1_PIN );
  Serial1.print( "1401A VIRGO test program from USART1.\r\n    designed by hamayan.\r\n" );

  /* setup i2c1 */
  i2c1.begin( I2C1, SDA1, SCL1 );

  /* setup 24aa025e48 */
  uint8_t mac[6];
  EEP24AA025 *eeprom = new EEP24AA025( &i2c1 );
  eeprom->read( MAC_ADDRESS_IN_24AA025E48, mac, sizeof(mac) );
  Serial1.printf( "MAC ADDRESS = %02x.%02x.%02x.%02x.%02x.%02x\r\n",
    mac[0],mac[1],mac[2],mac[3],mac[4],mac[5] );
  memcpy( xMAC, mac, sizeof(xMAC) );
  delete eeprom;

  /* setup pca8574pw */
  brdAddress.begin( &i2c1, I2C_BOARD_ADDRESS );
  brdAddress.write( 0xFF );
  boardID = bd1401.boardSwitchRead( &brdAddress );
  Serial1.printf( "BOARD ID = 0x%02x.\r\n", boardID );

#if 1
  if( eth1.begin( mac, xSIP ) < 0 )
//  if( eth1.begin( xMAC ) < 0 )
  {
    Serial1.print( "W5300 configuration failed.\r\n" );
    blinkLED( &actLED, 50UL );
  }
  else
  {
    Serial1.print( "W5300 configuration done.\r\n" );
  }

  int8_t sn = socket(6, Sn_MR_UDP, 5000, 0);
  uint8_t echoServer[] = {192,168,100,85};
  static const char testMsg[] = "hello world hamayan.";
  #define  UDP_ECHO  9750  // original = 7
  while( sn >= 0 && sn <= 7 )
  {
    int ret = sendto( sn, (uint8_t *)testMsg, sizeof(testMsg), echoServer, UDP_ECHO );
    Serial1.printf( "request from W5300. result = %d\r\n", ret );
    dly_tsk( 1000UL );
  }
  close( sn );
#endif

  /*setup gps serial*/
  gps.begin( USART2, 4800UL, TXD2_PIN, RXD2_PIN );
//  while( 1 ) { int c = gps.read(); if( c > 0 ) Serial1.write( c ); }
//  Serial1.loopBack();

  /* configure exti interrupt. */
  /* 1PPS:PA1 */
  extiConfig( PPS, EXTI_Trigger_Falling );
  extiCallBack( PPS_INT_NUM, cb1PPS_INT );
  /* EXTI_INT1:PC9 */
  extiConfig( EXT_INT1, EXTI_Trigger_Falling );
  extiCallBack( EXT_INT1_NUM, cbEXTI_INT1 );
  /* EXTI_INT0:PG8 */
  extiConfig( EXT_INT0, EXTI_Trigger_Falling );
  extiCallBack( EXT_INT0_NUM, cbEXTI_INT0 );
  /* TRIGGER1:PD3 */
  extiConfig( TRIGGER1, EXTI_Trigger_Falling );
  extiCallBack( TRIGGER1_INT_NUM, cbTRIGGER1_INT );
  /* TRIGGER2:PD2 */
  extiConfig( TRIGGER2, EXTI_Trigger_Falling );
  extiCallBack( TRIGGER2_INT_NUM, cbTRIGGER1_INT );

  /* blink led start. */
  blinkTimer.begin( TIM14 );
//  blinkTimer.frequency( 2UL );
  blinkTimer.period( 0.5 );
  blinkTimer.callBack( TIM14_INT_UP, blink );
  blinkTimer.startInterrupt();
  blinkTimer.start();

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
  while(1)
  {
    dly_tsk( 1 * 1000UL );
    uint32_t sz = RemainStack( (const void *)tsk1_stk, sizeof(tsk1_stk) );
/*
    Serial1.printf( "BOARD:%02x TASK1 stack %lu/%d\r\n",
      brd.boardSwitchRead( &brdAddress ), sz, sizeof(tsk1_stk) );
*/
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

  /*stm32f407zetなので、ポートの最大はPGまでとなる*/
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE );

  /* DMA1,DMA2 clock enable */
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2, ENABLE );

  /* FSMC clock enable */
  RCC_AHB3PeriphResetCmd( RCC_AHB3Periph_FSMC, DISABLE );
  RCC_AHB3PeriphClockCmd( RCC_AHB3Periph_FSMC, ENABLE );

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
    exti1 call back functions.
---------------------------------------- */
/* ----------------------------------------
    1PPS interrupt call back function.
---------------------------------------- */
void cb1PPS_INT( void )
{
}

/* ----------------------------------------
    EXTI_INT0 interrupt call back function.
---------------------------------------- */
void cbEXTI_INT0( void )
{
}

/* ----------------------------------------
    EXTI_INT1 interrupt call back function.
---------------------------------------- */
void cbEXTI_INT1( void )
{
}

/* ----------------------------------------
    TRIGGER1 interrupt call back function.
---------------------------------------- */
void cbTRIGGER1_INT( void )
{
}

/* ----------------------------------------
    TRIGGER2 interrupt call back function.
---------------------------------------- */
void cbTRIGGER2_INT( void )
{
}

/* ----------------------------------------
    ETHERNET interrupt call back function.
---------------------------------------- */
void cbETHERNET_INT( void )
{
}

/* ----------------------------------------
    ARCNET interrupt call back function.
---------------------------------------- */
void cbARCNET_INT( void )
{
}

/* ----------------------------------------
    timer test blink blue led.
---------------------------------------- */
void blink( void )
{
  blueLED.toggle();
}

extern "C"
{
/* ----------------------------------------
    debug message.
---------------------------------------- */
void debugMsg( const char *msg )
{
  serialStringOutput( USART1_IF_NUMBER, msg );
}

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
