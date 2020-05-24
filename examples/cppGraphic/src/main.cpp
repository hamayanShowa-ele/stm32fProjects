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
#include  <Timer.h>
#include  <led.h>
#include  <0900.h>
#include  <lt3593.h>
#include  <draw.h>
#include  <textPrint.h>
#include  <button.h>

extern "C"
{
//  #include  <semihosting.h>
  #include  <system.h>
  #include  <mul_tsk.h>
  #include  <sw8025.h>
  #include  <sjisTxt.h>
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

SYSTIM systim;
time_t unixTime;
time_t sumTotalTime;  /* Total time since startup */

Serial Serial1;  /* hardware serial 1 */
BOARD_0900 board;  /* initialize gpio */
LT3593 backLight;

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

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void lcdDemo01( void );
void lcdDemo02( void );
void lcdDemo03( void );
void lcdDemo04( void );
void lcdDemo05( void );
void lcdDemo06( void );
void lcdDemo07( void );

uint8_t tsk1_stk[256 * 4];  // stack for task1
uint8_t tsk2_stk[256 * 3];  // stack for task2
uint8_t tsk3_stk[256 * 4];  // stack for task3
uint8_t tsk4_stk[256 * 4];  // stack for task4
uint8_t tsk5_stk[256 * 3];  // stack for task5
uint8_t tsk6_stk[256 * 3];  // stack for task6
uint8_t tsk7_stk[256 * 3];  // stack for task7
uint8_t tsk8_stk[256 * 6];  // stack for task8

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo01,(void *)lcdDemo01, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo02,(void *)lcdDemo02, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo03,(void *)lcdDemo03, tsk4_stk, sizeof(tsk4_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo04,(void *)lcdDemo04, tsk5_stk, sizeof(tsk5_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo05,(void *)lcdDemo05, tsk6_stk, sizeof(tsk6_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo06,(void *)lcdDemo06, tsk7_stk, sizeof(tsk7_stk), 0,0,0,0 );
  reg_tsk( ID_lcdDemo07,(void *)lcdDemo07, tsk8_stk, sizeof(tsk8_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
  sta_tsk( ID_lcdDemo01 );
  sta_tsk( ID_lcdDemo02 );
  sta_tsk( ID_lcdDemo03 );
  sta_tsk( ID_lcdDemo04 );
  sta_tsk( ID_lcdDemo05 );
  sta_tsk( ID_lcdDemo06 );
  sta_tsk( ID_lcdDemo07 );
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

  /* initialize LCD and graphic controller IC. */
  board.glcdClockInit();
  board.glcdReset();
  board.glcdSleep( false );
  backLight.begin( BRIGHT );
  backLight.brightness( BRIGH_LV16 ); // BRIGHTEST
  DRAW draw;
  if( draw.begin() < 0 )
  {
    while( 1 )
    {
      dly_tsk( 1000UL );
    }
  }
  board.glcdInterruptInit();
  draw.vaConfig();
  draw.fillRectangle( 0, 0, 480 - 1, 272 - 1, RGB565_DARKGRAY );  //

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
      sprintf( buffer, "  TASK5:%d/%d\r\n", RemainStack( tsk5_stk, sizeof(tsk5_stk) ), sizeof(tsk5_stk) );
      SH_SendString( (const char *)buffer );
      sprintf( buffer, "  TASK6:%d/%d\r\n", RemainStack( tsk6_stk, sizeof(tsk6_stk) ), sizeof(tsk6_stk) );
      SH_SendString( (const char *)buffer );
      sprintf( buffer, "  TASK7:%d/%d\r\n", RemainStack( tsk7_stk, sizeof(tsk7_stk) ), sizeof(tsk7_stk) );
      SH_SendString( (const char *)buffer );
      sprintf( buffer, "  TASK8:%d/%d\r\n", RemainStack( tsk8_stk, sizeof(tsk8_stk) ), sizeof(tsk8_stk) );
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
  DRAW draw;
  draw.vaConfig( 0,0,199,199 );

  while( 1 )
  {
    draw.fillRectangle( 0, 0, 199, 199, RGB565_BLACK );
    for( int i = 0; i < 1000; i++ )
    {
      /* Use a random number to generate a color code. */
      uint32_t color24;
      color24 = ((rand() % 0x80) + 0x80) << 16;
      color24 |= ((rand() % 0x80) + 0x80) << 8;
      color24 |= ((rand() % 0x80) + 0x80) << 0;

      /* Converts 24 bits to 16 bits. */
      uint16_t color16 = draw.rgb24to16( color24 );

      /* Generate coordinates. */
      int x1 = rand() % 480;
      int x2 = rand() % 480;
      int y1 = rand() % 272;
      int y2 = rand() % 272;
      /* Drawing a line. */
      draw.line( x1, y1, x2, y2, color16 );
      dly_tsk( 0.05 * 1000UL );
    }
  }
}

/* ----------------------------------------
    lcd demo 02
---------------------------------------- */
void lcdDemo02( void )
{
  TEXT_PRINT dateTxt(48);
  dateTxt.vaConfig( 200,0,399,49 );
  dateTxt.color( RGB565_GREEN );
  dateTxt.clear();

  TEXT_PRINT timeTxt(48);
  timeTxt.vaConfig( 200,50,399,99 );
  timeTxt.color( RGB565_CYAN );
  timeTxt.clear();

  time_t baseTim = unixTime;
  while( 1 )
  {
    while( unixTime == baseTim ) rot_rdq();

    baseTim = unixTime;

    struct tm localTime;
    localtime_r( &unixTime, &localTime );

    char buffer[16];
    sprintf( buffer, "  %02d/%02d",
//      localTime.tm_year + 1900,
      localTime.tm_mon + 1,
      localTime.tm_mday );
    dateTxt.clear();
    dateTxt.puts( buffer, TYPE_ASCII_48 );

    sprintf( buffer, "%02d:%02d:%02d\r\n",
      localTime.tm_hour,
      localTime.tm_min,
      localTime.tm_sec );
    timeTxt.clear();
    timeTxt.puts( buffer, TYPE_ASCII_48 );

    dly_tsk( 0.9 * 1000UL );
  }
}

/* ----------------------------------------
    lcd demo 03
---------------------------------------- */
void lcdDemo03( void )
{
  DRAW draw;
  draw.vaConfig( 300,100,399,199 );
  int angle = 0;
  while( 1 )
  {
    APEX apex[3];
    for( int i = 0; i < (int)(sizeof(apex) / sizeof(apex[0])); i++ )
    {
      double R = 50.0;
      double yD = sin( (M_PI * (2 * (angle + (i * 120)))) / 360.0 );
      double xD = cos( (M_PI * (2 * (angle + (i * 120)))) / 360.0 );
      int xI = 50 + (int)((xD * R) + 0.5);
      int yI = 50 + (int)((yD * R) + 0.5);
      apex[i].x = xI; apex[i].y = yI;
      rot_rdq();
    }

    draw.fillRectangle( 0, 0, 100, 100, RGB565_DARKGREEN );
    draw.fillTriangle( apex, RGB565_GOLD );
    angle += 10;
    angle %= 360;

    dly_tsk( 100UL );
  }
}

/* ----------------------------------------
    lcd demo 04
---------------------------------------- */
void lcdDemo04( void )
{
  DRAW draw;
  draw.vaConfig( 200,100,299,199 );
  int angle = 90;
  while( 1 )
  {
    APEX apex[5];
    for( int i = 0; i < (int)(sizeof(apex) / sizeof(apex[0])); i++ )
    {
      double R = 40.0;
      double yD = sin( (M_PI * (2 * (angle + (i * 72)))) / 360.0 );
      double xD = cos( (M_PI * (2 * (angle + (i * 72)))) / 360.0 );
      int xI = 50 + (int)((xD * R) + 0.5);
      int yI = 50 + (int)((yD * R) + 0.5);
      apex[i].x = xI; apex[i].y = yI;
      rot_rdq();
    }

    draw.fillRectangle( 0, 0, 100, 100, RGB565_BLACK );
    draw.polygon( apex, sizeof(apex) / sizeof(apex[0]), RGB565_CYAN );

    angle += (-10);
    angle %= 360;

    dly_tsk( 50UL );
  }
}

/* ----------------------------------------
    lcd demo 05
---------------------------------------- */
void lcdDemo05( void )
{
  TEXT_PRINT txt(14);
  txt.vaConfig( 0,200,199,271 );
  while( 1 )
  {
    for( int count = 0; count < (int)AliceInWonderlandSize; count++ )
    {
      txt.clear();
      if( count & 1 ) txt.color( RGB565_YELLO );
      else txt.color( RGB565_CYAN );
      txt.puts( AliceInWonderland[ count ] );
      dly_tsk( 2 * 1000UL );
    }
    dly_tsk( 10 * 1000UL );
  }
}


#if  defined( __KANJI_USE__ )
void lcdDemo06( void )
{
  TEXT_PRINT kanji;
  kanji.vaConfig( 205,205,479,271 );
  static const uint16_t rgb[] = {RGB565_RED,RGB565_CYAN,RGB565_YELLO,RGB565_LIME};
  int rgbIndex = 0;
  int wakaIndex = 0;
  while( 1 )
  {
    kanji.clear();
    kanji.color( rgb[ rgbIndex++ ] );
    rgbIndex %= 4;
    kanji.puts( sjisMiyabi[ wakaIndex++ ] );
    wakaIndex %= sjisMiyabiSize;
    dly_tsk( 5 * 1000UL );
  }
}
#endif  /* __KANJI_USE__ */

void lcdDemo07( void )
{
  BUTTON btn[8];
  static const char *label[] =
  {
	"BUTTON01",
	"BUTTON02",
	"BUTTON03",
	"BUTTON04",
	"BUTTON05",
	"BUTTON06",
	"BUTTON07",
	"BUTTON08",
  };
  /* initialize buttons. */
  for( int i = 0; i < 8; i++ )
  {
    btn[i].vaConfig( 400, 25 * i, 480 - 1, (25 * (i + 1)) - 1 );
    btn[i].buttonImage( sw8025_BMPTable, 0, 0, 80 - 1, 25 - 1 );
    btn[i].txtConfig( 5,5,80 - 5 - 1,25 - 5 - 1,14, RGB565_BLACK, RGB565_SILVER );  // RGB565_SILVER
    btn[i].print( label[i] );
  }

  int index = 0;
  while( 1 )
  {
    int pre = index - 1; if( pre < 0 ) pre = 7;
    btn[pre].buttonImage( sw8025_BMPTable );
    btn[pre].txtColor( RGB565_BLACK, RGB565_SILVER );
    btn[pre].print( label[pre] );
    btn[index].buttonImage( sw8025p_BMPTable );
    btn[index].txtColor( RGB565_RED, RGB565_SILVER );
    btn[index].print( label[index] );
    index++;
    index %= 8;
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
