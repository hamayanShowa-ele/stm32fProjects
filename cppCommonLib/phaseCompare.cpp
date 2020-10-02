/* ----------------------------------------
  analog wave phase comparing code
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
#include  "phaseCompare.h"

/* ----------------------------------------
  defines.
---------------------------------------- */

/* ----------------------------------------
  prototypes.
---------------------------------------- */

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
extern Serial Serial1;  /* hardware serial 1 */

/* ----------------------------------------
  constructor destructor
---------------------------------------- */
PHASE_COMPARE::PHASE_COMPARE()
{
  VAbuffer = nullptr;
  VBbuffer = nullptr;
}

PHASE_COMPARE::PHASE_COMPARE( int count )
{
  begin( count );
}

PHASE_COMPARE::~PHASE_COMPARE()
{
  end();
}

/* ----------------------------------------
  begin and end.
---------------------------------------- */
void PHASE_COMPARE::begin( int count )
{
  VAbuffer = new int16_t[ count ];
  VBbuffer = new int16_t[ count ];
  elementCount = count;
}

void PHASE_COMPARE::end()
{
  if( VAbuffer != nullptr ) { delete [] VAbuffer; }
  if( VBbuffer != nullptr ) { delete [] VBbuffer; }
}

/* ----------------------------------------
  averages for wave A and wave B.
---------------------------------------- */
void PHASE_COMPARE::averages( const uint16_t *data, int size )
{
  int i;
  int32_t *Abuf,*Bbuf;
  Abuf = new int32_t[elementCount];
  Bbuf = new int32_t[elementCount];
  for( i = 0; i < elementCount; i++ )
  {
    Abuf[ i ] = *data++ & 0x0FFF;
    Bbuf[ i ] = *data++ & 0x0FFF;
  }
  for( i = elementCount; i < size; i++ )
  {
    Abuf[ i % elementCount ] += *data++ & 0x0FFF;
    Bbuf[ i % elementCount ] += *data++ & 0x0FFF;
  }
  for( i = 0; i < elementCount; i++ )
  {
    VAbuffer[ i ] = (int16_t)(Abuf[ i ] / elementCount);
    VBbuffer[ i ] = (int16_t)(Bbuf[ i ] / elementCount);
  }
  delete [] Abuf;
  delete [] Bbuf;
}

void PHASE_COMPARE::averages( const uint16_t *dataA, const uint16_t *dataB, int size )
{
  int i;
  int32_t *Abuf,*Bbuf;
  Abuf = new int32_t[elementCount];
  Bbuf = new int32_t[elementCount];
  for( i = 0; i < elementCount; i++ )
  {
    Abuf[ i ] = *dataA++ & 0x0FFF;
    Bbuf[ i ] = *dataB++ & 0x0FFF;
  }
  for( i = elementCount; i < size; i++ )
  {
    Abuf[ i % elementCount ] += *dataA++ & 0x0FFF;
    Bbuf[ i % elementCount ] += *dataB++ & 0x0FFF;
  }
  for( i = 0; i < elementCount; i++ )
  {
    VAbuffer[ i ] = (int16_t)(Abuf[ i ] / elementCount);
    VBbuffer[ i ] = (int16_t)(Bbuf[ i ] / elementCount);
  }
  delete [] Abuf;
  delete [] Bbuf;
}

/* ----------------------------------------
  get offset.
---------------------------------------- */
void PHASE_COMPARE::getOffset()
{
  VAoffset = VBoffset = 0;
  uint32_t aveA,aveB;
  aveA = aveB = 0UL;
  for( int i = 0; i < elementCount; i++ )
  {
    aveA += VAbuffer[i];
    aveB += VBbuffer[i];
  }
  VAoffset = (uint16_t)(aveA / elementCount);
  VBoffset = (uint16_t)(aveB / elementCount);
}


/* ----------------------------------------
  clear offset and max min.
---------------------------------------- */
void PHASE_COMPARE::clearOffset()
{
  VAmax = VBmax = -32768;
  VAmin = VBmin = 32767;
  for( int i = 0; i < elementCount; i++ )
  {
    VAbuffer[i] -= VAoffset;
    VBbuffer[i] -= VBoffset;
    if( VAbuffer[i] > VAmax ) VAmax = VAbuffer[i];
    if( VBbuffer[i] > VBmax ) VBmax = VBbuffer[i];
    if( VAbuffer[i] < VAmin ) VAmin = VAbuffer[i];
    if( VBbuffer[i] < VBmin ) VBmin = VBbuffer[i];
  }
}

