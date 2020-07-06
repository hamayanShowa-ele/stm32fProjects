/*********************************************************************************/
/* 周辺IOの初期化                                                                */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
#include  <string.h>
#include  "stm32f4xx.h"
#include  "derivertive.h"
#include  "peripheral.h"
//#include  "mul_tsk.h"
#include  "usart.h"

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
#define  wait6ns()       __NOP()  /*168MHz時は1cpu clock時間は約6ns*/
#define  LOOP_DLY_100US  1676

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
//extern unsigned long systim;

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* ソフトウエアwait                                                      */
/*************************************************************************/
void Dly100us( int delay )
{
  volatile int i;

  if( delay == 0 ) return;

  while( delay-- )
    for( i = LOOP_DLY_100US; i; i--);
}

/*************************************************************************/
/* ソフトウエアwait                                                      */
/*************************************************************************/
void Dly50us( int delay )
{
  volatile int i;

  if( delay == 0 ) return;

  while( delay-- )
    for( i = (LOOP_DLY_100US / 2); i; i--);
}

/*************************************************************************/
/* ソフトウエアwait                                                      */
/*************************************************************************/
void Dly10us( int delay )
{
  volatile int i;

  if( delay == 0 ) return;

  while( delay-- )
    for( i = (LOOP_DLY_100US / 10); i; i--);
}

/*************************************************************************/
/* ソフトウエアwait                                                      */
/*************************************************************************/
void Dly1us( int delay )
{
  volatile int i;

  if( delay == 0 ) return;

  while( delay-- )
    for( i = 12; i; i-- );
}

#if 0
/*************************************************************************/
/* ソフトウエアms wait                                                   */
/*************************************************************************/
void WaitMs( int delay )
{
//  unsigned long cmptim = systim + delay;
//  while( cmptim >= systim ) rot_rdq();
}

/*************************************************************************/
/* ソフトウエアus wait                                                   */
/*************************************************************************/
void WaitUs( unsigned long delay )
{
  unsigned long compTim,load;
  unsigned long oldTim = SysTick->VAL & 0x00ffffff;  //Systickカウンターの値を取得
  delay *= 72UL;  //遅延時間をカウントに換算
  compTim = oldTim - delay;  //カウンターはダウンカウントなので、引き算となる
  load = SysTick->LOAD + 1;  //リロード値で丸め
  compTim %= load;  //リロード値で丸め
  if(compTim > oldTim)
  {
    while(compTim < ((SysTick->VAL & 0x00ffffff) + load)) rot_rdq();
  }
  else
  {
    while(compTim < (SysTick->VAL & 0x00ffffff)) rot_rdq();
  }
}
#endif

/*************************************************************************/
/* GPIO端子初期化                                                        */
/*************************************************************************/
void GPIO_Terminal_Init( GPIO_TypeDef* GPIOx, int Mode, u16 Pins )
{
  GPIO_InitTypeDef GPIO_InitStructure;

  switch (Mode)
  {
    case OUT2M_PP :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      break;
    case OUT25M_PP :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
      break;
    case OUT50M_PP :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      break;
    case OUT100M_PP :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      break;
    case OUT2M_OD :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      break;
    case OUT25M_OD :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
      break;
    case OUT50M_OD :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      break;
    case OUT100M_OD :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      break;
    case IN_FLOAT :
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      break;
    case IN_UP :
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      break;
    case IN_DOWN :
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
      break;
    case AF_PP :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      break;
    case AF_OD :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      break;
    case AN_IN :
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      break;
  }

  GPIO_InitStructure.GPIO_Pin = Pins;
  GPIO_Init( GPIOx, &GPIO_InitStructure );
}

/*************************************************************************/
/* SCIの起動                                                             */
/*************************************************************************/
/*************************************************************************/
/* UART関連大域変数宣言                                                  */
/*************************************************************************/
//USART_InitTypeDef USART_InitStructure;
#if (SCI_1_SND_BUFFER_SIZE > 0 || SCI_1_RCV_BUFFER_SIZE > 0)
SCI_BUFFER snd1,rcv1;
SCI_ERROR_COUNT err1;
char SndBuf1[ SCI_1_SND_BUFFER_SIZE ],RcvBuf1[ SCI_1_RCV_BUFFER_SIZE ];
#endif

#if (SCI_2_SND_BUFFER_SIZE > 0 || SCI_2_RCV_BUFFER_SIZE > 0)
SCI_BUFFER snd2,rcv2;
SCI_ERROR_COUNT err2;
char SndBuf2[ SCI_2_SND_BUFFER_SIZE ],RcvBuf2[ SCI_2_RCV_BUFFER_SIZE ];
#endif

#if (SCI_3_SND_BUFFER_SIZE > 0 || SCI_3_RCV_BUFFER_SIZE > 0)
SCI_BUFFER snd3,rcv3;
SCI_ERROR_COUNT err3;
char SndBuf3[ SCI_3_SND_BUFFER_SIZE ],RcvBuf3[ SCI_3_RCV_BUFFER_SIZE ];
#endif

#if (SCI_4_SND_BUFFER_SIZE > 0 || SCI_4_RCV_BUFFER_SIZE > 0)
SCI_BUFFER snd4,rcv4;
SCI_ERROR_COUNT err4;
char SndBuf4[ SCI_4_SND_BUFFER_SIZE ],RcvBuf4[ SCI_4_RCV_BUFFER_SIZE ];
#endif

