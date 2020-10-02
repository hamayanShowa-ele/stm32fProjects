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
#include  <time.h>
#include  <math.h>
#include  <HardwareSerial.h>
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
//#include  <pca8574.h>
#include  <softWire.h>
#include  <adcDac.h>
#include  <strutil.h>

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
//STM32F_I2C i2c1;
BOARD  board;  /* initialize gpio */
LT3593 backLight;
ARC_TEST arc;  /* board aries arc test instance. */
STM32F_SOFT_I2C softI2C;
LED actLed;
//STM32F_ADC adc1;

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
//void temperature( void );
void dacOutput( void );

uint8_t tsk1_stk[256 * 5];  // stack for task1
uint8_t tsk2_stk[256 * 4];  // stack for task2
uint8_t tsk3_stk[256 * 4];  // stack for task3

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
//  reg_tsk( ID_temperature,(void *)temperature, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_dacOutput,(void *)dacOutput, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );

  sta_tsk( ID_stackMonitor );
//  sta_tsk( ID_temperature );
  sta_tsk( ID_dacOutput );
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

  struct tm localTime =
    {0,21,21,24,5-1,2020-1900,0,0,0};
#if 1
  unixTime = mktime( &localTime );
#endif

  /* initialize GPIO and external CPU bus. */
  board.gpioInit();
  board.extBusInit();
  arc.gpioInit();
  actLed.begin( ACT_LED );
  actLed.On();

  /* initialize serial */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.puts( "ARIES 0900 Arc Detector Test.\r\n" );

  LT3593 lt3593( BRIGHT );
  while( 1 )
  {
    lt3593.brightness( BRIGHTEST );
    dly_tsk( 1000UL );
    lt3593.brightness( BRIGH_LV5 );
    dly_tsk( 1000UL );
    lt3593.brightness( BRIGH_LV10 );
    dly_tsk( 1000UL );
    lt3593.brightness( BRIGH_LV15 );
    dly_tsk( 1000UL );
    lt3593.brightness( BRIGH_LV20 );
    dly_tsk( 1000UL );
    lt3593.brightness( BRIGH_LV25 );
    dly_tsk( 1000UL );
    lt3593.brightness( BRIGH_LV30 );
    dly_tsk( 1000UL );
    lt3593.brightness( DARK );
    dly_tsk( 1000UL );
  }

  /* initialize i2c */
#if 0
  /* stm32f103のエラッタに拠ると、外部バスを有効にするとI2C1のPB6、PB7で不具合が起きる。 */
  /* 対策としてPB6、PB7ではなくPB8、PB9をREMAPして使うか、あきらめてソフトウエアI2Cとするか。 */
  if( i2c1.begin( I2C1, SDA1, SCL1 ) != I2C_SUCCESS )
  {
    while( 1 ) {}
  }
#else
  softI2C.begin( I2C1, SDA1, SCL1 );
#endif

  /* initialize LCD and graphic controller IC. */
