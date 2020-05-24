/* ----------------------------------------
  text image print utilities
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

#include  "textPrint.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
TEXT_PRINT::TEXT_PRINT( uint16_t gap, uint16_t fg, uint16_t bg )
{
  gapY = gap;  /* space between the lines */
  fgColor = fg;  /* foreground color and background color. */
  bgColor = bg;  /* foreground color and background color. */
  posX = 0;  /* The position at which to start writing the letter. */
  posY = 0;  /* The position at which to start writing the letter. */
}

TEXT_PRINT::~TEXT_PRINT()
{
}

/* ----------------------------------------
    color configure
---------------------------------------- */
void TEXT_PRINT::color( uint16_t fg, uint16_t bg )
{
  fgColor = fg;  /* foreground color and background color. */
  bgColor = bg;  /* foreground color and background color. */
}

/* ----------------------------------------
    clear
---------------------------------------- */
void TEXT_PRINT::clear()
{
  fillRectangle( 0, 0, right() - left(), down() - up(), bgColor );
  posX = 0;  /* The position at which to start writing the letter. */
  posY = 0;  /* The position at which to start writing the letter. */
}

/* ----------------------------------------
    locate
---------------------------------------- */
void TEXT_PRINT::locate( int x, int y )
{
  posX = x;  /* The position at which to start writing the letter. */
  posY = y;  /* The position at which to start writing the letter. */
}

/* ----------------------------------------
    is this kanji?
---------------------------------------- */
bool TEXT_PRINT::isKanji( char code )
{
  if( code >= 0x80 ) return true;
  return false;
}

/* ----------------------------------------
    length of sjis string
---------------------------------------- */
int TEXT_PRINT::sjisLength( const char *str )
{
  int count = 0;
  while( *str )
  {
    if( isKanji( *str ) )
    {
      count += 2;
      str += 2;
    }
    else
    {
      count++;
      str++;
    }
  }
  return count;
}

/* ----------------------------------------
    moji
---------------------------------------- */
void TEXT_PRINT::txtLine( uint16_t *dst, uint8_t code, int width )
{
  for( int i = 0; i < width; i++ )
  {
    if( code & (0x80 >> i) ) *dst = fgColor;
    dst++;
  }
}

void TEXT_PRINT::txtLine( uint16_t *dst, uint16_t code, int width )
{
  for( int i = 0; i < width; i++ )
  {
    if( code & (0x8000 >> i) ) *dst = fgColor;
    dst++;
  }
}

void TEXT_PRINT::txtLine( uint16_t *dst, uint32_t code, int width )
{
  for( int i = 0; i < width; i++ )
  {
    if( code & (0x00800000 >> i) ) *dst = fgColor;
    dst++;
  }
}

void TEXT_PRINT::moji( uint16_t *dst, char code, int width, int height, int type )
{
  if( code < ' ' || code > '~' ) code = ' ';
  code -= ' ';  /* start from space. */

  /* clear by background */
  uint16_t *ptr = dst;
  for( int i = 0; i < width * gapY; i++ )
  {
    *ptr++ = bgColor;
  }

  /* put by foreground */
  ptr = dst; ptr += width * (gapY - height);
  if( type == TYPE_ASCII_8 )
  {
    const uint8_t *bmp = asciiGraphicBMPTable;
    uint32_t nextLine = asciiPixWidth / 8;
    for( int h = 0; h < height; h++ )
    {
  //    uint8_t c = *(asciiGraphicBMPTable + code + (h * asciiPixWidth / 8));
      uint8_t c = *(bmp + code + (h * nextLine));
      txtLine( ptr, c, width );
      ptr += width;
    }
  }
  else if( type == TYPE_ASCII_48 )
  {
    const uint8_t *bmp = ascii48GraphicBMPTable;
    uint32_t nextLine = ascii48PixWidth / 8;
    code *= width / 8;
    for( int h = 0; h < height; h++ )
    {
      uint32_t c;
      c =  (uint32_t)*(bmp + code + (h * nextLine) + 0) << 16;
      c |= (uint32_t)*(bmp + code + (h * nextLine) + 1) << 8;
      c |= (uint32_t)*(bmp + code + (h * nextLine) + 2) << 0;
      txtLine( ptr, c, width );
      ptr += width;
    }
  }
}

