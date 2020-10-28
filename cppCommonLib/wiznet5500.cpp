/* ----------------------------------------
  wiznet w5500 utilities
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
#include "wiznet.h"

extern volatile SYSTIM systim;

/* ----------------------------------------
    prototypes
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern uint8_t MAC[];      // self hardware address
extern uint8_t IPVAL[];    // self ip address
extern uint8_t GATEWAY[];  // default gateway
extern uint8_t SUBNET[];   // sub net mask
extern uint8_t DNS[];      // primary dns server

/* ----------------------------------------
    wizchip_select and wizchip_deselect
---------------------------------------- */
extern "C"
{
static void wizchip_select( void ) { W5500_CS_IS_0; }
static void wizchip_deselect( void ) { W5500_CS_IS_1; }

static uint8_t wizchip_read_SPI1( void ) { return spi_rw_SPI1( 0xFF ); }
static uint8_t wizchip_read_SPI2( void ) { return spi_rw_SPI2( 0xFF ); }
static uint8_t wizchip_read_SPI3( void ) { return spi_rw_SPI3( 0xFF ); }

static void wizchip_write_SPI1( uint8_t wb ) { spi_rw_SPI1( wb ); }
static void wizchip_write_SPI2( uint8_t wb ) { spi_rw_SPI2( wb ); }
static void wizchip_write_SPI3( uint8_t wb ) { spi_rw_SPI3( wb ); }

static void wizchip_spi_readburst_SPI1( uint8_t* pBuf, uint16_t len )
{
  for( ; len > 0; len-- ) *pBuf++ = wizchip_read_SPI1();
}
static void wizchip_spi_readburst_SPI2( uint8_t* pBuf, uint16_t len )
{
  for( ; len > 0; len-- ) *pBuf++ = wizchip_read_SPI2();
}
static void wizchip_spi_readburst_SPI3( uint8_t* pBuf, uint16_t len )
{
  for( ; len > 0; len-- ) *pBuf++ = wizchip_read_SPI3();
}

static void wizchip_spi_writeburst_SPI1( uint8_t* pBuf, uint16_t len )
{
  for( ; len > 0; len-- ) wizchip_write_SPI1( *pBuf++ );
}
static void wizchip_spi_writeburst_SPI2( uint8_t* pBuf, uint16_t len )
{
  for( ; len > 0; len-- ) wizchip_write_SPI2( *pBuf++ );
}
static void wizchip_spi_writeburst_SPI3( uint8_t* pBuf, uint16_t len )
{
  for( ; len > 0; len-- ) wizchip_write_SPI3( *pBuf++ );
}

static void cris_en_SPI1( void ){ waiSema_SPI1(); }
static void cris_en_SPI2( void ){ waiSema_SPI2(); }
static void cris_en_SPI3( void ){ waiSema_SPI3(); }

static void cris_ex_SPI1( void ){ sigSema_SPI1(); }
static void cris_ex_SPI2( void ){ sigSema_SPI2(); }
static void cris_ex_SPI3( void ){ sigSema_SPI3(); }
}  /* extern "C" */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
WIZNET::WIZNET()
{
}

WIZNET::~WIZNET()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void WIZNET::begin( SPI *wire, uint8_t cs, const uint8_t *memsize, dhcp_mode dhcp )
{
  spi = wire;
  csPin = cs;

  /* wizchip initialize */
  wizchip_deselect();
  reg_wizchip_cs_cbfunc( wizchip_select, wizchip_deselect );

  if( spi->whatTypeDef() == SPI1 )
  {
    reg_wizchip_spi_cbfunc( wizchip_read_SPI1, wizchip_write_SPI1 );
    reg_wizchip_spiburst_cbfunc( wizchip_spi_readburst_SPI1, wizchip_spi_writeburst_SPI1 );
    reg_wizchip_cris_cbfunc( cris_en_SPI1, cris_ex_SPI1 );
  }
  else if( spi->whatTypeDef() == SPI2 )
  {
    reg_wizchip_spi_cbfunc( wizchip_read_SPI2, wizchip_write_SPI2 );
    reg_wizchip_spiburst_cbfunc( wizchip_spi_readburst_SPI2, wizchip_spi_writeburst_SPI2 );
    reg_wizchip_cris_cbfunc( cris_en_SPI2, cris_ex_SPI2 );
  }
  else
  {
    reg_wizchip_spi_cbfunc( wizchip_read_SPI3, wizchip_write_SPI3 );
    reg_wizchip_spiburst_cbfunc( wizchip_spi_readburst_SPI3, wizchip_spi_writeburst_SPI3 );
    reg_wizchip_cris_cbfunc( cris_en_SPI3, cris_ex_SPI3 );
  }

  /* The order of the memsize array is sending and receiving. */
  if( ctlwizchip( CW_INIT_WIZCHIP, (void*)memsize ) == -1 )
  {
//    printf( "WIZCHIP Initialized fail.\r\n" );
    while( true ) rot_rdq();
  }
  dly_tsk( 2UL );

  /* wizchip software reset. */
  wizchip_sw_reset();
  /* check wizchip chip version. */
  if( getVERSIONR() != _CHIP_VERSION_ ) while( true ) rot_rdq();

  /* retry count and period settings. */
  wiz_NetTimeout nettime = { 10, 200 * 10 };  /* number of resends, resend interval */
  wizchip_settimeout( &nettime );
//  wizchip_gettimeout( &nettime );

  /* Network information settings. */
  wiz_NetInfo pnetinfo;
  memcpy( pnetinfo.mac, MAC, sizeof(pnetinfo.mac) );
  memcpy( pnetinfo.ip, IPVAL, sizeof(pnetinfo.ip) );
  memcpy( pnetinfo.sn, SUBNET, sizeof(pnetinfo.sn) );
  memcpy( pnetinfo.gw, GATEWAY, sizeof(pnetinfo.gw) );
  memcpy( pnetinfo.dns, DNS, sizeof(pnetinfo.dns) );
  pnetinfo.dhcp = dhcp;  /* 1:NETINFO_STATIC 2:NETINFO_DHCP */
  wizchip_setnetinfo( &pnetinfo );
  /* Check the settings. */
  wizchip_getnetinfo( &pnetinfo );
  if( memcmp( MAC, pnetinfo.mac, sizeof(pnetinfo.mac) ) != 0 ) while( true ) rot_rdq();
  if( memcmp( IPVAL, pnetinfo.ip, sizeof(pnetinfo.ip) ) != 0 ) while( true ) rot_rdq();
  if( memcmp( SUBNET, pnetinfo.sn, sizeof(pnetinfo.sn) ) != 0 ) while( true ) rot_rdq();
  if( memcmp( GATEWAY, pnetinfo.gw, sizeof(pnetinfo.gw) ) != 0 ) while( true ) rot_rdq();
  if( memcmp( DNS, pnetinfo.dns, sizeof(pnetinfo.dns) ) != 0 ) while( true ) rot_rdq();
}

