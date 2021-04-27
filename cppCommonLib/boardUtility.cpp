/* ----------------------------------------
  Board utility code
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
#include  "boardUtility.h"

/* ----------------------------------------
  defines.
---------------------------------------- */
//#define __ACCESS_IS_8BIT__

/* ----------------------------------------
  prototypes.
---------------------------------------- */
static int  ramCheck( uint8_t *ram, size_t size );
static int  ramCheck( uint16_t *ram, size_t size );
static int  ramCheck( uint32_t *ram, size_t size );

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */

/* ----------------------------------------
  act LED busy turn on and off.
---------------------------------------- */
void blinkLED( LED *led, RELTIM dly )
{
  while( 1 )
  {
    led->toggle();
    dly_tsk( dly );
  }
}

/* ----------------------------------------
  RAM byte check.
---------------------------------------- */
static int ramCheck( uint8_t *ram, size_t size )
{
  int i;
  volatile uint8_t *ptr,data;

  ptr = (volatile uint8_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint8_t)); i++ )
  {
    *ptr++ = (uint8_t)i;
  }

  ptr = (volatile uint8_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint8_t)); i++ )
  {
    data = *ptr++;
    if(data != (uint8_t)i)
    {
      return (-1);
    }
  }

  return 0;
}


/* ----------------------------------------
  RAM word check.
---------------------------------------- */
#if defined( __ACCESS_IS_16BIT__ )
static int ramCheck( uint16_t *ram, size_t size, uint16_t dataOffset )
{
  int i;
  volatile uint16_t *ptr,data;

  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    *ptr++ = (uint16_t)i + dataOffset;
  }

  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    data = *ptr++;
    if(data != (uint16_t)i + dataOffset)
    {
      return (-1);
    }
  }

  return 0;
}
#endif  /* defined( __ACCESS_IS_16BIT__ ) */


/* ----------------------------------------
  RAM double word check.
---------------------------------------- */
#if defined( __ACCESS_IS_16BIT__ )
static int ramCheck( uint32_t *ram, size_t size )
{
  int i;
  volatile uint32_t *ptr,data;

  ptr = (volatile uint32_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint32_t)); i++ )
  {
    *ptr++ = (uint32_t)i;
  }

  ptr = (volatile uint32_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint32_t)); i++ )
  {
    data = *ptr;
    uint32_t tempUL = i;
    if( data != tempUL )
    {
      return (-1);
    }
    ptr++;
  }

  return 0;
}
#endif  /* defined( __ACCESS_IS_16BIT__ ) */


/* ----------------------------------------
  random RAM double word check.
---------------------------------------- */
/* word access 8bit type. */
#if defined( __ACCESS_IS_8BIT__ )
int randomRamCheck( uint16_t *ram, size_t size, uint32_t seed )
{
  int i;
  uint16_t rnd;
  volatile uint16_t *ptr,data;

  srand( seed );
  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    rnd = (uint16_t)rand() | 0xFF00;
    *ptr++ = rnd;
  }

  srand( seed );
  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    data = *ptr & 0x00FF;
    rnd = (uint16_t)rand() & 0x00FF;
    if( data != rnd )
    {
      return (-1);
    }
    ptr++;
  }

  return 0;
}
#else  /* defined( __ACCESS_IS_16BIT__ ) */
/* word access 16bit type. */
int randomRamCheck( uint16_t *ram, size_t size, uint32_t seed )
{
  int i;
  uint16_t rnd;
  volatile uint16_t *ptr,data;
  size /= sizeof(uint16_t);

  srand( seed );
  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)size; i++ )
  {
    rnd = (uint16_t)rand();
    *ptr++ = rnd;
  }

  srand( seed );
  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)size; i++ )
  {
    data = *ptr;
    rnd = (uint16_t)rand();
    if( data != rnd )
    {
      return (-1);
    }
    ptr++;
  }

  return 0;
}
#endif  /* defined( __ACCESS_IS_8BIT__ or __ACCESS_IS_16BIT__ ) */

