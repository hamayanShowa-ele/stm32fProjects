/* ----------------------------------------
  stm32f103vet 1303 pixis and 1426 s25n 7segment display examples code
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
#include  <adcDac.h>
#include  <1303.h>
#include  <1426.h>
#include  <2003.h>
#include  <2004.h>
#include  <9405.h>
#include  <boardUtility.h>
#include  <phaseCompare.h>


extern "C"
{
  #include  <semihosting.h>
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
BOARD_1303  bd1303;
BD1426 bd1426;

STM32F_TIMER tim3;

/* ----------------------------------------
    tasks
---------------------------------------- */
void stackMonitor( void );
void segmentLED( void );
void relays( void );
void digitalSwitches( void );
void panelSwitches( void );
void encoder( void );
void phaseTest( void );

uint8_t tsk1_stk[256 * 6];  // stack for task1
uint8_t tsk2_stk[256 * 2];  // stack for task2
uint8_t tsk3_stk[256 * 2];  // stack for task3
uint8_t tsk4_stk[256 * 2];  // stack for task4
uint8_t tsk5_stk[256 * 6];  // stack for task5
uint8_t tsk6_stk[256 * 4];  // stack for task6
uint8_t tsk7_stk[256 * 6];  // stack for task7

/* ----------------------------------------
    task initialize
---------------------------------------- */
void tsk_ini( void )
{
  reg_tsk( ID_stackMonitor,(void *)stackMonitor, tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_segmentLED,(void *)segmentLED, tsk2_stk, sizeof(tsk2_stk), 0,0,0,0 );
  reg_tsk( ID_relays,(void *)relays, tsk3_stk, sizeof(tsk3_stk), 0,0,0,0 );
  reg_tsk( ID_digitalSwitches,(void *)digitalSwitches, tsk4_stk, sizeof(tsk4_stk), 0,0,0,0 );
  reg_tsk( ID_panelSwitches,(void *)panelSwitches, tsk5_stk, sizeof(tsk5_stk), 0,0,0,0 );
  reg_tsk( ID_encoder,(void *)encoder, tsk6_stk, sizeof(tsk6_stk), 0,0,0,0 );
  reg_tsk( ID_phaseTest,(void *)phaseTest, tsk7_stk, sizeof(tsk7_stk), 0,0,0,0 );

//  sta_tsk( ID_stackMonitor );
//  sta_tsk( ID_segmentLED );
//  sta_tsk( ID_relays );
//  sta_tsk( ID_digitalSwitches );
  sta_tsk( ID_panelSwitches );
//  sta_tsk( ID_encoder );
//  sta_tsk( ID_phaseTest );
}


/* ----------------------------------------
    main
---------------------------------------- */
int main(void)
{
  RCC_Configuration();
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  NVIC_Configuration();

  /* Setup SysTick Timer for 1 milli-sec interrupts, also enables Systick and Systick-Interrupt */
  if( SysTick_Config(SystemCoreClock / 1000UL) )
  {
    /* Capture error */
    while (1);
  }

  if( RTC_Init() != 0 )
  {
    struct tm localTime =
      {0,3,16,26,8-1,2020-1900,0,0,0};
    unixTime = mktime( &localTime );
    unixTime_Set( unixTime );
  }

  /* initialize gpio */
  bd1303.gpioInit();
  bd1426.gpioInit();

  /* initialize serial */
  Serial1.begin( SCI_1, 115200UL );
  Serial1.printf( "    1303 PIXIS01\r\n" );
  Serial1.printf( "    designed by hamayan.\r\n" );
//  serialLoopBack( &Serial1 );

#if 0
  /* Provide FCLK to match the phase delay of the filter. */
  GPIO_PinRemapConfig( GPIO_FullRemap_TIM3, ENABLE );
  tim3.begin( TIM3 );
  tim3.frequency( 50 * 1000UL );
  tim3.pwm1( TIMx_CH1, PC6, tim3.getAutoReload() / 2 );
  tim3.start();
#endif

  /* initialize tasks and start dispatch. */
  tsk_ini();  //
//  sta_rdq( ID_stackMonitor );  // start round robin
//  sta_rdq( ID_phaseTest );
  sta_rdq( ID_panelSwitches );

  while(1) {}
}

