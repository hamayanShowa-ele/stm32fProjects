/* ----------------------------------------
 board 1405 utilities.
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

#include  "1405.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */
#if defined( ARM_CBUS_LONG )
  static void cb1405_int6_CALL( int num );
  static void cb1405_CALL( int num );
#endif /* defined( ARM_CBUS_LONG ) */
#if defined( ISOLATE_AD_10CH )
  static void cb1415_CALL( int num );
#endif /* defined( ISOLATE_AD_10CH ) */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern volatile SYSTIM systim;
extern Serial Serial1;  /* hardware serial 1 */

#if defined( ARM_CBUS_LONG )
  static uint8_t V50INT_update[6];
  SYSTIM int6_preTime = 0UL;
  volatile uint16_t *dummy_io = (uint16_t *)CBUS_DUMMY_IO_ADR;
#endif /* defined( ARM_CBUS_LONG ) */
#if defined( ISOLATE_AD_10CH )
  static uint8_t convert_int_update;
#endif /* defined( ISOLATE_AD_10CH ) */


/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
---------------------------------------- */

#if defined( ISOLATE_AD_10CH )
/* ----------------------------------------
    gpio initialize
---------------------------------------- */
void BOARD_1405::gpioInit()
{
  swdEnable();
  allPulldownOnSWD();

  GPIOA->ODR = 0x0000;
  /*  PA0： ADC123_IN0 */
  /*  PA1： ADC123_IN1 */
  /*  PA2： ADC123_IN2 */
  /*  PA3： ADC123_IN3 */
  /*  PA4： ADC12_IN4 */
  /*  PA5： ADC12_IN5 */
  /*  PA6： ADC12_IN6 (2.5V reference) */
  /*  PA7：  not connection. */
  /*  PA8: not connection. */
  /*  PA9: USART1 TXD：Alternate function */
  /*  PA10: USART1 RXD：Alternate function */
  /*  PA11: ACT LED output */
  /*  PA12: not connection. */
  /*  PA13: SWDIO */
  /*  PA14: SWDCLK */
  /*  PA15： JTAG JTDI but not use. */

  GPIOB->ODR = 0x0000;
  /*  PB0： FIFO D0 output. */
  /*  PB1： FIFO D1 output. */
  /*  PB2: FIFO D2 output. (BOOT1) */
  /*  PB3: JTAG TDO but use for FIFO D3 output.(need remap).*/
  /*  PB4: JTAG TRST but use for FIFO D4 output.(need remap).*/
  /*  PB5: FIFO D5 output. */
  /*  PB6: FIFO D6 output. */
  /*  PB7: FIFO D7 output. */
  /*  PB8: not connection. */
  /*  PB9: not connection. */
  /*  PB10: not connection. */
  /*  PB11: not connection. */
  /*  PB12: not connection. */
  /*  PB13: not connection. */
  /*  PB14: not connection. */
  /*  PB15: not connection. */

  GPIOC->ODR = 0x0000;
  /*  PC0： ADC123_IN10 */
  /*  PC1： ADC123_IN11 */
  /*  PC2： ADC123_IN12 */
  /*  PC3： ADC123_IN13 */
  /*  PC4：  not connection. */
  /*  PC5：  not connection. */
  /*  PC6: not connection. */
  /*  PC7: not connection. */
  /*  PC8: not connection. */
  /*  PC9: not connection. */
  /*  PC10: INT to HOST(V50) output. */
  /*  PC11: CONVERT input. */
  /*  PC12: FIFOWR1 output. */
  /*  PC13: IDSW0 input. */
  /*  PC14: IDSW1 input. */
  /*  PC15: IDSW2 input. */

  GPIOD->ODR = 0x0000;
  /*  PD0：  oscillator input. */
  /*  PD1：  oscillator output. */
  /*  PD2: FIFOWR0 output. */
  /*  PD3：  not used. */
  /*  PD4：  not used. */
  /*  PD5：  not used. */
  /*  PD6：  not used. */
  /*  PD7：  not used. */
  /*  PD8： not used. */
  /*  PD9： not used. */
  /*  PD10： not used. */
  /*  PD11： not used. */
  /*  PD12： not used. */
  /*  PD13： not used. */
  /*  PD14： not used. */
  /*  PD15： not used. */

  pinMode( FIFO_D0, OUTPUT );
  pinMode( FIFO_D1, OUTPUT );
  pinMode( FIFO_D2, OUTPUT );
  pinMode( FIFO_D3, OUTPUT );
  pinMode( FIFO_D4, OUTPUT );
  pinMode( FIFO_D5, OUTPUT );
  pinMode( FIFO_D6, OUTPUT );
  pinMode( FIFO_D7, OUTPUT );

  set( FIFOWR0 );
  set( FIFOWR1 );
  pinMode( FIFOWR0, OUTPUT );
  pinMode( FIFOWR1, OUTPUT );

  set( INT2HOST );
  pinMode( INT2HOST, OUTPUT_OPEN_DRAIN );
}

