/* ----------------------------------------
  rtc utilities code
---------------------------------------- */
#include  "rtc.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern time_t unixTime;
extern time_t sumTotalTime;  /*起動時からの通算時間*/

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
STM32F_RTC::STM32F_RTC( int in, int out )
{
  (void)begin( in, out );
}

STM32F_RTC::~STM32F_RTC()
{
  end();
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
int STM32F_RTC::begin( int in, int out )
{
  inPin = in;
  outPin = out;
  pinMode( inPin, INPUT );
  pinMode( outPin, OUTPUT, GPIO_SPEED_NORMAL );

  return RTC_Init();
}

void STM32F_RTC::end()
{
  pinMode( inPin, INPUT_PULLUP );
  pinMode( outPin, INPUT_PULLUP );
}


/* ----------------------------------------
   Unix時間をRTCカウンターに設定する
---------------------------------------- */
void STM32F_RTC::unixTime_Set( time_t unixTime )
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter( unixTime );
  /* Wait until last write operation on RTC registers has finished */
  //  RTC_WaitForLastTask();
}

#if 0
/* ----------------------------------------
    alarm initialize
---------------------------------------- */
void STM32F_RTC::alarmInit()
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


extern "C"
{
}  /* extern "C" */
