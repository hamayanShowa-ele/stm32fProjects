/* ----------------------------------------
  stm32f103 graphic lcd examples code
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
//#include  <Wire.h>
#include  <Timer.h>
#include  <led.h>
#include  <lt3593.h>
#include  <draw.h>
#include  <textPrint.h>
#include  <button.h>
#include  <adcDac.h>
#include  <0900.h>
#include  <arcTest.h>
#include  <am2320.h>
#include  <pca8574.h>
#include  <softWire.h>

extern "C"
{
//  #include  <semihosting.h>
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <sw8025.h>
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
STM32F_I2C i2c1;

BOARD  board;  /* initialize gpio */
LT3593 backLight;
ARC_TEST arc;  /* board aries arc test instance. */

const uint16_t rgb565_colors[] =
{
  RGB565_WHITE,
  RGB565_SILVER,
  RGB565_GRAY,
  RGB565_BLACK,
  RGB565_BLUE,
  RGB565_NAVY,
  RGB565_CYAN,
  RGB565_GREEN,
  RGB565_DARKGREEN,
  RGB565_LIME,
  RGB565_RED,
  RGB565_MAROON,
  RGB565_ORANGE,
  RGB565_MAGENTA,
  RGB565_YELLO,
  RGB565_GOLD,
};

STM32F_ADC_DAC adc1;
/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void lcdDemo01( void );
void actLed( void );
void temperature( void );

