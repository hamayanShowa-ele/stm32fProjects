/* ----------------------------------------
 board 1804 utilities.
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

#include  "1804.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern volatile uint8_t intdpUpdate;
volatile static uint16_t dummy;

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
---------------------------------------- */

/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void BOARD_1804::gpioInit()
{
  /* initialize gpio */
  swdEnable();
  allPulldownOnSWD();

/*  PA0: MPX1 : output. select for pulse input */
/*  PA1: MPX0 : output. select for pulse input*/
/*  PA2: PULSE8 : input. pulse input */
/*  PA3: PULSE7 : input. pulse input */
/*  PA4: not used */
/*  PA5: not used */
/*  PA6: PULSE6 : input. pulse input */
/*  PA7: PULSE5 : input. pulse input */
/*  PA8: MCO1 output by 168 / 5 = 33.6Mhz */
/*  PA9: USART1 TXD:Alternate function*/
//  pinMode( PA9, ALTERNATE_PP );
//  pinAFConfig( PA9, GPIO_AF_USART1 );
/*  PA10: USART1 RXD:Alternate function*/
//  pinMode( PA10, ALTERNATE_PP );
//  pinAFConfig( PA10, GPIO_AF_USART1 );
/*  PA11: not used */
/*  PA12: not used */
/*  PA13:JTAG TMS:SWD */
/*  PA14:JTAG TCK:SWD */
/*  PA15: not used */

/*  PB0: PULSE2 : input. pulse input */
/*  PB1: PULSE1 : input. pulse input */
/*  PB2: BOOT0 : GND */
/*  PB3: not used */
/*  PB4: not used */
/*  PB5: not used */
/*  PB6: SCL1 : bidirectional. I2C1 */
/*  PB7: SDA1 : bidirectional. I2C1 */
/*  PB8: not used */
/*  PB9: not used */
/*  PB10: USART3 TXD:Alternate function */
/*  PB11: USART3 RXD:Alternate function */
/*  PB12: not used */
/*  PB13: not used */
/*  PB14: not used */
/*  PB15: not used */

/*  PC0: not used */
/*  PC1: not used */
/*  PC2: not used */
/*  PC3: not used */
/*  PC4: PULSE4 : input. pulse input */
/*  PC5: PULSE3 : input. pulse input */
/*  PC6: not used */
/*  PC7: not used */
/*  PC8: CONVERT : output. analog sampling clock. */
/*  PC9: not used */
/*  PC10: UART4 TXD */
//  pinMode( PC10, ALTERNATE_PP );
//  pinAFConfig( PC10, GPIO_AF_UART4 );
/*  PC11: UART4 RXD */
//  pinMode( PC11, ALTERNATE_PP );
//  pinAFConfig( PC11, GPIO_AF_UART4 );
/*  PC12: UART5 TXD */
//  pinMode( PC12, ALTERNATE_PP );
//  pinAFConfig( PC12, GPIO_AF_UART5 );
/*  PC13: LCD R/W : output. sc1602 lcd control line. */
/*  PC14: LCD RS : output. sc1602 lcd control line. */
/*  PC15: not used */

/*  PD0: D2 */
/*  PD1: D3 */
/*  PD2: UART5 RXD */
//  pinMode( PD2, ALTERNATE_PP );
//  pinAFConfig( PD2, GPIO_AF_UART5 );
/*  PD3: not used */
/*  PD4: RD */
/*  PD5: WR */
/*  PD6: WAIT : input */
/*  PD7: DPRAM CS */
/*  PD8: D13 */
/*  PD9: D14 */
/*  PD10: D15 */
/*  PD11: V50 INT : output. INT request for V50. */
  pinMode( PD11, OUTPUT );
  digitalWrite( PD11, LOW );  /* active high, connect to a npn transistor. */
/*  PD12: Master / Slave : input. Master is HIGH, Slave is LOW. */
/*  PD13: Filter clock : output. frequency is cutoff frequency * 100 */
/*  PD14: D0 */
/*  PD15: D1 */

/*  PE0: BLE */
/*  PE1: BHE */
/*  PE2: LCD D4 : output. sc1602 lcd data line. */
/*  PE3: LCD D5 : output. sc1602 lcd data line. */
/*  PE4: LCD D6 : output. sc1602 lcd data line. */
/*  PE5: LCD D7 : output. sc1602 lcd data line. */
/*  PE6: LCD E : output. sc1602 lcd control line. */
/*  PE7: D4 */
/*  PE8: D5 */
/*  PE9: D6 */
/*  PE10: D7 */
/*  PE11: D8 */
/*  PE12: D9 */
/*  PE13: D10 */
/*  PE14: D11 */
/*  PE15: D12 */

/*  PF0: A0 */
/*  PF1: A1 */
/*  PF2: A2 */
/*  PF3: A3 */
/*  PF4: A4 */
/*  PF5: A5 */
/*  PF6: GPIO for universal purpose. */
/*  PF7: GPIO for universal purpose. */
/*  PF8: GPIO for universal purpose. */
/*  PF9: GPIO for universal purpose. */
/*  PF10: GPIO for universal purpose. */
/*  PF11: GPIO for universal purpose. */
/*  PF12: A6 */
/*  PF13: A7 */
/*  PF14: A8 */
/*  PF15: A9 */

/*  PG0: A10 */
/*  PG1: A11 */
/*  PG2: SEMR : output. LOW is dpram semaphore and HIGH is normal dpram operation. */
  pinMode( PG2, OUTPUT );
  digitalWrite( PG2, HIGH );
/*  PG3: INTDP : input. interrupt from dpram.  */
  pinMode( PG3, INPUT_PULLUP );
/*  PG4: EOLC2 : input. interrupt from adc2 end of conversion. */
/*  PG5: EOLC1 : input. interrupt from adc1 end of conversion. */
/*  PG6: not used */
/*  PG7: GPIO0 : bidirectional. for another analog board. */
/*  PG8: GPIO1 : bidirectional. for another analog board. */
/*  PG9: AD1_CS. adc1 chip select. */
/*  PG10: AD2_CS. adc1 chip select. */
/*  PG11: ACT LED : output. blink led */
/*  PG12: not used */
/*  PG13: not used */
/*  PG14: not used */
/*  PG15: not used */
}


