/* ----------------------------------------
  gpio initialize code
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
#include  "gpioInit.h"

/* ----------------------------------------
    defines
---------------------------------------- */

/* ----------------------------------------
    prototypes
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
GPIO_INIT::GPIO_INIT()
{
  begin();
}

GPIO_INIT::~GPIO_INIT()
{
}

/* ----------------------------------------
    begin initialize
---------------------------------------- */
void GPIO_INIT::begin()
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
  pinMode( PA9, ALTERNATE_PP );
  pinAFConfig( PA9, GPIO_AF_USART1 );
/*  PA10: USART1 RXD:Alternate function*/
  pinMode( PA10, ALTERNATE_PP );
  pinAFConfig( PA10, GPIO_AF_USART1 );
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
  pinMode( PC10, ALTERNATE_PP );
  pinAFConfig( PC10, GPIO_AF_UART4 );
/*  PC11: UART4 RXD */
  pinMode( PC11, ALTERNATE_PP );
  pinAFConfig( PC11, GPIO_AF_UART4 );
/*  PC12: UART5 TXD */
  pinMode( PC12, ALTERNATE_PP );
  pinAFConfig( PC12, GPIO_AF_UART5 );
/*  PC13: LCD R/W : output. sc1602 lcd control line. */
/*  PC14: LCD RS : output. sc1602 lcd control line. */
/*  PC15: not used */

/*  PD0: D2 */
/*  PD1: D3 */
/*  PD2: UART5 RXD */
  pinMode( PD2, ALTERNATE_PP );
  pinAFConfig( PD2, GPIO_AF_UART5 );
/*  PD3: not used */
/*  PD4: RD */
/*  PD5: WR */
/*  PD6: WAIT : input */
/*  PD7: DPRAM CS */
/*  PD8: D13 */
/*  PD9: D14 */
/*  PD10: D15 */
/*  PD11: V50 INT : output. INT request for V50. */
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
/*  PG3: INTDP : input. interrupt from dpram.  */
/*  PG4: EOLC2 : input. interrupt from adc2 end of conversion. */
/*  PG5: EOLC1 : input. interrupt from adc1 end of conversion. */
/*  PG6: not used */
/*  PG7: GPIO0 : bidirectional. for another analog board. */
/*  PG8: GPIO1 : bidirectional. for another analog board. */
/*  PG9: AD1_CS. adc1 chip select. */
/*  PG10: AD2_CS. adc1 chip select. */
/*  PG11: ACT LED : output. blink led */
  pinMode( PG11, OUTPUT );
/*  PG12: not used */
/*  PG13: not used */
/*  PG14: not used */
/*  PG15: not used */
}

/* ----------------------------------------
    configure cpu bus port.
---------------------------------------- */
void GPIO_INIT::busPort()
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
}

