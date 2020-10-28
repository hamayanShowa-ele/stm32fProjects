/* ----------------------------------------
  stm32f1xx Timer utilities
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
#include  <Timer.h>

/* ----------------------------------------
    prototypes 
---------------------------------------- */
extern "C"
{
  static void defaultHandler( void ) { while(1) ; }
}

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern uint32_t SystemCoreClock;  /*!< System Clock Frequency (Core Clock) */

static void (*cbList[ TIM_INT_END ])( void );


/* ----------------------------------------
    constructor destructor
---------------------------------------- */
STM32F_TIMER::STM32F_TIMER()
{
}

STM32F_TIMER::STM32F_TIMER( TIM_TypeDef *tim )
{
  begin( tim );
}

STM32F_TIMER::~STM32F_TIMER()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
int STM32F_TIMER::begin( TIM_TypeDef *tim )
{
  TIMx = tim;
  /* initialize call back routine to default. */
  for( int i = 0; i < TIM_INT_END; i++ )
  {
    if( cbList[i] == nullptr ) cbList[i] = defaultHandler;
  }

  /* timer clock enable. */
  if( TIMx == TIM1 || TIMx == TIM8 || TIMx == TIM9 ||
      TIMx == TIM10 || TIMx == TIM11 || TIMx == TIM15 ||
      TIMx == TIM16 || TIMx == TIM17 )
  {
    /* Enable apb2 peripheral clock */
    if( TIMx == TIM1 ) rcc = RCC_APB2Periph_TIM1;
    else if( TIMx == TIM8 ) rcc = RCC_APB2Periph_TIM8;
    else if( TIMx == TIM9 ) rcc = RCC_APB2Periph_TIM9;
    else if( TIMx == TIM10 ) rcc = RCC_APB2Periph_TIM10;
    else if( TIMx == TIM11 ) rcc = RCC_APB2Periph_TIM11;
    else if( TIMx == TIM15 ) rcc = RCC_APB2Periph_TIM15;
    else if( TIMx == TIM16 ) rcc = RCC_APB2Periph_TIM16;
    else if( TIMx == TIM17 ) rcc = RCC_APB2Periph_TIM17;
    else{}
    RCC_APB2PeriphClockCmd( rcc, ENABLE );
    RCC_APB2PeriphResetCmd( rcc, DISABLE );
    timerType = ADVANCED_CONTROL_TIMER;
  }
  else if( TIMx == TIM2 || TIMx == TIM3 || TIMx == TIM4 ||
           TIMx == TIM5 ||
           TIMx == TIM12|| TIMx == TIM13 || TIMx == TIM14 )
  {
    /* Enable apb1 peripheral clock */
    if( TIMx == TIM2 ) rcc = RCC_APB1Periph_TIM2;
    else if( TIMx == TIM3 ) rcc = RCC_APB1Periph_TIM3;
    else if( TIMx == TIM4 ) rcc = RCC_APB1Periph_TIM4;
    else if( TIMx == TIM5 ) rcc = RCC_APB1Periph_TIM5;
    else if( TIMx == TIM12 ) rcc = RCC_APB1Periph_TIM12;
    else if( TIMx == TIM13 ) rcc = RCC_APB1Periph_TIM13;
    else if( TIMx == TIM14 ) rcc = RCC_APB1Periph_TIM14;
    else{}
    RCC_APB1PeriphClockCmd( rcc, ENABLE );
    RCC_APB1PeriphResetCmd( rcc, DISABLE );
    timerType = GENERAL_PURPOSE_TIMER;
  }
  else if( TIMx == TIM6 || TIMx == TIM7 )
  {
    /* Enable apb1 peripheral clock */
    if( TIMx == TIM6 ) rcc = RCC_APB1Periph_TIM6;
    else if( TIMx == TIM7 ) rcc = RCC_APB1Periph_TIM7;
    else{}
    RCC_APB1PeriphClockCmd( rcc, ENABLE );
    RCC_APB1PeriphResetCmd( rcc, DISABLE );
    timerType = BASIC_TIMER;
  }
  else return(-1);

  /* TIM disable counter */
  TIM_Cmd( TIMx, DISABLE );

  return 0;
}

