/* ----------------------------------------
 stm32f1xx dma utility header
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
#ifndef  __DMA_H__
#define  __DMA_H__

#include  <stm32f10x.h>
extern "C"
{
  #include  <mul_tsk.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */

/* ----------------------------------------
    register bit
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class STM32F_DMA // : public GPIO
{
public:
  STM32F_DMA();
  ~STM32F_DMA();

  void m2p(
    DMA_Channel_TypeDef* dmaCh,
    uint16_t *dst, const uint16_t *src, uint32_t count );
  void p2p(
    DMA_Channel_TypeDef* dmaCh,
    uint16_t *dst, const uint16_t *src, uint32_t count );

private:
};


#endif  /* __DMA_H__ */