uint8_t tsk1_stk[256 * 4];  // stack for task1
uint8_t tsk2_stk[256 * 6];  // stack for task2
uint8_t tsk3_stk[256 * 2];  // stack for task3
uint8_t tsk4_stk[256 * 6];  // stack for task4

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo01,(void *)lcdDemo01, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_actLed,(void *)actLed, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );
  reg_tsk( ID_temperature,(void *)temperature, tsk4_stk, sizeof(tsk4_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_lcdDemo01 );
  sta_tsk( ID_actLed );
  sta_tsk( ID_temperature );
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
  board.extBusInit();
  arc.gpioInit();

  dly_tsk( 1 * 1000UL );

  /* initialize i2c */
#if 0
//  if( i2c1.begin( I2C1, SDA1, SCL1 ) != I2C_SUCCESS )
  if( i2c1.begin( I2C2, SDA2, SCL2 ) != I2C_SUCCESS )
  {
    while( 1 ) {}
  }
#endif

#if 0
  PCA8574 pca1( &i2c1, 0x3F );
  PCA8574 pca2( &i2c1, 0x3E );
  dly_tsk( 1UL );
  pca2.write( 0xFF );
  volatile uint8_t pcaCount = 0;
  while( 1 )
  {
    int result;
    result = pca1.write( pcaCount );
    dly100us(1);
    if( result < 0 )
    {
      dly100us(1);
      continue;
    }
    result = pca2.read();
    if( result >= 0 )
    {
      pcaCount = (uint8_t)result;
      pcaCount++;
    }
    dly_tsk( 500UL );
  }
#endif

#if 0
//  STM32F_SOFT_I2C softI2C( I2C1, SDA1, SCL1 );
  STM32F_SOFT_I2C softI2C( I2C2, SDA2, SCL2 );
  SOFT_PCA8574 pca1( &softI2C, 0x3F );
  SOFT_PCA8574 pca2( &softI2C, 0x3E );
  dly_tsk( 1UL );
  int result;
  while( (result = pca2.write( 0xFF )) < 0 )
  {
	  dly_tsk( 1UL );
  }

  volatile uint8_t pcaCount = 0;
  while( 1 )
  {
    result = pca1.write( pcaCount );
    dly100us(1);
    result = pca2.read();
    if( result>= 0 )
    {
      pcaCount = (uint8_t)result;
      pcaCount++;
    }

    dly_tsk( 500UL );
  }
#endif

  LED led( ACT_LED );
  led.On();
#if 1
  STM32F_SOFT_I2C softI2C( I2C1, SDA1, SCL1 );
  SOFT_AM2320 am2320( &softI2C );
  dly_tsk( 1UL );

  /* initialize LCD and graphic controller IC. */
  board.glcdClockInit();
  board.glcdReset();
  board.glcdSleep( false );
  backLight.begin( BRIGHT );
  backLight.brightness( BRIGHTEST ); // BRIGHTEST
  DRAW draw;
  /* There is a problem with the read operation from S1D13743 that stops after a while. */
//  uint8_t s1d13743Revision = draw.revision();
  if( draw.begin() < 0 )
  {
    while( 1 )
    {
      dly_tsk( 1000UL );
    }
  }
  board.glcdInterruptInit();
  draw.vaConfig();
  draw.fillRectangle( 0, 0, 480 - 1, 272 - 1, RGB565_DARKGRAY );  // RGB565_DARKGRAY

  TEXT_PRINT tempTxt(48);
  tempTxt.vaConfig( 0,0,143,49 );
  tempTxt.color( RGB565_MAGENTA );
  tempTxt.clearScreen();
  tempTxt.printf( TYPE_ASCII_48, "Temperature." );

  TEXT_PRINT humiTxt(48);
  humiTxt.vaConfig( 0,50,143,99 );
  humiTxt.color( RGB565_CYAN );
  humiTxt.clearScreen();
  humiTxt.printf( TYPE_ASCII_48, "Humidity." );
  while( 1 )
  {
    am2320.wakeUp();
    float temperature,humidity;
    int result = am2320.update( &temperature, &humidity );

    char buffer[16];
    dtostrf( temperature, 4, 1, buffer );
    tempTxt.clearScreen();
    tempTxt.printf( TYPE_ASCII_48, "%s%c", buffer, 'c' );

    dtostrf( humidity, 3, 1, buffer );
    humiTxt.clearScreen();
    humiTxt.printf( TYPE_ASCII_48, "%s%c", buffer, '%' );

    dly_tsk( 2 * 1000UL );
  }
#else
#if 1
  STM32F_SOFT_I2C softI2C( I2C1, SDA1, SCL1 );
//  STM32F_SOFT_I2C softI2C( I2C1, SDA2, SCL2 );
  SOFT_AM2320 am2320( &softI2C );
  dly_tsk( 1UL );
  while( 1 )
  {
    am2320.wakeUp();
    float temperature,humidity;
    int result = am2320.update( &temperature, &humidity );
    dly_tsk( 2 * 1000UL );
  }
#else
  AM2320 am2320( &i2c1 );
  while( 1 )
  {
    int result = am2320.wakeUp();
    float temperature,humidity;
    result = am2320.update( &temperature, &humidity );
    if( result == AM2320_SUCCESS )
    {
#if defined( __SIMIHOSTTING_IMPL )
      char buffer[128];
      sprintf( buffer, "temperature = %f humidity = %f\r\n", temperature, humidity );
      SH_SendString( (const char *)buffer );
#else
      led.toggle();
#endif
    }
    else
    {
#if 0
      SH_SendString( "am2320 read error.\r\n" );
#endif
    }
    dly_tsk( 2 * 1000UL );
  }
#endif
#endif

  struct tm localTime =
    {0,21,21,24,5-1,2020-1900,0,0,0};
#if 1
  unixTime = mktime( &localTime );
#endif

  /* initialize serial */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    0900 ARIES rev.2\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );

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
    if( (systim - baseTim) >= 1 * 1000UL )
    {
      baseTim = systim;
#if defined( __SIMIHOSTTING_IMPL )
      char buffer[128];
      sprintf( buffer, "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      SH_SendString( (const char *)buffer );
      sprintf( buffer, "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      SH_SendString( (const char *)buffer );
      sprintf( buffer, "  TASK3:%d/%d\r\n", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
      SH_SendString( (const char *)buffer );
      sprintf( buffer, "  TASK4:%d/%d\r\n", RemainStack( tsk4_stk, sizeof(tsk4_stk) ), sizeof(tsk4_stk) );
      SH_SendString( (const char *)buffer );
      SH_SendString( "\r\n" );
#endif /* __SIMIHOSTTING_IMPL */
    }
#if 0
    struct tm localTime;
    localtime_r( &unixTime, &localTime );
    Serial1.printf( "  %d/%d/%d %d:%d:%d\r\n",
      localTime.tm_year + 1900,
      localTime.tm_mon + 1,
      localTime.tm_mday,
      localTime.tm_hour,
      localTime.tm_min,
      localTime.tm_sec
    );
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
    lcd demo 01
---------------------------------------- */
void lcdDemo01( void )
{
  TEXT_PRINT dateTxt(48);
  dateTxt.vaConfig( 200,0,399,49 );
  dateTxt.color( RGB565_GREEN );
  dateTxt.clearScreen();

  TEXT_PRINT timeTxt(48);
  timeTxt.vaConfig( 200,50,399,99 );
  timeTxt.color( RGB565_CYAN );
  timeTxt.clearScreen();

  time_t baseTim = unixTime;
  while( 1 )
  {
    while( unixTime == baseTim ) rot_rdq();

    baseTim = unixTime;

    struct tm localTime;
    localtime_r( (time_t *)&unixTime, &localTime );

    dateTxt.clearScreen();
    dateTxt.printf( TYPE_ASCII_48, "   %02d/%02d", localTime.tm_mon + 1, localTime.tm_mday );

    timeTxt.clearScreen();
    timeTxt.printf( TYPE_ASCII_48, "%02d:%02d:%02d", localTime.tm_hour, localTime.tm_min, localTime.tm_sec );

    dly_tsk( 0.9 * 1000UL );
  }
}

/* ----------------------------------------
    act led
---------------------------------------- */
void actLed( void )
{
  LED led( ACT_LED );
  while( 1 )
  {
    led.toggle();
    dly_tsk( 0.1 * 1000UL );
  }
}

/* ----------------------------------------
    temperature and humidity
---------------------------------------- */
void temperature( void )
{
  while( 1 )
  {
    dly_tsk( 1 * 1000UL );
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

  /* FSMC clock enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_FSMC, ENABLE );

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
