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

#include  "draw.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

static APEX *bubbleSort( APEX *apex, int len )
{
  for( int i = 0; i < len; i++ )
  {
    for( int j = len - 1; j > i; j-- )
    {
      if( apex[j].y < apex[j - 1].y || (apex[j].y == apex[j - 1].y && apex[j].x < apex[j - 1].x) )
      {
        APEX temp = apex[j];
        apex[j] = apex[j - 1];
        apex[j - 1] = temp;
      }
    }
  }
  return apex;
}


/* ----------------------------------------
    constructor destructor
---------------------------------------- */
DRAW::DRAW()
{
}

DRAW::~DRAW()
{
}

/* ----------------------------------------
    pixel set and get
    x,y : All of these variables are relative values.
---------------------------------------- */
void DRAW::pset( uint16_t x, uint16_t y, uint16_t color )
{
  wai_sem( SEMID_GCLD );  /* Waiting to get Semaphore. */
  coordinate( x + left(), y + up() );
  write( color );
  sig_sem( SEMID_GCLD );  /* Disarming the semaphore */
}

uint16_t DRAW::pget( uint16_t x, uint16_t y )
{
  x += left(); y += up();
  uint16_t pixel;
  wai_sem( SEMID_GCLD );  /* Waiting to get Semaphore. */
  read( (uint32_t)x * (uint32_t)y, &pixel, 1 );
  sig_sem( SEMID_GCLD );  /* Disarming the semaphore */
  return pixel;
}


/* ----------------------------------------
    draw line
    startX,startY,stopX,stopY : All of these variables are relative values.
---------------------------------------- */
void DRAW::line(
  int startX, int startY,
  int stopX, int stopY,
  uint16_t color )
{
  int dx,dy,sx,sy;
  sx = ( stopX > startX ) ? 1 : -1;
  sy = ( stopY > startY ) ? 1 : -1;
  dx = ( stopX > startX ) ? stopX - startX : startX - stopX;
  dy = ( stopY > startY ) ? stopY - startY : startY - stopY;

  /* horizontal line */
  if( dx && dy == 0 )
  {
    fillRectangle( startX, startY, stopX, stopY, color );
    return;
  }
  /* vertical line */
  if( dy && dx == 0 )
  {
    fillRectangle( startX, startY, stopX, stopY, color );
    return;
  }
  /* If the slope is less than or equal to 1 */
  int x = startX;
  int y = startY;
  int E;
  if( dx >= dy )
  {
    E = -dx;
    for( int i = 0; i <= dx; i++ )
    {
      if( x < 0 || x >= width() || y < 0 || y >= height() ) continue;
      pset( x, y, color );
      x += sx;
      E += 2 * dy;
      if( E >= 0 )
      {
        y += sy;
        E -= 2 * dx;
      }
    }
    return;
  }
  /* If the slope is more than to 1 */
  else
  {
    E = -dy;
    for( int i = 0; i <= dy; i++ )
    {
      if( x < 0 || x >= width() || y < 0 || y >= height() ) continue;
      pset( x, y, color );
      y += sy;
      E += 2 * dx;
      if( E >= 0 )
      {
        x += sx;
        E -= 2 * dy;
      }
    }
  }
}


/* ----------------------------------------
    draw rectangle
    x_0,y_0 : All of these variables are relative values.
    r : r is the radius.
---------------------------------------- */
void DRAW::rectangle(
  int x_1, int y_1, int x_2, int y_2,
  uint16_t color )
{
  line( x_1, y_1, x_1, y_2, color );
  line( x_1, y_2, x_2, y_2, color );
  line( x_2, y_2, x_2, y_1, color );
  line( x_2, y_1, x_1, y_1, color );
}

