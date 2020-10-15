/* ----------------------------------------
 board 2007 sram 8m utilities.
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

#include  "2007.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern Serial Serial1;  /* hardware serial 1 */
extern LED actLed;

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
---------------------------------------- */

/* ----------------------------------------
    sram 8m check program.
---------------------------------------- */
int SRAM_8M::sram8m()
{
  int ret = 0;
  while( 1 )
  {
#if 0
    /* back up check. */
    for( int b = 0; b < BANK_COUNT; b++ )
    {
      /* bank setup. */
      bank( b );
      /* byte */
      ret = randomCheck( (const uint8_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint8_t), b + 1, true );
      /* word */
      ret = randomCheck( (const uint16_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint16_t), b + 1, true );
      /* double word */
      ret = randomCheck( (const uint32_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint32_t), b + 1, true );
      /* act led toggle. */
      actLed.toggle();
    }
#endif

    /* memory check. */
    for( int b = 0; b < BANK_COUNT; b++ )
    {
      /* bank setup. */
      bank( b );
      /* byte */
      randomWrite( (uint8_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint8_t), b + 1 );
      randomCheck( (const uint8_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint8_t), b + 1 );
      /* word */
      randomWrite( (uint16_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint16_t), b + 1 );
      randomCheck( (const uint16_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint16_t), b + 1 );
      /* double word */
      randomWrite( (uint32_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint32_t), b + 1 );
      randomCheck( (const uint32_t *)memBaseAdr, BYTE_SIZE_AT_ONE_BANK / sizeof(uint32_t), b + 1 );
      /* act led toggle. */
      actLed.toggle();
    }
#if 0
    break;
#endif
  }
  return ret;
}

/* ----------------------------------------
    random data write
---------------------------------------- */
void SRAM_8M::randomWrite( uint8_t *adr, int loop, uint32_t seed )
{
  srand( seed );
  for( int i = 0; i < loop; i++ )
  {
    *adr++ = (uint8_t)rand();
  }
}

void SRAM_8M::randomWrite( uint16_t *adr, int loop, uint32_t seed )
{
  srand( seed );
  for( int i = 0; i < loop; i++ )
  {
    *adr++ = (uint16_t)rand();
  }
}

void SRAM_8M::randomWrite( uint32_t *adr, int loop, uint32_t seed )
{
  srand( seed );
  for( int i = 0; i < loop; i++ )
  {
    *adr++ = (uint32_t)rand();
  }
}

/* ----------------------------------------
    random data check
---------------------------------------- */
int SRAM_8M::randomCheck( const uint8_t *adr, int loop, uint32_t seed, bool escape )
{
  srand( seed );
  for( int i = 0; i < loop; i++ )
  {
    uint8_t a = *adr;
    uint8_t b = (uint8_t)rand();
    if( a != b )
    {
      char buffer[64];
      sprintf( buffer, "    BYTE ERROR ADR=%08X SRAM:%02X DATA:%02X\r\n", (unsigned int)adr ,a, b );
      Serial1.printf( buffer );
      if( escape == true ) return (-1);
      ledBusy();
    }
    adr++;
  }
  return 0;
}

int SRAM_8M::randomCheck( const uint16_t *adr, int loop, uint32_t seed, bool escape )
{
  srand( seed );
  for( int i = 0; i < loop; i++ )
  {
    uint16_t a = *adr;
    uint16_t b = (uint16_t)rand();
    if( a != b )
    {
      char buffer[64];
      sprintf( buffer, "    WORD ERROR ADR=%08X SRAM:%04X DATA:%04X\r\n", (unsigned int)adr ,a, b );
      Serial1.printf( buffer );
      if( escape == true ) return (-1);
      ledBusy();
    }
    adr++;
  }
  return 0;
}

int SRAM_8M::randomCheck( const uint32_t *adr, int loop, uint32_t seed, bool escape )
{
  srand( seed );
  for( int i = 0; i < loop; i++ )
  {
    uint32_t a = *adr;
    uint32_t b = (uint32_t)rand();
    if( a != b )
    {
      char buffer[64];
      sprintf( buffer, "    DWORD ERROR ADR=%08X SRAM:%08X DATA:%08X\r\n", (unsigned int)adr ,(unsigned int)a, (unsigned int)b );
      Serial1.printf( buffer );
      if( escape == true ) return (-1);
      ledBusy();
    }
    adr++;
  }
  return 0;
}