#if (SCI_5_SND_BUFFER_SIZE > 0 || SCI_5_RCV_BUFFER_SIZE > 0)
SCI_BUFFER snd5,rcv5;
SCI_ERROR_COUNT err5;
char SndBuf5[ SCI_5_SND_BUFFER_SIZE ],RcvBuf5[ SCI_5_RCV_BUFFER_SIZE ];
#endif

#if (SCI_6_SND_BUFFER_SIZE > 0 || SCI_6_RCV_BUFFER_SIZE > 0)
SCI_BUFFER snd6,rcv6;
SCI_ERROR_COUNT err6;
char SndBuf6[ SCI_6_SND_BUFFER_SIZE ],RcvBuf6[ SCI_6_RCV_BUFFER_SIZE ];
#endif

void beginSCI( int ch, unsigned long brr )
{
  #if (SCI_1_SND_BUFFER_SIZE > 0 || SCI_1_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_1 )
  {
    /*USART1 バッファの確保と初期化*/
    snd1.buf = SndBuf1;
    snd1.wptr = snd1.rptr = 0;
    snd1.size = sizeof(SndBuf1);
    rcv1.buf = RcvBuf1;
    rcv1.wptr = rcv1.rptr = 0;
    rcv1.size = sizeof(RcvBuf1);
    (void)SCI_Init( SCI_1, &rcv1, &snd1, &err1, brr );
  }
  #endif

  #if (SCI_2_SND_BUFFER_SIZE > 0 || SCI_2_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_2 )
  {
    /*USART2 バッファの確保と初期化*/
    snd2.buf = SndBuf2;
    snd2.wptr = snd2.rptr = 0;
    snd2.size = sizeof(SndBuf2);
    rcv2.buf = RcvBuf2;
    rcv2.wptr = rcv2.rptr = 0;
    rcv2.size = sizeof(RcvBuf2);
    (void)SCI_Init( SCI_2, &rcv2, &snd2, &err2, brr );
  }
  #endif

  #if (SCI_3_SND_BUFFER_SIZE > 0 || SCI_3_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_3 )
  {
    /*USART3 バッファの確保と初期化*/
    snd3.buf = SndBuf3;
    snd3.wptr = snd3.rptr = 0;
    snd3.size = sizeof(SndBuf3);
    rcv3.buf = RcvBuf3;
    rcv3.wptr = rcv3.rptr = 0;
    rcv3.size = sizeof(RcvBuf3);
    (void)SCI_Init( SCI_3, &rcv3, &snd3, &err3, brr );
  }
  #endif

  #if (SCI_4_SND_BUFFER_SIZE > 0 || SCI_4_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_4 )
  {
    /*UART4 バッファの確保と初期化*/
    snd4.buf = SndBuf4;
    snd4.wptr = snd4.rptr = 0;
    snd4.size = sizeof(SndBuf4);
    rcv4.buf = RcvBuf4;
    rcv4.wptr = rcv4.rptr = 0;
    rcv4.size = sizeof(RcvBuf4);
    (void)SCI_Init( SCI_4, &rcv4, &snd4, &err4, brr );
  }
  #endif

  #if (SCI_5_SND_BUFFER_SIZE > 0 || SCI_5_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_5 )
  {
    /*UART5 バッファの確保と初期化*/
    snd5.buf = SndBuf5;
    snd5.wptr = snd5.rptr = 0;
    snd5.size = sizeof(SndBuf5);
    rcv5.buf = RcvBuf5;
    rcv5.wptr = rcv5.rptr = 0;
    rcv5.size = sizeof(RcvBuf5);
    (void)SCI_Init( SCI_5, &rcv5, &snd5, &err5, brr );
  }
  #endif

  #if (SCI_6_SND_BUFFER_SIZE > 0 || SCI_6_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_6 )
  {
    /*USART6 バッファの確保と初期化*/
    snd6.buf = SndBuf6;
    snd6.wptr = snd6.rptr = 0;
    snd6.size = sizeof(SndBuf6);
    rcv6.buf = RcvBuf6;
    rcv6.wptr = rcv6.rptr = 0;
    rcv6.size = sizeof(RcvBuf6);
    (void)SCI_Init( SCI_6, &rcv6, &snd6, &err6, brr );
  }
  #endif
}

void terminateSCI( int ch )
{
#if (SCI_1_SND_BUFFER_SIZE > 0 || SCI_1_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_1 )
  {
    /*USART1*/
    USART_DeInit(USART1);
  }
#endif

#if (SCI_2_SND_BUFFER_SIZE > 0 || SCI_2_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_2 )
  {
    /*USART2*/
    USART_DeInit(USART2);
  }
#endif

#if (SCI_3_SND_BUFFER_SIZE > 0 || SCI_3_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_3 )
  {
    /*USART3*/
    USART_DeInit(USART3);
  }
#endif

#if (SCI_4_SND_BUFFER_SIZE > 0 || SCI_4_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_4 )
  {
    /*UART4*/
    USART_DeInit(UART4);
  }
#endif

#if (SCI_5_SND_BUFFER_SIZE > 0 || SCI_5_RCV_BUFFER_SIZE > 0)
  if( ch == SCI_5 )
  {
    /*UART5*/
    USART_DeInit(UART5);
  }
#endif
}

/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
