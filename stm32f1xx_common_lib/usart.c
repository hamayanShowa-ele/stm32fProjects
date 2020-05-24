/* ----------------------------------------
 stm32f1xx defines for usart and uart.
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
#include  <usart.h>

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
static SCI_BUFFER *sci_snd[ 5 ];
static SCI_BUFFER *sci_rcv[ 5 ];
static SCI_ERROR_COUNT *sci_err[ 5 ];
extern SYSTIM systim;

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
static USART_TypeDef *Select_SCI( int ch );
//void IWDG_Clr( void );

/*************************************************************************/
/* SCI(USART)初期化                                                      */
/*************************************************************************/
static USART_TypeDef *Select_SCI( int ch )
{
  switch ( ch )
  {
    case SCI_1 : return USART1;
    case SCI_2 : return USART2;
    case SCI_3 : return USART3;
#if defined (STM32F10X_HD)
    case SCI_4 : return UART4;
    case SCI_5 : return UART5;
#endif  /*defined (STM32F10X_HD)*/
    default : break;
  }

  return (USART_TypeDef *)0;
}

/*************************************************************************/
/* SCI(USART)初期化                                                      */
/*************************************************************************/
int SCI_Init( int ch, SCI_BUFFER *rcv, SCI_BUFFER *snd, SCI_ERROR_COUNT *err, unsigned long brr )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_InitTypeDef USART_InitStruct;
  USART_TypeDef *SCI = (USART_TypeDef *)Select_SCI( ch );

  if( SCI == (USART_TypeDef *)0 ) return (-1);

  sci_rcv[ ch ] = rcv;
  sci_snd[ ch ] = snd;
  sci_err[ ch ] = err;

  switch ( ch )
  {
    case SCI_1 :
      GPIO_Alternate_Init( USART1_TXD_PORT, USART1_TXD_PIN );  /*PA9はUSART TXD*/
      GPIO_Input_Init( USART1_RXD_PORT, USART1_RXD_PIN );  /*PA10はUSART RXD*/
      /*USART1へクロックの供給開始*/
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
      RCC_APB2PeriphResetCmd( RCC_APB2Periph_USART1, DISABLE );
      //brr /= 2;
      /*USART1の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      break;

    case SCI_2 :
      GPIO_Alternate_Init( USART2_TXD_PORT, USART2_TXD_PIN );  /*PD8はUSART TXD*/
      GPIO_InputFloat_Init( USART2_RXD_PORT, USART2_RXD_PIN );  /*PD9はUSART RXD*/
      /*USART2へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_USART2, DISABLE );
      /*USART2の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
      break;

    case SCI_3 :
      GPIO_Alternate_Init( USART3_TXD_PORT, USART3_TXD_PIN );  /*PD8はUSART TXD*/
      GPIO_InputFloat_Init( USART3_RXD_PORT, USART3_RXD_PIN );  /*PD9はUSART RXD*/
      GPIO_PinRemapConfig( GPIO_FullRemap_USART3, ENABLE );
      /*USART3へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_USART3, DISABLE );
      /*USART3の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
      break;

#if defined (STM32F10X_HD)
    case SCI_4 :
      GPIO_Alternate_Init( UART4_TXD_PORT, UART4_TXD_PIN );  /*PC10はUSART TXD*/
      GPIO_Input_Init( UART4_RXD_PORT, UART4_RXD_PIN );  /*PC11はUSART RXD*/
      /*USART4へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_UART4, DISABLE );
      /*USART4の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
      break;

    case SCI_5 :
      GPIO_Alternate_Init( UART5_TXD_PORT, UART5_TXD_PIN );  /*PC12はUSART TXD*/
      GPIO_Input_Init( UART5_RXD_PORT, UART5_RXD_PIN );  /*PD2はUSART RXD*/
      /*USART5へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_UART5, DISABLE );
      /*USART4の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
      break;
#endif /*defined (STM32F10X_HD)*/
  }

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;  /*修正*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = BASE_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  USART_StructInit( &USART_InitStruct );
  USART_InitStruct.USART_BaudRate = brr;
  USART_Init( SCI, &USART_InitStruct );
  USART_ITConfig( SCI, USART_IT_TXE, ENABLE );
  USART_ITConfig( SCI, USART_IT_RXNE, ENABLE );
  USART_ITConfig( SCI, USART_IT_ERR, ENABLE );
  USART_Cmd( SCI, ENABLE );

  return 0;
}

/*************************************************************************/
/* SCI(USART)解除                                                        */
/*************************************************************************/
int SCI_Deinit( int ch )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_TypeDef *SCI = Select_SCI( ch );

  if( SCI == (USART_TypeDef *)0 ) return (-1);

  SCI->CR1 = 0;  /*USART停止*/
  SCI->CR2 = 0;  /**/
  SCI->CR3 = 0;  /**/
  SCI->BRR = 0;  /**/

  switch ( ch )
  {
    case SCI_1 :
      /*USART1へクロックの供給停止*/
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, DISABLE );
      /*USART1の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      break;

    case SCI_2 :
      /*USART2へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, DISABLE );
      /*USART2の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
      break;

    case SCI_3 :
      /*USART3へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, DISABLE );
      /*USART3の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
      break;

#if defined (STM32F10X_HD)
    case SCI_4 :
      /*USART4へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, DISABLE );
      /*USART4の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
      break;

    case SCI_5 :
      /*USART5へクロックの供給開始*/
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, DISABLE );
      /*USART4の割り込み設定*/
      NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
      break;