void STM32F_TIMER::end()
{
  /* TIM disable counter */
  TIM_Cmd( TIMx, DISABLE );

  /* initialize call back routine to nullptr. */
  /* Use rejectCallBack to unregister the callback routine. */
#if 0
  for( int i = 0; i < TIM_INT_END; i++ )
  {
    cbList[i] = nullptr;
  }
#endif

  /* timer clock disable. */
  if( timerType == ADVANCED_CONTROL_TIMER )
  {
    /* Disable apb2 peripheral clock */
    RCC_APB2PeriphClockCmd( rcc, DISABLE );
    RCC_APB2PeriphResetCmd( rcc, ENABLE );
  }
  else if( timerType == GENERAL_PURPOSE_TIMER || timerType == BASIC_TIMER )
  {
    /* Disable apb1 peripheral clock */
    RCC_APB1PeriphClockCmd( rcc, DISABLE );
    RCC_APB1PeriphResetCmd( rcc, ENABLE );
  }
  else return;
}


/* ----------------------------------------
    timer counter start and stop.
---------------------------------------- */
void STM32F_TIMER::start()
{
  /* TIM enable counter */
  TIM_Cmd( TIMx, ENABLE );

  if( timerType == ADVANCED_CONTROL_TIMER )
  {
    /* TIM Main Output Enable */
    TIM_CtrlPWMOutputs( TIMx, ENABLE );
  }
}

void STM32F_TIMER::stop()
{
  if( timerType == ADVANCED_CONTROL_TIMER )
  {
    /* TIM Main Output Enable */
    TIM_CtrlPWMOutputs( TIMx, DISABLE );
  }
  /* TIM enable counter */
  TIM_Cmd( TIMx, DISABLE );
}

/* ----------------------------------------
    get counter value.
---------------------------------------- */
volatile uint16_t STM32F_TIMER::getCounter()
{
  return TIM_GetCounter( TIMx );
}

/* ----------------------------------------
    set auto reload register.
---------------------------------------- */
void STM32F_TIMER::setAutoReload( uint16_t reload )
{
  TIM_SetAutoreload( TIMx, reload );
}


/* ----------------------------------------
    get auto reload register.
---------------------------------------- */
uint16_t STM32F_TIMER::getAutoReload()
{
  return TIMx->ARR;
}

/* ----------------------------------------
    timer trigger.
---------------------------------------- */
void STM32F_TIMER::trigger( uint16_t trig )
{
  TIM_SelectOutputTrigger( TIMx, trig );
}

/* ----------------------------------------
    configure time base.
---------------------------------------- */
void STM32F_TIMER::config( uint16_t prescaler, uint16_t period )
{
  /* Time base configuration */
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );

  TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;  /* prescaler */
  TIM_TimeBaseStructure.TIM_Period = period - 1;  /* 周期設定 */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  /* クロック分周比=1 内部クロックを使う限り関係しない？ */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit( TIMx, &TIM_TimeBaseStructure );

  TIM_ARRPreloadConfig( TIMx, ENABLE );
}

/* ----------------------------------------
    calculate period.
---------------------------------------- */
int STM32F_TIMER::prePeri( uint32_t freq, uint32_t *prescaler, uint32_t *period )
{
  /* ck_int setup. */
  uint32_t ck_int = SystemCoreClock;
  if( timerType == GENERAL_PURPOSE_TIMER || timerType == BASIC_TIMER )
  {
//    ck_int /= 2;
    ck_int /= 1;
  }

  uint32_t presc;
  for( presc = 1; presc < 65536UL; presc++ )
  {
    if( (ck_int / presc / freq) < 65536UL  ) break;
  }
  if( presc == 65536UL ) return (-1);

  *prescaler = presc;
  *period = ck_int / presc / freq;  /* 周期設定 */
  return 0;
}



/* ----------------------------------------
    time base set for frequency.
    プリスケーラが1なら72MHz÷65536で1099Hzから36MHz程度が限界
---------------------------------------- */
int STM32F_TIMER::frequency( uint32_t freq )
{
  /* Time base configuration */
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );

  /* get prescaler and period */
  uint32_t prescaler,period;
  if( prePeri( freq, &prescaler, &period ) < 0 ) return (-1);
#if 1
  config( (uint16_t)prescaler, (uint16_t)period );
#else
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(prescaler - 1);  /* prescaler */
  /* set period */
