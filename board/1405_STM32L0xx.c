/* ----------------------------------------
  NVIC utilities
  for STMicroelectronics HAL library

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
#include  "1405_STM32L0xx.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern volatile SYSTIM systim;
static uint8_t convert_int_update;
static SYSTIM exti11_preTime = 0UL;

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    gpioInit()
---------------------------------------- */
void gpioInit( void )
{
  // GPIO PA
  pinMode( PA0, ANALOG );  // PA0: analog input 0
  pinMode( PA1, ANALOG );  // PA1: analog input 1
  pinMode( PA2, ANALOG );  // PA2: analog input 2
  pinMode( PA3, ANALOG );  // PA3: analog input 3
  pinMode( PA4, ANALOG );  // PA4: analog input 4
  pinMode( PA5, ANALOG );  // PA5: analog input 5
  pinMode( PA6, ANALOG );  // PA6: analog input 6
  // PA7: connect to gnd.
  // PA8: connect to gnd.
  pinMode( PA9, OUTPUT );  // PA9: usart1 tx.
  pinMode( PA10, INPUT );  // PA10: usart1 rx.
  pinMode( PA11, OUTPUT );  // PA11: act led.
  // PA12: not connected.
  // PA13: swdio.
  // PA14: swclk.
  // PA15: not connected.

  // GPIO PB
  pinMode( PB0, OUTPUT );  // PB0: fifo data bus 0.
  pinMode( PB1, OUTPUT );  // PB1: fifo data bus 1.
  pinMode( PB2, OUTPUT );  // PB2: fifo data bus 2.
  pinMode( PB3, OUTPUT );  // PB3: fifo data bus 3.
  pinMode( PB4, OUTPUT );  // PB4: fifo data bus 4.
  pinMode( PB5, OUTPUT );  // PB5: fifo data bus 5.
  pinMode( PB6, OUTPUT );  // PB6: fifo data bus 6.
  pinMode( PB7, OUTPUT );  // PB6: fifo data bus 7.
  // PB8: connect to gnd.
  // PB9: connect to gnd.
  // PB10: scl2 but not used.
  // PB11: sda2 but not used.
  // PB12: connect to gnd.
  // PB13: connect to gnd.
  // PB14: connect to gnd.
  // PB15: connect to gnd.

  // GPIO PC
  pinMode( PC0, ANALOG );  // PC0: analog input 10
  pinMode( PC1, ANALOG );  // PC1: analog input 11
  pinMode( PC2, ANALOG );  // PC2: analog input 12
  pinMode( PC3, ANALOG );  // PC3: analog input 13
  // PC4: connect to gnd.
  // PC5: connect to gnd.
  // PC6: connect to gnd.
  // PC7: connect to gnd.
  // PC8: connect to gnd.
  // PC9: connect to gnd.
  pinMode( PC10, OUTPUT );  // PC10: int2host.
  pinMode( PC11, INPUT );  // PC11: convert timing.
  pinMode( PC12, OUTPUT );  // PC12: fifo write 1.
  pinMode( PC13, INPUT );  // PC13: idsw0 input
  pinMode( PC14, INPUT );  // PC14: idsw1 input
  pinMode( PC15, INPUT );  // PC15: idsw2 input

  // GPIO PD
  pinMode( PD2, OUTPUT );  // PD2: fifo write 0.

  // GPIO PH
  // PH0: connect to oscillator input.
  // PH1: connect to oscillator output.
}

/* ----------------------------------------
    fifo write
---------------------------------------- */
void fifoWrite( uint16_t data )
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
  FIFOWR0_IS_0;  // about ???ns
  FIFOWR0_IS_1;  // about ???ns

  /* upper side. */
  FIFOWR1_IS_1;
  loc_cpu();
  pb = BUS_PORT->ODR;
  pb &= ~BUS_PIN;
  pb |= (data >> 8) & 0x00ff;  /* upper side. */
  BUS_PORT->ODR = pb;
  unl_cpu();
  FIFOWR1_IS_0;  // about ???ns
  FIFOWR1_IS_1;  // about ???ns
}

/* ----------------------------------------
    request interrupt for the host.
---------------------------------------- */
void knock1415( void )
{
  INT2HOST_IS_0;
  dly2R5us();
  INT2HOST_IS_1;
}


/* ----------------------------------------
    increment pattern data write to fifo.
---------------------------------------- */
void fifoIncrementWrite( void )
{
  uint16_t count = 0;
//  extiCallBack( 11, cb1415_CALL );  // CONVERT:PC11
//  extiConfig( PC11, EXTI_Trigger_Rising );  // CONVERT:PC11 EXTI_Trigger_Falling

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
      //led->toggle();
      pinToggle( ACTLED );
    }
  }
}

/* ----------------------------------------
  1405 exti interrupt from 1415 call back routine.
---------------------------------------- */
void cb1415_CALL( int num )
{
  if( (systim - exti11_preTime) >= 3UL )
  {
    exti11_preTime = systim;
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