#endif /*defined (STM32F10X_HD)*/
  }

  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init( &NVIC_InitStructure );

  return 0;
}

/*************************************************************************/
/* SCI一文字送信                                                         */
/*************************************************************************/
int SCI_Putc( int ch, char c )
{
  USART_TypeDef *SCI = Select_SCI( ch );

  if( SCI == (USART_TypeDef *)0 ) return (-1);

  while( USART_GetFlagStatus( SCI, USART_FLAG_TXE ) == RESET ) rot_rdq();
  USART_SendData( SCI, (uint16_t)c );

  return 0;
}

/*************************************************************************/
/* SCI文字列送信                                                         */
/*************************************************************************/
int SCI_Puts( int ch, const char *str )
{
  char *ptr;
  unsigned short next;
  int count;
  SCI_BUFFER *snd;
  USART_TypeDef *SCI;

  if( ch != SCI_1 && ch != SCI_2 && ch != SCI_3 && ch != SCI_4 && ch != SCI_5 ) return (-1);  /**/
  if( SndSizeOfBuf( ch ) == 0 ) return (-1);  /*書き込む余裕が無かった場合*/

  SCI = Select_SCI( ch );
  snd = sci_snd[ ch ];  /*バッファの選択*/

  count = 0;
  ptr = (char *)snd->buf;
  while( *str )
  {
    next = snd->wptr + 1;  /*ポインタを進めてみる*/
    if( next >= snd->size ) next = 0;  /*書き込みポインタの丸め*/
    if( next == snd->rptr ) break;  /*代入出来ない時*/
    ptr[ snd->wptr ] = *str++;  /*バッファに代入*/
    __disable_irq();
    snd->wptr = next;  /*書き込みポインタを進める*/
    __enable_irq();
    USART_ITConfig( SCI, USART_IT_TXE, ENABLE );  /*送信割込み許可*/
    count++;  /*書き込んだカウント値を更新*/
  }

  return count;
}