/* ----------------------------------------
    led busy
---------------------------------------- */
void SRAM_8M::ledBusy()
{
  while( true )
  {
    actLed.toggle();
    dly_tsk( 50UL );
  }
}

#if 0
/*************************************************************************/
/* 外部 CBUSチェック 0x6C000000                                          */
/* アクセスは16bit単位とする。                                           */
/* A22でメモリとIOを切り替える。A22が1ならばメモリ、0ならばIO            */
/* 書込み時はNOP挿入すると、連続時のサイクルタイムを満足できる。         */
/*   *mem_0 = temp; __NOP();__NOP();                                     */
/* ※試作品の場合はアドレッシングの関係から、コメントアウトと、c_bus.h   */
/* 内のコメントを外す必要がある。                                        */
/*************************************************************************/
void CBUSCheck( void )
{
#define  BLOCK_SIZE  65536
//#define  BLOCK_SIZE  4096
#define  BANK_SIZE   64
//#define  BANK_SIZE   1
  int i,bank,xor,cnt;
  char buf[32];
  volatile short *memw;
  volatile char  *memb;

#if 0
  /*C-BUS RESET*/
  C_RESET_LO;
  Dly100us( 10 * 10 );  /*10ms*/
  C_RESET_HI;
  Dly100us( 10 * 10 );  /*10ms*/
#endif

#if 0
  while( 0 )
  {
    dump( (const void *)SRAM_8M_ADR, 1 );
    Dly100us( 1000 * 10 );  /*10ms*/
  }
#endif

  /*memory area*/
  xor = cnt = 0;
  while( 1 )
  {
    /*word access*/
    for( bank = 0; bank < BANK_SIZE; bank++ )
    {
      /*BANK SET*/
      SRAM_8M_IO_ADR = bank;
      /*全力で書込み*/
      for( i = 0, memw = (volatile short *)SRAM_8M_ADR; i < BLOCK_SIZE / 2; i++ )
      {
        *memw++ = i ^ xor;
//        memw++;  /*試作品用*/
      }
    }

    for( bank = 0; bank < BANK_SIZE; bank++ )
    {
      /*BANK SET*/
      SRAM_8M_IO_ADR = bank;
      /*全力で読込*/
      for( i = 0, memw = (volatile short *)SRAM_8M_ADR; i < BLOCK_SIZE / 2; i++ )
      {
        volatile short sramData = *memw++;
        volatile short compData = (short)(i ^ xor);
        if( sramData != compData )
        {
          (void)SCI_Puts( SCI_1, "sram 8m no good.\r\n" );
        }
//        memw++;  /*試作品用*/
      }
    }
    (void)SCI_Puts( SCI_1, "sram 8m word check end. " );
    xor++;
#if 1
    /*byte access*/
    for( bank = 0; bank < BANK_SIZE; bank++ )
    {
      /*BANK SET*/
      SRAM_8M_IO_ADR = bank;
      /*全力で書込み*/
      for( i = 0, memb = (volatile char *)SRAM_8M_ADR; i < BLOCK_SIZE / 2; i++ )
      {
        *memb++ = i ^ xor;
        *memb++ = (i ^ xor) >> 8;
//        memb++;  /*試作品用*/
//        memb++;  /*試作品用*/
      }
    }

    for( bank = 0; bank < BANK_SIZE; bank++ )
    {
      /*BANK SET*/
      SRAM_8M_IO_ADR = bank;
      /*全力で読込*/
      for( i = 0, memb = (volatile char *)SRAM_8M_ADR; i < BLOCK_SIZE / 2; i++ )
      {
        if( *memb++ != (char)(i ^ xor) )
        {
          (void)SCI_Puts( SCI_1, "sram 8m no good 2.\r\n" );
        }
        if( *memb++ != (char)((i ^ xor) >> 8) )
        {
          (void)SCI_Puts( SCI_1, "sram 8m no good 2.\r\n" );
        }
//        memb++;  /*試作品用*/
//        memb++;  /*試作品用*/
      }
    }
    xor++;
    (void)SCI_Puts( SCI_1, "sram 8m byte check end. count= " );
#endif
    int2Ascii( buf, cnt++ );
    (void)SCI_Puts( SCI_1, buf );
    (void)SCI_Puts( SCI_1, ".\r\n" );
//break;
    Dly100us( 50 * 10 );  /*50ms*/
  }
}
#endif


extern "C"
{
  /* If necessary, write the C code here. */
}  /* extern "C" */