/* ----------------------------------------
    プロセッサ外部バスを有効にする
---------------------------------------- */
void GPIO_INIT::extBus()
{
  busPort();

  /*dpram bank1のsram領域を有効化*/
  volatile unsigned long *bcr1 = (volatile unsigned long *)FSMC_Bank1_R_BASE;
  volatile unsigned long *btr1 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + 4);
  volatile unsigned long *bwtr1 = (volatile unsigned long *)(FSMC_Bank1E_R_BASE);

  /*NE1 SRAM設定 0x60000000*/
  *bcr1 = 0x000030db;
  *bcr1 &= ~(FSMC_BCR1_ASYNCWAIT | FSMC_BCR1_WAITEN | FSMC_BCR1_MTYP | FSMC_BCR1_MUXEN | FSMC_BCR1_EXTMOD | FSMC_BCR1_MWID | FSMC_BCR1_MBKEN);
  *bcr1 |= (FSMC_BCR1_EXTMOD | FSMC_BCR1_MWID_0 | FSMC_BCR1_MBKEN);

  /*読み込み側設定*/
  /*idt70v24s55のreadのサイクルタイムは55ns*/
  *btr1 = 0x0fffffff;
  *btr1 &= ~(FSMC_BTR1_ACCMOD | FSMC_BTR1_DATAST | FSMC_BTR1_ADDHLD | FSMC_BTR1_ADDSET | FSMC_BTR1_BUSTURN );
  /*アドレス有効からデータ有効まで35ns*/
  /*cs有効からデータ有効まで35ns*/
  /*bhe,ble有効からデータ有効まで15ns*/
  *btr1 |= FSMC_BTR1_ADDSET_2 | FSMC_BTR1_ADDSET_1;  //  chip select pulse width = 90ns
  /*rd有効からデータ有効まで15ns*/
  *btr1 |= FSMC_BTR1_DATAST_3 | FSMC_BTR1_DATAST_0;  // read pulse width = 54ns
  /*cs無効からデータ無効まで15ns*/
  /*rd無効からデータ無効まで10ns*/
  /*bhe,ble無効からデータ無効まで10ns*/
  /*時間調整*/
  *btr1 |= FSMC_BTR1_BUSTURN_1;  //18ns

  /*書き込み側設定*/
  /*idt70v24s55のwriteのサイクルタイムはmin:55ns -> 112ns */
  *bwtr1 = 0x0fffffff;
  *bwtr1 &= ~(FSMC_BWTR1_ACCMOD | FSMC_BWTR1_DATAST | FSMC_BWTR1_ADDSET | FSMC_BTR1_BUSTURN );
  /*アドレスセットアップ0ns*/
  /*cs幅95ns*/
  *bwtr1 |= FSMC_BWTR1_ADDSET_2 | FSMC_BWTR1_ADDSET_1;
  /*wr幅 min:40ns -> 55ns*/
  /*データセットアップ min:30ns*/
  *bwtr1 |= FSMC_BWTR1_DATAST_3 | FSMC_BWTR1_DATAST_0;
  /*時間調整*/
  *bwtr1 |= FSMC_BTR1_BUSTURN_1;  //12ns -> 18ns


  /*adc1 bank1のsram領域を有効化*/
  volatile unsigned long *bcr2 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + (8 * 1));
  volatile unsigned long *btr2 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + 4 + (8 * 1));
  volatile unsigned long *bwtr2 = (volatile unsigned long *)(FSMC_Bank1E_R_BASE + (8 * 1));

  /*NE2 SRAM設定 0x64000000*/
  *bcr2 = 0x000030db;
  *bcr2 &= ~(FSMC_BCR2_ASYNCWAIT | FSMC_BCR2_WAITEN | FSMC_BCR2_MTYP | FSMC_BCR2_MUXEN | FSMC_BCR2_EXTMOD | FSMC_BCR2_MWID | FSMC_BCR2_MBKEN);
  *bcr2 |= (FSMC_BCR2_EXTMOD | FSMC_BCR2_MWID_0 | FSMC_BCR2_MBKEN);

  /*読み込み側設定*/
  /*max1316ecmのreadのサイクルタイムはmin:60ns -> 100ns??*/
  *btr2 = 0x0fffffff;
  *btr2 &= ~(FSMC_BTR2_ACCMOD | FSMC_BTR2_DATAST | FSMC_BTR2_ADDHLD | FSMC_BTR2_ADDSET | FSMC_BTR2_BUSTURN );
  /*csの幅はmin:30ns -> 77ns*/
  /*rdの幅はmin:30ns -> 72ns*/
  /*rdからのデータセットアップはmax:30ns*/
  /*rdからのデータホールドはmax:30ns*/
  *btr2 |= FSMC_BTR2_DATAST_3 | FSMC_BTR2_DATAST_2; //72ns
  /*アドレスセットアップは事実上規定無し*/
  *btr2 |= FSMC_BTR2_ADDSET_0;
  /*時間調整*/
  *btr2 |= FSMC_BTR2_BUSTURN_2;  //??ns

  /*書き込み側設定*/
  /*max1316ecmのwriteのサイクルタイムは??ns*/
  *bwtr2 = 0x0fffffff;
  *bwtr2 &= ~(FSMC_BWTR2_ACCMOD | FSMC_BWTR2_DATAST | FSMC_BWTR2_ADDSET | FSMC_BTR2_BUSTURN );
  /*csの幅は min:30ns -> 71ns*/
  /*wrの幅は min:30ns -> 60ns*/
  /*wrからのデータセットアップは min:10ns*/
  /*wrからのデータホールドは min:10ns*/
  *bwtr2 |= FSMC_BWTR2_DATAST_3 | FSMC_BWTR2_DATAST_1;  //54ns
  /*アドレスセットアップは事実上規定無し*/
  *bwtr2 |= FSMC_BWTR2_ADDSET_0;
  /*時間調整*/
  *bwtr2 |= FSMC_BTR2_BUSTURN_2;  //??ns


  /*adc2 bank2のsram領域を有効化*/
  volatile unsigned long *bcr3 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + (8 * 2));
  volatile unsigned long *btr3 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + 4 + (8 * 2));
  volatile unsigned long *bwtr3 = (volatile unsigned long *)(FSMC_Bank1E_R_BASE + (8 * 2));

  /*NE3 SRAM設定 0x68000000*/
  *bcr3 = 0x000030db;
  *bcr3 &= ~(FSMC_BCR3_ASYNCWAIT | FSMC_BCR3_WAITEN | FSMC_BCR3_MTYP | FSMC_BCR3_MUXEN | FSMC_BCR3_EXTMOD | FSMC_BCR3_MWID | FSMC_BCR3_MBKEN);
  *bcr3 |= (FSMC_BCR3_EXTMOD | FSMC_BCR3_MWID_0 | FSMC_BCR3_MBKEN);

  /*読み込み側設定*/
  /*max1316ecmのreadのサイクルタイムはmin:60ns -> 137ns??*/
  *btr3 = 0x0fffffff;
  *btr3 &= ~(FSMC_BTR3_ACCMOD | FSMC_BTR3_DATAST | FSMC_BTR3_ADDHLD | FSMC_BTR3_ADDSET | FSMC_BTR3_BUSTURN );
  /*csの幅はmin:30ns -> 77ns*/
  /*rdの幅はmin:30ns -> 72ns*/
  /*rdからのデータセットアップはmax:30ns*/
  /*rdからのデータホールドはmax:30ns*/
  *btr3 |= FSMC_BTR3_DATAST_3 | FSMC_BTR3_DATAST_2; //72ns
  /*アドレスセットアップは事実上規定無し*/
  *btr3 |= FSMC_BTR3_ADDSET_0;
  /*時間調整*/
  *btr3 |= FSMC_BTR3_BUSTURN_2;  //??ns

  /*書き込み側設定*/
  /*max1316ecmのwriteのサイクルタイムは100ns*/
  *bwtr3 = 0x0fffffff;
  *bwtr3 &= ~(FSMC_BWTR3_ACCMOD | FSMC_BWTR3_DATAST | FSMC_BWTR3_ADDSET | FSMC_BTR3_BUSTURN );
  /*csの幅は min:30ns -> 71ns*/
  /*wrの幅は min:30ns -> 60ns*/
  /*wrからのデータセットアップは min:10ns*/
  /*wrからのデータホールドは min:10ns*/
  *bwtr3 |= FSMC_BWTR3_DATAST_3 | FSMC_BWTR3_DATAST_1;  //54ns
  /*アドレスセットアップは事実上規定無し*/
  *bwtr3 |= FSMC_BWTR3_ADDSET_0;
  /*時間調整*/
  *bwtr3 |= FSMC_BTR3_BUSTURN_2;  //??ns
}



/* ----------------------------------------
    end of file
    designed by hamayan since 2009/05/21
---------------------------------------- */

