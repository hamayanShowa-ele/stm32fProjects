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

#include  <stm32f_rtc.h>
#include  <nvic.h>

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
volatile unsigned long sumTotalTime;

#if 0
extern unsigned long RTC_Counter;     /*RTCから取得したカウンターの値*/
extern signed char date_time[];       /*yy/mm/dd hh:mm:ss*/
extern const signed char default_date_time_strings[];  /*yy/mm/dd hh:mm:ss*/
#else
extern time_t unixTime;
#endif

#if 0
static const unsigned long month_of_secs[][12] =
{
  {  /*閏年では無い時*/
    DAY31,  /*1月*/
    DAY31 + DAY28,  /*2月*/
    DAY31 + DAY28 + DAY31,  /*3月*/
    DAY31 + DAY28 + DAY31 + DAY30,  /*4月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31,  /*5月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31 + DAY30,  /*6月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31,  /*7月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31,  /*8月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30,  /*9月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30 + DAY31,  /*10月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30 + DAY31 + DAY30,  /*11月*/
    DAY31 + DAY28 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31,  /*12月*/
  },
  {  /*閏年*/
    DAY31,  /*1月*/
    DAY31 + DAY29,  /*2月*/
    DAY31 + DAY29 + DAY31,  /*3月*/
    DAY31 + DAY29 + DAY31 + DAY30,  /*4月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31,  /*5月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31 + DAY30,  /*6月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31,  /*7月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31,  /*8月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30,  /*9月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30 + DAY31,  /*10月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30 + DAY31 + DAY30,  /*11月*/
    DAY31 + DAY29 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31 + DAY31 + DAY30 + DAY31 + DAY30 + DAY31,  /*12月*/
  },
};
#endif

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
static int RTC_LSE_Clock_Start( void );
#if 0
static void Sec2Time( unsigned long sec );
static void Sec2Date( unsigned long sec );
#endif

/*************************************************************************/
/* RTC LSEクロックスタート                                               */
/*************************************************************************/
static int RTC_LSE_Clock_Start( void )
{
  int limit;
  volatile unsigned short temp;
  FlagStatus BKP_WUP_Type_Status;
  int reStartRTC = 0;

#if 0
  RCC_BDCR_REG *BDCR = (RCC_BDCR_REG *)&RCC_BDCR;
  PWR_CR_REG *PWRCR = (PWR_CR_REG *)PWR_CR;
#endif

  /*RTC APB1へクロックの供給開始*/
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd( ENABLE );
  dly1ms( 1000UL );

  temp = BKP_ReadBackupRegister( BKP_DR1 );

  if( temp != 0xa5a5 )  /*バックアップレジスタの一番最初の奴にA5A5が書かれていない時は、再初期化*/
  {
    reStartRTC = 1;
    /* Enable PWR and BKP clocks */
//    RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
    /* Allow access to BKP Domain */
//    PWR_BackupAccessCmd( ENABLE );
    /* Reset Backup Domain */
    BKP_DeInit();
    /* Enable LSE */
    RCC_LSEConfig( RCC_LSE_ON );
    /* Wait till LSE is ready */
#if 1
    for( limit = 10000000L; limit > 0; limit-- )
    {
      if( RCC_GetFlagStatus( RCC_FLAG_LSERDY ) == SET )
        break;
    }
    if( limit == 0 ) return (-1);
#else
    while( RCC_GetFlagStatus( RCC_FLAG_LSERDY ) == RESET ) ;
#endif

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig( RCC_RTCCLKSource_LSE );
    /* Enable RTC Clock */
    RCC_RTCCLKCmd( ENABLE );
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Enable the RTC Second */
    RTC_ITConfig( RTC_IT_SEC, ENABLE );
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler( 32767 ); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

#if 0
    Date_Time_Set( default_date_time_strings );
#else
    unixTime_Set( unixTime );
#endif

    /*A5A5を書いて置く*/
    BKP_WriteBackupRegister( BKP_DR1, 0xa5a5 );
  }
  else
  {
    /* Enable PWR and BKP clocks */
//    RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
    /* Allow access to BKP Domain */
//    PWR_BackupAccessCmd( ENABLE );

    /* Check if the Power On Reset flag is set */
    BKP_WUP_Type_Status = RCC_GetFlagStatus( RCC_FLAG_PORRST );
    /* Check if the Pin Reset flag is set */
    BKP_WUP_Type_Status = RCC_GetFlagStatus( RCC_FLAG_PINRST );
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Enable the RTC Second */
    RTC_ITConfig( RTC_IT_SEC, ENABLE );
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Wait till LSE is ready */
#if 1
    for( limit = 10000000L; limit > 0; limit-- )
    {
      if( RCC_GetFlagStatus( RCC_FLAG_LSERDY ) == SET )
        break;
    }
    if( limit == 0 ) return (-1);
#endif
  }

  /* Clear reset flags */
  RCC_ClearFlag();

  return reStartRTC;
}

