/* ----------------------------------------
 rtc utility header

---------------------------------------- */
#ifndef  __RTC_H__
#define  __RTC_H__

#include  <stm32f10x.h>
#include  <gpio.h>
#include  <time.h>
extern "C"
{
  #include  <stm32f_rtc.h>
  #include  <peripheral.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */

/* ----------------------------------------
    register bit
---------------------------------------- */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class STM32F_RTC : public GPIO
{
public:
  STM32F_RTC( int in = PC14, int out = PC15 );
  ~STM32F_RTC();
  int  begin( int in = PC14, int out = PC15 );
  void end();
  void unixTime_Set( time_t unixTime );

#if 0
  void alarmInit();
#endif

private:
  int inPin,outPin;
};


#endif  /* __RTC_H__ */

