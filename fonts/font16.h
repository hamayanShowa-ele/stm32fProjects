/* ----------------------------------------
  japanese style kanji text font16 header
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

#ifndef  __KANJI_FONT16_H__
#define  __KANJI_FONT16_H__


/* ----------------------------------------
    defines
---------------------------------------- */
typedef struct
{
  unsigned short start;  /* Start number of the kanji code. */
  unsigned short end;    /* End number of the kanji code. */
  const unsigned char *font;      /* Head address of font data area.  */
} AREA_RECORD;


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern const unsigned short KanjiXSize;
extern const unsigned short KanjiYSize;
extern const unsigned short KanjiTnum;
extern const AREA_RECORD KanjiAreaRecord[];

#endif  /* __KANJI_FONT16_H__ */

