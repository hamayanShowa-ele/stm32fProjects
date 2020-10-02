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
static int ramCheck( uint16_t *ram, size_t size )
{
  int i;
  volatile uint16_t *ptr,data;

  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    *ptr++ = (uint16_t)i;
  }

  ptr = (volatile uint16_t *)ram;
  for( i = 0; i < (int)(size / sizeof(uint16_t)); i++ )
  {
    data = *ptr++;
    if(data != (uint16_t)i)
    {
      return (-1);
    }
  }

  return 0;
}


/* ----------------------------------------
  RAM double word check.
---------------------------------------- */
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
    data = *ptr++;
    if(data != (uint32_t)i)
    {
      return (-1);
    }
  }

  return 0;
}


/* ----------------------------------------
  RAM check.
---------------------------------------- */
void ramCheck( void *ram, size_t size, LED *led )
{
  uint32_t baseTim = millis();

  while( 1 )
  {
    if( ramCheck( (uint8_t *)ram, size ) != 0 )
    {
      blinkLED( led, 50UL );
    }

    if( ramCheck( (uint16_t *)ram, size ) != 0 )
    {
      blinkLED( led, 50UL );
    }

    if( ramCheck( (uint32_t *)ram, size ) != 0 )
    {
      blinkLED( led, 50UL );
    }

    if( (millis() - baseTim) >= 500UL )
    {
      baseTim = millis();
      led->toggle();
    }
  }
}

/* ----------------------------------------
  Serial loop back.
---------------------------------------- */
void serialLoopBack( Serial *s1 )
{
  while( 1 )
  {
    if( s1->available() )
    {
      int c = s1->read();
      s1->write( c );
    }
  }
}

void serialLoopBack( Serial *s1, Serial *s2 )
{
  while( 1 )
  {
    if( s1->available() )
    {
      int c = s1->read();
      s2->write( c );
    }
    if( s2->available() )
    {
      int c = s2->read();
      s1->write( c );
    }
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