/* ----------------------------------------
  FFT.
---------------------------------------- */
void PHASE_COMPARE::fft( int32_t *VAcos, int32_t *VAsin, int32_t *VBcos, int32_t *VBsin )
{
  *VAcos =  ((int32_t)VAbuffer[0]                                                - (int32_t)VAbuffer[10]) * COS_00;
  *VAcos += ((int32_t)VAbuffer[1] + (int32_t)VAbuffer[19] - (int32_t)VAbuffer[9] - (int32_t)VAbuffer[11]) * COS_18;
  *VAcos += ((int32_t)VAbuffer[2] + (int32_t)VAbuffer[18] - (int32_t)VAbuffer[8] - (int32_t)VAbuffer[12]) * COS_36;
  *VAcos += ((int32_t)VAbuffer[3] + (int32_t)VAbuffer[17] - (int32_t)VAbuffer[7] - (int32_t)VAbuffer[13]) * COS_54;
  *VAcos += ((int32_t)VAbuffer[4] + (int32_t)VAbuffer[16] - (int32_t)VAbuffer[6] - (int32_t)VAbuffer[14]) * COS_72;
/* *VAcos +=                       ((int32_t)VAbuffer[15] - (int32_t)VAbuffer[5])                         * COS_90; */

/* *VAsin =((int32_t)VAbuffer[0] +                      - (int32_t)VAbuffer[10])                         * SIN_00; */
  *VAsin =  ((int32_t)VAbuffer[1] + (int32_t)VAbuffer[9] - (int32_t)VAbuffer[11] - (int32_t)VAbuffer[19]) * SIN_18;
  *VAsin += ((int32_t)VAbuffer[2] + (int32_t)VAbuffer[8] - (int32_t)VAbuffer[12] - (int32_t)VAbuffer[18]) * SIN_36;
  *VAsin += ((int32_t)VAbuffer[3] + (int32_t)VAbuffer[7] - (int32_t)VAbuffer[13] - (int32_t)VAbuffer[17]) * SIN_54;
  *VAsin += ((int32_t)VAbuffer[4] + (int32_t)VAbuffer[6] - (int32_t)VAbuffer[14] - (int32_t)VAbuffer[16]) * SIN_72;
  *VAsin += (                       (int32_t)VAbuffer[5]                         - (int32_t)VAbuffer[15]) * SIN_90;

  *VBcos =  ((int32_t)VBbuffer[0]                                                - (int32_t)VBbuffer[10]) * COS_00;
  *VBcos += ((int32_t)VBbuffer[1] + (int32_t)VBbuffer[19] - (int32_t)VBbuffer[9] - (int32_t)VBbuffer[11]) * COS_18;
  *VBcos += ((int32_t)VBbuffer[2] + (int32_t)VBbuffer[18] - (int32_t)VBbuffer[8] - (int32_t)VBbuffer[12]) * COS_36;
  *VBcos += ((int32_t)VBbuffer[3] + (int32_t)VBbuffer[17] - (int32_t)VBbuffer[7] - (int32_t)VBbuffer[13]) * COS_54;
  *VBcos += ((int32_t)VBbuffer[4] + (int32_t)VBbuffer[16] - (int32_t)VBbuffer[6] - (int32_t)VBbuffer[14]) * COS_72;
/* *VBcos +=                      ((int32_t)VBbuffer[15] - (int32_t)VBbuffer[5])                         * COS_90;*/

/* *VBsin =((int32_t)VBbuffer[0] +                      - (int32_t)VBbuffer[10])                         * SIN_00; */
  *VBsin =  ((int32_t)VBbuffer[1] + (int32_t)VBbuffer[9] - (int32_t)VBbuffer[11] - (int32_t)VBbuffer[19]) * SIN_18;
  *VBsin += ((int32_t)VBbuffer[2] + (int32_t)VBbuffer[8] - (int32_t)VBbuffer[12] - (int32_t)VBbuffer[18]) * SIN_36;
  *VBsin += ((int32_t)VBbuffer[3] + (int32_t)VBbuffer[7] - (int32_t)VBbuffer[13] - (int32_t)VBbuffer[17]) * SIN_54;
  *VBsin += ((int32_t)VBbuffer[4] + (int32_t)VBbuffer[6] - (int32_t)VBbuffer[14] - (int32_t)VBbuffer[16]) * SIN_72;
  *VBsin +=                        ((int32_t)VBbuffer[5]                         - (int32_t)VBbuffer[15]) * SIN_90;
}