#if  defined( __KANJI_USE__ )
void TEXT_PRINT::moji( uint16_t *dst, uint16_t code, int width, int height )
{
  /* clear by background */
  int i;
  uint16_t *ptr = dst;
  for( i = 0; i < width * gapY; i++ )
  {
    *ptr++ = bgColor;
  }

  /* put by foreground */
  ptr = dst; ptr += width * (gapY - height);
  const uint8_t *font;
  for( i = 0; i < KanjiTnum; i++ )  /* Search codes from the Shift-JIS code table. */
  {
    if( code >= KanjiAreaRecord[ i ].start && code <= KanjiAreaRecord[ i ].end )
    {
      font = (const uint8_t *)KanjiAreaRecord[ i ].font + ((code - KanjiAreaRecord[ i ].start) * KanjiXSize / 8 * KanjiYSize);
      break;  /* When they find us, we're out of the loop. */
    }
  }

  if( i != KanjiTnum )  /* What to do when the code is found.  */
  {
    /* It examines the bits of FONT data and converts them to 16-bit length data. */
    for( int h = 0; h < height; h++ )
    {
      uint16_t c;
      c  = (uint16_t)*font << 8;
      font++;
      c |= (uint16_t)*font << 0;
      font++;
      txtLine( ptr, c, width );
      ptr += width;
    }
  }
}
#endif /* __KANJI_USE__ */


/* ----------------------------------------
    putc
---------------------------------------- */
int TEXT_PRINT::putc( const char *code, int type )
{
  int ret = 1;
  if( !isKanji( *code ) )
  {
    int w = fontWidth( type );
    int h = fontHeight( type );
    if( *code == '\r' )
    {
      posX = 0;  /* The position at which to start writing the letter. */
      return ret;
    }
    else if( *code == '\n' || (posX + left() + w) > right() )
    {
      posX = 0;  /* The position at which to start writing the letter. */
      posY += gapY;  /* The position at which to start writing the letter. */
      if( (posY + up() + gapY) > down() )
      {
        posY = 0;  /* The position at which to start writing the letter. */
      }
      if( *code == '\n' ) return ret;
    }

//    uint16_t buffer[ w * gapY ];
    uint16_t *buffer = new uint16_t[ w * gapY ];
    moji( buffer, *code, w , h, type );
    DRAW::pasteRectangle( posX, posY, posX + w - 1, posY + gapY - 1, (const uint16_t *)buffer );
    posX += w;  /* The position at which to start writing the letter. */
    delete [] buffer;
  }
#if  defined( __KANJI_USE__ )
  else
  {
    uint16_t mojiUS = (uint16_t)*code << 8;
    code++;
    mojiUS |= (uint16_t)*code;

    int w = fontWidth( TYPE_KANJI_16 );
    int h = fontHeight( TYPE_KANJI_16 );
    if( (posX + left() + w) > right() )
    {
      posX = 0;  /* The position at which to start writing the letter. */
      posY += gapY;  /* The position at which to start writing the letter. */
      if( (posY + up() + gapY) > down() )
      {
        posY = 0;  /* The position at which to start writing the letter. */
      }
    }

//    uint16_t buffer[ w * gapY ];
    uint16_t *buffer = new uint16_t[ w * gapY ];
    moji( buffer, mojiUS, w, h );
    DRAW::pasteRectangle( posX, posY, posX + w - 1, posY + gapY - 1, (const uint16_t *)buffer );
    posX += w;  /* The position at which to start writing the letter. */
    delete [] buffer;
    ret = 2;
  }
#endif /* __KANJI_USE__ */
  return ret;
}


/* ----------------------------------------
    puts
---------------------------------------- */
void TEXT_PRINT::puts( const char *code, int type )
{
  while( *code )
  {
    int len = putc( code, type );
    code += len;
  }
}


/* ----------------------------------------
    font width and font height
---------------------------------------- */
uint16_t TEXT_PRINT::fontWidth( int type )
{
  if( type == TYPE_ASCII_8 ) return asciiXSize;
#if  defined( __KANJI_USE__ )
  else if( type == TYPE_KANJI_16 ) return KanjiXSize;
#endif /* __KANJI_USE__ */
  else if( type == TYPE_ASCII_48 ) return ascii48XSize;
  return 0;
}

uint16_t TEXT_PRINT::fontHeight( int type )
{
  if( type == TYPE_ASCII_8 ) return asciiYSize;
#if  defined( __KANJI_USE__ )
  else if( type == TYPE_KANJI_16 ) return KanjiYSize;
#endif /* __KANJI_USE__ */
  else if( type == TYPE_ASCII_48 ) return ascii48YSize;
  return 0;
}
