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
#include  <gpioInit.h>
#include  <HardwareSerial.h>
#include  <led.h>
#include  <pca8574.h>
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
void gpioInit( void );

void RCC_Configuration( void );
void NVIC_Configuration( void );

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
SYSTIM systim;
time_t unixTime;
time_t startUpTime;

LED actLED;
LED redLED;
LED blueLED;
LED greenLED;

STM32F_I2C i2c1;
BOARD brd;
PCA8574 brdAddress;

Serial Serial1;
Serial gps;

Ethernet eth1;
//static const uint8_t mac[] = {0xDE,0xAD,0xBE,0xEF,0x00,0x01};
static const uint8_t mac[] = {0x00, 0x08, 0xDC, 0x00, 0x00, 0x01 };  // wiznet bender code
static const uint8_t sip[] = {192,168,100,254};

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

extern "C"
{
void debugMsg( const char *msg )
{
  SCI_Puts( SCI_1, msg );
}
}

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
uint8_t txBuf[256];
uint8_t rxBuf[256];
uint8_t gpsTxBuf[256];
uint8_t gpsRxBuf[256];

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
  struct tm localTime;
  int year,month,day;
  compileDate( &year, &month, &day );
  int hour,minute,second;
  compileTime( &hour, &minute, &second );
  localTime.tm_year = year - 1900;
  localTime.tm_mon = month - 1;
  localTime.tm_mday = day;
  localTime.tm_hour = hour;
  localTime.tm_min = minute;
  localTime.tm_sec = second;
  unixTime = mktime( &localTime );
  startUpTime = unixTime;

  /* initialize board gpio */
  GPIO_INIT board;
  board.extBus();
  board.exti( EXT_INT1, EXTI_Trigger_Falling );
  actLED.begin( ACT_LED );
  redLED.begin( LED_RED );
  blueLED.begin( LED_BLUE );
  greenLED.begin( LED_GREEN );
  dly_tsk( 10UL );
  IO_RST_IS_1;  // io reset goes HIGH.
  dly_tsk( 500UL );

  /*setup sci1*/
  Serial1.begin( SCI_1, 115200UL, txBuf, rxBuf, sizeof(txBuf), sizeof(rxBuf) );  // SCI?,baud rate,tx buffer size,rx buffer size
  Serial1.puts( "1401A Virgo test program from SCI_1.\r\n    designed by hamayan.\r\n" );
  /*setup sci2 for GPS*/
  gps.begin( SCI_2, 4800UL, gpsTxBuf, gpsRxBuf, sizeof(gpsTxBuf), sizeof(gpsRxBuf) );  // SCI?,baud rate,tx buffer size,rx buffer size

  i2c1.begin( I2C1, SDA1, SCL1 );
  brdAddress.begin( &i2c1, I2C_BOARD_ADDRESS );
  brdAddress.write( 0xFF );

#if 0
//  if( eth1.begin( mac, sip ) < 0 )
  if( eth1.begin( mac ) < 0 )
  {
    Serial1.puts( "W5300 configuration failed.\r\n" );
    blinkLED( &actLED, 50UL );
  }
  else
  {
    Serial1.puts( "W5300 configuration done.\r\n" );
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

  /* mram or sram checker. */
//  ramCheck( (void *)SRAM_BASE_ADDRESS, MRAM_SIZE, &actLED );
  /* Serial 1 loop back. */
  serialLoopBack( &Serial1, &gps );
//  dump( (const uint8_t *)SRAM_BASE_ADDRESS, 256, &Serial1 );
//  dump( (const uint16_t *)SRAM_BASE_ADDRESS, 256, &Serial1 );
//  dump( (const uint32_t *)SRAM_BASE_ADDRESS, 256, &Serial1 );

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
    Serial1.printf( "BOARD:%02x TASK1 stack %lu/%d\r\n",
      brd.boardSwitchRead( &brdAddress ), sz, sizeof(tsk1_stk) );
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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

  /* DMA1,DMA2 clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  /* FSMC clock enable */
  RCC_AHB3PeriphResetCmd( RCC_AHB3Periph_FSMC, DISABLE );
  RCC_AHB3PeriphClockCmd( RCC_AHB3Periph_FSMC, ENABLE );

  /* I2C1 clock enable */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );
  RCC_APB1PeriphResetCmd( RCC_APB1Periph_I2C1, DISABLE );

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

/* ----------------------------------------
    exti4 interrupt handler.
---------------------------------------- */
void EXTI4_IRQHandler( void )
{
  if( EXTI_GetITStatus( EXTI_Line4 ) == SET )  /*XOVR*/
  {
    EXTI_ClearITPendingBit( EXTI_Line4 );  /* Release of interrupt request factors. */
  }
}

/* ----------------------------------------
    exti5,6,7,8,9 interrupt handler.
---------------------------------------- */
void EXTI9_5_IRQHandler( void )
{
  if( EXTI_GetITStatus( EXTI_Line5 ) == SET )  /*XOVR*/
  {
    EXTI_ClearITPendingBit( EXTI_Line5 );  /* Release of interrupt request factors. */
  }
}

}
/* ----------------------------------------
  end of file
  designed by hamayan since 2008/11/06
---------------------------------------- */