/* ----------------------------------------
    draw fill rectangle
    startX,startY,stopX,stopY : All of these variables are relative values.
---------------------------------------- */
void DRAW::fillRectangle(
  int startX, int startY,
  int stopX, int stopY,
  uint16_t color )
{
  if( startX > stopX )  /* When the number on the left is greater than the number on the right. */
  {
    int temp = startX;
    startX = stopX;
    stopX = temp;
  }
  if( startY > stopY )  /* When the number on the above is greater than the number on the below. */
  {
    int temp = startY;
    startY = stopY;
    stopY = temp;
  }
  startX += left();  /* Draw from the left edge of the display area. */
  stopX += left();  /* Draw from the left edge of the display area. */
  startY += up();  /* Draw from the up edge of the display area. */
  stopY += up();  /* Draw from the up edge of the display area. */
  if( startX < left() ) startX = left();  /* Make sure you don't go beyond the left side of the area. */
  if( startX > right() ) startX = right();  /* Make sure you don't go beyond the right side of the area. */
  if( startY < up() ) startY = up();  /* Make sure you don't go beyond the above side of the area. */
  if( startY > down() ) startY = down();  /* Make sure you don't go beyond the below side of the area. */
  if( stopX < left() ) stopX = left();  /* Make sure you don't go beyond the left side of the area. */
  if( stopX > right() ) stopX = right();  /* Make sure you don't go beyond the right side of the area. */
  if( stopY < up() ) stopY = up();  /* Make sure you don't go beyond the above side of the area. */
  if( stopY > down() ) stopY = down();  /* Make sure you don't go beyond the below side of the area. */

  uint16_t deltaX = stopX - startX + 1;  /* Calculate the length from the start to the end. */
  uint16_t deltaY = stopY - startY + 1;  /* Calculate the length from the start to the end. */
  wai_sem( SEMID_GCLD );  /* Waiting to get Semaphore. */
  window( startX, startY, stopX, stopY );  /* Set the window range. */
  write( color, (uint32_t)deltaX * (uint32_t)deltaY );  /* write to glcd controller. */
  sig_sem( SEMID_GCLD );  /* Disarming the semaphore */
}

/* ----------------------------------------
    paste rectangle
    startX,startY,stopX,stopY : All of these variables are relative values.
---------------------------------------- */
void DRAW::pasteRectangle(
  int startX, int startY,
  int stopX, int stopY,
  const uint16_t *src )
{
  if( startX > stopX )  /* When the number on the left is greater than the number on the right. */
  {
    int temp = startX;
    startX = stopX;
    stopX = temp;
  }
  if( startY > stopY )  /* When the number on the above is greater than the number on the below. */
  {
    int temp = startY;
    startY = stopY;
    stopY = temp;
  }
  startX += left();  /* Draw from the left edge of the display area. */
  stopX += left();  /* Draw from the left edge of the display area. */
  startY += up();  /* Draw from the up edge of the display area. */
  stopY += up();  /* Draw from the up edge of the display area. */
  if( startX < left() ) startX = left();  /* Make sure you don't go beyond the left side of the area. */
  if( startX > right() ) startX = right();  /* Make sure you don't go beyond the right side of the area. */
  if( startY < up() ) startY = up();  /* Make sure you don't go beyond the above side of the area. */
  if( startY > down() ) startY = down();  /* Make sure you don't go beyond the below side of the area. */
  if( stopX < left() ) stopX = left();  /* Make sure you don't go beyond the left side of the area. */
  if( stopX > right() ) stopX = right();  /* Make sure you don't go beyond the right side of the area. */
  if( stopY < up() ) stopY = up();  /* Make sure you don't go beyond the above side of the area. */
  if( stopY > down() ) stopY = down();  /* Make sure you don't go beyond the below side of the area. */

  uint16_t deltaX = stopX - startX + 1;  /* Calculate the length from the start to the end. */
  uint16_t deltaY = stopY - startY + 1;  /* Calculate the length from the start to the end. */
  wai_sem( SEMID_GCLD );  /* Waiting to get Semaphore. */
  window( startX, startY, stopX, stopY );  /* Set the window range. */
  write( src, (uint32_t)deltaX * (uint32_t)deltaY );  /* write to glcd controller. */
  sig_sem( SEMID_GCLD );  /* Disarming the semaphore */
}

/* ----------------------------------------
    draw circle
    x_0,y_0 : All of these variables are relative values.
    r : r is the radius.
---------------------------------------- */
void DRAW::circle(
  int x_0, int y_0, int r,
  uint16_t color )
{
  if( r < 1 ) return;

  int x = r;
  int y = 0;
  int F = -2 * r + 3;
  while( x >= y )
  {
    pset( x_0 + x, y_0 + y, color );
    pset( x_0 - x, y_0 + y, color );
    pset( x_0 + x, y_0 - y, color );
    pset( x_0 - x, y_0 - y, color );
    pset( x_0 + y, y_0 + x, color );
    pset( x_0 - y, y_0 + x, color );
    pset( x_0 + y, y_0 - x, color );
    pset( x_0 - y, y_0 - x, color );
    if( F >= 0 )
    {
      x--;
      F -= 4 * x;
    }
    y++;
    F += 4 * y + 2;
  }
}

void DRAW::fillCircle(
  int x_0, int y_0, int r,
  uint16_t color )
{
  if( r < 1 ) return;

  int x = r;
  int y = 0;
  int F = -2 * r + 3;
  while( x >= y )
  {
    line( x_0 - x, y_0 + y, x_0 + x, y_0 + y, color );
    line( x_0 - x, y_0 - y, x_0 + x, y_0 - y, color );
    line( x_0 - y, y_0 + x, x_0 + y, y_0 + x, color );
    line( x_0 - y, y_0 - x, x_0 + y, y_0 - x, color );
    if( F >= 0 )
    {
      x--;
      F -= 4 * x;
    }
    y++;
    F += 4 * y + 2;
  }
}