/* ----------------------------------------
    configure cpu bus port.
---------------------------------------- */
void BOARD_1804::busPort()
{
  pinMode( PF0, ALTERNATE_PP );  //a0
  pinAFConfig( PF0, GPIO_AF_FSMC );

  pinMode( PF1, ALTERNATE_PP );  //a1
  pinAFConfig( PF1, GPIO_AF_FSMC );

  pinMode( PF2, ALTERNATE_PP );  //a2
  pinAFConfig( PF2, GPIO_AF_FSMC );

  pinMode( PF3, ALTERNATE_PP );  //a3
  pinAFConfig( PF3, GPIO_AF_FSMC );

  pinMode( PF4, ALTERNATE_PP );  //a4
  pinAFConfig( PF4, GPIO_AF_FSMC );

  pinMode( PF5, ALTERNATE_PP );  //a5
  pinAFConfig( PF5, GPIO_AF_FSMC );

  pinMode( PF12, ALTERNATE_PP );  //a6
  pinAFConfig( PF12, GPIO_AF_FSMC );

  pinMode( PF13, ALTERNATE_PP );  //a7
  pinAFConfig( PF13, GPIO_AF_FSMC );

  pinMode( PF14, ALTERNATE_PP );  //a8
  pinAFConfig( PF14, GPIO_AF_FSMC );

  pinMode( PF15, ALTERNATE_PP );  //a9
  pinAFConfig( PF15, GPIO_AF_FSMC );

  pinMode( PG0, ALTERNATE_PP );  //a10
  pinAFConfig( PG0, GPIO_AF_FSMC );

  pinMode( PG1, ALTERNATE_PP );  //a11
  pinAFConfig( PG1, GPIO_AF_FSMC );

  pinMode( PD14, ALTERNATE_PP );  //d0
  pinAFConfig( PD14, GPIO_AF_FSMC );

  pinMode( PD15, ALTERNATE_PP );  //d1
  pinAFConfig( PD15, GPIO_AF_FSMC );

  pinMode( PD0, ALTERNATE_PP );  //d2
  pinAFConfig( PD0, GPIO_AF_FSMC );

  pinMode( PD1, ALTERNATE_PP );  //d3
  pinAFConfig( PD1, GPIO_AF_FSMC );

  pinMode( PE7, ALTERNATE_PP );  //d4
  pinAFConfig( PE7, GPIO_AF_FSMC );

  pinMode( PE8, ALTERNATE_PP );  //d5
  pinAFConfig( PE8, GPIO_AF_FSMC );

  pinMode( PE9, ALTERNATE_PP );  //d6
  pinAFConfig( PE9, GPIO_AF_FSMC );

  pinMode( PE10, ALTERNATE_PP );  //d7
  pinAFConfig( PE10, GPIO_AF_FSMC );

  pinMode( PE11, ALTERNATE_PP );  //d8
  pinAFConfig( PE11, GPIO_AF_FSMC );

  pinMode( PE12, ALTERNATE_PP );  //d9
  pinAFConfig( PE12, GPIO_AF_FSMC );

  pinMode( PE13, ALTERNATE_PP );  //d10
  pinAFConfig( PE13, GPIO_AF_FSMC );

  pinMode( PE14, ALTERNATE_PP );  //d11
  pinAFConfig( PE14, GPIO_AF_FSMC );

  pinMode( PE15, ALTERNATE_PP );  //d12
  pinAFConfig( PE15, GPIO_AF_FSMC );

  pinMode( PD8, ALTERNATE_PP );  //d13
  pinAFConfig( PD8, GPIO_AF_FSMC );

  pinMode( PD9, ALTERNATE_PP );  //d14
  pinAFConfig( PD9, GPIO_AF_FSMC );

  pinMode( PD10, ALTERNATE_PP );  //d15
  pinAFConfig( PD10, GPIO_AF_FSMC );

  pinMode( PD4, ALTERNATE_PP );  //rd
  pinAFConfig( PD4, GPIO_AF_FSMC );

  pinMode( PD5, ALTERNATE_PP );  //we
  pinAFConfig( PD5, GPIO_AF_FSMC );

  pinMode( PD7, ALTERNATE_PP );  //dpram cs
  pinAFConfig( PD7, GPIO_AF_FSMC );

  pinMode( PG9, ALTERNATE_PP );  //adc1 cs
  pinAFConfig( PG9, GPIO_AF_FSMC );

  pinMode( PG10, ALTERNATE_PP );  //adc2 cs
  pinAFConfig( PG10, GPIO_AF_FSMC );

  pinMode( PE0, ALTERNATE_PP );  //ble
  pinAFConfig( PE0, GPIO_AF_FSMC );

  pinMode( PE1, ALTERNATE_PP );  //bhe
  pinAFConfig( PE1, GPIO_AF_FSMC );

//  pinMode( PD6, ALTERNATE_PP );  //wait
  pinMode( PD6, INPUT_PULLUP );  //wait
  pinAFConfig( PD6, GPIO_AF_FSMC );
}