/* ----------------------------------------
  get difference of phase.
---------------------------------------- */
int PHASE_COMPARE::getDiffPhase( int32_t VAcos, int32_t VAsin, int32_t VBcos, int32_t VBsin )
{
  int16_t VAk,VBk;

  if( VAcos == 0 )
  {
    if( VAsin >= 0 ) VAk = 0;
    else VAk = 180;
  }
  else if( VAsin == 0 )
  {
    if( VAcos >= 0 ) VAk = 90;
    else VAk = 270;
  }
  else
  {
    double tempD = atan((double)VAsin / (double)VAcos) * RADIAN;
    VAk = (tempD == 0.0) ? 0 : (tempD > 0.0) ? (int16_t)(tempD + 0.5) : (int16_t)(tempD - 0.5);
    if( VAcos > 0 ) VAk = 90 - VAk;  // first quadrant and second quadrant
    else VAk = 270 - VAk;  // third quadrant and fourth quadrant
  }

  if( VBcos == 0 )
  {
    if( VBsin >= 0 ) VBk = 0;
    else VBk = 180;
  }
  else if( VBsin == 0 )
  {
    if( VBcos >= 0 ) VBk = 90;
    else VBk = 270;
  }
  else
  {
    double tempD = atan((double)VBsin / (double)VBcos) * RADIAN;
    VBk = (tempD == 0.0) ? 0 : (tempD > 0.0) ? (int16_t)(tempD + 0.5) : (int16_t)(tempD - 0.5);
    if( VBcos > 0 ) VBk = 90 - VBk;  // first quadrant and second quadrant
    else VBk = 270 - VBk;  // third quadrant and fourth quadrant
  }

  int diff = VBk - VAk;
//  diff = -diff;  /* sign reversal */
  if( diff > 180 ) diff = diff - 360;

  return diff;
}


float PHASE_COMPARE::getDiffPhaseF( int32_t VAcos, int32_t VAsin, int32_t VBcos, int32_t VBsin )
{
  float VAk,VBk;
  if( VAcos == 0 )
  {
    if( VAsin >= 0 ) VAk = 0.0F;
    else VAk = 180.0F;
  }
  else if( VAsin == 0 )
  {
    if( VAcos >= 0 ) VAk = 90.0F;
    else VAk = 270.0F;
  }
  else
  {
    double tempD = atan((double)VAsin / (double)VAcos) * RADIAN;
    VAk = (tempD == 0.0) ? 0.0F : (float)tempD;
    if( VAcos > 0 ) VAk = 90.0F - VAk;  // first quadrant and second quadrant
    else VAk = 270.0F - VAk;  // third quadrant and fourth quadrant
  }

  if( VBcos == 0 )
  {
    if( VBsin >= 0 ) VBk = 0.0F;
    else VBk = 180.0F;
  }
  else if( VBsin == 0 )
  {
    if( VBcos >= 0 ) VBk = 90.0F;
    else VBk = 270.0F;
  }
  else
  {
    double tempD = atan((double)VBsin / (double)VBcos) * RADIAN;
    VBk = (tempD == 0.0) ? 0.0F : (float)tempD;
    if( VBcos > 0 ) VBk = 90.0F - VBk;  // first quadrant and second quadrant
    else VBk = 270.0F - VBk;  // third quadrant and fourth quadrant
  }

  float diff = VBk - VAk;
//  diff = -diff;  /* sign reversal */
  if( diff > 180.0F ) diff = diff - 360.0F;

  return diff;
}


/* ----------------------------------------
  compare phase A and phase B.
   but 0.1 degree units.
---------------------------------------- */
int PHASE_COMPARE::compare( const uint16_t *data, int size )
{
  averages( data, size );
  getOffset();
  clearOffset();

  int32_t VAcos,VAsin,VBcos,VBsin;
  fft( &VAcos, &VAsin, &VBcos, &VBsin );
  float diff = getDiffPhaseF( VAcos, VAsin, VBcos, VBsin );

  int tempI = (int)(diff * 100.0F);
  if( tempI == 0 ) {}
  else if( tempI > 0 ) tempI += 5;
  else tempI -= 5;

  return tempI / 10;
}

int PHASE_COMPARE::compare( const uint16_t *dataA, const uint16_t *dataB, int size )
{
  averages( dataA, dataB, size );
  getOffset();
  clearOffset();

  int32_t VAcos,VAsin,VBcos,VBsin;
  fft( &VAcos, &VAsin, &VBcos, &VBsin );
  float diff = getDiffPhaseF( VAcos, VAsin, VBcos, VBsin );

  int tempI = (int)(diff * 100.0F);
  if( tempI == 0 ) {}
  else if( tempI > 0 ) tempI += 5;
  else tempI -= 5;

  return tempI / 10;
}