//  TIM_TimeBaseStructure.TIM_Period = (uint16_t)((ck_int / freq) - 1);  /* 周期設定 */
  TIM_TimeBaseStructure.TIM_Period = (uint16_t)(period - 1);  /* 周期設定 */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  /* クロック分周比=1 内部クロックを使う限り関係しない？ */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit( TIMx, &TIM_TimeBaseStructure );

  TIM_ARRPreloadConfig( TIMx, ENABLE );
#endif

  return 0;
}

/* ----------------------------------------
    output compare match.
---------------------------------------- */
int STM32F_TIMER::OC( int ch, uint16_t ocMode, int pin, uint16_t pulse, uint16_t pol, uint16_t rst )
{
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_OCStructInit( &TIM_OCInitStructure );

  TIM_OCInitStructure.TIM_OCMode = ocMode;
  /* TIM_OutputState : The conversion of AD does not start with disable. */
  TIM_OCInitStructure.TIM_OutputState = (pin >= PA0 && pin < PORT_END) ? TIM_OutputState_Enable : TIM_OutputState_Disable;

  //  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = pulse;  /*PWM */
  TIM_OCInitStructure.TIM_OCPolarity = pol;  // TIM_OCPolarity_High
//  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = rst;  // TIM_OCIdleState_Reset
//  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  if( ch == TIMx_CH1 )
  {
    TIM_OC1Init( TIMx, &TIM_OCInitStructure );
    TIM_OC1PreloadConfig( TIMx, TIM_OCPreload_Enable );
  }
  else if( ch == TIMx_CH2 )
  {
    TIM_OC2Init( TIMx, &TIM_OCInitStructure );
    TIM_OC2PreloadConfig( TIMx, TIM_OCPreload_Enable );
  }
  else if( ch == TIMx_CH3 )
  {
    TIM_OC3Init( TIMx, &TIM_OCInitStructure );
    TIM_OC3PreloadConfig( TIMx, TIM_OCPreload_Enable );
  }
  else if( ch == TIMx_CH4 )
  {
    TIM_OC4Init( TIMx, &TIM_OCInitStructure );
    TIM_OC4PreloadConfig( TIMx, TIM_OCPreload_Enable );
  }

  return 0;
}


int STM32F_TIMER::toggle( int ch, int pin )
{
  /* gpio initialize (alternate function). */
  if( pin >= PA0 && pin < PORT_END )
  {
    pinMode( pin, ALTERNATE_PP );
  }
  return OC( ch, TIM_OCMode_Toggle, pin, 0, TIM_OCPolarity_High, TIM_OCIdleState_Reset );
}


int STM32F_TIMER::pwm1( int ch, int pin, uint16_t pulse )
{
  /* gpio initialize (alternate function). */
  if( pin >= PA0 && pin < PORT_END )
  {
    pinMode( pin, ALTERNATE_PP );
  }
  return OC( ch, TIM_OCMode_PWM1, pin, pulse, TIM_OCPolarity_High, TIM_OCIdleState_Reset );
}


int STM32F_TIMER::adcTrigger( int ch, int pin, uint16_t pulse )
{
#if 0
  /* TIMx TRGO selection */
  TIM_SelectOutputTrigger( TIMx, TIM_TRGOSource_Update );
  return 0;
#else
  // PORT_END -> PB9 for ARIES ver.2
//  TIM_SelectOutputTrigger( TIMx, TIM_TRGOSource_OC1 );  // Not required.
//  return OC( ch, TIM_OCMode_PWM1, pin, pulse, TIM_OCPolarity_High, TIM_OCIdleState_Reset );  // TIM_OCMode_Toggle,PORT_END
  return OC( ch, TIM_OCMode_PWM1, pin, pulse, TIM_OCPolarity_Low, TIM_OCIdleState_Set );  // TIM_OCMode_Toggle,PORT_END
#endif
}

/* ----------------------------------------
    set pulse, get pulse
---------------------------------------- */
void STM32F_TIMER::setPulse( int ch, uint16_t pulse )
{
  /* Set the Capture Compare Register value */
  if( ch == TIMx_CH1 )
  {
    TIMx->CCR1 = pulse;
  }
  else if( ch == TIMx_CH2 )
  {
    TIMx->CCR2 = pulse;
  }
  else if( ch == TIMx_CH3 )
  {
    TIMx->CCR3 = pulse;
  }
  else if( ch == TIMx_CH4 )
  {
    TIMx->CCR4 = pulse;
  }
}