/*************************************************************************/
/* SCIブロック送信                                                       */
/*************************************************************************/
int SCI_Write( int ch, const void *src, int size )
{
  char *ptr;
  const char *data;
  unsigned short next;
  int count;
  SCI_BUFFER *snd;
  USART_TypeDef *SCI;

  if( ch != SCI_1 && ch != SCI_2 && ch != SCI_3 && ch != SCI_4 && ch != SCI_5 ) return (-1);  /**/
  if( SndSizeOfBuf( ch ) == 0 ) return (-1);  /*書き込む余裕が無かった場合*/

  SCI = Select_SCI( ch );
  snd = sci_snd[ ch ];  /*バッファの選択*/

  count = 0;
  ptr = (char *)snd->buf;
  data = (const char *)src;
  while( count < size )
  {
    next = snd->wptr + 1;  /*ポインタを進めてみる*/
    if( next >= snd->size ) next = 0;  /*書き込みポインタの丸め*/
    //if( next == snd->rptr ) break;  /*代入出来ない時*/
    while( next == snd->rptr ) rot_rdq();  /*代入出来ない時*/
    ptr[ snd->wptr ] = *data++;  /*バッファに代入*/
    snd->wptr = next;  /*書き込みポインタを進める*/
    USART_ITConfig( SCI, USART_IT_TXE, ENABLE );  /*送信割込み許可*/
    count++;  /*書き込んだカウント値を更新*/
  }

  return count;
}

/*************************************************************************/
/* SCI受信バッファの残りサイズ                                           */
/*************************************************************************/
unsigned short RcvSizeOfBuf( int ch )
{
  SCI_BUFFER *rcv;
  unsigned short w,r;

  rcv = sci_rcv[ ch ];  /*バッファの選択*/
  w = rcv->wptr;
  r = rcv->rptr;

  if( w == r ) return 0;
  else if( w < r ) return w + rcv->size - r;
  else return w - r;
}

/*************************************************************************/
/* SCI送信バッファの空きサイズ                                           */
/*************************************************************************/
unsigned short SndSizeOfBuf( int ch )
{
  SCI_BUFFER *snd;
  unsigned short w,r;

  snd = sci_snd[ ch ];  /*バッファの選択*/
  w = snd->wptr;
  r = snd->rptr;

  if( w == r ) return snd->size - 1;
  else if( r < w ) return r + snd->size - w - 1;
  else return r - w - 1;
}

/*************************************************************************/
/* SCI受信バッファから1文字取得                                          */
/*************************************************************************/
int SCI_Getc( int ch )
{
  int c;
  char *buf;
  SCI_BUFFER *rcv;

  rcv = sci_rcv[ ch ];  /*バッファの選択*/
  if( RcvSizeOfBuf( ch ) == 0 ) return (-1);
  buf = (char *)rcv->buf;
  c = buf[ rcv->rptr ];
  if( ++rcv->rptr >= rcv->size ) rcv->rptr = 0;

  return c & 0x00ff;
}

/*************************************************************************/
/* SCIステータスの読み出し                                               */
/*************************************************************************/
unsigned long SCI_Status( int ch )
{
  USART_TypeDef *SCI = Select_SCI( ch );
  return SCI->SR;
}

/*************************************************************************/
/* SCI直接データ出力                                                     */
/*************************************************************************/
char SCI_DirectRead( int ch )
{
  USART_TypeDef *SCI = Select_SCI( ch );
  return SCI->DR;
}

/*************************************************************************/
/* SCI受信バッファパージ                                                 */
/*************************************************************************/
void SCI_Purge( int ch )
{
  SCI_BUFFER *rcv;

  rcv = sci_rcv[ ch ];  /*バッファの選択*/
  rcv->wptr = rcv->rptr = 0;
}

/*************************************************************************/
/* 一文字受信待ち                                                        */
/*************************************************************************/
int Rcv1Char( int handle )
{
  while( RcvSizeOfBuf( handle ) == 0) rot_rdq();
  return SCI_Getc( handle );
}