/* ----------------------------------------
    draw triangle
---------------------------------------- */
void DRAW::triangle( const APEX *apex, uint16_t color )
{
  line( apex[0].x, apex[0].y, apex[1].x, apex[1].y, color );
  line( apex[1].x, apex[1].y, apex[2].x, apex[2].y, color );
  line( apex[2].x, apex[2].y, apex[0].x, apex[0].y, color );
}

/* ----------------------------------------
    draw filed triangle
---------------------------------------- */
void DRAW::ftriangle( const POLYGON *tri, int minY, int maxY, uint16_t color )
{
  if( tri[0].dy == 0 )
  {
    for( int y = minY; y <= maxY; y++ )
    {
      if( tri[1].dx == 0 )
        line( ((float)y - tri[2].b) / tri[2].a + 0.5f, y, tri[2].apexX, y, color );
      else if( tri[2].dx == 0 )
        line( ((float)y - tri[1].b) / tri[1].a + 0.5f, y, tri[2].apexX, y, color );
      else
        line( ((float)y - tri[1].b) / tri[1].a + 0.5f, y, ((float)y - tri[2].b) / tri[2].a + 0.5f, y, color );
    }
  }
  else if( tri[1].dy == 0 )
  {
    for( int y = minY; y <= maxY; y++ )
    {
      if( tri[0].dx == 0 )
        line( ((float)y - tri[2].b) / tri[2].a + 0.5f, y, tri[0].apexX, y, color );
      else if( tri[2].dx == 0 )
        line( ((float)y - tri[0].b) / tri[0].a + 0.5f, y, tri[0].apexX, y, color );
      else
        line( ((float)y - tri[2].b) / tri[2].a + 0.5f, y, ((float)y - tri[0].b) / tri[0].a + 0.5f, y, color );
    }
  }
  else  /* tri[2].dy == 0 */
  {
    for( int y = minY; y <= maxY; y++ )
    {
      if( tri[0].dx == 0 )
        line( ((float)y - tri[1].b) / tri[1].a + 0.5f, y, tri[1].apexX, y, color );
      else if( tri[1].dx == 0 )
        line( ((float)y - tri[0].b) / tri[0].a + 0.5f, y, tri[1].apexX, y, color );
      else
        line( ((float)y - tri[0].b) / tri[0].a + 0.5f, y, ((float)y - tri[1].b) / tri[1].a + 0.5f, y, color );
    }
  }
}