uint16_t STM32F_TIMER::getPulse( int ch )
{
  uint16_t pulse;
  /* Set the Capture Compare Register value */
  if( ch == TIMx_CH1 )
  {
    pulse = TIMx->CCR1;
  }
  else if( ch == TIMx_CH2 )
  {
    pulse = TIMx->CCR2;
  }
  else if( ch == TIMx_CH3 )
  {
    pulse = TIMx->CCR3;
  }
  else if( ch == TIMx_CH4 )
  {
    pulse = TIMx->CCR4;
  }
  return pulse;
}


/* ----------------------------------------
    configure the encoder
    and read the encoder counter.
---------------------------------------- */
void STM32F_TIMER::encoderEnable()
{
  TIM_SetCounter( TIMx, 0 );
  TIM_SetAutoreload( TIMx, 0xFFFF );
  TIM_EncoderInterfaceConfig
  ( TIMx,
    TIM_EncoderMode_TI12,
    TIM_ICPolarity_Rising,
    TIM_ICPolarity_Rising );
  /* set fDTS (1/tDTS). */
  volatile uint16_t tempUS = TIMx->CR1;
  tempUS &= ~0x0300;
  tempUS |= 0x0200;  // 0b0000 00 10 0000 0000
  TIMx->CR1 = tempUS;
  /* set digital filter. */
  tempUS = TIMx->CCMR1;
  tempUS &= ~0xFCFC;
  tempUS |= 0xF0F0;  // 0x3030
  tempUS |= 0x0000;  // 0b0000 00 00 0000 00 00
  TIMx->CCMR1 = tempUS;

  TIM_Cmd( TIMx, ENABLE );
}

uint16_t STM32F_TIMER::encoderRead()
{
  return TIM_GetCounter( TIMx );
}


/* ----------------------------------------
    set interrupt handler
---------------------------------------- */
uint8_t STM32F_TIMER::updateInterruptChannel()
{
  if( TIMx == TIM1 ) return TIM1_UP_IRQn;
  else if( TIMx == TIM8 ) return TIM8_UP_IRQn;
  else if( TIMx == TIM2 ) return TIM2_IRQn;
  else if( TIMx == TIM3 ) return TIM3_IRQn;
  else if( TIMx == TIM4 ) return TIM4_IRQn;
  else if( TIMx == TIM5 ) return TIM5_IRQn;
  else if( TIMx == TIM6 ) return TIM6_IRQn;
  else if( TIMx == TIM7 ) return TIM7_IRQn;
//  else if( TIMx == TIM9 ) return TIM9_UP_IRQn;
//  else if( TIMx == TIM10 ) return TIM10_UP_IRQn;
//  else if( TIMx == TIM11 ) return TIM11_UP_IRQn;
//  else if( TIMx == TIM15 ) return TIM15_UP_IRQn;
//  else if( TIMx == TIM16 ) return TIM16_UP_IRQn;
//  else if( TIMx == TIM17 ) return TIM17_UP_IRQn;
 //  else if( TIMx == TIM12 ) return TIM12_UP_IRQn;
 //  else if( TIMx == TIM13 ) return TIM13_UP_IRQn;
 //  else if( TIMx == TIM14 ) return TIM14_UP_IRQn;

  return 255;
}

uint8_t STM32F_TIMER::ccInterruptChannel()
{
  if( TIMx == TIM1 ) return TIM1_CC_IRQn;
  else if( TIMx == TIM8 ) return TIM8_CC_IRQn;
  else if( TIMx == TIM2 ) return TIM2_IRQn;
  else if( TIMx == TIM3 ) return TIM3_IRQn;
  else if( TIMx == TIM4 ) return TIM4_IRQn;
  else if( TIMx == TIM5 ) return TIM5_IRQn;

  return 255;
}

void STM32F_TIMER::startInterrupt( uint8_t pri, uint8_t sub )
{
  /* timerX update interrupt initialize. */
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = updateInterruptChannel();
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pri;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_Init( &NVIC_InitStructure );

  TIM_ITConfig( TIMx, TIM_IT_Update, ENABLE );
}

