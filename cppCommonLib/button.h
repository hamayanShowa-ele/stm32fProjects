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

#ifndef  __BUTTON_H__
#define  __BUTTON_H__

#include  <textPrint.h>
extern "C"
{
}


/* ----------------------------------------
    defines
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class BUTTON : public TEXT_PRINT
{
public:
  BUTTON();
  ~BUTTON();

  void txtConfig( int x1, int y1, int x2, int y2, uint16_t gap = 16, uint16_t fg = RGB565_WHITE, uint16_t bg = RGB565_BLACK );
  void txtColor( uint16_t fg, uint16_t bg );
  void txtGapY( uint16_t gap );
  void txtClear();

  void buttonImage( const uint16_t *image, int x1, int y1, int x2, int y2 );
  void buttonImage( const uint16_t *image );
  void buttonImage();
  void print( const char *msg, int x, int y );
  void print( const char *msg );

  void backImage( const uint16_t *image );
  void backImage();

private:
  const uint16_t *fgImage;  /* button image data. */
  const uint16_t *bgImage;  /* background image data. */
  int txtX1,txtX2,txtY1,txtY2;
  int imageX1,imageX2,imageY1,imageY2;
};


#endif  /* __BUTTON_H__ */