/* ----------------------------------------
  Enable processor external bus.
---------------------------------------- */
void BOARD_1804::extBus()
{
  busPort();

  FSMC *fsmc = new FSMC;
  /* dpram (idt70v24s55) area,16bit */
  /* read cycle time = min 55ns and access time = max 55ns. */
  /* write cycle time = min 55ns and address setup time = min 15ns and data hold time = min 40ns. */
  fsmc->sram( FSMC_Bank1_NORSRAM1,FSMC_MemoryDataWidth_16b, 1,11, 3,9 );
  /* adc1 area,16bit */
  /* read cycle time = min 60ns and data access time = max 30ns. */
  /* write cycle time = min 60ns and data setup time = min 10ns and data hold time = min 20ns. */
  fsmc->sram( FSMC_Bank1_NORSRAM2,FSMC_MemoryDataWidth_16b, 4,8, 4,8 );
  /* adc2 area,16bit */
  /* read cycle time = min 60ns and data access time = max 30ns. */
  /* write cycle time = min 60ns and data setup time = min 10ns and data hold time = min 20ns. */
  fsmc->sram( FSMC_Bank1_NORSRAM3,FSMC_MemoryDataWidth_16b, 4,8, 4,8 );
  /* dummy access. */
  fsmc->sram( FSMC_Bank1_NORSRAM4,FSMC_MemoryDataWidth_16b, 1,1, 1,1 );
  delete fsmc;
}