void STM32F_TIMER::startInterrupt( int ch, uint8_t pri, uint8_t sub )
{
  uint16_t it_cc;
  if( ch == TIMx_CH1 )
  {
    it_cc = TIM_IT_CC1;
  }
  else if( ch == TIMx_CH2 )
  {
    it_cc = TIM_IT_CC2;
  }
  else if( ch == TIMx_CH3 )
  {
    it_cc = TIM_IT_CC3;
  }
  else if( ch == TIMx_CH4 )
  {
    it_cc = TIM_IT_CC4;
  }

  /* timerX channelX compare or capture interrupt initialize. */
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = ccInterruptChannel();
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pri;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  TIM_ITConfig( TIMx, it_cc, ENABLE );
}

/* ----------------------------------------
    register call back routine
    and unregister call back routine.
---------------------------------------- */
#if 0
void STM32F_TIMER::callBack( int name, void *cb )
{
  cbList[ name ] = (void(*)())cb;
  startInterrupt();
}
#endif
void STM32F_TIMER::callBack( int name, void(*cb)(void) )
{
  cbList[ name ] = cb;
//  startInterrupt();
}

void STM32F_TIMER::rejectCallBack( int name )
{
  cbList[ name ] = nullptr;
}

/* ----------------------------------------
    timer master cascade connection
    and timer slave cascade connection.
---------------------------------------- */
void STM32F_TIMER::master()
{
  TIM_SelectOutputTrigger( TIMx, TIM_TRGOSource_Update );
}

void STM32F_TIMER::slave( uint16_t parent, uint16_t prescaler, uint16_t period )
{
  config( prescaler, period );
  TIM_SelectMasterSlaveMode( TIMx, TIM_MasterSlaveMode_Enable );
  TIM_ITRxExternalClockConfig( TIMx, parent );
  TIM_SelectSlaveMode( TIMx, TIM_SlaveMode_External1 );
}




