/*************************************************************************/
/* STM32F407ZET6のRTCを何とかする                                        */
/* カレンダー機能を持たせる。                                            */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
#include  <time.h>
#include  "stm32f4xx.h"
#include  "strutil.h"
#include  "configRTC.h"

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

/*************************************************************************/
/* RTC初期化 (※LSEの32768Hzを使用する)                                  */
/*************************************************************************/
void RTC_Config(void)
{
  RTC_InitTypeDef   RTC_InitStructure;
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_DateTypeDef   RTC_DateStruct;
//  NVIC_InitTypeDef  NVIC_InitStructure;
  uint32_t LSEFreq = 32768;

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to Backup Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Clear Wakeup flag */
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable the LSE OSC */
  //RCC_LSICmd(ENABLE);
  RCC_LSEConfig( RCC_LSE_ON );

  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) ;

  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {
    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);

    /* Check if the StandBy flag is cleared */
    if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
    {
      while(1) ;
    }

    if( RTC_WaitForSynchro() == ERROR )
    {
      while(1) ;
    }

    /* No need to configure the RTC as the RTC config(clock source, enable,
    prescaler,...) are kept after wake-up from STANDBY */
  }
  else
  {
    /* RTC Configuration ******************************************************/
    /* Reset Backup Domain */
    //RCC_BackupResetCmd(ENABLE);  //これを行うとLSEが止まる
    RCC_BackupResetCmd(DISABLE);

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig( RCC_RTCCLKSource_LSE );

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    if( RTC_WaitForSynchro() == ERROR )
    {
      while(1) ;
    }

    /*A5A55A5Aが書かれていれば、バックアップが行われている*/
    volatile uint32_t temp;
    temp = RTC_ReadBackupRegister( RTC_BKP_DR0 );
    if(temp != 0xa5a55a5a)
    {
      RTC_WriteBackupRegister( RTC_BKP_DR0, 0xa5a55a5a );

      RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
      RTC_InitStructure.RTC_AsynchPrediv = 128 - 1;
      RTC_InitStructure.RTC_SynchPrediv =  (LSEFreq/128) - 1;

      if( RTC_Init(&RTC_InitStructure) == ERROR )
      {
        while(1) ;
      }

      /* Set the time to 01h 00mn 00s AM */
      int hour,minute,second;
      compileTime( &hour, &minute, &second );
      RTC_TimeStructure.RTC_H12     = (hour < 12) ? RTC_H12_AM : RTC_H12_PM;
      RTC_TimeStructure.RTC_Hours   = hour;
      RTC_TimeStructure.RTC_Minutes = minute;
      RTC_TimeStructure.RTC_Seconds = second;
      if( RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR )
      {
        while(1) ;
      }

      int year,month,day;
      compileDate( &year, &month, &day );
      RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Friday;
      RTC_DateStruct.RTC_Year = year - 2000;
      RTC_DateStruct.RTC_Month = month;
      RTC_DateStruct.RTC_Date = day;
      if( RTC_SetDate( RTC_Format_BIN, &RTC_DateStruct ) == ERROR )
      {
        while(1) ;
      }
    }

#if 0
    /* Set wakeup interrupt */
    RTC_ITConfig( RTC_IT_WUT, ENABLE );
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
#endif
    /* Clear reset flags */
    RCC_ClearFlag();
  }
}


/*************************************************************************/
/* local timeをRTCに設定する                                             */
/*************************************************************************/
void localTime_Set( const struct tm *localTime )
{
  RTC_TimeTypeDef   RTC_TimeStruct;
  RTC_DateTypeDef   RTC_DateStruct;

  RTC_TimeStruct.RTC_H12     = (localTime->tm_hour < 12) ? RTC_H12_AM : RTC_H12_PM;
  RTC_TimeStruct.RTC_Hours   = localTime->tm_hour;
  RTC_TimeStruct.RTC_Minutes = localTime->tm_min;
  RTC_TimeStruct.RTC_Seconds = localTime->tm_sec;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

  RTC_DateStruct.RTC_WeekDay = localTime->tm_wday;
  RTC_DateStruct.RTC_Year = localTime->tm_year + (1900 - 2000);
  RTC_DateStruct.RTC_Month = localTime->tm_mon + 1;
  RTC_DateStruct.RTC_Date = localTime->tm_mday;
  RTC_SetDate( RTC_Format_BIN, &RTC_DateStruct );

}

/*************************************************************************/
/* local timeをRTCから取得する                             */
/*************************************************************************/
void localTime_Get( struct tm *localTime )
{
  RTC_TimeTypeDef   RTC_TimeStruct;
  RTC_DateTypeDef   RTC_DateStruct;

  RTC_GetTime( RTC_Format_BIN, &RTC_TimeStruct );
  RTC_GetDate( RTC_Format_BIN, &RTC_DateStruct );

  localTime->tm_year = RTC_DateStruct.RTC_Year - (1900 - 2000);
  localTime->tm_mon = RTC_DateStruct.RTC_Month - 1;
  localTime->tm_mday = RTC_DateStruct.RTC_Date;
  localTime->tm_wday = RTC_DateStruct.RTC_WeekDay;

  localTime->tm_hour = RTC_TimeStruct.RTC_Hours;
  localTime->tm_min = RTC_TimeStruct.RTC_Minutes;
  localTime->tm_sec = RTC_TimeStruct.RTC_Seconds;
}

/*************************************************************************/
/* RTC割り込み                                                           */
/*************************************************************************/
#if 0
void RTC_IRQHandler( void )
{
  RTC->CRL &= ~RTC_CRL_SECF;  /*ステータスのクリア*/
  unixTime = RTC_GetCounter();  /*unix time*/
}
#endif

/*************************************************************************/
/* RTCアラーム割り込み                                                   */
/*************************************************************************/
#if 0
void RTCAlarm_IRQHandler( void )
{
}
#endif

/*************************************************************************/
/* RTC wake up割り込み                                                   */
/*************************************************************************/
#if 0
void RTC_WKUP_IRQHandler(void)
{
  FlagStatus flg;
  ITStatus its;

  unixTime++;
  flg = RTC_GetFlagStatus( RTC_FLAG_WUTF );
  its = RTC_GetITStatus( RTC_IT_WUT );
  RTC_ClearFlag( RTC_FLAG_WUTF );
  RTC_ClearITPendingBit( RTC_IT_WUT );
}
#endif

/*************************************************************************/
/* end of file                                                           */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