/*************************************************************************/
/* RTC初期化                                                             */
/* クロックの供給と割込みの設定のみ                                      */
/*************************************************************************/
int RTC_Init( void )
{
  /*RTC APB1へクロックの供給開始*/
  int ret = RTC_LSE_Clock_Start();

  /* interrupt enable */
  nvicInit( RTC_IRQn, BASE_PRIORITY, BASE_SUB_PRIORITY );

  return ret;
}

/*************************************************************************/
/* RTCアラーム初期化                                                     */
/* クロックの供給と割込みの設定のみ                                      */
/*************************************************************************/
#if 0
void RTC_Alarm_Init( void )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_BDCR_REG *BDCR = (RCC_BDCR_REG *)&RCC_BDCR;

  /*RTCのクロックとしてLSEを選択*/
  BDCR->BIT.RTCSEL = 1;

  /*RTC APB1へクロックの供給開始*/
  RTC_LSE_Clock_Start();

  /* interrupt enable */
  nvicInit( RTCAlarm_IRQChannel, BASE_PRIORITY, BASE_SUB_PRIORITY );
  NVIC_Init( &NVIC_InitStructure );
}
#endif

#if 0
/*************************************************************************/
/* 時刻換算処理                                                          */
/*************************************************************************/
static void Sec2Time( unsigned long sec )
{
  date_time[ HOUR ] = (sec % (24 * 3600UL)) / 3600UL;
  date_time[ MINUTE ] = (sec % 3600UL) / 60;
  date_time[ SECOND ] = sec % 60;
}
#endif

#if 0
/*************************************************************************/
/* 日付換算処理                                                          */
/*************************************************************************/
static void Sec2Date( unsigned long sec )
{
  int i,leap;
  unsigned long temp,old_temp;

  for( i = 9, temp = old_temp = 0; i < 100; i++ )
  {
    temp += (i % 4) ? YEAR_OF_SEC : LEAP_YEAR_OF_SEC;  /*新しい比較値*/
    if( sec <= temp ) break;  /*年が判ったのでここで脱出*/
    old_temp = temp;  /*ここまでの秒数を覚えておく*/
  }
  date_time[ YEAR ] = i;  /*年が判った*/
  leap = (i % 4) ? 0 : 1;  /*閏年なら1*/
  sec -= old_temp;  /*年の秒数を引いて置く*/
  old_temp = sec;  /* 年変わりの1/1の日がおかしくなるため(by naito) */

  for( i = 0, temp = 0; i < 12; i++ )
  {
    temp = month_of_secs[ leap ][ i ];
    if( sec <= temp ) break;
    old_temp = temp;
  }
  date_time[ MONTH ] = i + 1;  /*月が判った*/
  sec -= old_temp;

  date_time[ DAY ] = (sec / DAY_OF_SEC) + 1;
}
#endif

#if 0
/*************************************************************************/
/* RTC設定                                                               */
/* Date_Time_Set( date_time_strings );                                   */
/* date_time_stringsはyy/mm/dd hh:mm:ssの形式を取る。                    */
/* 2009年10月26日 21時26分0秒ならば{9,10,26,21,26,0}                     */
/*************************************************************************/
void Date_Time_Set( const signed char str[] )
{
  int i,leap;
  unsigned long sec,temp;

  sec = 0;
  if( str[ YEAR ] > 9 )  /*2009年以降を累積させる*/
  {
    for( i = 9, temp = 0; i < str[ YEAR ]; i++ )
    {
      temp += (i % 4) ? YEAR_OF_SEC : LEAP_YEAR_OF_SEC;  /*新しい比較値*/
    }
    sec += temp;  /*累積*/
  }
  leap = (str[ YEAR ] % 4) ? 0 : 1;  /*閏年判定*/

  if( str[ MONTH ] > 1 )  /*1月以降を累積させる*/
  {
    sec += month_of_secs[ leap ][ str[ MONTH ] - 2 ];  /*累積*/
  }

  if( str[ DAY ] > 1 )  /*1日以降を累積させる*/
  {
    for( i = 1, temp = 0; i < str[ DAY ]; i++ )
    {
      temp += DAY_OF_SEC;
    }
    sec += temp;  /*累積*/
  }

  if( str[ HOUR ] >= 1 )  /*1時間以降を累積させる*/
  {
    for( i = 0, temp = 0; i < str[ HOUR ]; i++ )
    {
      temp += 3600UL;
    }
    sec += temp;  /*累積*/
  }

  if( str[ MINUTE ] >= 1 )  /*1分以降を累積させる*/
  {
    for( i = 0, temp = 0; i < str[ MINUTE ]; i++ )
    {
      temp += 60UL;
    }
    sec += temp;  /*累積*/
  }

  sec += str[ SECOND ];  /*累積*/

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter( sec );
  /* Wait until last write operation on RTC registers has finished */
//  RTC_WaitForLastTask();
}
#endif

