/* ----------------------------------------
 stm32f1xx Timer utility header
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
#ifndef  __TIMER_H__
#define  __TIMER_H__

#include  <gpio.h>

extern "C"
{
  #include  <nvic.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
enum CK_INT_TYPE { ADVANCED_CONTROL_TIMER, GENERAL_PURPOSE_TIMER, BASIC_TIMER, };

enum TIMx_CHx { TIMx_CH1, TIMx_CH2, TIMx_CH3, TIMx_CH4, };

enum TIMER_INTERRUPT_NAME
{
  TIM1_INT_UP,
  TIM1_INT_CC1,
  TIM1_INT_CC2,
  TIM1_INT_CC3,
  TIM1_INT_CC4,
  TIM1_INT_COM,
  TIM1_INT_TRIGGER,
  TIM1_INT_BREAK,

  TIM2_INT_UP,
  TIM2_INT_CC1,
  TIM2_INT_CC2,
  TIM2_INT_CC3,
  TIM2_INT_CC4,
  TIM2_INT_COM,
  TIM2_INT_TRIGGER,

  TIM3_INT_UP,
  TIM3_INT_CC1,
  TIM3_INT_CC2,
  TIM3_INT_CC3,
  TIM3_INT_CC4,
  TIM3_INT_COM,
  TIM3_INT_TRIGGER,

  TIM4_INT_UP,
  TIM4_INT_CC1,
  TIM4_INT_CC2,
  TIM4_INT_CC3,
  TIM4_INT_CC4,
  TIM4_INT_COM,
  TIM4_INT_TRIGGER,

  TIM5_INT_UP,
  TIM5_INT_CC1,
  TIM5_INT_CC2,
  TIM5_INT_CC3,
  TIM5_INT_CC4,
  TIM5_INT_COM,
  TIM5_INT_TRIGGER,

  TIM6_INT_UP,

  TIM7_INT_UP,

  TIM8_INT_UP,
  TIM8_INT_CC1,
  TIM8_INT_CC2,
  TIM8_INT_CC3,
  TIM8_INT_CC4,
  TIM8_INT_COM,
  TIM8_INT_TRIGGER,
  TIM8_INT_BREAK,

  TIM_INT_END,  /* Always put it at the end of the array. */
};

/* timers cascade connection. */
#define  SRC_TIM5_DST_TIM1  TIM_TS_ITR0
#define  SRC_TIM2_DST_TIM1  TIM_TS_ITR1
#define  SRC_TIM3_DST_TIM1  TIM_TS_ITR2
#define  SRC_TIM4_DST_TIM1  TIM_TS_ITR3

#define  SRC_TIM1_DST_TIM8  TIM_TS_ITR0
#define  SRC_TIM2_DST_TIM8  TIM_TS_ITR1
#define  SRC_TIM4_DST_TIM8  TIM_TS_ITR2
#define  SRC_TIM5_DST_TIM8  TIM_TS_ITR3

#define  SRC_TIM1_DST_TIM2  TIM_TS_ITR0
#define  SRC_TIM8_DST_TIM2  TIM_TS_ITR1
#define  SRC_TIM3_DST_TIM2  TIM_TS_ITR2
#define  SRC_TIM4_DST_TIM2  TIM_TS_ITR3

#define  SRC_TIM1_DST_TIM3  TIM_TS_ITR0
#define  SRC_TIM2_DST_TIM3  TIM_TS_ITR1
#define  SRC_TIM5_DST_TIM3  TIM_TS_ITR2
#define  SRC_TIM4_DST_TIM3  TIM_TS_ITR3

#define  SRC_TIM1_DST_TIM4  TIM_TS_ITR0
#define  SRC_TIM2_DST_TIM4  TIM_TS_ITR1
#define  SRC_TIM3_DST_TIM4  TIM_TS_ITR2
#define  SRC_TIM8_DST_TIM4  TIM_TS_ITR3

#define  SRC_TIM2_DST_TIM5  TIM_TS_ITR0
#define  SRC_TIM3_DST_TIM5  TIM_TS_ITR1
#define  SRC_TIM4_DST_TIM5  TIM_TS_ITR2
#define  SRC_TIM8_DST_TIM5  TIM_TS_ITR3

#define  SRC_TIM2_DST_TIM9   TIM_TS_ITR0
#define  SRC_TIM3_DST_TIM9   TIM_TS_ITR1
#define  SRC_TIM10_DST_TIM9  TIM_TS_ITR2
#define  SRC_TIM11_DST_TIM9  TIM_TS_ITR3

#define  SRC_TIM4_DST_TIM12  TIM_TS_ITR0
#define  SRC_TIM5_DST_TIM12  TIM_TS_ITR1
#define  SRC_TIM13_DST_TIM12 TIM_TS_ITR2
#define  SRC_TIM14_DST_TIM12 TIM_TS_ITR3


/* ----------------------------------------
    prototypes
---------------------------------------- */


/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class STM32F_TIMER : public GPIO
{
private:
  TIM_TypeDef *TIMx;
  uint32_t rcc;
  int timerType;

  void config( uint16_t prescaler, uint16_t period );
  int  prePeri( uint32_t freq, uint32_t *prescaler, uint32_t *period );

  uint8_t updateInterruptChannel();  /* timer update interrupt */
  uint8_t ccInterruptChannel();  /* timer compare capture interrupt */

  int  OC( int ch, uint16_t ocMode, int pin, uint16_t pulse, uint16_t pol, uint16_t rst );

public:
  STM32F_TIMER();
  STM32F_TIMER( TIM_TypeDef *tim );
  ~STM32F_TIMER();
  int  begin( TIM_TypeDef *tim );
  void end();

  void start();
  void stop();
  int  frequency( uint32_t freq );
  volatile uint16_t getCounter();
  void setAutoReload( uint16_t reload );
  volatile uint16_t getAutoReload();
  void master();
  void slave( uint16_t parent, uint16_t prescaler, uint16_t period );

  int  toggle( int ch, int pin = 0 );
  int  pwm1( int ch, int pin, uint16_t pulse );
  int  adcTrigger( int ch, int pin, uint16_t pulse );
  void setPulse( int ch, uint16_t pulse );
  uint16_t getPulse( int ch );

  void encoderEnable();
  uint16_t encoderRead();

  void startInterrupt( uint8_t pri = BASE_PRIORITY, uint8_t sub = BASE_SUB_PRIORITY );
  void startInterrupt( int ch, uint8_t pri = BASE_PRIORITY, uint8_t sub = BASE_SUB_PRIORITY );

//  void callBack( int name, void *cb );
  void callBack( int name, void(*cb)(void) );
  void rejectCallBack( int name );

  void trigger( uint16_t trig );
};


#endif  /* __TIMER_H__ */