void DRAW::fillTriangle( APEX *apex, uint16_t color )
{
  bubbleSort( (APEX *)apex, 3 );
  /* Calculate the amount of change in the Y direction. */
  POLYGON tri[3];
  tri[0].dy = apex[0].y - apex[1].y;
  tri[1].dy = apex[1].y - apex[2].y;
  tri[2].dy = apex[2].y - apex[0].y;

  if( tri[0].dy == 0 || tri[1].dy == 0 || tri[2].dy == 0 )  /* Treat as one triangle. */
  {
    tri[0].dx = apex[0].x - apex[1].x;
    tri[0].apexX = apex[0].x;
    tri[0].apexY = apex[0].y;
    if( tri[0].dx )
    {
      tri[0].a = (float)tri[0].dy / (float)tri[0].dx;
      tri[0].b = (float)tri[0].apexY - tri[0].a * (float)apex[0].x;
    }

    tri[1].dx = apex[1].x - apex[2].x;
    tri[1].apexX = apex[1].x;
    tri[1].apexY = apex[1].y;
    if( tri[1].dx )
    {
      tri[1].a = (float)tri[1].dy / (float)tri[1].dx;
      tri[1].b = (float)tri[1].apexY - tri[1].a * (float)apex[1].x;
    }

    tri[2].dx = apex[2].x - apex[0].x;
    tri[2].apexX = apex[2].x;
    tri[2].apexY = apex[2].y;
    if( tri[2].dx )
    {
      tri[2].a = (float)tri[2].dy / (float)tri[2].dx;
      tri[2].b = (float)tri[2].apexY - tri[2].a * (float)apex[2].x;
    }

    ftriangle( (const POLYGON *)tri, apex[0].y, apex[2].y, color );
  }
  else  /* Treat them as two triangles. */
  {
    /* Calculate the value on the x-axis of the line passing through points tri[2] and tri[0]. */
    tri[2].dx = apex[2].x - apex[0].x;
    tri[2].apexX = apex[2].x;
    tri[2].apexY = apex[2].y;
    if( tri[2].dx )
    {
      tri[2].a = (float)tri[2].dy / (float)tri[2].dx;
      tri[2].b = (float)tri[2].apexY - tri[2].a * (float)apex[2].x;
    }
    int y3 = apex[1].y;
    int x3 = ( tri[2].dx ) ? ((float)y3 - tri[2].b) / tri[2].a + 0.5f : tri[2].apexX;

    POLYGON tri3[3];
    /* tri3[0] : from tri[0] to tri[1] */
    tri3[0].dx = apex[1].x - apex[0].x;
    tri3[0].dy = apex[1].y - apex[0].y;
    tri3[0].apexX = apex[0].x;
    tri3[0].apexY = apex[0].y;
    if( tri3[0].dx )
    {
      tri3[0].a = (float)tri3[0].dy / (float)tri3[0].dx;
      tri3[0].b = (float)tri3[0].apexY - tri3[0].a * (float)tri3[0].apexX;
    }

    /* tri3[0] : from tri[1] to x3,y3 */
    tri3[1].dx = x3 - apex[1].x;
    tri3[1].dy = y3 - apex[1].y;
    tri3[1].apexX = apex[1].x;
    tri3[1].apexY = apex[1].y;
    if( tri3[1].dx )
    {
      tri3[1].a = (float)tri3[1].dy / (float)tri3[1].dx;
      tri3[1].b = (float)tri3[1].apexY - tri3[1].a * (float)tri3[1].apexX;
    }

    tri3[2].dx = apex[0].x - x3;
    tri3[2].dy = apex[0].y - y3;
    tri3[2].apexX = x3;
    tri3[2].apexY = y3;
    if( tri3[2].dx )
    {
      tri3[2].a = (float)tri3[2].dy / (float)tri3[2].dx;
      tri3[2].b = (float)tri3[2].apexY - tri3[2].a * (float)x3;
    }
    ftriangle( (const POLYGON *)tri3, apex[0].y, y3, color );

    /* tri3[0] : from tri[1] to x3,y3 */
    tri3[0].dx = x3 - apex[1].x;
    tri3[0].dy = y3 - apex[1].y;
    tri3[0].apexX = apex[1].x;
    tri3[0].apexY = apex[1].y;
    if( tri3[0].dx )
    {
      tri3[0].a = (float)tri3[0].dy / (float)tri3[0].dx;
      tri3[0].b = (float)tri3[0].apexY - tri3[0].a * (float)tri3[0].apexX;
    }

    /* tri3[1] : from x3,y3 to tri[2] */
    tri3[1].dx = apex[2].x - x3;
    tri3[1].dy = apex[2].y - y3;
    tri3[1].apexX = x3;
    tri3[1].apexY = y3;
    if( tri3[1].dx )
    {
      tri3[1].a = (float)tri3[1].dy / (float)tri3[1].dx;
      tri3[1].b = (float)tri3[1].apexY - tri3[1].a * (float)tri3[1].apexX;
    }

    /* tri3[2] : from tri[2] to tri[1] */
    tri3[2].dx = apex[1].x - apex[2].x;
    tri3[2].dy = apex[1].y - apex[2].y;
    tri3[2].apexX = apex[2].x;
    tri3[2].apexY = apex[2].y;
    if( tri3[2].dx )
    {
      tri3[2].a = (float)tri3[2].dy / (float)tri3[2].dx;
      tri3[2].b = (float)tri3[2].apexY - tri3[2].a * (float)tri3[2].apexX;
    }
    ftriangle( (const POLYGON *)tri3, y3, tri3[2].apexY, color );
  }
}

/* ----------------------------------------
    draw polygon
---------------------------------------- */
void DRAW::polygon( APEX *apex, int size, uint16_t color )
{
  int i; size--;
  for( i = 0; i < size; i++ )
  {
    line( apex[i].x, apex[i].y, apex[i + 1].x, apex[i + 1].y, color );
  }
  line( apex[i].x, apex[i].y, apex[0].x, apex[0].y, color );
}

/* ----------------------------------------
    rgb data format converter.
---------------------------------------- */
uint16_t DRAW::rgb24to16( uint32_t color )
{
  color &= 0x00FFFFFF;
  if( color == 0 ) return 0;
  if( color == 0x00FFFFFF ) return 0xFFFF;

  uint16_t tempUS = (uint16_t)((color & 0x0000F8) >> 3);
  tempUS |= (uint16_t)((color & 0x00FC00) >> 5);
  tempUS |= (uint16_t)((color & 0xF80000) >> 8);

  return tempUS;
}

uint16_t DRAW::rgb24to16( uint8_t r, uint8_t g, uint8_t b )
{
  uint16_t tempUS = (b & 0xF8) >> 3;
  tempUS |= (uint16_t)(g & 0xFC) << 3;
  tempUS |= (uint16_t)(r & 0xF8) << 8;

  return tempUS;
}

