/* ----------------------------------------
  1401 Virgo test program for STM32F407ZET
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
#include  "mramTest.h"

/* ----------------------------------------
  defines.
---------------------------------------- */

/* ----------------------------------------
  prototypes.
---------------------------------------- */
static void actLEDBlinkBusy();
static int mramCheckByte();
static int mramCheckWord();
static int mramCheckLong();

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
extern Serial Serial1;
extern PCA8574 brdAddress;

/* ----------------------------------------
  act LED busy turn on and off.
---------------------------------------- */
static void actLEDBlinkBusy()
{
  while( 1 )
  {
    if( ACT_IS ) ACT_IS_0;
    else ACT_IS_1;
    dly100us( 50UL * 10UL );
  }
}

/* ----------------------------------------
  MRAM byte check.
---------------------------------------- */
static int mramCheckByte()
{
  int i;
  volatile unsigned char *ptr,data;
  volatile void *mram = (volatile void *)SRAM_BASE_ADDRESS;

  ptr = (volatile unsigned char *)mram;
  for( i = 0; i < (int)(MRAM_SIZE / sizeof(uint8_t)); i++ )
  {
    *ptr++ = (unsigned char)i;
  }

  ptr = (volatile unsigned char *)mram;
  for( i = 0; i < (int)(MRAM_SIZE / sizeof(uint8_t)); i++ )
  {
    data = *ptr++;
    if(data != (unsigned char)i)
    {
      return (-1);
    }
  }

  return 0;
}


/* ----------------------------------------
  MRAM word check.
---------------------------------------- */
static int mramCheckWord()
{
  int i;
  volatile unsigned short *ptr,data;
  volatile void *mram = (volatile void *)SRAM_BASE_ADDRESS;

  ptr = (volatile unsigned short *)mram;
  for( i = 0; i < (int)(MRAM_SIZE / sizeof(uint16_t)); i++ )
  {
    *ptr++ = (unsigned short)i;
  }

  ptr = (volatile unsigned short *)mram;
  for( i = 0; i < (int)(MRAM_SIZE / sizeof(uint16_t)); i++ )
  {
    data = *ptr++;
    if(data != (unsigned short)i)
    {
      return (-1);
    }
  }

  return 0;
}


/* ----------------------------------------
  MRAM double word check.
---------------------------------------- */
static int mramCheckLong()
{
  int i;
  volatile unsigned long *ptr,data;
  volatile void *mram = (volatile void *)SRAM_BASE_ADDRESS;

  ptr = (volatile unsigned long *)mram;
  for( i = 0; i < (int)(MRAM_SIZE / sizeof(uint32_t)); i++ )
  {
    *ptr++ = (unsigned long)i;
  }

  ptr = (volatile unsigned long *)mram;
  for( i = 0; i < (int)(MRAM_SIZE / sizeof(uint32_t)); i++ )
  {
    data = *ptr++;
    if(data != (unsigned long)i)
    {
      return (-1);
    }
  }

  return 0;
}


/* ----------------------------------------
  MRAM check.
---------------------------------------- */
void mramCheck()
{
  while(1)
  {
    if( mramCheckByte() != 0 )
    {
      actLEDBlinkBusy();
    }
    if( ACT_IS ) ACT_IS_0;
    else ACT_IS_1;

    if( mramCheckWord() != 0 )
    {
      actLEDBlinkBusy();
    }
    if( ACT_IS ) ACT_IS_0;
    else ACT_IS_1;

    if( mramCheckLong() != 0 )
    {
      actLEDBlinkBusy();
    }
    if( ACT_IS ) ACT_IS_0;
    else ACT_IS_1;
  }
}

/* ----------------------------------------
  board address switch read.
---------------------------------------- */
void boardSwitchRead()
{
  while( 1 )
  {
    Serial1.printf( "  BOARD ADDRESS = %02x\r\n", swap( brdAddress.read() ) );
    dly1ms( 1000UL );
  }
}

/* ----------------------------------------
  Serial 1 loop back.
---------------------------------------- */
void serial1LoopBack()
{
  while( 1 )
  {
    if( Serial1.available() )
    {
      int c = Serial1.read();
      Serial1.write( c );
    }
  }
}
