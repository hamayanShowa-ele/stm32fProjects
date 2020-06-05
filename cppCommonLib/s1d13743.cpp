/* ----------------------------------------
  S1D13743(SEIKO EPSON) utilities
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
#include  "s1d13743.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern SYSTIM systim;
extern volatile uint8_t dma2ch4TC_Update;

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
S1D13743::S1D13743()
{
}

S1D13743::~S1D13743()
{
}

/* ----------------------------------------
    S1D13743 device revision code
---------------------------------------- */
uint8_t S1D13743::revision()
{
  GLCD_ADDRESS_SET = GLCD_ADR_REVISION_CODE;
  uint8_t rev = GLCD_DATA_BYTE_ADR;
  return rev;
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
/* ----------------------------------------
    graphic controller IC(S1D13743 SEIKO EPSON) initialize
    l : left size
    r : right size
    u : up size
    d : down size
---------------------------------------- */
int S1D13743::begin()
{
  GLCD_ADDRESS_SET = GLCD_ADR_POWER_SAVE;      /* enter to sleep mode. */
  GLCD_DATA_BYTE_ADR = 0x02;                   /* enter to sleep mode. 0x56 */
  GLCD_ADDRESS_SET = GLCD_ADR_PLL_M_DIVEIDER;  /* Set the M division register to one division. */

#if defined( __CLKI_IS_1M__ )  /* When the CLKI is 1MHz */
  GLCD_DATA_BYTE_ADR = 1 - 1;                  /* Set the M division register to one division. 0x04 */
#elif defined( __CLKI_IS_8M__ )  /* When the CLKI is 8MHz */
  GLCD_DATA_BYTE_ADR = 8 - 1;                  /* M division register is set to 8 divisions. 0x04 */
#else  /* When the CLKI is 36MHz */
  GLCD_DATA_BYTE_ADR = 18 - 1;                 /* M division register is set to 18 divisions. 0x04 */
#endif
  GLCD_DATA_BYTE_ADR = 0xF8;                   /* specified value 0x06 */
  GLCD_DATA_BYTE_ADR = 0x80;                   /* specified value 0x08 */
  GLCD_DATA_BYTE_ADR = 0x28;                   /* specified value 0x0a */
  GLCD_DATA_BYTE_ADR = 0x00;                   /* specified value 0x0c */
#if defined( __CLKI_IS_1M__ ) || defined( __CLKI_IS_8M__ )  /* When the CLKI is 1MHz or 8MHz.  */
//  GLCD_DATA_BYTE_ADR = 54 - 1;                 /* target value 0x0e */
  GLCD_DATA_BYTE_ADR = 60 - 1;                 /* target value 0x0e */
#else  /* When the CLKI is 36MHz */
  GLCD_DATA_BYTE_ADR = (54 / 2) - 1;           /* target value 0x0e */
#endif
  GLCD_DATA_BYTE_ADR = 0x00;                   /* DUMMY 0x10 */
  GLCD_DATA_BYTE_ADR = (5 << 3) | 0x01;        /* sysclk / 6,pll select 0x12 */
  GLCD_DATA_BYTE_ADR = 0x01;                   /* No panel data swap, 24bit 0x14 */
  GLCD_DATA_BYTE_ADR = X_MAX_PIXEL / 8;        /* HDISP size / 8 0x16 */
  GLCD_DATA_BYTE_ADR = HSYNC_PERIOD - X_MAX_PIXEL; /* HNDISP=45 0x18 */
  GLCD_DATA_BYTE_ADR = Y_MAX_PIXEL % 256;      /* VDISP size 0x1a */
  GLCD_DATA_BYTE_ADR = Y_MAX_PIXEL / 256;      /* VDISP size 0x1c */
  GLCD_DATA_BYTE_ADR = VSYNC_PERIOD - Y_MAX_PIXEL; /*VNDISP=14 0x1e */
  GLCD_DATA_BYTE_ADR = 41;                     /* HSYNC width 0x20 */
  GLCD_DATA_BYTE_ADR = 2;                      /* HSYNC start position 0x22 */
  GLCD_DATA_BYTE_ADR = 10;                     /* VSYNC width 0x24 */
  GLCD_DATA_BYTE_ADR = 2;                      /* VSYNC start position 0x26 */
  GLCD_DATA_BYTE_ADR = 0x00;                   /* PCLK polarity 0x28 */

  GLCD_DATA_BYTE_ADR = 1;                      /* Set the input mode to RGB5:6:5 0x2a */
  GLCD_ADDRESS_SET = GLCD_ADR_POWER_SAVE;      /* Cancelling sleep mode */
  GLCD_DATA_BYTE_ADR = 0x00;                   /* Cancelling sleep mode 0x56 */
//  GLCD_DATA_BYTE_ADR = 0x07;                   /* Outputs logical ORs for horizontal and vertical synchronization in TE 0x58 */
  GLCD_DATA_BYTE_ADR = 0x06;                   /* Outputs the hidden period of vertical synchronization in TE 0x58 */
//  GLCD_DATA_BYTE_ADR = 0x05;                   /* Outputs the hidden period of horizontal synchronization in TE 0x58 */

  SYSTIM baseTim = systim;
  while( 1 )  /* Wait for the PLL to stabilize. */
  {
    GLCD_ADDRESS_SET = GLCD_ADR_PLL_M_DIVEIDER;
    if( GLCD_DATA_BYTE_ADR & 0x80 ) break;
    if( (systim - baseTim) >= 1000UL ) return (-1);
  }

  GLCD_ADDRESS_SET = GLCD_ADR_WINDOW_X_START_POSITION_0;
  GLCD_DATA_BYTE_ADR = 0;    /*window x Starting position. 0x38 */
  GLCD_DATA_BYTE_ADR = 0;    /*window x Starting position. 0x3a */
  GLCD_DATA_BYTE_ADR = 0;    /*window y Starting position. 0x3c */
  GLCD_DATA_BYTE_ADR = 0;    /*window y Starting position. 0x3e */
  GLCD_DATA_BYTE_ADR = (X_MAX_PIXEL - 1) % 256;    /*window x stop position 0x40 */
  GLCD_DATA_BYTE_ADR = (X_MAX_PIXEL - 1) / 256;    /*window x stop position 0x42 */
  GLCD_DATA_BYTE_ADR = (Y_MAX_PIXEL - 1) % 256;    /*window y stop position 0x44 */
  GLCD_DATA_BYTE_ADR = (Y_MAX_PIXEL - 1) / 256;    /*window y stop position 0x46 */

  GLCD_ADDRESS_SET = GLCD_ADR_GPIO_PINS_CONFIGURATION_0;
  GLCD_DATA_BYTE_ADR = LCD_DISP_PIN;    /*bit7=DISP is output 0x5A*/
  GLCD_ADDRESS_SET = GLCD_ADR_GPIO_PINS_STATUS_CONTROL_0;
  GLCD_DATA_BYTE_ADR = LCD_DISP_PIN;    /*DISP is high. Enable the display by setting the DISP pin to High. 0x5C*/
//  GLCD_DATA_BYTE_ADR = 0x00;    /*DISP is low. Disable the display by setting the DISP pin to High. */

  swivelView( SWIVEL_VIEW_180 );  /* 180 degree rotation */

  return 0;
}

/* ----------------------------------------
    swivel view.
---------------------------------------- */
void S1D13743::swivelView( uint8_t rotation )
{
  GLCD_ADDRESS_SET = GLCD_ADR_DISPLAY_MODE;  /* Display Mode Register 0x34 */
  rotation &= 0x03;
  rotation |= 0x08;
  GLCD_DATA_BYTE_ADR = rotation;  /* bit2,1,0=SwivelView 0:normal,1:90,2:180,3:270*/
}

/* ----------------------------------------
    configure va area.
    l : left size
    r : right size
    u : up size
    d : down size
---------------------------------------- */
int S1D13743::vaConfig( uint16_t l, uint16_t u, uint16_t r, uint16_t d )
{
  if( l == r || u == d ) return (-1);
  if( l >= X_MAX_PIXEL || r >= X_MAX_PIXEL ) return (-1);
  if( u >= Y_MAX_PIXEL || d >= Y_MAX_PIXEL ) return (-1);

  if( l < r )
  {
    va_left = l;
    va_right = r;
  }
  else
  {
    va_left = r;
    va_right = l;
  }

  if( u < d )
  {
    va_up = u;
    va_down = d;
  }
  else
  {
    va_up = d;
    va_down = u;
  }
  return 0;
}

/* ----------------------------------------
    graphic controller configure window.
---------------------------------------- */
void S1D13743::window( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end )
{
  GLCD_ADDRESS_SET = GLCD_ADR_WINDOW_X_START_POSITION_0;  /**/
  GLCD_DATA_BYTE_ADR = x_start;       /* window x Lower 8 bits of the start position. */
  GLCD_DATA_BYTE_ADR = x_start >> 8;  /* window x Upper 8 bits of the start position. */
  GLCD_DATA_BYTE_ADR = y_start;       /* window y Lower 8 bits of the start position. */
  GLCD_DATA_BYTE_ADR = y_start >> 8;  /* window y Upper 8 bits of the start position. */
  GLCD_DATA_BYTE_ADR = x_end;         /* window x Lower 8 bits of the stop position. */
  GLCD_DATA_BYTE_ADR = x_end >> 8;    /* window x Upper 8 bits of the stop position. */
  GLCD_DATA_BYTE_ADR = y_end;         /* window y Lower 8 bits of the stop position. */
  GLCD_DATA_BYTE_ADR = y_end >> 8;    /* window y Upper 8 bits of the stop position. */
}

/* ----------------------------------------
    graphic controller set x and y position.
---------------------------------------- */
void S1D13743::coordinate( uint16_t x, uint16_t y )
{
  GLCD_ADDRESS_SET = GLCD_ADR_WINDOW_X_START_POSITION_0; /**/
  GLCD_DATA_BYTE_ADR = x;      /* window x Lower 8 bits of the start position. */
  GLCD_DATA_BYTE_ADR = x >> 8; /* window x Upper 8 bits of the start position. */
  GLCD_DATA_BYTE_ADR = y;      /* window y Lower 8 bits of the start position. */
  GLCD_DATA_BYTE_ADR = y >> 8; /* window y Upper 8 bits of the start position. */
}

/* ----------------------------------------
    graphic controller vram write.
---------------------------------------- */
void S1D13743::write( uint16_t data )
{
  GLCD_ADDRESS_SET = GLCD_ADR_MEMORY_DATA_PORT_0;
  GLCD_DATA_WORD_ADR = data;
}

void S1D13743::write( const uint16_t *data, uint32_t size )
{
  GLCD_ADDRESS_SET = GLCD_ADR_MEMORY_DATA_PORT_0;

  if( size <= 32 )
  {
    while( size )
    {
      uint32_t sz = ( size > 128UL ) ? 128UL : size;
      size -= sz;
      while( sz-- ) GLCD_DATA_WORD_ADR = *data++;  /**/
      rot_rdq();
    }
  }
  else
  {
    while( size )
    {
      uint32_t sz = ( size > 65535UL ) ? 65535UL : size;  // max 65535UL
      size -= sz;

      STM32F_DMA::begin( LCD_DMA_Channel );
      STM32F_DMA::m2p( (uint16_t *)&GLCD_DATA_WORD_ADR, data, sz );
//      STM32F_DMA::ITConfig( DMA_IT_TC, ENABLE );  /* interrupt set. */
      STM32F_DMA::command( ENABLE );  /* enable dma. */
      while( DMA_GetCurrDataCounter( LCD_DMA_Channel ) ) rot_rdq();
    }
  }
}

void S1D13743::write( uint16_t data, uint32_t size )
{
  GLCD_ADDRESS_SET = GLCD_ADR_MEMORY_DATA_PORT_0;

  if( size <= 32 )
  {
    while( size )
    {
      uint32_t sz = ( size > 128UL ) ? 128UL : size;
      size -= sz;
      while( sz-- ) GLCD_DATA_WORD_ADR = data;  /**/
      rot_rdq();
    }
  }
  else
  {
    while( size )
    {
      uint32_t sz = ( size > 65535UL ) ? 65535UL : size;  // max 65535UL
      size -= sz;

      STM32F_DMA::begin( LCD_DMA_Channel );
      STM32F_DMA::p2p( (uint16_t *)&GLCD_DATA_WORD_ADR, (const uint16_t *)&data, sz );
//      STM32F_DMA::ITConfig( DMA_IT_TC, ENABLE );  /* interrupt set. */
      STM32F_DMA::command( ENABLE );  /* enable dma. */
      while( DMA_GetCurrDataCounter( LCD_DMA_Channel ) ) rot_rdq();
    }
  }
}

/* ----------------------------------------
    graphic controller vram read from red and green plane.
---------------------------------------- */
int S1D13743::redGreenRead( uint32_t adr, uint16_t *dst, uint32_t size )
{
  if( size > (X_MAX_PIXEL * Y_MAX_PIXEL) ) return (-1);
  if( adr & 0xfff80000 ) return (-1);

  /* Setting the read address. */
  GLCD_ADDRESS_SET = GLCD_ADR_MEMORY_READ_ADDRESS_0;
  GLCD_DATA_BYTE_ADR = adr;
  GLCD_DATA_BYTE_ADR = adr >> 8;
  GLCD_DATA_BYTE_ADR = (adr >> 16) & 0x07;

  GLCD_ADDRESS_SET = GLCD_ADR_MEMORY_DATA_PORT_0;
  while( size-- ) *dst++ = GLCD_DATA_WORD_ADR;  /**/

  return 0;
}

/* ----------------------------------------
    graphic controller vram read from blue plane.
---------------------------------------- */
int S1D13743::blueRead( uint32_t adr, uint16_t *dst, uint32_t size )
{
  volatile unsigned short blue;
  volatile unsigned long rgb;

  if( size > (X_MAX_PIXEL * Y_MAX_PIXEL) ) return (-1);
  if( adr & 0xfff80000 ) return (-1);
  adr += 0x4c000;  /* bank 2 */

  /* Setting the read address. */
  GLCD_ADDRESS_SET = GLCD_ADR_MEMORY_READ_ADDRESS_0;
  GLCD_DATA_BYTE_ADR = adr;
  GLCD_DATA_BYTE_ADR = adr >> 8;
  GLCD_DATA_BYTE_ADR = (adr >> 16) & 0x07;

  GLCD_ADDRESS_SET = GLCD_ADR_MEMORY_DATA_PORT_0;
  while( size-- )
  {
    blue = GLCD_DATA_WORD_ADR;
    rgb = *dst << 8;  /* Retrieving RED and GREEN data */
    rgb |= blue & 0x0ff;  /* Lower 8bit from 16bit data. */
    /* Converting 24bit data to 16bit data. */
    *dst++ = (((rgb & 0x00f80000) >> 8) | ((rgb & 0x0000fc00) >> 5) | ((rgb & 0x000000f8) >> 3));

    rgb = *dst << 8;  /* Retrieving RED and GREEN data */
    rgb |= blue >> 8;  /* Upper 8bit from 16bit data. */
    /* Converting 24bit data to 16bit data. */
    *dst++ = (((rgb & 0x00f80000) >> 8) | ((rgb & 0x0000fc00) >> 5) | ((rgb & 0x000000f8) >> 3));
  }

  return 0;
}

/* ----------------------------------------
    graphic controller vram read.
---------------------------------------- */
void S1D13743::read( uint32_t adr, uint16_t *dst, uint32_t size )
{
  redGreenRead( adr, dst, size );
  blueRead( adr, dst, size );
}

/* ----------------------------------------
    VA left and right position.
---------------------------------------- */
uint16_t S1D13743::left()
{
  return va_left;
}

uint16_t S1D13743::right()
{
  return va_right;
}

/* ----------------------------------------
    VA up and down position.
---------------------------------------- */
uint16_t S1D13743::up()
{
  return va_up;
}

uint16_t S1D13743::down()
{
  return va_down;
}

/* ----------------------------------------
    VA width and height size.
---------------------------------------- */
uint16_t S1D13743::width()
{
  return va_right - va_left;
}

uint16_t S1D13743::height()
{
  return va_down - va_up;
}