#if 0
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
#endif

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
      char buffer[64];
      sprintf( buffer, "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      SH_SendString( (const char *)buffer );
      sprintf( buffer, "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      SH_SendString( (const char *)buffer );
      SH_SendString( "\r\n" );
#else /* __SIMIHOSTTING_IMPL */
      Serial1.printf( "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      Serial1.printf( "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      Serial1.printf( "  TASK3:%d/%d\r\n", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
      Serial1.puts( "\r\n" );
#endif /* __SIMIHOSTTING_IMPL */
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

/* ----------------------------------------
    temperature and humidity
---------------------------------------- */
#if 0
void temperature( void )
{
  SOFT_AM2320 am2320( &softI2C );
  dly_tsk( 1UL );

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
    am2320.update( &temperature, &humidity );

    char buffer[16];
    dtostrf( temperature, 4, 1, buffer );
    tempTxt.clearScreen();
    tempTxt.printf( TYPE_ASCII_48, "%s%c", buffer, 'c' );

    dtostrf( humidity, 3, 1, buffer );
    humiTxt.clearScreen();
    humiTxt.printf( TYPE_ASCII_48, "%s%c", buffer, '%' );

    dly_tsk( 2 * 1000UL );
  }
}
#endif

/* ----------------------------------------
    dac output
---------------------------------------- */
uint32_t dacData[36];

void dacOutput( void )
{
  /* generate sine data.  */
  for( int i = 0; i < 36; i++ )
  {
    float tempF = sin(2 * M_PI * 10.0 * i / 360.0);
    tempF *= 1024.0F;  /* scale 2048.0F */
    tempF += 2047.0F;  /* offset */
#if 0
    union
    {
      uint16_t us[2];
      uint32_t ul;
    } d;
    d.us[0] = (uint16_t)tempF;
    d.us[1] = (uint16_t)tempF;
    dacData[ i ] = d.ul;
#else
    dacData[ i ] = ((uint32_t)tempF << 16) | ((uint32_t)tempF << 0);
#endif
  }
#if 1
  STM32F_DAC dac1( DAC_Channel_1, DAC_Trigger_T7_TRGO );
  dac1.start();
  dac1.write( (const uint32_t *)dacData, sizeof(dacData) / sizeof(dacData[0]),
    50 * 36, TIM7 );
  while( 1 )
  {
    rot_rdq();
  }
#else
  /* DAC defines */
#define  DAC_TIMx             TIM7
#define  RCC_APBxPeriph_TIMx  RCC_APB1Periph_TIM7
#define  DAC_RCC_APBxPeriph_TIMx  RCC_APB1Periph_TIM7
#define  DAC_Trigger_Tx_TRGO  DAC_Trigger_T7_TRGO
//  #define  DAC_DHR12RD_Address      &(DAC->DHR12RD)  /* DHR12RD = 0x40007420 */
//  #define  DAC_TIMx                 TIM7
//  #define  DAC_Trigger_Tx_TRGO      DAC_Trigger_T7_TRGO
#define  DAC_DMAx_Channelx        DMA2_Channel3
#define  TIMx_CKCNT  (APB1_CLK / 1)
#define  DAC_DHR12RD_Address      &(DAC->DHR12RD)  /* DHR12RD = 0x40007420 */
#define  DMAx_Channelx        DAC_DMAx_Channelx
//  #define  DMAx_Channelx        DMA2_Channel4  /* not able */
#define  DMAx_Flag_TCx        DMA2_FLAG_TC3
#define  DMAx_Flag_TEx        DMA2_FLAG_TE3

  uint32_t tempUL = dacData[0];
  uint16_t elementSize = sizeof(dacData) / sizeof(dacData[0]);
  TIM_TypeDef *TIMx = DAC_TIMx;
//  uint16_t period = (uint16_t)((72000000UL / 3600 / 50) - 1);
  uint16_t period = (uint16_t)(72000000UL / 36 / 50);

  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE );
  /* TIMx Periph clock enable */
  RCC_APB1PeriphClockCmd( DAC_RCC_APBxPeriph_TIMx, ENABLE );

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* TIMx Configuration */
  /* Time base configuration */
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
  TIM_TimeBaseStructure.TIM_Period = period;  /* period */
  TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;  /**/
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  /**/
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit( TIMx, &TIM_TimeBaseStructure );

  /* TIMx TRGO selection */
  TIM_SelectOutputTrigger( TIMx, TIM_TRGOSource_Update );

  DAC_InitTypeDef DAC_InitStructure;

  /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_Tx_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init( DAC_Channel_1, &DAC_InitStructure );
  /* DAC channel2 Configuration */
  DAC_Init( DAC_Channel_2, &DAC_InitStructure );

  /* dma initialize */
  DMA_DeInit( DAC_DMAx_Channelx );
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC_DHR12RD_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)dacData;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = elementSize;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init( DAC_DMAx_Channelx, &DMA_InitStructure );
  DMA_ClearFlag( DMAx_Flag_TCx | DMAx_Flag_TEx );
  DMA_ITConfig( DAC_DMAx_Channelx, DMA_IT_TC | DMA_IT_TE, ENABLE );   /*転送/エラー割り込み許可*/
  /* Enable DMAx Channelx */
  DMA_Cmd( DMAx_Channelx, ENABLE );

  DAC->DHR12RD = tempUL;
  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
     automatically connected to the DAC converter. */
  DAC_Cmd( DAC_Channel_1, ENABLE );
  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
     automatically connected to the DAC converter. */
  DAC_Cmd( DAC_Channel_2, ENABLE );

  /* Enable DMA for DAC Channel2 */
  DAC_DMACmd( DAC_Channel_1, ENABLE );  /* DMA2_Channel3 */
  //DAC_DMACmd( DAC_Channel_2, ENABLE );  /* DMA2_Channel4 : not able */
  TIM_Cmd( TIMx, ENABLE );
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