/********************************************************************/
/*	指定インタフェースから一行取り込み処理							*/
/*	bufは格納先、chはハンドル、BufSzはバッファのサイズ		        */
/*	戻り値に格納先アドレスを返すが、ユーザーアボートの時はヌルポ	*/
/*	インタを返す。													*/
/********************************************************************/
char *SCI_Gets( char *buf, int ch, int BufSz )
{
  char c,*str;

  str = buf;
  while( 1 )
  {
    while( RcvSizeOfBuf( ch ) <= 0 ) rot_rdq();  /*文字を取得待ち*/
    c = SCI_Getc( ch );

    if( c == '\n' ) break;              /*改行を見つけたので次の処理*/
    if( c == '\r' ) continue;  /*何もせず*/
    if( str == &buf[ BufSz ] )          /*バッファ一杯になってしまったのでこれ以上の取り込みを止める*/
    {
      SCI_Purge( ch );                  /*入力をFLASHして終了*/
      *(--str) = '\0';                  /*強制的に文字列に変換*/
      return buf;
    }
    *str++ = c;
  }

  *str = '\0';                          /*行単位に文字列に変換*/

  return buf;
}

int SCI_GetsWithTmout( char *buf, int ch, int BufSz, unsigned long tmout )
{
  char *str = buf;
  unsigned long baseTim = systim;
  while( 1 )
  {
    while( RcvSizeOfBuf( ch ) <= 0 )
    {
      if( (systim - baseTim) > tmout ) return (-1);
      rot_rdq();  /*文字を取得待ち*/
    }
    int c = SCI_Getc( ch );

    if( c == '\n' ) break;              /*改行を見つけたので次の処理*/
    if( c == '\r' ) continue;  /*何もせず*/
    if( str == &buf[ BufSz ] )          /*バッファ一杯になってしまったのでこれ以上の取り込みを止める*/
    {
      SCI_Purge( ch );                  /*入力をFLASHして終了*/
      *(--str) = '\0';                  /*強制的に文字列に変換*/
      return 0;
    }
    *str++ = c;
  }

  *str = '\0';                          /*行単位に文字列に変換*/

  return 0;
}

/********************************************************************/
/*	指定インタフェースから一行取り込み処理							*/
/*	bufは格納先、chはハンドル、BufSzはバッファのサイズ		        */
/*	戻り値に格納先アドレスを返すが、ユーザーアボートの時はヌルポ	*/
/*	インタを返す。													*/
/********************************************************************/
#define	BACKSPACE   0x08
#define	USER_BREAK  0x03
#define	CTRL_C      0x03
#define	CTRL_Z      0x1a
#define  IWDG_Clr()
char *GetLine( char *buf, int ch, int BufSz )
{
  char c,*str;

  str = buf;
  while( 1 )
  {
    while( RcvSizeOfBuf( ch ) <= 0 ) rot_rdq();  /*文字を取得待ち*/
    c = SCI_Getc( ch );
    if( c == USER_BREAK ) return 0;     /*ユーザーブレークの時は終了*/

//    SCI_Write( ch, &c, sizeof(char) );  /*エコーバック*/
    SCI_Putc( ch, c );                  /*エコーバック*/

    if( c == '\r' ) break;              /*改行を見つけたので次の処理*/
    if( str == &buf[ BufSz ] )          /*バッファ一杯になってしまったのでこれ以上の取り込みを止める*/
    {
      SCI_Purge( ch );                  /*入力をFLASHして終了*/
      *(--str) = '\0';                  /*強制的に文字列に変換*/
      return buf;
    }
    if( c == BACKSPACE )                /*簡単にBACK SPACEへの対応*/
    {
      if( str != buf ) str--;
      continue;
    }
    *str++ = c;
  }

  c = '\n';
  SCI_Write( ch, &c, sizeof(char) );
  *str = '\0';                          /*行単位に文字列に変換*/

  return buf;
}