/* ----------------------------------------
    interrupt handlers
---------------------------------------- */
extern "C"
{
void TIM1_BRK_IRQHandler( void ){}
void TIM1_TRG_COM_IRQHandler( void ){}
void TIM1_UP_IRQHandler( void )
{
#if 0
  if( TIM1->SR & TIM_SR_UIF )
  {
    TIM1->SR &= ~TIM_SR_UIF;  /*ステータスフラグのクリア*/
    (*cbList[ TIM1_INT_UP ])();
  }
#else
  if( TIM_GetITStatus(TIM1, TIM_SR_UIF) == SET )
  {
    TIM_ClearITPendingBit(TIM1, TIM_SR_UIF);
    (*cbList[ TIM1_INT_UP ])();
  }
#endif
}
void TIM1_CC_IRQHandler( void )
{
#if 0
  if( TIM1->SR & TIM_SR_CC1IF )
  {
    TIM1->SR &= ~TIM_SR_CC1IF;  /*ステータスフラグのクリア*/
    (*cb_TIM1_CC1_IRQ)();
  }
  if( TIM1->SR & TIM_SR_CC2IF )
  {
    TIM1->SR &= ~TIM_SR_CC2IF;  /*ステータスフラグのクリア*/
    (*cb_TIM1_CC2_IRQ)();
  }
  if( TIM1->SR & TIM_SR_CC3IF )
  {
    TIM1->SR &= ~TIM_SR_CC3IF;  /*ステータスフラグのクリア*/
    (*cb_TIM1_CC3_IRQ)();
  }
  if( TIM1->SR & TIM_SR_CC4IF )
  {
    TIM1->SR &= ~TIM_SR_CC4IF;  /*ステータスフラグのクリア*/
    (*cb_TIM1_CC4_IRQ)();
  }
#else
  if( TIM_GetITStatus(TIM1, TIM_SR_CC1IF) == SET )
  {
    TIM_ClearITPendingBit(TIM1, TIM_SR_CC1IF);
    (*cbList[ TIM1_INT_CC1 ])();
  }
  if( TIM_GetITStatus(TIM1, TIM_SR_CC2IF) == SET )
  {
    TIM_ClearITPendingBit(TIM1, TIM_SR_CC2IF);
    (*cbList[ TIM1_INT_CC2 ])();
  }
  if( TIM_GetITStatus(TIM1, TIM_SR_CC3IF) == SET )
  {
    TIM_ClearITPendingBit(TIM1, TIM_SR_CC3IF);
    (*cbList[ TIM1_INT_CC3 ])();
  }
  if( TIM_GetITStatus(TIM1, TIM_SR_CC4IF) == SET )
  {
    TIM_ClearITPendingBit(TIM1, TIM_SR_CC4IF);
    (*cbList[ TIM1_INT_CC4 ])();
  }
#endif
}

void TIM2_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM2, TIM_IT_Update) == SET )  /* update */
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    (*cbList[ TIM2_INT_UP ])();
  }

  if( TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET )  /* cc1 */
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    (*cbList[ TIM2_INT_CC1 ])();
  }
  if( TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET )  /* cc2 */
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    (*cbList[ TIM2_INT_CC2 ])();
  }
  if( TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET )  /* cc3 */
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    (*cbList[ TIM2_INT_CC3 ])();
  }
  if( TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET )  /* cc4 */
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    (*cbList[ TIM2_INT_CC4 ])();
  }
}
void TIM3_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM3, TIM_IT_Update) == SET )  /* update */
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    (*cbList[ TIM3_INT_UP ])();
  }

  if( TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET )  /* cc1 */
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    (*cbList[ TIM3_INT_CC1 ])();
  }
  if( TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET )  /* cc2 */
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    (*cbList[ TIM3_INT_CC2 ])();
  }
  if( TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET )  /* cc3 */
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
    (*cbList[ TIM3_INT_CC3 ])();
  }
  if( TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET )  /* cc4 */
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
    (*cbList[ TIM3_INT_CC4 ])();
  }
}
void TIM4_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM4, TIM_IT_Update) == SET )  /* update */
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    (*cbList[ TIM4_INT_UP ])();
  }

  if( TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET )  /* cc1 */
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    (*cbList[ TIM4_INT_CC1 ])();
  }
  if( TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET )  /* cc2 */
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    (*cbList[ TIM4_INT_CC2 ])();
  }
  if( TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET )  /* cc3 */
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
    (*cbList[ TIM4_INT_CC3 ])();
  }
  if( TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET )  /* cc4 */
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
    (*cbList[ TIM4_INT_CC4 ])();
  }
}
void TIM5_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM5, TIM_IT_Update) == SET )  /* update */
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    (*cbList[ TIM5_INT_UP ])();
  }

  if( TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET )  /* cc1 */
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
    (*cbList[ TIM5_INT_CC1 ])();
  }
  if( TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET )  /* cc2 */
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
    (*cbList[ TIM5_INT_CC2 ])();
  }
  if( TIM_GetITStatus(TIM5, TIM_IT_CC3) == SET )  /* cc3 */
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
    (*cbList[ TIM5_INT_CC3 ])();
  }
  if( TIM_GetITStatus(TIM5, TIM_IT_CC4) == SET )  /* cc4 */
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
    (*cbList[ TIM5_INT_CC4 ])();
  }
}
void TIM6_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM6, TIM_IT_Update) == SET )  /* update */
  {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    (*cbList[ TIM6_INT_UP ])();
  }
}
void TIM7_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM7, TIM_IT_Update) == SET )  /* update */
  {
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    (*cbList[ TIM7_INT_UP ])();
  }
}

void TIM8_BRK_IRQHandler( void ){}
void TIM8_TRG_COM_IRQHandler( void ){}
void TIM8_UP_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM8, TIM_SR_UIF) == SET )
  {
    TIM_ClearITPendingBit(TIM8, TIM_SR_UIF);
    (*cbList[ TIM8_INT_UP ])();
  }
}

void TIM8_CC_IRQHandler( void )
{
  if( TIM_GetITStatus(TIM8, TIM_SR_CC1IF) == SET )
  {
    TIM_ClearITPendingBit(TIM8, TIM_SR_CC1IF);
    (*cbList[ TIM8_INT_CC1 ])();
  }

  if( TIM_GetITStatus(TIM8, TIM_SR_CC2IF) == SET )
  {
    TIM_ClearITPendingBit(TIM8, TIM_SR_CC2IF);
    (*cbList[ TIM8_INT_CC2 ])();
  }

  if( TIM_GetITStatus(TIM8, TIM_SR_CC3IF) == SET )
  {
    TIM_ClearITPendingBit(TIM8, TIM_SR_CC3IF);
    (*cbList[ TIM8_INT_CC3 ])();
  }

  if( TIM_GetITStatus(TIM8, TIM_SR_CC4IF) == SET )
  {
    TIM_ClearITPendingBit(TIM8, TIM_SR_CC4IF);
    (*cbList[ TIM8_INT_CC4 ])();
  }
}

}