//#if defined( __ACCESS_IS_16BIT__ )
#if 1
static int randomRamCheck( uint32_t *ram, size_t size, uint32_t seed )
{
  int i;
  volatile uint32_t *ptr,data;
  srand( seed );
  ptr = (volatile uint32_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint32_t)); i++ )
  {
    *ptr++ = (uint32_t)rand();
  }

  srand( seed );
  ptr = (volatile uint32_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint32_t)); i++ )
  {
    data = *ptr;
    uint32_t rnd = (uint32_t)rand();
    if(data != rnd)
    {
      return (-1);
    }
    ptr++;
  }

  return 0;
}
#endif  /* defined( __ACCESS_IS_16BIT__ ) */

/* ----------------------------------------
  fixed data RAM word check.
---------------------------------------- */
#if defined( __ACCESS_IS_16BIT__ )
static int fixedRamCheck( uint16_t *ram, size_t size, uint16_t data )
{
  int i;
  volatile uint16_t *ptr;
  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    *ptr++ = data;
  }

  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    uint16_t rcv = *ptr;
    if( rcv != data )
    {
      return (-1);
    }
    ptr++;
  }
  return 0;
}
#endif  /* defined( __ACCESS_IS_16BIT__ ) */


/* ----------------------------------------
  RAM check.
---------------------------------------- */
void ramCheck( void *ram, size_t size, LED *led )
{
  uint32_t baseTim = millis();
  uint16_t dataOffset = 0x0000;
  uint16_t dataFixed = 0x0000; (void)dataFixed; // UNUSED( dataFixed );
  uint32_t seed = 1234;

  while( 1 )
  {
#if defined( __ACCESS_IS_16BIT__ )  /* byte access incremental pattern. */
    if( ramCheck( (uint8_t *)ram, size ) != 0 )
    {
      blinkLED( led, 50UL );
    }
#endif  /* defined( __ACCESS_IS_16BIT__ ) */

#if defined( __ACCESS_IS_16BIT__ )  /* word access incremental pattern. */
    if( ramCheck( (uint16_t *)ram, size, dataOffset ) != 0 )
    {
      blinkLED( led, 50UL );
    }
#endif  /* defined( __ACCESS_IS_16BIT__ ) */

#if 1  /* word access random pattern. */
    if( randomRamCheck( (uint16_t *)ram, size, seed ) != 0 )
    {
      blinkLED( led, 50UL );
    }
    seed++;
#endif

#if defined( __ACCESS_IS_16BIT__ )  /* double word access incremental pattern. */
    if( ramCheck( (uint32_t *)ram, size ) != 0 )
    {
      blinkLED( led, 50UL );
    }
#endif  /* defined( __ACCESS_IS_16BIT__ ) */

//#if defined( __ACCESS_IS_16BIT__ )  /* double word access random pattern. */
#if 1
    if( randomRamCheck( (uint32_t *)ram, size, seed ) != 0 )
    {
      blinkLED( led, 50UL );
    }
    seed++;
#endif  /* defined( __ACCESS_IS_16BIT__ ) */

#if defined( __ACCESS_IS_16BIT__ )  /* word access fixed pattern. */
    if( fixedRamCheck( (uint16_t *)ram, size, dataFixed ) != 0 )
    {
      blinkLED( led, 50UL );
    }
    dataFixed += 0x1111;
#endif  /* defined( __ACCESS_IS_16BIT__ ) */

    if( (millis() - baseTim) >= 500UL )
    {
      baseTim = millis();
      led->toggle();
    }
    dataOffset += size;
  }
}

/* ----------------------------------------
  Answer the remaining stack size.
---------------------------------------- */
uint32_t RemainStack( const void *stk, uint32_t sz )
{
  uint32_t i;
  const uint8_t *ptr = (const uint8_t *)stk;

  for( i = 0; i < sz; i++ )
  {
    if( *ptr++ != 0 ) break;
  }

  return sz - i;
}

