/*************************************************************************/
/* STM32F407ZET6のRTCを何とかする                                        */
/* カレンダー機能を持たせる。開始は2009年1月1日とし、カウンター=0が該当  */
/* する。                                                                */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
#include  <time.h>
#include  "stm32f4xx.h"
#include  "priority.h"
#include  "delivertive.h"
#include  "peripheral.h"
#include  "stm32f_rtc.h"
//#include  "rtc.h"
//#include  "integer.h"
//#include  "eeprom.h"

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
#define  YEAR_OF_SEC       (365UL * 24 * 3600)
#define  LEAP_YEAR_OF_SEC  (366UL * 24 * 3600)
#define  DAY_OF_SEC        (24UL * 3600)
#define  DAY28             (28 * 24 * 3600UL)
#define  DAY29             (29 * 24 * 3600UL)
#define  DAY30             (30 * 24 * 3600UL)
#define  DAY31             (31 * 24 * 3600UL)

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
//FlagStatus BKP_WUP_Type_Status;
extern time_t unixTime;

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
static int RTC_LSE_Clock_Start( void );

/*************************************************************************/
/* RTC LSEクロックスタート                                               */
/*************************************************************************/
#if 0
static int RTC_LSE_Clock_Start( void )
{
  int limit;
  volatile unsigned short temp;

  /*RTC APB1へクロックの供給開始*/
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd( ENABLE );
  Dly100us( 1000 * 10 );

  temp = BKP_ReadBackupRegister( BKP_DR1 );

  if( temp != 0xa5a5 )  /*バックアップレジスタの一番最初の奴にA5A5が書かれていない時は、再初期化*/
  {

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

    unixTime_Set( unixTime );

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

  return 0;
}
#endif

/*************************************************************************/
/* RTC初期化                                                             */
/* クロックの供給と割込みの設定のみ                                      */
/*************************************************************************/
#if 0
int RTC_Init( void )
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /*RTC APB1へクロックの供給開始*/
  if( RTC_LSE_Clock_Start() != 0 ) return (-1);

  /*RTCの割り込み設定*/
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  return 0;
}
#endif

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

  /*RTC alarmの割り込み設定*/
  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
}
#endif


#if 0
bool rtc_gettime (RTC_t* rtc)  /* Get time */
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
#endif

/*************************************************************************/
/* Unix時間をRTCカウンターに設定する                                     */
/*************************************************************************/
#if 0
void unixTime_Set( time_t unixTime )
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForSynchro();
  /* Change the current time */
  RTC_SetCounter( unixTime );
  /* Wait until last write operation on RTC registers has finished */
  //  RTC_WaitForLastTask();
}
#endif

/*************************************************************************/
/* RTC割り込み                                                           */
/*************************************************************************/
#if 0
void RTC_IRQHandler( void )
{
  RTC->CRL &= ~RTC_CRL_SECF;  /*ステータスのクリア*/
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
#endif

/*************************************************************************/
/* RTCアラーム割り込み                                                   */
/*************************************************************************/
void RTCAlarm_IRQHandler( void )
{
}

/*************************************************************************/
/* end of file                                                           */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