/* ----------------------------------------
    fifo write
---------------------------------------- */
void BOARD_1405::fifoWrite( uint16_t data )
{
  volatile unsigned short pb;

  /* lower side. */
  FIFOWR0_IS_1;
  loc_cpu();
  pb = BUS_PORT->ODR;
  pb &= ~BUS_PIN;
  pb |= data & 0x00ff;
  BUS_PORT->ODR = pb;  /* lower side. */
  unl_cpu();
  FIFOWR0_IS_0;  // about 120ns
  FIFOWR0_IS_0;  // about 120ns
  FIFOWR0_IS_1;  // about 120ns
  FIFOWR0_IS_1;  // about 120ns

  /* upper side. */
  FIFOWR1_IS_1;
  loc_cpu();
  pb = BUS_PORT->ODR;
  pb &= ~BUS_PIN;
  pb |= (data >> 8) & 0x00ff;  /* upper side. */
  BUS_PORT->ODR = pb;
  unl_cpu();
  FIFOWR1_IS_0;  // about 120ns
  FIFOWR1_IS_0;  // about 120ns
  FIFOWR1_IS_1;  // about 120ns
  FIFOWR1_IS_1;  // about 120ns
}

void BOARD_1405::fifoWrite( uint16_t *data, size_t size )
{
  for( int i = 0; i < (int)size; i++ ) fifoWrite( *data++ );
}

/* ----------------------------------------
    request interrupt for the host.
---------------------------------------- */
void BOARD_1405::knock1415()
{
  INT2HOST_IS_0;
//  dly_tsk( 2UL );
  dly2R5us();
  INT2HOST_IS_1;
}

/* ----------------------------------------
    increment pattern data write to fifo.
---------------------------------------- */
void BOARD_1405::fifoIncrementWrite( LED *led )
{
  uint16_t count = 0;
  extiCallBack( PC11 % 16, cb1415_CALL );  // CONVERT:PC11
  extiConfig( PC11, EXTI_Trigger_Rising );  // CONVERT:PC11 EXTI_Trigger_Falling
  uint8_t convert_int_update_Base = convert_int_update;
  SYSTIM ledBlinkTim = systim;
  while( true )
  {
    for( int i = 0; i < ALVC7804_WORD_SIZE; i++ )
    {
      fifoWrite( count++ );
    }
    knock1415();

    /* wait for ack from 1415 */
    SYSTIM baseTim = systim;
    while( convert_int_update == convert_int_update_Base )
    {
      if( (systim - baseTim) >= 10UL )
      {
//        knock1415();
        baseTim = systim;
      }
      rot_rdq();
    }
    convert_int_update_Base = convert_int_update;

    if( (systim - ledBlinkTim) >= 500UL )
    {
      ledBlinkTim = systim;
      led->toggle();
    }
  }
}

/* ----------------------------------------
  1405 exti interrupt from 1415 call back routine.
---------------------------------------- */
static void cb1415_CALL( int num )
{
  if( (systim - int6_preTime) >= 3UL )
  {
    int6_preTime = systim;
    convert_int_update++;
  }
  else
  {
    while( 1 )
    {
      ACTLED_IS_1;
      dly1ms( 100UL );
      ACTLED_IS_0;
      dly1ms( 400UL );
    }
  }
}

#endif /* ISOLATE_AD_10CH */