/*************************************************************************/
/* SCI色々割り込み                                                       */
/* どうも要因別の割り込みは無いみたいだな                                */
/*************************************************************************/
void SCI_ISR( int ch )
{
  unsigned short next;
  char data,*ptr;
  USART_TypeDef *SCI = Select_SCI( ch );
  SCI_BUFFER *rcv;
  SCI_BUFFER *snd;
  SCI_ERROR_COUNT *err;

  /*各種エラーのチェック*/
  if( USART_GetFlagStatus( SCI, USART_FLAG_FE ) == SET )
  {
	data = (char)USART_ReceiveData( SCI );
	USART_ClearITPendingBit( SCI, USART_IT_FE );
    if( (err = sci_err[ ch ]) != 0 )
      err->FE_cnt++;  /*フレーミングエラー数の累積*/
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_PE ) == SET )
  {
	data = (char)USART_ReceiveData( SCI );
	USART_ClearITPendingBit( SCI, USART_FLAG_PE );
    if( (err = sci_err[ ch ]) != 0 )
      err->PE_cnt++;  /*パリティエラー数の累積*/
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_NE ) == SET )
  {
	data = (char)USART_ReceiveData( SCI );
	USART_ClearITPendingBit( SCI, USART_FLAG_NE );
    if( (err = sci_err[ ch ]) != 0 )
      err->NE_cnt++;  /*ノイズエラー数の累積*/
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_ORE ) == SET )
  {
	data = (char)USART_ReceiveData( SCI );
	USART_ClearITPendingBit( SCI, USART_FLAG_ORE );
    if( (err = sci_err[ ch ]) != 0 )
      err->ORE_cnt++;  /*オーバーランエラー数の累積*/
  }

  /*受信割り込みの判定*/
  if( USART_GetFlagStatus( SCI, USART_FLAG_RXNE ) == SET )
  {
    data = (char)USART_ReceiveData( SCI );
    rcv = sci_rcv[ ch ];  /*バッファの選択*/
    ptr = (char *)rcv->buf;
    next = rcv->wptr + 1;
    if( next >= rcv->size ) next = 0;
    if( next != rcv->rptr )
    {
      *(ptr + rcv->wptr) = data;
      rcv->wptr = next;
    }
  }

  /*送信割り込みの判定*/
  if( USART_GetFlagStatus( SCI, USART_FLAG_TXE ) == SET )
  {
    snd = sci_snd[ ch ];  /*バッファの選択*/
    if( snd->rptr == snd->wptr )  /*送信すべきデータが無い時*/
    {
      USART_ITConfig( SCI, USART_IT_TXE, DISABLE );  /*送信割込み禁止*/
      USART_ClearITPendingBit( SCI, USART_FLAG_TXE );
      return;
    }
    ptr = (char *)snd->buf;
    uint16_t tempUS = (uint16_t)*(ptr + snd->rptr);
    USART_SendData( SCI, tempUS );
    if( ++snd->rptr >= snd->size ) snd->rptr = 0;
  }
}

/********************************************************************/
/*	USART1割り込みハンドラ                                          */
/********************************************************************/
void USART1_IRQHandler( void )
{
  SCI_ISR( SCI_1 );
}

/********************************************************************/
/*	USART2割り込みハンドラ                                          */
/********************************************************************/
void USART2_IRQHandler( void )
{
  SCI_ISR( SCI_2 );
}

/********************************************************************/
/*	USART3割り込みハンドラ                                          */
/********************************************************************/
void USART3_IRQHandler( void )
{
  SCI_ISR( SCI_3 );
}

#if defined (STM32F10X_HD)
/********************************************************************/
/*	UART4割り込みハンドラ                                           */
/********************************************************************/
void UART4_IRQHandler( void )
{
  SCI_ISR( SCI_4 );
}

/********************************************************************/
/*	UART5割り込みハンドラ                                           */
/********************************************************************/
void UART5_IRQHandler( void )
{
  SCI_ISR( SCI_5 );
}
#endif /*defined (STM32F10X_HD)*/

/********************************************************************/
/*	エラー統計情報の取得                                            */
/********************************************************************/
void GetErrorInfo( int ch, unsigned long *fe, unsigned long *pe, unsigned long *ne, unsigned long *ore )
{
  SCI_ERROR_COUNT *err;

  if( (err = sci_err[ ch ]) == 0 ) return;  /*エラーカウント用の領域が確保されていない時*/

  *fe = err->FE_cnt;
  *pe = err->PE_cnt;
  *ne = err->NE_cnt;
  *ore = err->ORE_cnt;
}