#if 0
/*************************************************************************/
/* 日付及び時刻の文字列の生成                                            */
/*************************************************************************/
char *DateTime2Strings( char *dest, const signed char str[] )
{
  dest[0]  = '2';
  dest[1]  = '0';
  dest[2]  = (str[ YEAR ] / 10) + '0';
  dest[3]  = (str[ YEAR ] % 10) + '0';
  dest[4]  = '/';
  dest[5]  = (str[ MONTH ] / 10) + '0';
  dest[6]  = (str[ MONTH ] % 10) + '0';
  dest[7]  = '/';
  dest[8]  = (str[ DAY ] / 10) + '0';
  dest[9]  = (str[ DAY ] % 10) + '0';
  dest[10] = ' ';
  dest[11] = (str[ HOUR ] / 10) + '0';
  dest[12] = (str[ HOUR ] % 10) + '0';
  dest[13] = ':';
  dest[14] = (str[ MINUTE ] / 10) + '0';
  dest[15] = (str[ MINUTE ] % 10) + '0';
  dest[16] = ':';
  dest[17] = (str[ SECOND ] / 10) + '0';
  dest[18] = (str[ SECOND ] % 10) + '0';
  dest[19] = '\0';

  return dest;
}
#endif

#if 0
/*************************************************************************/
/* get fattime                                                           */
/* date_timeはyy/mm/dd hh:mm:ssの形式となっている。                      */
/*************************************************************************/
unsigned long get_fattime( void )
{
  struct tm localTime;
  unsigned long res;

#if 0
  res = (date_time[ YEAR ] + (2000 - 1980)) << 25;
  res |= date_time[ MONTH ] << 21;
  res |= date_time[ DAY ] << 16;
  res |= date_time[ HOUR ] << 11;
  res |= date_time[ MINUTE ] << 5;
  res |= date_time[ SECOND ] >> 1;
#else
  localtime_r( &unixTime, &localTime );  /*unixTimeをlocalTimeに変換*/
  res = (localTime.tm_year + (1900 - 1980)) << 25;  /*unix timeは1900年を基準としている。fat timeは1980年を基準*/
  res |= (localTime.tm_mon + 1) << 21;
  res |= (localTime.tm_mday) << 16;
  res |= (localTime.tm_hour) << 11;
  res |= (localTime.tm_min) << 5;
  res |= (localTime.tm_sec) >> 1;
#endif

  return res;
}
#endif

#ifdef  _FATFSUTIL_H_
#if 1
bool rtc_gettime( RTC_t *rtc )					/* Get time */
{
  struct tm localTime;

  localtime_r( &unixTime, &localTime );  /*unixTimeをlocalTimeに変換*/
  rtc->year  = localTime.tm_year + 1900;
  rtc->month = localTime.tm_mon + 1;
  rtc->mday  = localTime.tm_mday;
  rtc->wday  = localTime.tm_wday;
  rtc->hour  = localTime.tm_hour;
  rtc->min   = localTime.tm_min;
  rtc->sec   = localTime.tm_sec;
  rtc->dst   = localTime.tm_isdst;

  return true;
}
#else
int rtc_gettime (RTCTIME* rtc)
{
  struct tm localTime;

  localtime_r( &unixTime, &localTime );  /*unixTimeをlocalTimeに変換*/
  rtc->year  = localTime.tm_year + 1900;
  rtc->month = localTime.tm_mon + 1;
  rtc->mday  = localTime.tm_mday;
  rtc->wday  = localTime.tm_wday;
  rtc->hour  = localTime.tm_hour;
  rtc->min   = localTime.tm_min;
  rtc->sec   = localTime.tm_sec;
//  rtc->dst   = localTime.tm_isdst;

  return 1;
}
#endif
#endif  /*_FATFSUTIL_H_*/

/*************************************************************************/
/* Unix時間をRTCカウンターに設定する                                     */
/*************************************************************************/
void unixTime_Set( time_t unixTime )
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter( unixTime );
  /* Wait until last write operation on RTC registers has finished */
  //  RTC_WaitForLastTask();
}

/*************************************************************************/
/* RTC割り込み                                                           */
/*************************************************************************/
void RTC_IRQHandler( void )
{
  RTC->CRL &= ~RTC_CRL_SECF;  /*ステータスのクリア*/
  sumTotalTime++;  /*起動時からの通算時間*/
#if 1
  unixTime = RTC_GetCounter();  /*unix time*/
#else
  unixTime++;  /*unix time*/
#endif
#if 0
  RTC_Counter = RTC_GetCounter() + 1;
  Sec2Date( RTC_Counter );
  Sec2Time( RTC_Counter );
#endif
}

/*************************************************************************/
/* RTCアラーム割り込み                                                   */
/*************************************************************************/
void RTCAlarm_IRQHandler( void )
{
}