/* ----------------------------------------
  V50 INT.
---------------------------------------- */
void BOARD_1804::v50INT()
{
  digitalWrite( PD11, HIGH );  /* active high, connect to a npn transistor. */
  dly2R5us();
  digitalWrite( PD11, LOW );  /* active high, connect to a npn transistor. */
}

/* ----------------------------------------
  dpram check.
---------------------------------------- */
/* ----------------------------------------
  dpram divid write.
---------------------------------------- */
void BOARD_1804::dpRamDividWrite( uint16_t *ram, uint16_t data )
{
  *ram = (data >> 8) | 0xFF00;
  ram++;
  *ram = data | 0xFF00;
}

/* ----------------------------------------
  dpram divid read.
---------------------------------------- */
uint16_t BOARD_1804::dpRamDividRead( const uint16_t *ram )
{
  uint16_t h,l;
  h = *ram;
  ram++;
  l = *ram;
  l &= 0x00FF;

  return (h << 8 | l);
}

/* ----------------------------------------
  dpram random write.
---------------------------------------- */
void BOARD_1804::dpRamRandomWrite( void *ram, size_t size, uint32_t seed, LED *led )
{
  led->Off();
  srand( seed );
  while( true )
  {
    volatile uint16_t *ptr = (volatile uint16_t *)ram;
#if 1
    int sz = size / (sizeof(uint16_t) * 2);
    for( int i = 0; i < sz; )
    {
      uint16_t rnd = (uint16_t)rand();
__retry001 :
      rot_rdq();
      dpRamDividWrite( (uint16_t *)ptr, rnd );
      uint16_t rcv = dpRamDividRead( (const uint16_t *)ptr );
      if( rnd != rcv )
      {
        led->toggle();
        dly_tsk( 50UL );
        goto __retry001;
      }
      ptr += 2;
      i++;
    }
#else
    int sz = size / sizeof(uint16_t);
    for( int i = 0; i < sz; )
    {
      uint16_t rnd = (uint16_t)rand();
__retry002 :
      rot_rdq();
      *ptr = rnd;
      uint16_t ret = *ptr;
      if( rnd != ret )
      {
        led->toggle();
        dly_tsk( 50UL );
        goto __retry002;
      }
      ptr++;
      i++;
    }
#endif
    dummy = *((volatile uint16_t *)GANYMEDE_DUMMY_ADDRESS);
    uint8_t intdpUpdateBase = intdpUpdate;
    v50INT();
    while( intdpUpdate == intdpUpdateBase ) rot_rdq();
//    led->toggle();
  }
}

/* ----------------------------------------
  dpram fixed write.
---------------------------------------- */
void BOARD_1804::dpRamFixedWrite( void *ram, size_t size, uint16_t fixed, LED *led )
{
  led->On();
  size /= sizeof(uint16_t);
  while( true )
  {
    volatile uint16_t *ptr = (volatile uint16_t *)ram;
    for( int i = 0; i < (int)size; i++ )
    {
      uint16_t dat;
      if( i & 1 ) dat = fixed ^ 0xFFFF;
      else dat = fixed;
      *ptr++ = dat & 0x00FF;
    }
    volatile uint8_t intdpUpdateBase = intdpUpdate;
    v50INT();
    while( intdpUpdate == intdpUpdateBase ) rot_rdq();
    led->toggle();
  }
}

