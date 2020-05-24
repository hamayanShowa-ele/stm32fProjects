/* ----------------------------------------
 stm32f1xx defines for rtc.
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

#ifndef  __STM32F_RTC_H__
#define  __STM32F_RTC_H__

#include  <stm32f10x.h>
#include  <time.h>
#include  <derivertive.h>
#include  <peripheral.h>
#include  <system.h>

#ifdef  _FATFSUTIL_H_
//#include  <rtc_stm32f1.h>
#include  <rtc.h>
#endif  /* _FATFSUTIL_H_ */

/*************************************************************************/
/* 定義                                                          */
/*************************************************************************/
#define  YEAR_OF_SEC       (365UL * 24 * 3600)
#define  LEAP_YEAR_OF_SEC  (366UL * 24 * 3600)
#define  DAY_OF_SEC        (24UL * 3600)
#define  DAY28             (28 * 24 * 3600UL)
#define  DAY29             (29 * 24 * 3600UL)
#define  DAY30             (30 * 24 * 3600UL)
#define  DAY31             (31 * 24 * 3600UL)

//#ifndef __STM32F10x_H
//typedef  unsigned char   bool;
//enum BOOL { FALSE = 0, TRUE = ~FALSE };
//#endif

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* IOレジスタ定義                                                        */
/*************************************************************************/

/*************************************************************************/
/* レジスタ bit定義                                                      */
/*************************************************************************/
/*CRH*/
#define  SECIE_Bit      0x00000001
#define  ALRIE_Bit      0x00000002
#define  OWIE_Bit       0x00000004

/*CRL*/
#define  SECF_Bit       SECIE_Bit
#define  ALRF_Bit       ALRIE_Bit
#define  OWF_Bit        OWIE_Bit
#define  RSF_Bit        0x00000008
#define  CNF_Bit        0x00000010
#define  RTOFF_Bit      0x00000020

/*************************************************************************/
/* ビットフィールド定義                                                  */
/*************************************************************************/
/*CRH*/
typedef union
{
  unsigned long DWORD;
  struct
  {
    unsigned long SECIE : 1;  /*bit0*/
    unsigned long ALRIE : 1;  /*bit1*/
    unsigned long OWIE : 1;  /*bit2*/
    unsigned long rsv1 : 29;  /*bit31~3*/
  } BIT;
} RTC_CRH_REG;

/*CRL*/
typedef union
{
  unsigned long DWORD;
  struct
  {
    unsigned long SECF : 1;  /*bit0*/
    unsigned long ALRF : 1;  /*bit1*/
    unsigned long OWF : 1;  /*bit2*/
    unsigned long RSF : 1;  /*bit3*/
    unsigned long CNF : 1;  /*bit4*/
    unsigned long RTOFF : 1;  /*bit5*/
    unsigned long rsv1 : 26;  /*bit31~6*/
  } BIT;
} RTC_CRL_REG;

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
enum DATE_AND_TIME { YEAR = 0, MONTH, DAY, HOUR, MINUTE, SECOND };

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
int RTC_Init( void );
void RTC_Alarm_Init( void );
void RTC_Int( void );
void RTC_Alarm_Int( void );
void Date_Time_Set( const signed char str[] );
char *DateTime2Strings( char *dest, const signed char str[] );

void unixTime_Set( time_t unixTime );


#endif   /* __STM32F_RTC_H__ */
