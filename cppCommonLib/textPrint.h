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

#ifndef  __TEXT_PRINT_H__
#define  __TEXT_PRINT_H__

#include  <draw.h>
#include  <derivertive.h>
extern "C"
{
  #include  <font8.h>
  #include  <font16.h>
  #include  <font48.h>
}


/* ----------------------------------------
    defines
---------------------------------------- */
enum FONT_TYPE { TYPE_ASCII_8, TYPE_KANJI_16, TYPE_ASCII_48, };

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class TEXT_PRINT : public DRAW
{
public:
  uint16_t gapY;  /* distance between the lines */
  uint16_t fgColor,bgColor;  /* foreground color and background color. */

  TEXT_PRINT( uint16_t gap = 16, uint16_t fg = RGB565_WHITE, uint16_t bg = RGB565_BLACK );
  ~TEXT_PRINT();

  void color( uint16_t fg = RGB565_WHITE, uint16_t bg = RGB565_BLACK );
  void clearScreen();
  void clearLine();
  void clearLine( int line );
  void locate( int x, int y );
  int  putc( const char *code, int type = TYPE_ASCII_8 );
  void puts( const char *code, int type = TYPE_ASCII_8 );
  void write( const char *code, size_t len, int type = TYPE_ASCII_8 );

  uint16_t fontWidth( int type = TYPE_ASCII_8 );
  uint16_t fontHeight( int type = TYPE_ASCII_8 );

  int sjisLength( const char *str );

private:
  int posX,posY;  /* The position at which to start writing the letter. */

  void moji( uint16_t *dst, char code, int w, int h, int type = TYPE_ASCII_8 );
  void moji( uint16_t *dst, uint16_t code, int w, int h );
  void txtLine( uint16_t *dst, uint8_t code, int w );
  void txtLine( uint16_t *dst, uint16_t code, int w );
  void txtLine( uint16_t *dst, uint32_t code, int w );
  bool isKanji( char code );
};


#endif  /* __TEXT_PRINT_H__ */