/* ----------------------------------------
  dpram increment write.
---------------------------------------- */
void BOARD_1804::dpRamIncrementWrite( void *ram, size_t size, LED *led )
{
  led->Off();
  uint16_t data = 0;

  while( true )
  {
    volatile uint16_t *ptr = (volatile uint16_t *)ram;
#if 0
    int sz = size / (sizeof(uint16_t) * 2);
    for( int i = 0; i < sz; )
    {
      uint16_t dat;
      if( i & 1 ) dat = data ^ 0xFFFF;
      else dat = data;

      dpRamDividWrite( (uint16_t *)ptr, dat );
      uint16_t ret = dpRamDividRead( (const uint16_t *)ptr );
      if( ret != dat )
      {
        led->toggle();
        dly_tsk( 50UL );
        continue;
      }
      ptr += 2;
#else
    int sz = size / (sizeof(uint16_t) * 1);
    for( int i = 0; i < sz; )
    {
      uint16_t dat;
      if( i & 1 ) dat = data ^ 0xFFFF;
      else dat = data;

      *ptr = dat;
      uint16_t ret = *ptr;
      if( ret != dat )
      {
        led->toggle();
        dly_tsk( 50UL );
        continue;
      }
      ptr++;
#endif

      data++;
      i++;
    }
    dummy = *((volatile uint16_t *)GANYMEDE_DUMMY_ADDRESS);

    uint8_t intdpUpdateBase = intdpUpdate;
    v50INT();
    while( intdpUpdate == intdpUpdateBase ) rot_rdq();
//    led->toggle();
  }
}

/* ----------------------------------------
  dpram read and check.
---------------------------------------- */
void BOARD_1804::dpRamRead( const void *ram, size_t size, uint32_t seed, LED *led )
{
  uint16_t loop = 0;
  uint32_t baseTim = millis();
  size /= sizeof(uint16_t);
  srand( seed );
  while( true )
  {
    const volatile uint16_t *ptr = (const volatile uint16_t *)ram;
    for( int i = 0; i < (int)size; i++ )
    {
      uint16_t rnd = (uint16_t)rand() & 0x00FF;
      uint16_t dat = *ptr++ & 0x00FF;
      if( dat != rnd )
      {
        while( true )
        {
          dly_tsk( 50UL );
          led->toggle();
        }
      }
    }
    volatile uint16_t *intr = (volatile uint16_t *)DPRAM_INTR_ADDRESS;
    *intr = loop++;
    if( (millis() - baseTim) >= 500UL )
    {
      baseTim = millis();
      led->toggle();
    }
  }
}


/* ----------------------------------------
  fclk.
    perhaps filter clock.
    TIM4 ch2 PD13.
---------------------------------------- */
void BOARD_1804::fclk( uint32_t freq )
{
  /* gpio alternate for TIM4 ch2 output. */
  pinMode( PD13, ALTERNATE_PP );
  pinAFConfig( PD13, GPIO_AF_TIM4 );

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );

  STM32F_TIMER *Timer = new STM32F_TIMER( TIM4 );
  Timer->frequency( freq );
  uint16_t arr = (uint16_t)Timer->getAutoReload();
  Timer->pwm1( TIMx_CH2, PD13, arr / 2UL );
  Timer->start();
  delete Timer;
}


/* ----------------------------------------
  convert.
    perhaps adc convert clock.
    TIM3 ch3 PC8.
---------------------------------------- */
void BOARD_1804::convert( uint32_t freq )
{
  /* gpio alternate for TIM3 ch3 output. */
  pinMode( PC8, ALTERNATE_PP );
  pinAFConfig( PC8, GPIO_AF_TIM3 );

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

  STM32F_TIMER *Timer = new STM32F_TIMER( TIM3 );
  Timer->frequency( freq );
  uint16_t arr = (uint16_t)Timer->getAutoReload();
  Timer->pwm1( TIMx_CH3, PC8, arr - (arr / 10UL) );
  Timer->start();
  delete Timer;
}

/* ----------------------------------------
  adc initialize.
---------------------------------------- */
void ADC_1804::begin( volatile uint16_t *a, uint16_t mask )
{
  adcAddress = a;
  *adcAddress = mask;
}

/* ----------------------------------------
  adc read.
---------------------------------------- */
uint16_t ADC_1804::read()
{
  return *adcAddress & 0x3FFF;
}


extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
