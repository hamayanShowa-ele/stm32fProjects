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

/*  PA0: AR_A0,Arduino A0 input or output */
/*  PA1: 1PPS,GPS 1PPS input*/
  pinMode( PA1, INPUT_PULLDOWN );
/*  PA2: USART2 TX,alternate function */
  pinMode( PA2, ALTERNATE_PP );
  pinAFConfig( PA2, GPIO_AF_USART2 );
/*  PA3: USART2 RX,alternate function */
  pinMode( PA3, ALTERNATE_PP );
  pinAFConfig( PA3, GPIO_AF_USART2 );
/*  PA4: AR_D9,Arduino D9 input or output */
/*  PA5: AR_D10,Arduino D10 input or output */
/*  PA6: AR_D5,Arduino D5 input or output */
/*  PA7: AR_D6,Arduino D6 input or output */
/*  PA8: none or MCO1 output by 168 / 5 = 33.6Mhz */
/*  PA9: USART1 TXD:Alternate function*/
  pinMode( PA9, ALTERNATE_PP );
  pinAFConfig( PA9, GPIO_AF_USART1 );
/*  PA10: USART1 RXD:Alternate function*/
  pinMode( PA10, ALTERNATE_PP );
  pinAFConfig( PA10, GPIO_AF_USART1 );
/*  PA11: USBDM : Alternate function */
/*  PA12: USBDP : Alternate function  */
/*  PA13: JTAG TMS:SWDIO */
/*  PA14: JTAG TCK:SWDCLK */
/*  PA15: GCLK, GPS Global clock input */

/*  PB0: FCLK, filter clock output for 1824 ad board. */
/*  PB1: not used */
/*  PB2: BOOT1 : GND */
  pinMode( PB2, INPUT_PULLDOWN );
/*  PB3: CONVERT, convert pulse for 1824 ad board. */
/*  PB4: AR_D12,Arduino D12 input or output */
/*  PB5: AR_D11,Arduino D11 input or output */
/*  PB6: AR_D7,Arduino D7 input or output */
/*  PB7: AR_D3,Arduino D3 input or output */
/*  PB8: SCL1 */
  pinMode( PB8, ALTERNATE_PP );
  pinAFConfig( PB8, GPIO_AF_I2C1 );
/*  PB9: SDA1 */
  pinMode( PB9, ALTERNATE_PP );
  pinAFConfig( PB9, GPIO_AF_I2C1 );
/*  PB10: not used */
/*  PB11: not used */
/*  PB12: not used */
/*  PB13: not used */
/*  PB14: not used */
/*  PB15: not used */

/*  PC0: USBON, active low output */
/*  PC1: AR_A3,Arduino A3 input or output */
/*  PC2: AR_A2,Arduino A2 input or output */
/*  PC3: AR_A1,Arduino A1 input or output */
/*  PC4: AR_D4,Arduino D4 input or output */
/*  PC5: AR_D2,Arduino D2 input or output */
/*  PC6: not used */
/*  PC7: not used */
/*  PC8: not used */
/*  PC9: EXT_INT1, external interrupt input 1. */
/*  PC10: UART4 TXD, alternate function. */
  pinMode( PC10, ALTERNATE_PP );
  pinAFConfig( PC10, GPIO_AF_UART4 );
/*  PC11: UART4 RXD */
  pinMode( PC11, ALTERNATE_PP );
  pinAFConfig( PC11, GPIO_AF_UART4 );
/*  PC12: UART4 M/S, output for rs485 driver control. */
  pinMode( PC12, OUTPUT );
/*  PC13: TAMPER, input. */
/*  PC14: 32768Hz oscillator input. */
/*  PC15: 32768Hz oscillator output. */

/*  PD0: D2 */
/*  PD1: D3 */
/*  PD2: TRIGGER2, external trigger input 2. */
/*  PD3: TRIGGER1, external trigger input 1. */
/*  PD4: RD */
/*  PD5: WR */
/*  PD6: AR_D8,Arduino D8 input or output */
/*  PD7: MRAM CS */
/*  PD8: D13 */
/*  PD9: D14 */
/*  PD10: D15 */
/*  PD11: A16 */
/*  PD12: A17 */
/*  PD13: A18 */
/*  PD14: D0 */
/*  PD15: D1 */

