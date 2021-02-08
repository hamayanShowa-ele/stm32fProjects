/* ----------------------------------------
 FSMC utilities.
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
#ifndef  __STM32F1_FSMC_H__
#define  __STM32F1_FSMC_H__

#include  <stm32f10x.h>

extern "C"
{
}


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    defines
---------------------------------------- */
typedef union
{
  volatile uint32_t DWORD;
  struct
  {
    volatile uint32_t MBKEN : 1;
    volatile uint32_t MUXEN : 1;
    volatile uint32_t MTYP : 2;
    volatile uint32_t MWID : 2;
    volatile uint32_t FACCEN : 1;
    volatile uint32_t rsv3 : 1;
    volatile uint32_t BURSTEN : 1;
    volatile uint32_t WAITPOL : 1;
    volatile uint32_t WRAPMOD : 1;
    volatile uint32_t WAITCFG : 1;
    volatile uint32_t WREN : 1;
    volatile uint32_t WAITEN : 1;
    volatile uint32_t EXTMOD : 1;
    volatile uint32_t ASYNCWAIT : 1;
    volatile uint32_t rsv1 : 16;
  } BIT;
} FSMC_BCRx;

typedef union
{
  volatile uint32_t DWORD;
  struct
  {
    volatile uint32_t ADDSET : 4;
    volatile uint32_t ADDHLD : 4;
    volatile uint32_t DATAST : 8;
    volatile uint32_t BUSTURN : 4;
    volatile uint32_t CLKDIV : 4;
    volatile uint32_t DATLAT : 4;
    volatile uint32_t ACCMOD : 2;
    volatile uint32_t rsv1 : 2;
  } BIT;
} FSMC_TCRx;

typedef union
{
  volatile uint32_t DWORD;
  struct
  {
    volatile uint32_t ADDSET : 4;
    volatile uint32_t ADDHLD : 4;
    volatile uint32_t DATAST : 8;
    volatile uint32_t BUSTURN : 4;
    volatile uint32_t CLKDIV : 4;
    volatile uint32_t DATLAT : 4;
    volatile uint32_t ACCMOD : 2;
    volatile uint32_t rsv1 : 2;
  } BIT;
} FSMC_BWTRx;

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class STM32F1_FSMC
{
private:

public:
  STM32F1_FSMC() {}
  ~STM32F1_FSMC() {}

  void sram( uint32_t bank, uint32_t width,
    uint32_t rAS, uint32_t rDS,
    uint32_t wAS, uint32_t wDS,
    bool wait = false
  );
};

#endif  /* __STM32F1_FSMC_H__ */

