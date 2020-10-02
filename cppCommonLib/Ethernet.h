/* ----------------------------------------
 stm32f1xx and stm32f4xx ethernet utility header
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
#ifndef  __ETHERNET_H__
#define  __ETHERNET_H__

#include  <derivertive.h>
#include  <HardwareSerial.h>
extern "C"
{
  #include  <stdio.h>
  #include  <string.h>
  #include  <wizchip_conf.h>
  #include  <dhcp.h>
  #include  <mul_tsk.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
enum HARDWARE_STATUS
{
  EthernetNoHardware,
  EthernetW5100,
  EthernetW5200,
  EthernetW5500,
  EthernetW5300,
};

enum LINK_STATUS { LinkUnknown, LinkON, LinkOFF, };

enum MAINTAIN
{
  NOTHING_HAPPNED,
  RENEW_FAILED,
  RENEW_SUCCESS,
  REBIND_FAIL,
  REBIND_SUCCESS,
};

/* ----------------------------------------
    register bit
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class IPAddress
{
private:
  uint8_t ip[4];
public:
  IPAddress( const uint8_t adr[] )
  {
    ip[0] = adr[0];
    ip[1] = adr[1];
    ip[2] = adr[2];
    ip[3] = adr[3];
  }
  IPAddress( uint8_t ip1, uint8_t ip2, uint8_t ip3 ,uint8_t ip4 )
  {
    ip[0] = ip1;
    ip[1] = ip2;
    ip[2] = ip3;
    ip[3] = ip4;
  }
  IPAddress( const char *str )
  {
    int a[4];
    sscanf( str, "%d%*c%d%*c%d%*c%d", &a[0],&a[1],&a[2],&a[3] );
    ip[0] = (uint8_t)a[0];
    ip[1] = (uint8_t)a[1];
    ip[2] = (uint8_t)a[2];
    ip[3] = (uint8_t)a[3];
  }

  void set( const uint8_t *a ){ ip[0] = a[0]; ip[1] = a[1]; ip[2] = a[2]; ip[3] = a[3]; }
  uint8_t *get(){ return ip; }
};

class Ethernet // : public hoge
{
private:
  wiz_NetInfo_t info;
  int  wizInit();
  void discover();

public:
  Ethernet();
  ~Ethernet();

  int  begin( const uint8_t *mac );
  int begin(
    const uint8_t *mac,
    const uint8_t *ip, const uint8_t *dns, const uint8_t *gateway,
    const uint8_t *subnet );
  int begin(
    const uint8_t *mac,
    const uint8_t *ip, const uint8_t *dns, const uint8_t *gateway );
  int begin(
    const uint8_t *mac,
    const uint8_t *ip, const uint8_t *dns );
  int begin( const uint8_t *mac, const uint8_t *ip );

  void init( uint8_t ssPin ) {}
  void setDnsServerIP( const uint8_t *dns )
    { memcpy( info.dns, dns, sizeof(info.dns) ); wizchip_setnetinfo( &info ); }
  void setGatewayIP( const uint8_t *gw )
    { memcpy( info.gw, gw, sizeof(info.gw) ); wizchip_setnetinfo( &info ); }
  void setLocalIP( const uint8_t *ip )
    { memcpy( info.ip, ip, sizeof(info.ip) ); wizchip_setnetinfo( &info ); }
  void setSubnetMask( const uint8_t *subnet )
    { memcpy( info.sn, subnet, sizeof(info.sn) ); wizchip_setnetinfo( &info ); }
  void setMACAddress( const uint8_t *mac )
    { memcpy( info.mac, mac, sizeof(info.mac) ); wizchip_setnetinfo( &info ); }

  void MACAddress( uint8_t *mac ) { memcpy( mac, info.mac, sizeof(info.mac) ); }
  void localIP( uint8_t *ip ) { memcpy( ip, info.ip, sizeof(info.ip) ); }
  void dnsServerIP( uint8_t *dns ) { memcpy( dns, info.dns, sizeof(info.dns) ); }
  void gatewayIP( uint8_t *gw ) { memcpy( gw, info.gw, sizeof(info.gw) ); }
  void subnetMask( uint8_t *sn ) { memcpy( sn, info.sn, sizeof(info.sn) ); }

  int  hardwareStatus()
  {
  #if ( _WIZCHIP_ == W5100 )
    return EthernetW5100;
  #elif ( _WIZCHIP_ == W5200 )
    return EthernetW5200;
  #elif ( _WIZCHIP_ == W5300 )
    return EthernetW5300;
  #elif (_WIZCHIP_ == W5500 )
    return EthernetW5500;
  #else
    return EthernetNoHardware;
  #endif
  }

  int  linkStatus()
  {
    int ret = wizphy_getphylink();
    if( ret == PHY_LINK_ON ) return LinkON;
    else if( ret == PHY_LINK_OFF ) return LinkOFF;
    return LinkUnknown;
  }

  uint8_t maintain() { return NOTHING_HAPPNED; }
  void setRetransmissionCount( uint8_t number ) {}
  void setRetransmissionTimeout( uint16_t duration_ms ) {}
};


#endif  /* __ETHERNET_H__ */

