/* ----------------------------------------
  wiznet chip utilities
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

#ifndef __WIZ_NET_H__
#define __WIZ_NET_H__

#include  <derivertive.h>
#include  <strutil.h>

#if  (_WIZCHIP_ == 5500 || _WIZCHIP_ == 5100 || _WIZCHIP_ == 5200)
#include  <SPI.h>
#endif  /* (_WIZCHIP_ == 5500) */

extern "C"
{
  #include  <string.h>
  #include  <mul_tsk.h>
  #include  <wizchip_conf.h>
}

#if  (_WIZCHIP_ == 5500 || _WIZCHIP_ == 5100 || _WIZCHIP_ == 5200)
extern "C"
{
  #include  <w5500.h>
}
#define  _CHIP_VERSION_           0x04
#endif  /* (_WIZCHIP_ == 5500 || _WIZCHIP_ == 5100 || _WIZCHIP_ == 5200) */


class WIZNET : public GPIO
{
public:
  WIZNET();
  ~WIZNET();
  void begin( SPI *wire, uint8_t cs, const uint8_t *memsize, dhcp_mode dhcp = NETINFO_STATIC );

private:
#if  (_WIZCHIP_ == 5500 || _WIZCHIP_ == 5100 || _WIZCHIP_ == 5200)
  SPI *spi;
#endif  /* (_WIZCHIP_ == 5500 || _WIZCHIP_ == 5100 || _WIZCHIP_ == 5200) */
  uint8_t csPin;
};

#endif  /* __WIZ_NET_H__ */
