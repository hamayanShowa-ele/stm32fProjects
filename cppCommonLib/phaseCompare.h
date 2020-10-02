/* ----------------------------------------
  analog wave phase comparing header.
  for STMicroelectronics SPL library

  THE SOURCE CODE OF THE FOLLOWING url WAS MODIFIED FOR STM32F.
  https://github.com/asukiaaa/AM2320_asukiaaa

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

#ifndef __PHASE_COMPARE_H__
#define __PHASE_COMPARE_H__

#include  <stm32f10x.h>
#include  <HardwareSerial.h>
extern "C"
{
  #include  <math.h>
  #include  <stddef.h>
  #include  <string.h>
}

/* ----------------------------------------
  defines.
---------------------------------------- */
#define  ADC_SCALE  8192
#define  RADIAN     (360.0 / (2 * M_PI))

#define  COS00  (1)
#define  COS18  (0.951056516)
#define  COS36  (0.809016994)
#define  COS54  (0.587785252)
#define  COS72  (0.309016994)
#define  COS90  (0)

#define  SIN00  (0)
#define  SIN18  (0.309016994)
#define  SIN36  (0.587785252)
#define  SIN54  (0.809016994)
#define  SIN72  (0.951056516)
#define  SIN90  (1)

#if 0
/* Cos Table (*2048) */
#define  COS_00  8192  /* 0 degree */
#define  COS_18  7791  /* 18 degree*/
#define  COS_36  6627  /* 36 degree */
#define  COS_54  4815  /* 54 degree */
#define  COS_72  2531  /* 72 degree */

/* Sin Table (*2048) */
#define  SIN_18  2351  /* 18 degree */
#define  SIN_36  4815  /* 36 degree */
#define  SIN_54  6627  /* 54 degree */
#define  SIN_72  7791  /* 72 degree */
#define  SIN_90  8192  /* 90 degree */
#else
#define  COS_00  (int)(ADC_SCALE * COS00)  /* 0 degree */
#define  COS_18  (int)(ADC_SCALE * COS18 + 0.5)  /* 18 degree*/
#define  COS_36  (int)(ADC_SCALE * COS36 + 0.5)  /* 36 degree */
#define  COS_54  (int)(ADC_SCALE * COS54 + 0.5)  /* 54 degree */
#define  COS_72  (int)(ADC_SCALE * COS72 + 0.5)  /* 72 degree */

#define  SIN_18  (int)(ADC_SCALE * SIN18 + 0.5)  /* 18 degree */
#define  SIN_36  (int)(ADC_SCALE * SIN36 + 0.5)  /* 36 degree */
#define  SIN_54  (int)(ADC_SCALE * SIN54 + 0.5)  /* 54 degree */
#define  SIN_72  (int)(ADC_SCALE * SIN72 + 0.5)  /* 72 degree */
#define  SIN_90  (int)(ADC_SCALE * SIN90)  /* 90 degree */
#endif


/* ----------------------------------------
  prototypes.
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class PHASE_COMPARE
{
public:
  PHASE_COMPARE();
  PHASE_COMPARE( int count );
  ~PHASE_COMPARE();
  void begin( int count );
  void end();
  int  compare( const uint16_t *data, int size );
  int  compare( const uint16_t *dataA, const uint16_t *dataB, int size );

private:
  int16_t *VAbuffer,*VBbuffer;
  int elementCount;
  uint16_t VAoffset,VBoffset;
  int16_t VAmax,VAmin,VBmax,VBmin;

  void averages( const uint16_t *data, int size );
  void averages( const uint16_t *dataA, const uint16_t *dataB, int size );
  void getOffset();
  void clearOffset();
  void fft( int32_t *VAcos, int32_t *VAsin, int32_t *VBcos, int32_t *VBsin );
  int  getDiffPhase( int32_t VAcos, int32_t VAsin, int32_t  VBcos, int32_t VBsin );
  float getDiffPhaseF( int32_t VAcos, int32_t VAsin, int32_t  VBcos, int32_t VBsin );
};

#endif  /* __PHASE_COMPARE_H__ */
