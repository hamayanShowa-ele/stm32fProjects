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

/* ----------------------------------------
    defines.
---------------------------------------- */

/* ----------------------------------------
    global variables.
---------------------------------------- */
extern volatile unsigned long sumTotalTime;
extern volatile time_t unixTime;

/* ----------------------------------------
    prototypes.
---------------------------------------- */
static int RTC_LSE_Clock_Start( void );

/* ----------------------------------------
    start the RTC LSE clock.
---------------------------------------- */
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

  /* Start supplying the clock to the RTC APB1. */
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd( ENABLE );
  dly1ms( 1000UL );

  temp = BKP_ReadBackupRegister( BKP_DR1 );

  /* When A5A5 is not written at the beginning of the backup register, it is reinitialized. */
  if( temp != 0xa5a5 )
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
//    unixTime_Set( unixTime );
#endif

    /* Write A5A5. */
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

  (void)BKP_WUP_Type_Status;
  return reStartRTC;
}

/* ----------------------------------------
    initialize RTC.
---------------------------------------- */
int RTC_Init( void )
{
  /* Start with the LSE clock. */
  int ret = RTC_LSE_Clock_Start();
  /* interrupt enable */
  if( ret >= 0 ) nvicInit( RTC_IRQn, BASE_PRIORITY, BASE_SUB_PRIORITY );

  return ret;
}

#if 0
/* ----------------------------------------
    initialize RTC Alarm.
---------------------------------------- */
void RTC_Alarm_Init( void )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_BDCR_REG *BDCR = (RCC_BDCR_REG *)&RCC_BDCR;

  /* Select the LSE as the clock for the RTC. */
  BDCR->BIT.RTCSEL = 1;

  /* Start supplying the clock to RTC APB1. */
  RTC_LSE_Clock_Start();

  /* interrupt enable */
  nvicInit( RTCAlarm_IRQChannel, BASE_PRIORITY, BASE_SUB_PRIORITY );
  NVIC_Init( &NVIC_InitStructure );
}
#endif

#if 0
/* ----------------------------------------
    get fattime.
    The date_time is in the format of yy/mm/dd hh:mm:ss.
---------------------------------------- */
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
  localtime_r( &unixTime, &localTime );  /* Convert unixTime to localTime. */
  /* unix time is based on 1900, however, fat time is based on 1980. */
  res = (localTime.tm_year + (1900 - 1980)) << 25;
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
bool rtc_gettime( RTC_t *rtc )  /* Get time */
{
  struct tm localTime;

  localtime_r( &unixTime, &localTime );  /* Convert unixTime to localTime. */
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

  localtime_r( &unixTime, &localTime );  /* Convert unixTime to localTime. */
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

/* ----------------------------------------
    Set the Unix time to the RTC counter.
---------------------------------------- */
void unixTime_Set( time_t unixTime )
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter( unixTime );
  /* Wait until last write operation on RTC registers has finished */
  //  RTC_WaitForLastTask();
}

/* ----------------------------------------
    RTC initialize
---------------------------------------- */
void RTC_IRQHandler( void )
{
  RTC->CRL &= ~RTC_CRL_SECF;  /* Clear status. */
#if 1
  unixTime = RTC_GetCounter();  /*unix time*/
#else
  unixTime++;  /*unix time*/
#endif
}

/* ----------------------------------------
    RTC alarm initialize
---------------------------------------- */
void RTCAlarm_IRQHandler( void )
{
}