/*  PE0: BLE */
/*  PE1: BHE */
/*  PE2: not used */
/*  PE3: not used */
/*  PE4: not used */
/*  PE5: not used */
/*  PE6: not used */
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
/*  PF6: not used */
/*  PF7: not used */
/*  PF8: LED RED, output */
/*  PF9: LED BLUE, output */
/*  PF10: LED GREEN, output */
/*  PF11: not used */
/*  PF12: A6 */
/*  PF13: A7 */
/*  PF14: A8 */
/*  PF15: A9 */

/*  PG0: A10 */
/*  PG1: A11 */
/*  PG2: A12 */
/*  PG3: A13 */
/*  PG4: A14 */
/*  PG5: A15 */
/*  PG6: ETH_INT, ethernet interrupt input. */
/*  PG7: ARC_INT, arcnet interrupt input. */
/*  PG8: EXT_INT0, external interrupt input 0. */
/*  PG9: ETH_CS. ethernet chip select. */
/*  PG10: ARC_CS. arcnet chip select. */
/*  PG11: ACT LED : output. blink led */
  pinMode( PG11, OUTPUT );
/*  PG12: EXT_CS. external bus chip select. */
/*  PG13: IO_RST, external bus reset output. */
  pinMode( PG13, OUTPUT );
  digitalWrite( PG13, LOW );
/*  PG14: AR_D13,Arduino D13 input or output */
/*  PG15: CONVERT_MS, converter master/slave input. high = master low = slave. */
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
  /*  PG2: A12 */
  pinMode( PG2, ALTERNATE_PP );  //a12
  pinAFConfig( PG2, GPIO_AF_FSMC );
  /*  PG3: A13 */
  pinMode( PG3, ALTERNATE_PP );  //a13
  pinAFConfig( PG3, GPIO_AF_FSMC );
  /*  PG4: A14 */
  pinMode( PG4, ALTERNATE_PP );  //a14
  pinAFConfig( PG4, GPIO_AF_FSMC );
  /*  PG5: A15 */
  pinMode( PG5, ALTERNATE_PP );  //a15
  pinAFConfig( PG5, GPIO_AF_FSMC );
  /*  PD11: A16 */
  pinMode( PD11, ALTERNATE_PP );  //a16
  pinAFConfig( PD11, GPIO_AF_FSMC );
  /*  PD12: A17 */
  pinMode( PD12, ALTERNATE_PP );  //a17
  pinAFConfig( PD12, GPIO_AF_FSMC );
  /*  PD13: A18 */
  pinMode( PD13, ALTERNATE_PP );  //a18
  pinAFConfig( PD13, GPIO_AF_FSMC );

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

  pinMode( PD7, ALTERNATE_PP );  //mram cs
  pinAFConfig( PD7, GPIO_AF_FSMC );

  pinMode( PG9, ALTERNATE_PP );  //eth cs
  pinAFConfig( PG9, GPIO_AF_FSMC );

  pinMode( PG10, ALTERNATE_PP );  //arc cs
  pinAFConfig( PG10, GPIO_AF_FSMC );

  pinMode( PG12, ALTERNATE_PP );  //external bus cs
  pinAFConfig( PG12, GPIO_AF_FSMC );

  pinMode( PE0, ALTERNATE_PP );  //ble
  pinAFConfig( PE0, GPIO_AF_FSMC );

  pinMode( PE1, ALTERNATE_PP );  //bhe
  pinAFConfig( PE1, GPIO_AF_FSMC );
}

