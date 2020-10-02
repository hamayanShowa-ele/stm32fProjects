/* ----------------------------------------
  stm32f1xx and stm32f4xx ethernet utilities code
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
#include  <Ethernet.h>

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
static const uint8_t sn[] = {255,255,255,0};
static const uint8_t txsize[] = {8,8,8,8,8,8,8,8,};
static const uint8_t rxsize[] = {8,8,8,8,8,8,8,8,};

/* ----------------------------------------
    bus read and write call back functions.
---------------------------------------- */
extern "C"
{
  static uint16_t bus_rb( uint32_t addr ) { return *((uint16_t *)addr); }
  static void bus_wb( uint32_t addr, uint16_t wb ) { *((uint16_t *)addr) = wb; }
}

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
Ethernet::Ethernet()
{
}

Ethernet::~Ethernet()
{
}

/* ----------------------------------------
    wizInit
---------------------------------------- */
int Ethernet::wizInit()
{
  //  void reg_wizchip_bus_cbfunc(iodata_t (*bus_rb)(uint32_t addr), void (*bus_wb)(uint32_t addr, iodata_t wb));
  reg_wizchip_bus_cbfunc( bus_rb, bus_wb );

  int ret = wizchip_init( (uint8_t *)txsize, (uint8_t *)rxsize );
  if( ret < 0 ) return ret;
  if( getIDR() != 0x5300 ) return (-1);  //read the w5300 id.

  memset( &info, 0, sizeof(info) );

  return 0;
}

/* ----------------------------------------
    discover
---------------------------------------- */
extern Serial Serial1;
void my_ip_assign(void)
{
  Serial1.puts( "my_ip_assign\r\n" );
}
void my_ip_conflict(void)
{
  Serial1.puts( "my_ip_conflict\r\n" );
}

void Ethernet::discover()
{
  #define DATA_BUF_SIZE   1024
  uint8_t gDATABUF[ DATA_BUF_SIZE ];
  IPAddress *server = new IPAddress( 192,168,100,1 );
  delete server;

  // if you want different action instead default ip assign, update, conflict.
  // if cbfunc == 0, act as default.
  reg_dhcp_cbfunc( my_ip_assign, my_ip_assign, my_ip_conflict );
//  reg_dhcp_cbfunc( 0, 0, 0 );
#if 1
  /*
   * @brief DHCP client in the main loop
   * @return    The value is as the follow \n
   *            @ref DHCP_FAILED     \n
   *            @ref DHCP_RUNNING    \n
   *            @ref DHCP_IP_ASSIGN  \n
   *            @ref DHCP_IP_CHANGED \n
   *            @ref DHCP_IP_LEASED  \n
   *            @ref DHCP_STOPPED    \n
   *
   * @note This function is always called by you main task.
   */
  DHCP_init( DHCP_SOCKET_NUMBER, gDATABUF );  // DHCP_SOCKET_NUMBER is defined in derivertive.h.
  bool loop = true;
  while( loop )
  {
//	  DHCP_init( DHCP_SOCKET_NUMBER, gDATABUF );  // DHCP_SOCKET_NUMBER is defined in derivertive.h.
    int ret = DHCP_run();
    switch( ret )
    {
      case DHCP_FAILED :
        Serial1.puts( "dhcp failed.\r\n" );
        loop = false;
        break;
      case DHCP_RUNNING :
        Serial1.puts( "dhcp running.\r\n" );
        break;
      case DHCP_IP_ASSIGN :
        Serial1.puts( "dhcp assigned.\r\n" );
        break;
      case DHCP_IP_CHANGED :
        Serial1.puts( "dhcp changed.\r\n" );
        break;
      case DHCP_IP_LEASED :
        Serial1.puts( "dhcp leased.\r\n" );
        break;
      case DHCP_STOPPED :
      default :
        Serial1.puts( "dhcp stopped.\r\n" );
        loop = false;
        break;
    }
    dly_tsk( 1 * 1000UL );
  }
#endif
}


/* ----------------------------------------
    begin and end
---------------------------------------- */
int Ethernet::begin( const uint8_t *mac )
{
  if( wizInit() < 0 ) return (-1);
  memcpy( info.mac, mac, sizeof(info.mac) );
  info.dhcp = NETINFO_DHCP ; // 1 = static , 2 = dhcp
  wizchip_setnetinfo( &info );
  discover();

  return 0;
}


int Ethernet::begin(
  const uint8_t *mac,
  const uint8_t *ip, const uint8_t *dns, const uint8_t *gateway,
  const uint8_t *subnet )
{
  if( wizInit() < 0 ) return (-1);

  memcpy( info.mac, mac, sizeof(info.mac) );
  memcpy( info.ip, ip, sizeof(info.ip) );
  memcpy( info.dns, dns, sizeof(info.dns) );
  memcpy( info.gw, gateway, sizeof(info.gw) );
  memcpy( info.sn, subnet, sizeof(info.sn) );
  info.dhcp = NETINFO_STATIC ; // 1 = static , 2 = dhcp
  wizchip_setnetinfo( &info );

  return 0;
}

int Ethernet::begin(
  const uint8_t *mac,
  const uint8_t *ip, const uint8_t *dns, const uint8_t *gateway )
{
  return begin( mac, ip, dns, gateway, sn );
}

int Ethernet::begin(
  const uint8_t *mac,
  const uint8_t *ip, const uint8_t *dns )
{
  uint8_t gw[4];
  gw[0] = ip[0] & sn[0];
  gw[1] = ip[1] & sn[1];
  gw[2] = ip[2] & sn[2];
  gw[3] = 1;

  return begin( mac, ip, dns, gw, sn );
}

int Ethernet::begin( const uint8_t *mac, const uint8_t *ip )
{
  uint8_t gw[4];
  gw[0] = ip[0] & sn[0];
  gw[1] = ip[1] & sn[1];
  gw[2] = ip[2] & sn[2];
  gw[3] = 1;
  uint8_t ns[4];
  ns[0] = ip[0] & sn[0];
  ns[1] = ip[1] & sn[1];
  ns[2] = ip[2] & sn[2];
  ns[3] = 1;

  return begin( mac, ip, ns, gw, sn );
}