/* ----------------------------------------
    task stack monitor.
---------------------------------------- */
static unsigned int RemainStack( void *stk, unsigned int sz );

void stackMonitor( void )
{
  LED act( PC4 );
  SYSTIM baseTim = systim;
  while( 1 )
  {
    /*stack report.*/
    if( (systim - baseTim) >= 10 * 1000UL )
    {
      baseTim = systim;
      Serial1.printf( "  TASK1:%d/%d\r\n", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      Serial1.printf( "  TASK2:%d/%d\r\n", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      Serial1.printf( "  TASK3:%d/%d\r\n", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
      Serial1.printf( "  TASK4:%d/%d\r\n", RemainStack( tsk4_stk, sizeof(tsk4_stk) ), sizeof(tsk4_stk) );
      Serial1.printf( "  TASK5:%d/%d\r\n", RemainStack( tsk5_stk, sizeof(tsk5_stk) ), sizeof(tsk5_stk) );
      Serial1.printf( "  TASK6:%d/%d\r\n", RemainStack( tsk6_stk, sizeof(tsk6_stk) ), sizeof(tsk6_stk) );
      Serial1.printf( "  TASK7:%d/%d\r\n", RemainStack( tsk7_stk, sizeof(tsk7_stk) ), sizeof(tsk7_stk) );

      char buffer[32];
      Serial1.printf( "    %s\r\n", localDateTimeString( buffer, unixTime ) );
      act.toggle();
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
    task drive 7segment LEDs.
---------------------------------------- */
void segmentLED( void )
{
  /* initialize GPIO and external CPU bus. */
  int loop = 0;
  float value = 0.0F;
  float delat = 0.01F;
  while( 1 )
  {
    dly_tsk( 5UL );  /* 30hz * 5segment = 150hz -> 6.7ms */
    bd1426.numerical( value );
    segmentHandler();
    if( (loop % 40) == 39 )
    {
      value += delat;
      if( value < 1.0F ) delat = 0.01F;
      else if( value < 2.0F ) delat = 0.001F;
      else if( value < 10.0F ) delat = 0.01F;
      else if( value < 20.0F ) delat = 0.01F;
      else if( value < 100.0F ) delat = 0.1F;
      else if( value < 200.0F ) delat = 0.1F;
      else if( value < 1000.0F ) delat = 1.0F;
      else if( value < 2000.0F ) delat = 1.0F;
      else
      {
        value = 0.0F;
        delat = 0.01F;
      }
    }
    bd1426.status( (uint8_t)((loop / 200) % 5), ((loop % 200) < 100) );
    loop++;

    rot_rdq();
  }
}

/* ----------------------------------------
    task relay output.
---------------------------------------- */
void relays( void )
{
  dly_tsk( 1000UL );
  LED lagLead( LAG_LEAD );
  LED alarm( ALARM );
  LED ready( READY );
  LED relay25X( RELAY_25X );
  LED phase80( PHASE_80 );
  LED phase40( PHASE_40 );
  LED phase20( PHASE_20 );
  LED phase10( PHASE_10 );
  LED phase8( PHASE_08 );
  LED phase4( PHASE_04 );
  LED phase2( PHASE_02 );
  LED phase1( PHASE_01 );
  while( 1 )
  {
    lagLead.On();
    dly_tsk( 1 * 1000UL );
    alarm.On();
    dly_tsk( 1 * 1000UL );
    ready.On();
    dly_tsk( 1 * 1000UL );
    relay25X.On();
    dly_tsk( 1 * 1000UL );
    phase80.On();
    dly_tsk( 1 * 1000UL );
    phase40.On();
    dly_tsk( 1 * 1000UL );
    phase20.On();
    dly_tsk( 1 * 1000UL );
    phase10.On();
    dly_tsk( 1 * 1000UL );
    phase8.On();
    dly_tsk( 1 * 1000UL );
    phase4.On();
    dly_tsk( 1 * 1000UL );
    phase2.On();
    dly_tsk( 1 * 1000UL );
    phase1.On();
    dly_tsk( 1 * 1000UL );

    dly_tsk( 10 * 1000UL );

    lagLead.Off();
    dly_tsk( 1 * 1000UL );
    alarm.Off();
    dly_tsk( 1 * 1000UL );
    ready.Off();
    dly_tsk( 1 * 1000UL );
    relay25X.Off();
    dly_tsk( 1 * 1000UL );
    phase80.Off();
    dly_tsk( 1 * 1000UL );
    phase40.Off();
    dly_tsk( 1 * 1000UL );
    phase20.Off();
    dly_tsk( 1 * 1000UL );
    phase10.Off();
    dly_tsk( 1 * 1000UL );
    phase8.Off();
    dly_tsk( 1 * 1000UL );
    phase4.Off();
    dly_tsk( 1 * 1000UL );
    phase2.Off();
    dly_tsk( 1 * 1000UL );
    phase1.Off();
    dly_tsk( 1 * 1000UL );

    dly_tsk( 10 * 1000UL );
  }
}

/* ----------------------------------------
    task read digital switches.
---------------------------------------- */
void digitalSwitches( void )
{
  BD9405 digiSw;
  uint8_t sw1,sw2,sw3;
  while( 1 )
  {
    sw1 = digiSw.switchRead( TIMER_SWITCH );
    sw2 = digiSw.switchRead( VOLTAGE_SWITCH );
    sw3 = digiSw.switchRead( PHASE_SWITCH );
    dly_tsk( 1 * 1000UL );
  }
}

/* ----------------------------------------
    task read panel switches.
---------------------------------------- */
void panelSwitches( void )
{
  #define  MODE_PIN        ADC2_IN3_PIN  // PA3
  #define  ZERO_PIN        ADC2_IN2_PIN  // PA2
  #define  ONE_EIGHTY_PIN  ADC2_IN1_PIN  // PA1
  #define  VARIABLE_PIN    ADC2_IN0_PIN  // PA0
  STM32F_ADC panel( ADC2 );

  while( 1 )
  {
    uint16_t mode = panel.analogRead( MODE_PIN );
    uint16_t zero = panel.analogRead( ZERO_PIN );
    uint16_t oneEighty = panel.analogRead( ONE_EIGHTY_PIN );
    uint16_t variable = panel.analogRead( VARIABLE_PIN );
    Serial1.printf( "MODE:%d ZERO:%d 180:%d VARI:%d\r\n", mode,zero,oneEighty,variable );
    dly_tsk( 1 * 1000UL );
  }
}

/* ----------------------------------------
    task read rotary encoder.
---------------------------------------- */
void encoder( void )
{
//  GPIO_PinRemapConfig( GPIO_FullRemap_TIM3, ENABLE );
  STM32F_TIMER tim8( TIM8 );
  tim8.encoderEnable();

  uint16_t counter;
  while( 1 )
  {
    counter = tim8.encoderRead();
    char buffer[32];
    sprintf( buffer, "  COUNT = %u\r\n", counter );
//    SH_SendString( (const char *)buffer );
    Serial1.printf( buffer );

    dly_tsk( 1 * 100UL );
  }
}

/* ----------------------------------------
    task phase comparator.
---------------------------------------- */
volatile uint16_t VABuffer[400];
volatile uint16_t VBBuffer[400];
extern volatile uint8_t dma1ch1TC_Update;  /* full transmission complete */
extern volatile uint8_t dma2ch5TC_Update;  /* full transmission complete */

void phaseTest( void )
{
  PHASE_COMPARE phase( 20 );

#if 1
  STM32F_ADC adc1( ADC1 );
  STM32F_ADC adc3( ADC3 );
  ADC_PIN_AND_DATA adc1PinCfg[1] = { ADC1_IN10_PIN, ADC_SampleTime_239Cycles5, 0 };  // ADC_SampleTime_7Cycles5 or ADC_SampleTime_239Cycles5
  ADC_PIN_AND_DATA adc3PinCfg[1] = { ADC3_IN11_PIN, ADC_SampleTime_239Cycles5, 0 };  // ADC_SampleTime_7Cycles5 or ADC_SampleTime_239Cycles5

  adc1.analogRead(
    adc1PinCfg, sizeof(adc1PinCfg) / sizeof(adc1PinCfg[0]),
    VABuffer, sizeof(VABuffer) / sizeof(VABuffer[0]),
    ADC_ExternalTrigConv_T1_CC1 );  /* adc trigger is timer1 channel 1. */
  adc3.analogRead(
    adc3PinCfg, sizeof(adc3PinCfg) / sizeof(adc3PinCfg[0]),
    VBBuffer, sizeof(VBBuffer) / sizeof(VBBuffer[0]),
    ADC_ExternalTrigConv_T1_CC3 );  /* adc trigger is timer1 channel 3. */
#if 0  /* PWM output to the port to compare the phase state. */
  GPIO gpio;
  GPIO_PinRemapConfig( GPIO_FullRemap_TIM1, ENABLE );
  gpio.pinMode( PE9, ALTERNATE_PP, GPIO_SPEED_FAST );  // tim1 ch1
  gpio.pinMode( PE13, ALTERNATE_PP, GPIO_SPEED_FAST );  // tim1 ch3
#endif

  STM32F_TIMER adcTrigger( TIM1 );
  adcTrigger.frequency( 50 * 20 );  /* 50Hz 20th over sampling. */
//  #define  PHASE_OFFSET  1400  // MAX7405 (0.7degree)
//  #define  PHASE_OFFSET  2800  // MAX7404 (1.4degree)
#define  PHASE_OFFSET  0  // MAX7404 (0.0degree for FCLK input at 2005. )
  adcTrigger.adcTrigger( TIMx_CH1, ADC1_IN10_PIN, 18000 );
  adcTrigger.adcTrigger( TIMx_CH3, ADC3_IN11_PIN, 18000 - PHASE_OFFSET );

  uint8_t dma2ch5TC_Update_Base = dma2ch5TC_Update;  /* full transmission complete counter. */
  adcTrigger.start();

  while( 1 )
  {
    while( dma2ch5TC_Update == dma2ch5TC_Update_Base ) rot_rdq();
    dma2ch5TC_Update_Base = dma2ch5TC_Update;  /* full transmission complete counter. */

    int diff = phase.compare(
      (const uint16_t *)VABuffer, (const uint16_t *)VBBuffer,
      sizeof(VABuffer) / sizeof(VBBuffer[0]) );
    char buffer[64],little[16];
    dtostrf( (double)diff / 10.0, 7, 2, little );
    sprintf( buffer, "phase difference = %s\r\n", little );
//    SH_SendString( (const char *)buffer );
    Serial1.printf( buffer );

    rot_rdq();
  }

#else
  double pa,pb;
  pa = 0.0;
  for( pb = 0.0; pb < 360.0; pb += 9.0 )
  {
    for( int i = 0; i < 400; i++ )
    {
      adc1Buffer[i] = (uint16_t)((int)(sin( ((i % 20) * 2 * M_PI / 20.0) + ((pa / 360.0) * 2 * M_PI) ) * 1500.0) + 2048 - 10);
      adc3Buffer[i] = (uint16_t)((int)(sin( ((i % 20) * 2 * M_PI / 20.0) + ((pb / 360.0) * 2 * M_PI) ) * 1500.0) + 2048);
    }
    int diff = phase.compare(
      (const uint16_t *)adc1Buffer, (const uint16_t *)adc3Buffer,
      sizeof(adc1Buffer) / sizeof(adc1Buffer[0]) );
    char buffer[64];
    sprintf( buffer, "pa = %f pb = %f diff = %f\r\n", pa, pb, diff / 10.0 );
//    SH_SendString( (const char *)buffer );
    Serial1.printf( buffer );

    dly_tsk( 10UL );
  }
#endif
  phase.end();

  while( 1 ) rot_rdq();
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
