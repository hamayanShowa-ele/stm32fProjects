/* ----------------------------------------
  button utilities
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

#include  "button.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
BUTTON::BUTTON()
{
  fgImage = NULL;  /* button image data. const uint16_t * */
  bgImage = NULL;  /* background image data. */
}

BUTTON::~BUTTON()
{
}

/* ----------------------------------------
    text configure
---------------------------------------- */
void BUTTON::txtConfig( int x1, int y1, int x2, int y2, uint16_t gap, uint16_t fg, uint16_t bg )
{
  txtX1 = x1; txtX2 = x2; txtY1 = y1; txtY2 = y2;
  TEXT_PRINT::gapY = gap;  /*  distance between the lines. */
  TEXT_PRINT::fgColor = fg;  /* foreground color and background color. */
  TEXT_PRINT::bgColor = bg;  /* foreground color and background color. */
}

/* ----------------------------------------
    text color configure
---------------------------------------- */
void BUTTON::txtColor( uint16_t fg, uint16_t bg )
{
  TEXT_PRINT::fgColor = fg;  /* foreground color and background color. */
  TEXT_PRINT::bgColor = bg;  /* foreground color and background color. */
}

/* ----------------------------------------
    text distance between the lines.
---------------------------------------- */
void BUTTON::txtGapY( uint16_t gap )
{
  TEXT_PRINT::gapY = gap;  /*  distance between the lines. */
}

/* ----------------------------------------
    text clear
---------------------------------------- */
void BUTTON::txtClear()
{
  DRAW::fillRectangle( txtX1, txtY1, txtX2, txtY2, TEXT_PRINT::bgColor );
}

/* ----------------------------------------
    text print
---------------------------------------- */
void BUTTON::print( const char *msg, int x, int y )
{
  TEXT_PRINT::locate( x, y );
  int len = TEXT_PRINT::sjisLength( msg );
  len *= 8;
  if( (len + x) > txtX2 ) TEXT_PRINT::puts( "ERROR" );
  else TEXT_PRINT::puts( msg );
}

void BUTTON::print( const char *msg )
{
  print( msg, txtX1, txtY1 );
}


/* ----------------------------------------
    paste button image data
---------------------------------------- */
void BUTTON::buttonImage( const uint16_t *image, int x1, int y1, int x2, int y2 )
{
  fgImage = image;  /* button image data. */
  imageX1 = x1; imageX2 = x2;
  imageY1 = y1; imageY2 = y2;
  DRAW::pasteRectangle( imageX1, imageY1, imageX2, imageY2, fgImage );
}

void BUTTON::buttonImage( const uint16_t *image )
{
  fgImage = image;  /* button image data. */
  buttonImage( fgImage, imageX1, imageY1, imageX2, imageY2 );
}

void BUTTON::buttonImage()
{
  if( fgImage != NULL )
  {
    buttonImage( fgImage, imageX1, imageY1, imageX2, imageY2 );
  }
}

/* ----------------------------------------
    paste background image data
---------------------------------------- */
void BUTTON::backImage( const uint16_t *image )
{
  bgImage = image;  /* button image data. */
  DRAW::pasteRectangle( DRAW::left(), DRAW::up(), DRAW::right(), DRAW::down(), bgImage );
}

void BUTTON::backImage()
{
  if( bgImage != NULL )
  {
    backImage( bgImage );
  }
}