#if defined( ARM_CBUS_LONG )
/* ----------------------------------------
    dummy read from fifo.
---------------------------------------- */
void BOARD_1405::fifoDummyRead( int size )
{
  for(int i = 0; i < size; i++ )
  {
    uint16_t tempUS = *ioAddress;
    (void)tempUS;
  }
}

/* ----------------------------------------
    increment pattern data read from fifo.
---------------------------------------- */
uint16_t BOARD_1405::fifoIncrementRead( uint16_t count, int size )  //
{
  uint16_t success = 0;
  for( int i = 0; i < size; i++ )
  {
    uint16_t tempUS = *ioAddress;
    if( count == tempUS ) success++;
//    tempUS = *dummy_io;  // dummy read. if you need.
    count++;
    rot_rdq();
  }
  dly_tsk( 2UL );
  knock1405();

  return success;
}

void BOARD_1405::fifoIncrementRead( int portNum, LED *led )
{
  uint16_t count = 0;
  int intNum = EXTI_TO_INT_NUMBER( portNum % 16 );
  uint8_t updateBase = V50INT_update[ intNum ];
  extiCallBack( portNum % 16, cb1405_int6_CALL );  // INT0:4 INT6:10
  extiConfig( portNum, EXTI_Trigger_Falling );  // INT0:PC4 INT6:PF10
  SYSTIM baseTim = systim;
  while( true )
  {
    while( V50INT_update[ intNum ] == updateBase ) rot_rdq();
    updateBase = V50INT_update[ intNum ];
    for( int i = 0; i < ALVC7804_WORD_SIZE; i++ )
    {
      uint16_t tempUS = *ioAddress;
      if( count != tempUS )
      {
        while( true )
        {
          dly_tsk( 50UL );
          led->toggle();
//          break;
        }
      }
      count++;
    }
    dly_tsk( 2UL );
    knock1405();
    if( (systim - baseTim) >= 500UL )
    {
      baseTim = systim;
      led->toggle();
    }
  }
}

/* ----------------------------------------
    request interrupt for the 1405.
---------------------------------------- */
void BOARD_1405::knock1405()
{
  *ioAddress = 1;
  dly_tsk( 2UL );
  *ioAddress = 0;
}

/* ----------------------------------------
  check 1405 task.
---------------------------------------- */
extern volatile uint32_t actLedPeriod;

void check1405( void )
{
  VP_INT exinf[4];
  get_par( exinf, sizeof(exinf) / sizeof(exinf[0]) );
  uint16_t *ioadr = (uint16_t *)exinf[0];
  int pin = (int)exinf[1];

  BOARD_1405 bd1405( ioadr );

  uint16_t count = 0;
  int intNum = EXTI_TO_INT_NUMBER( pin % 16 );
  uint8_t updateBase = V50INT_update[ intNum ];
  extiCallBack( pin % 16, cb1405_CALL );  // INT0:4 INT6:10
  extiConfig( pin, EXTI_Trigger_Falling );  // INT0:PC4 INT6:PF10
  SYSTIM baseTim = systim;
  while( 1 )
  {
    while( V50INT_update[ intNum ] == updateBase ) rot_rdq();
    updateBase = V50INT_update[ intNum ];
    uint16_t ret = bd1405.fifoIncrementRead( count, ALVC7804_WORD_SIZE );
    if( ret != ALVC7804_WORD_SIZE )
    {
      actLedPeriod = 50UL;
    }
    else count += ret;
    if( (systim - baseTim) >= 3 * 1000UL )
    {
      baseTim = systim;
      Serial1.printf( "  IO ADDRESS 0x%08X running.\r\n", ioadr );
    }
  }
}

/* ----------------------------------------
  1415 exti interrupt from 1405 call back routine.
---------------------------------------- */
static void cb1405_CALL( int num )
{
  V50INT_update[ EXTI_TO_INT_NUMBER(num) ]++;
}

static void cb1405_int6_CALL( int num )
{
  if( (systim - int6_preTime) >= 3UL )
  {
    int6_preTime = systim;
    V50INT_update[ EXTI_TO_INT_NUMBER(num) ]++;
  }
  else
  {
    while( 1 )
    {
      ACTLED_IS_1;
      dly1ms( 100UL );
      ACTLED_IS_0;
      dly1ms( 400UL );
    }
  }
}

#endif /* ARM_CBUS_LONG */


extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