/* ----------------------------------------
  Enable processor external bus.
---------------------------------------- */
void GPIO_INIT::extBus()
{
  busPort();

#if 0
  /*-- FSMC Configuration ------------------------------------------------------*/
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  rTiming,wTiming;
  /* This will cause a Hard Fault Error. Is this a bug? */
//  FSMC_NORSRAMStructInit( &FSMC_NORSRAMInitStructure );

  /* Enable sram area in sram bank1 */
  /* Configuring the NE1 SRAM 0x60000000 */
  /* is62wv51216ebll Read Cycle Time is min 45ns or min 55ns. */
  /* is62wv51216ebll Address Access Time is max 45ns or max 55ns. */
  /* mr2a16ays35 Read Cycle Time is min 35ns. */
  /* mr2a16ays35 Address Access Time is max 35ns. */
  rTiming.FSMC_AddressSetupTime = 4;  // Define the number of HCLK cycles to configure the setup time of the address.
  rTiming.FSMC_AddressHoldTime = 1;  // Define the number of HCLK cycles to configure the address's hold time.
  rTiming.FSMC_DataSetupTime = 7;  // Define the number of HCLK cycles to configure the data setup time.
  rTiming.FSMC_BusTurnAroundDuration = 0;  // This parameter is only used for multiplexed NOR Flash memory.
  rTiming.FSMC_CLKDivision = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_DataLatency = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  /* is62wv51216ebll Write Cycle Time is min 45ns or min 55ns. */
  /* is62wv51216ebll Address Setup Time to Write End is min 35ns or min 40ns. */
  /* is62wv51216ebll Data Setup to Write End is min 28ns or min 28ns. */
  /* is62wv51216ebll Data Hold from Write End is min 0ns or min 0ns. */
  /* is62wv51216ebll Write pulse width is min 35ns or min 40ns. */
  /* mr2a16ays35 Write Cycle Time is min 35ns. */
  /* mr2a16ays35 Address Setup Time to Write End is min 18ns. */
  /* mr2a16ays35 Data Setup to Write End is min 10ns. */
  /* mr2a16ays35 Data Hold from Write End is min 0ns. */
  /* mr2a16ays35 Write pulse width is min 15ns. */
  wTiming.FSMC_AddressSetupTime = 2;  // Define the number of HCLK cycles to configure the setup time of the address.
  wTiming.FSMC_AddressHoldTime = 1;  // Define the number of HCLK cycles to configure the address's hold time.
  wTiming.FSMC_DataSetupTime = 7;  // Define the number of HCLK cycles to configure the data setup time.
  wTiming.FSMC_BusTurnAroundDuration = 0;  // 1 This parameter is only used for multiplexed NOR Flash memory.
  wTiming.FSMC_CLKDivision = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_DataLatency = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  //
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  //
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &rTiming;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &wTiming;
  FSMC_NORSRAMInit( &FSMC_NORSRAMInitStructure );
  /*!< Enable FSMC Bank1_SRAM1 Bank */
  FSMC_NORSRAMCmd( FSMC_Bank1_NORSRAM1, ENABLE );


  /* Enable w5300 area in sram bank1 */
  /* Configuring the NE2 SRAM 0x64000000 */
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
  /* w5300 Read Cycle Time is min 93ns. */
  /* w5300 CS Low Time is min 65ns. */
  /* w5300 RD Low Time is min 65ns. */
  /* w5300 Data setup time from RD is 42ns. */
  /* w5300 The data hold time from RD is 7ns. */
  rTiming.FSMC_AddressSetupTime = 4;  // Define the number of HCLK cycles to configure the setup time of the address.
  rTiming.FSMC_AddressHoldTime = 1;  // Define the number of HCLK cycles to configure the address's hold time.
  rTiming.FSMC_DataSetupTime = 11;  // Define the number of HCLK cycles to configure the data setup time.
  rTiming.FSMC_BusTurnAroundDuration = 0;  // This parameter is only used for multiplexed NOR Flash memory.
  rTiming.FSMC_CLKDivision = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_DataLatency = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  /* w5300 Write Cycle Time is min 78ns. */
  /* w5300 CS Low Time is min 50ns. */
  /* w5300 WR Low Time is min 50ns. */
  /* w5300 Data setup time from WR is 14ns. */
  /* w5300 The data hold time from WR is 7ns. */
  wTiming.FSMC_AddressSetupTime = 3;  // Define the number of HCLK cycles to configure the setup time of the address.
  wTiming.FSMC_AddressHoldTime = 1;  // Define the number of HCLK cycles to configure the address's hold time.
  wTiming.FSMC_DataSetupTime = 9;  // Define the number of HCLK cycles to configure the data setup time.
  wTiming.FSMC_BusTurnAroundDuration = 0;  // 1 This parameter is only used for multiplexed NOR Flash memory.
  wTiming.FSMC_CLKDivision = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_DataLatency = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &rTiming;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &wTiming;
  FSMC_NORSRAMInit( &FSMC_NORSRAMInitStructure );
  /*!< Enable FSMC Bank1_SRAM2 Bank */
  FSMC_NORSRAMCmd( FSMC_Bank1_NORSRAM2, ENABLE );


  /* Enable arcnet area in sram bank1 */
  /* Configuring the NE3 SRAM 0x68000000 */
  /* The cycle time of arcnet read is 100ns. */
  /* The width of rd is 60ns. */
  /* Address setup time is 15ns. */
  /* The cycle time of arcnet write is 100ns. */
  /* The width of wr is 20ns. */
  /* Address setup time is 15ns. */


  /* Enable ext io area in sram bank1 */
  /* Configuring the NE4 SRAM 0x6C000000 */
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
  /* The cycle time of max1316ecm read is min:60ns -> 100ns? */
  /* The width of cs is min:30ns -> 77ns. */
  /* The width of rd is min:30ns -> 72ns. */
  /* The data setup time from rd is max:30ns. */
  /* The data hold time from rd is max:30ns. */
  /* Address setup is virtually unspecified. */
  rTiming.FSMC_AddressSetupTime = 3;  // Define the number of HCLK cycles to configure the setup time of the address.
  rTiming.FSMC_AddressHoldTime = 1;  // Define the number of HCLK cycles to configure the address's hold time.
  rTiming.FSMC_DataSetupTime = 13;  // Define the number of HCLK cycles to configure the data setup time.
  rTiming.FSMC_BusTurnAroundDuration = 0;  // This parameter is only used for multiplexed NOR Flash memory.
  rTiming.FSMC_CLKDivision = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_DataLatency = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  rTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  /* What is the cycle time of max1316ecm write? ns */
  /* The width of cs is min:30ns -> 71ns. */
  /* The width of wr is min:30ns -> 60ns. */
  /* The data setup time from wr is min:10ns. */
  /* The data hold time from wr is min:10ns. */
  /* Address setup is virtually unspecified. */
  wTiming.FSMC_AddressSetupTime = 0;  // Define the number of HCLK cycles to configure the setup time of the address.
  wTiming.FSMC_AddressHoldTime = 1;  // Define the number of HCLK cycles to configure the address's hold time.
  wTiming.FSMC_DataSetupTime = 12;  // Define the number of HCLK cycles to configure the data setup time.
  wTiming.FSMC_BusTurnAroundDuration = 0;  // 1 This parameter is only used for multiplexed NOR Flash memory.
  wTiming.FSMC_CLKDivision = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_DataLatency = 0;  // This parameter is not used for asynchronous NOR Flash, SRAM or ROM access.
  wTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &rTiming;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &wTiming;
  FSMC_NORSRAMInit( &FSMC_NORSRAMInitStructure );
  /*!< Enable FSMC Bank1_SRAM1 Bank */
  FSMC_NORSRAMCmd( FSMC_Bank1_NORSRAM4, ENABLE );
#else
  /* Enable sram area in sram bank1 */
  volatile unsigned long *bcr1 = (volatile unsigned long *)FSMC_Bank1_R_BASE;
  volatile unsigned long *btr1 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + 4);
  volatile unsigned long *bwtr1 = (volatile unsigned long *)(FSMC_Bank1E_R_BASE);

  /* Configuring the NE1 SRAM 0x60000000 */
  *bcr1 = 0x000030db;
  *bcr1 &= ~(FSMC_BCR1_ASYNCWAIT | FSMC_BCR1_WAITEN | FSMC_BCR1_MTYP | FSMC_BCR1_MUXEN | FSMC_BCR1_EXTMOD | FSMC_BCR1_MWID | FSMC_BCR1_MBKEN);
  *bcr1 |= (FSMC_BCR1_EXTMOD | FSMC_BCR1_MWID_0 | FSMC_BCR1_MBKEN);

  /* Reader Settings */
  *btr1 = 0x0fffffff;
  /* The cycle time of mr2a16ays35 read is 35ns. */
  /* The cycle time of is62wv51216ebll-45tli's read is 45ns. */
  /* Cycle time for cy62157ev30ll-55zsxe's read is 55ns. */
  *btr1 &= ~(FSMC_BTR1_ACCMOD | FSMC_BTR1_DATAST | FSMC_BTR1_ADDHLD | FSMC_BTR1_ADDSET | FSMC_BTR1_BUSTURN );
  /* 35ns from address valid to data valid. */
  /* 35ns from cs enabled to data enabled. */
  /* 15ns from bhe,ble valid to data valid. */
  *btr1 |= FSMC_BTR1_ADDSET_2;  //4 * 6ns = 24ns
  /* 15ns from rd enabled to data enabled. */
  *btr1 |= FSMC_BTR1_DATAST_1 | FSMC_BTR1_DATAST_0;  //3 * 6ns = 18ns
  /* 15ns from cs disabled to data disabled. */
  /* 10ns from rd disabled to data disabled. */
  /* 10ns from bhe,ble invalid to data invalid. */
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *btr1 |= FSMC_BTR1_BUSTURN_1 | FSMC_BTR1_BUSTURN_0;  //3 * 6ns = 18ns

  /* Write side setting. */
  *bwtr1 = 0x0fffffff;
  /* The cycle time for write in mr2a16a is 35ns. */
  *bwtr1 &= ~(FSMC_BWTR1_ACCMOD | FSMC_BWTR1_DATAST | FSMC_BWTR1_ADDSET | FSMC_BTR1_BUSTURN );
  /* Address setup time is 0ns. */
  /* The cs width is 20ns. */
  *bwtr1 |= FSMC_BWTR1_ADDSET_2;  //4 * 6ns = 24ns
  /* The wr width is 15ns. */
  /* Data setup time is 10ns. */
  *bwtr1 |= FSMC_BWTR1_DATAST_1 | FSMC_BWTR1_DATAST_0;  //3 * 6ns = 18ns
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *bwtr1 |= FSMC_BTR1_BUSTURN_1 | FSMC_BTR1_BUSTURN_0;  //3 * 6s = 18ns


  /* Enable w5300 area in sram bank1 */
  volatile unsigned long *bcr2 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + (8 * 1));
  volatile unsigned long *btr2 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + 4 + (8 * 1));
  volatile unsigned long *bwtr2 = (volatile unsigned long *)(FSMC_Bank1E_R_BASE + (8 * 1));

  /* Configuring the NE2 SRAM 0x64000000 */
  *bcr2 = 0x000030db;
  *bcr2 &= ~(FSMC_BCR2_ASYNCWAIT | FSMC_BCR2_WAITEN | FSMC_BCR2_MTYP | FSMC_BCR2_MUXEN | FSMC_BCR2_EXTMOD | FSMC_BCR2_MWID | FSMC_BCR2_MBKEN);
  *bcr2 |= (FSMC_BCR2_EXTMOD | FSMC_BCR2_MWID_0 | FSMC_BCR2_MBKEN);

  /* Reader Settings */
  *btr2 = 0x0fffffff;
  /* The cycle time of w5300 read is 93ns. */
  *btr2 &= ~(FSMC_BTR2_ACCMOD | FSMC_BTR2_DATAST | FSMC_BTR2_ADDHLD | FSMC_BTR2_ADDSET | FSMC_BTR2_BUSTURN );
  /* 65ns from cs disabled to data disabled. */
  /* 65ns from rd disabled to data disabled. */
  /* Data setup time from rd is 42ns. */
  /* The data hold time from rd is 7ns. */
  *btr2 |= FSMC_BTR2_DATAST_3 | FSMC_BTR2_DATAST_2; //72ns
  /* Address setup is virtually unspecified. */
  //*btr2 |= FSMC_BTR2_ADDSET_0;
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *btr2 |= FSMC_BTR2_BUSTURN_2;  //24ns

  /* Write side setting. */
  *bwtr2 = 0x0fffffff;
  /*  The cycle time of the w5300 write is 78ns. */
  *bwtr2 &= ~(FSMC_BWTR2_ACCMOD | FSMC_BWTR2_DATAST | FSMC_BWTR2_ADDSET | FSMC_BTR2_BUSTURN );
  /* 50ns from cs disabled to data disabled. */
  /* 50ns from wr disabled to data disabled. */
  /* Data setup time from wr is 7ns. */
  /* The data hold time from wr is 7ns. */
  *bwtr2 |= FSMC_BWTR2_DATAST_3 | FSMC_BWTR2_DATAST_1;  //54ns
  /* Address setup is virtually unspecified. */
  //*bwtr2 |= FSMC_BWTR2_ADDSET_0;
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *bwtr2 |= FSMC_BTR2_BUSTURN_2;  //24ns


  /* Enable arcnet area in sram bank1 */
  volatile unsigned long *bcr3 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + (8 * 2));
  volatile unsigned long *btr3 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + 4 + (8 * 2));
  volatile unsigned long *bwtr3 = (volatile unsigned long *)(FSMC_Bank1E_R_BASE + (8 * 2));

  /* Configuring the NE3 SRAM 0x68000000 */
  *bcr3 = 0x000030db;
  *bcr3 &= ~(FSMC_BCR3_ASYNCWAIT | FSMC_BCR3_WAITEN | FSMC_BCR3_MTYP | FSMC_BCR3_MUXEN | FSMC_BCR3_EXTMOD | FSMC_BCR3_MWID | FSMC_BCR3_MBKEN);
  *bcr3 |= (FSMC_BCR3_EXTMOD | FSMC_BCR3_MWID_0 | FSMC_BCR3_MBKEN);

  /* Reader Settings */
  *btr3 = 0x0fffffff;
  /* The cycle time of arcnet read is 100ns. */
  *btr3 &= ~(FSMC_BTR3_ACCMOD | FSMC_BTR3_DATAST | FSMC_BTR3_ADDHLD | FSMC_BTR3_ADDSET | FSMC_BTR3_BUSTURN );
  /* The width of rd is 60ns. */
  *btr3 |= FSMC_BTR3_DATAST_3 | FSMC_BTR3_DATAST_1 | FSMC_BTR3_DATAST_0;  //66ns
  /* Address setup time is 15ns. */
  *btr3 |= FSMC_BTR3_ADDSET_1 | FSMC_BTR3_ADDSET_0;  //18ns
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *btr3 |= FSMC_BTR3_BUSTURN_1 | FSMC_BTR3_BUSTURN_0;  //18ns

  /* Write side setting. */
  *bwtr3 = 0x0fffffff;
  /* The cycle time of arcnet write is 100ns. */
  *bwtr3 &= ~(FSMC_BWTR3_ACCMOD | FSMC_BWTR3_DATAST | FSMC_BWTR3_ADDSET | FSMC_BTR3_BUSTURN );
  /* The width of wr is 20ns. */
  *bwtr3 |= FSMC_BWTR3_DATAST_2;  //24ns
  /* Address setup time is 15ns. */
  *bwtr3 |= FSMC_BWTR3_ADDSET_1 | FSMC_BWTR3_ADDSET_0;  //18ns
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *bwtr3 |= FSMC_BTR3_BUSTURN_3 | FSMC_BTR3_BUSTURN_2;  //72ns


  /* Enable ext io area in sram bank1 */
  volatile unsigned long *bcr4 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + (8 * 3));
  volatile unsigned long *btr4 = (volatile unsigned long *)(FSMC_Bank1_R_BASE + 4 + (8 * 3));
  volatile unsigned long *bwtr4 = (volatile unsigned long *)(FSMC_Bank1E_R_BASE + (8 * 3));

  /* Configuring the NE4 SRAM 0x6C000000 */
  *bcr4 = 0x000030db;
  *bcr4 &= ~(FSMC_BCR4_ASYNCWAIT | FSMC_BCR4_WAITEN | FSMC_BCR4_MTYP | FSMC_BCR4_MUXEN | FSMC_BCR4_EXTMOD | FSMC_BCR4_MWID | FSMC_BCR4_MBKEN);
  *bcr4 |= (FSMC_BCR4_EXTMOD | FSMC_BCR4_MWID_0 | FSMC_BCR4_MBKEN);

  /* Reader Settings */
  *btr4 = 0x0fffffff;
  /* The cycle time of max1316ecm read is min:60ns -> 100ns? */
  *btr4 &= ~(FSMC_BTR4_ACCMOD | FSMC_BTR4_DATAST | FSMC_BTR4_ADDHLD | FSMC_BTR4_ADDSET | FSMC_BTR4_BUSTURN );
  /* The width of cs is min:30ns -> 77ns. */
  /* The width of rd is min:30ns -> 72ns. */
  /* The data setup time from rd is max:30ns. */
  /* The data hold time from rd is max:30ns. */
  *btr4 |= FSMC_BTR4_DATAST_3 | FSMC_BTR4_DATAST_2; //72ns
  /* Address setup is virtually unspecified. */
  *btr4 |= FSMC_BTR4_ADDSET_0;
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *btr4 |= FSMC_BTR4_BUSTURN_2;  //??ns

  /* Write side setting. */
  *bwtr4 = 0x0fffffff;
  /* What is the cycle time of max1316ecm write? ns */
  *bwtr4 &= ~(FSMC_BWTR4_ACCMOD | FSMC_BWTR4_DATAST | FSMC_BWTR4_ADDSET | FSMC_BTR4_BUSTURN );
  /* The width of cs is min:30ns -> 71ns. */
  /* The width of wr is min:30ns -> 60ns. */
  /* The data setup time from wr is min:10ns. */
  /* The data hold time from wr is min:10ns. */
  *bwtr4 |= FSMC_BWTR4_DATAST_3 | FSMC_BWTR4_DATAST_1;  //54ns
  /* Address setup is virtually unspecified. */
  *bwtr4 |= FSMC_BWTR4_ADDSET_0;
  /* Bus Turnaround Time. However, it is not available on the SRAM interface. */
  *bwtr4 |= FSMC_BTR4_BUSTURN_2;  //??ns

#endif
}



/* ----------------------------------------
    end of file
    designed by hamayan since 2009/05/21
---------------------------------------- */

