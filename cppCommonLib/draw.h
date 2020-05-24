/* ----------------------------------------
  lcd draw utilities
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

#ifndef  __LCD_DRAW_H__
#define  __LCD_DRAW_H__

#include  <s1d13743.h>

extern "C"
{
  #include  <stdlib.h>
  #include  <mul_tsk.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  RGB24_WHITE     (uint32_t)0x00FFFFFF
#define  RGB24_SILVER    (uint32_t)0x00D6D6CE  //0x00C0C0C0
#define  RGB24_GRAY      (uint32_t)0x00848484  //0x00808080
#define  RGB24_DARKGRAY  (uint32_t)0x00424242
#define  RGB24_BLACK     (uint32_t)0x00000000
#define  RGB24_BLUE      (uint32_t)0x000000FF
#define  RGB24_NAVY      (uint32_t)0x00000080
#define  RGB24_CYAN      (uint32_t)0x0000FFFF
#define  RGB24_GREEN     (uint32_t)0x00008000
#define  RGB24_DARKGREEN (uint32_t)0x00006400
#define  RGB24_LIME      (uint32_t)0x0000FF00
#define  RGB24_RED       (uint32_t)0x00FF0000
#define  RGB24_MAROON    (uint32_t)0x00800000
#define  RGB24_ORANGE    (uint32_t)0x00FF4500
#define  RGB24_MAGENTA   (uint32_t)0x00FF00FF
#define  RGB24_YELLO     (uint32_t)0x00FFFF00
#define  RGB24_GOLD      (uint32_t)0x00FFD700

#define  MASK_R  (uint32_t)0x00F80000
#define  MASK_G  (uint32_t)0x0000FC00
#define  MASK_B  (uint32_t)0x000000F8

#define  RGB565_WHITE     (uint16_t)(((RGB24_WHITE & MASK_R) >> 8)  | ((RGB24_WHITE & MASK_G) >> 5)  | ((RGB24_WHITE & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_SILVER    (uint16_t)(((RGB24_SILVER & MASK_R) >> 8) | ((RGB24_SILVER & MASK_G) >> 5) | ((RGB24_SILVER & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_GRAY      (uint16_t)(((RGB24_GRAY & MASK_R) >> 8)   | ((RGB24_GRAY & MASK_G) >> 5)   | ((RGB24_GRAY & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_DARKGRAY  (uint16_t)(((RGB24_DARKGRAY & MASK_R) >> 8) | ((RGB24_DARKGRAY & MASK_G) >> 5) | ((RGB24_DARKGRAY & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_BLACK     (uint16_t)(((RGB24_BLACK & MASK_R) >> 8)  | ((RGB24_BLACK & MASK_G) >> 5)  | ((RGB24_BLACK & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_BLUE      (uint16_t)(((RGB24_BLUE & MASK_R) >> 8)   | ((RGB24_BLUE & MASK_G) >> 5)   | ((RGB24_BLUE & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_NAVY      (uint16_t)(((RGB24_NAVY & MASK_R) >> 8)   | ((RGB24_NAVY & MASK_G) >> 5)   | ((RGB24_NAVY & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_CYAN      (uint16_t)(((RGB24_CYAN & MASK_R) >> 8)   | ((RGB24_CYAN & MASK_G) >> 5)   | ((RGB24_CYAN & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_GREEN     (uint16_t)(((RGB24_GREEN & MASK_R) >> 8)  | ((RGB24_GREEN & MASK_G) >> 5)  | ((RGB24_GREEN & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_DARKGREEN (uint16_t)(((RGB24_DARKGREEN & MASK_R) >> 8) | ((RGB24_DARKGREEN & MASK_G) >> 5) | ((RGB24_DARKGREEN & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_LIME      (uint16_t)(((RGB24_LIME & MASK_R) >> 8)   | ((RGB24_LIME & MASK_G) >> 5)   | ((RGB24_LIME & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_RED       (uint16_t)(((RGB24_RED & MASK_R) >> 8)    | ((RGB24_RED & MASK_G) >> 5)    | ((RGB24_RED & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_MAROON    (uint16_t)(((RGB24_MAROON & MASK_R) >> 8) | ((RGB24_MAROON & MASK_G) >> 5) | ((RGB24_MAROON & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_ORANGE    (uint16_t)(((RGB24_ORANGE & MASK_R) >> 8) | ((RGB24_ORANGE & MASK_G) >> 5) | ((RGB24_ORANGE & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_MAGENTA   (uint16_t)(((RGB24_MAGENTA & MASK_R) >> 8) | ((RGB24_MAGENTA & MASK_G) >> 5) | ((RGB24_MAGENTA & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_YELLO     (uint16_t)(((RGB24_YELLO & MASK_R) >> 8)  | ((RGB24_YELLO & MASK_G) >> 5)  | ((RGB24_YELLO & MASK_B) >> 3))  /* R5 G6 B5 */
#define  RGB565_GOLD      (uint16_t)(((RGB24_GOLD & MASK_R) >> 8)   | ((RGB24_GOLD & MASK_G) >> 5)   | ((RGB24_GOLD & MASK_B) >> 3))  /* R5 G6 B5 */

typedef struct
{
  int x,y;
} APEX;

typedef struct
{
  int dx,dy;
  int apexX,apexY;
  float a,b;
} POLYGON;

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class DRAW : public S1D13743
{
public:
  DRAW();
  ~DRAW();

  void pset( uint16_t x, uint16_t y, uint16_t color );
  uint16_t pget( uint16_t x, uint16_t y );
  void line( int startX, int startY, int stopX, int stopY, uint16_t color );
  void rectangle( int x_1, int y_1, int x_2, int y_2, uint16_t color );
  void fillRectangle( int startX, int startY, int stopX, int stopY, uint16_t color );
  void circle( int x_0, int y_0, int r, uint16_t color );
  void fillCircle( int x_0, int y_0, int r, uint16_t color );
  void triangle( const APEX *apex, uint16_t color );
  void fillTriangle( APEX *apex, uint16_t color );
  void pasteRectangle( int startX, int startY, int stopX, int stopY, const uint16_t *src );
  void polygon( APEX *apex, int size, uint16_t color );
  uint16_t rgb24to16( uint32_t color );
  uint16_t rgb24to16( uint8_t r, uint8_t g, uint8_t b );

private:
  void ftriangle( const POLYGON *tri, int minY, int maxY, uint16_t color );
};


#endif  /* __LCD_DRAW_H__ */

