/* ----------------------------------------
 board 1401 utilities.
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

#ifndef  __BOARD_1401_H__
#define  __BOARD_1401_H__

//#include  <HardwareSerial.h>
#include  <pca8574.h>
#include  <strutil.h>
#include  <Timer.h>

extern "C"
{
  #include  <system.h>
  #include  <mul_tsk.h>
}


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    defines
---------------------------------------- */
/* arduino terminals. */
#define  ARDUINO_A0  PA0  // uart4_tx,adc123_in0
#define  ARDUINO_A1  PC3  // spi2_mosi,adc123_in13
#define  ARDUINO_A2  PC2  // spi2_miso,adc123_in12
#define  ARDUINO_A3  PC1  // adc123_in11
#define  ARDUINO_A4  PB9  // SDA1,spi2_nss,tim4_ch4,tim11_ch1,can1_tx
#define  ARDUINO_A5  PB8  // SCL1,tim4_ch3,tim10_ch1,can1_rx

#define  ARDUINO_D0       //
#define  ARDUINO_D1       //
#define  ARDUINO_D2  PC5  // adc12_in15
#define  ARDUINO_D3  PB7  // tim4_ch2,i2c1_sda,usart1_rx,
#define  ARDUINO_D4  PC4  // adc12_in14
#define  ARDUINO_D5  PA6  // tim3_ch1,tim13_ch1,spi1_miso,adc12_in6
#define  ARDUINO_D6  PA7  // tim3_ch2,tim14_ch1,spi1_mosi,adc12_in7
#define  ARDUINO_D7  PB6  // tim4_ch1,i2c1_scl,usart1_tx,can2_tx,
#define  ARDUINO_D8  PD6  // usart2_rx,
#define  ARDUINO_D9  PA4  // dac_out1,spi1_nss,adc12_in4,spi3_nss,
#define  ARDUINO_D10  PA5  // dac_out2,spi1_sck,adc12_in5,
#define  ARDUINO_D11  PB5  // tim3_ch2,spi1_mosi,spi3_mosi,can2_rx,
#define  ARDUINO_D12  PB4  // tim3_ch1,spi1_miso,spi3_miso,
#define  ARDUINO_D13  PG14  // usart6_tx,

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class BOARD_1401 : public GPIO
{
private:
  void busPort();

public:
  BOARD_1401(){}
  ~BOARD_1401(){}

  void gpioInit();
  void extBus();
  void ioReset();
  uint8_t boardSwitchRead( PCA8574 *pca );
  void usbOn( bool onOff );
  void fclk( STM32F_TIMER *Timer, uint32_t freq );
  void convert( STM32F_TIMER *Timer, uint32_t freq );
};

#endif  /* __BOARD_1401_H__ */

