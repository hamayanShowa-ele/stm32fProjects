/* ----------------------------------------
  STM32F407ZET usart code
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
#include  "usart.h"

/* ----------------------------------------
  defines.
---------------------------------------- */
#ifndef _MULTITASK_H_
#define  rot_rdq()
#define  dly_tsk(dly)  Dly100us(dly * 10UL)
#endif /*_MULTITASK_H_*/

/* ----------------------------------------
  prototypes.
---------------------------------------- */
static USART_TypeDef *Select_SCI( int ch );

/* ----------------------------------------
  global variables and instance.
---------------------------------------- */
static SCI_BUFFER *sci_snd[ 6 ];
static SCI_BUFFER *sci_rcv[ 6 ];
static SCI_ERROR_COUNT *sci_err[ 6 ];
extern SYSTIM systim;

/* ----------------------------------------
  SCI (USART) selector.
---------------------------------------- */
static USART_TypeDef *Select_SCI( int ch )
{
  switch ( ch )
  {
    case SCI_1 : return USART1;
    case SCI_2 : return USART2;
    case SCI_3 : return USART3;
  #if defined (STM32F407ZE)
    case SCI_4 : return UART4;
    case SCI_5 : return UART5;
    case SCI_6 : return USART6;
  #endif  /*defined (STM32F407ZE)*/
    default : break;
  }

  return (USART_TypeDef *)0;
}

/* ----------------------------------------
  SCI (USART) initialize.
---------------------------------------- */
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
      /* USART1 supply clock. */
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
      RCC_APB2PeriphResetCmd( RCC_APB2Periph_USART1, DISABLE );
      //brr /= 2;
      /* configure USART1 interrupt number. */
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      break;

    case SCI_2 :
      /* USART2 supply clock. */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_USART2, DISABLE );
      /* configure USART2 interrupt number. */
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
      break;

    case SCI_3 :
      /* USART3 supply clock. */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_USART3, DISABLE );
      /* configure USART3 interrupt number. */
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
      break;

  #if defined (STM32F407ZE)
    case SCI_4 :
      /* UART4 supply clock. */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_UART4, DISABLE );
      /* configure UART4 interrupt number. */
      NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
      break;

    case SCI_5 :
      /* UART5 supply clock. */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE );
      RCC_APB1PeriphResetCmd( RCC_APB1Periph_UART5, DISABLE );
      /* configure UART5 interrupt number. */
      NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
      break;

    case SCI_6 :
      /* USART6 supply clock. */
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART6, ENABLE );
      RCC_APB2PeriphResetCmd( RCC_APB2Periph_USART6, DISABLE );
      /* configure USART6 interrupt number. */
      NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
      break;
  #endif /*defined (STM32F407ZE)*/
  }

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BASE_PRIORITY;
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

/* ----------------------------------------
  SCI (USART) deinitialize.
---------------------------------------- */
int SCI_Deinit( int ch )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_TypeDef *SCI = Select_SCI( ch );

  if( SCI == (USART_TypeDef *)0 ) return (-1);

  SCI->CR1 = 0;  /*USART��~*/
  SCI->CR2 = 0;  /**/
  SCI->CR3 = 0;  /**/
  SCI->BRR = 0;  /**/

  switch ( ch )
  {
    case SCI_1 :
      /* supply USART1 clock */
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, DISABLE );
      /* USART1 ireq number set. */
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      break;

    case SCI_2 :
      /* supply USART2 clock */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, DISABLE );
      /* USART2 ireq number set. */
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
      break;

    case SCI_3 :
      /* supply USART3 clock */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, DISABLE );
      /* USART3 ireq number set. */
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
      break;

  #if defined (STM32F407ZE)
    case SCI_4 :
      /* supply UART4 clock */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, DISABLE );
      /* USART4 ireq number set. */
      NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
      break;

    case SCI_5 :
      /* supply UART5 clock */
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, DISABLE );
      /* USART4 ireq number set. */
      NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
      break;

    case SCI_6 :
      /* supply USART6 clock */
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART6, DISABLE );
      /* USART6 ireq number set. */
      NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
      break;
  #endif /*defined (STM32F407ZE)*/
  }

  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init( &NVIC_InitStructure );

  return 0;
}

/* ----------------------------------------
  SCI (USART) putc.
---------------------------------------- */
int SCI_Putc( int ch, char c )
{
  USART_TypeDef *SCI = Select_SCI( ch );

  if( SCI == (USART_TypeDef *)0 ) return (-1);

  while( USART_GetFlagStatus( SCI, USART_FLAG_TXE ) == RESET ) rot_rdq();
  USART_SendData( SCI, (uint16_t)c );

  return 0;
}

/* ----------------------------------------
  SCI (USART) put string.
---------------------------------------- */
int SCI_Puts( int ch, const char *str )
{
  return SCI_Write( ch, (const void *)str, strlen(str) );
}

/* ----------------------------------------
  SCI (USART) write.
---------------------------------------- */
int SCI_Write( int ch, const void *src, size_t size )
{
  char *ptr;
  const char *data;
  unsigned short next;
  int count;
  SCI_BUFFER *snd;
  USART_TypeDef *SCI;

  if( ch != SCI_1 && ch != SCI_2 && ch != SCI_3 && ch != SCI_4 && ch != SCI_5 && ch != SCI_6 ) return (-1);  /**/
  T_RTST pk_rtst;
  ref_tst( TSK_SELF, &pk_rtst );
  if(pk_rtst.tskstat == TTS_RDY)
  {
    while( SndSizeOfBuf( ch ) == 0 ) rot_rdq();
  }
  else
  {
    if( SndSizeOfBuf( ch ) == 0 ) return (-1);
  }

  SCI = Select_SCI( ch );
  snd = sci_snd[ ch ];

  count = 0;
  ptr = (char *)snd->buf;
  data = (const char *)src;
  while( count < size )
  {
    next = snd->wptr + 1;
    if( next >= snd->size ) next = 0;
    //if( next == snd->rptr ) break;
    while( next == snd->rptr ) dly_tsk(10);
    ptr[ snd->wptr ] = *data++;
    snd->wptr = next;
    USART_ITConfig( SCI, USART_IT_TXE, ENABLE );
    count++;
  }

  return count;
}

/* ----------------------------------------
  SCI (USART) recieve size of recieve buffer.
---------------------------------------- */
unsigned short RcvSizeOfBuf( int ch )
{
  SCI_BUFFER *rcv;
  unsigned short w,r;

  rcv = sci_rcv[ ch ];
  w = rcv->wptr;
  r = rcv->rptr;

  if( w == r ) return 0;
  else if( w < r ) return w + rcv->size - r;
  else return w - r;
}

/* ----------------------------------------
  SCI (USART) transmit size of send buffer.
---------------------------------------- */
unsigned short SndSizeOfBuf( int ch )
{
  SCI_BUFFER *snd;
  unsigned short w,r;

  snd = sci_snd[ ch ];
  w = snd->wptr;
  r = snd->rptr;

  if( w == r ) return snd->size - 1;
  else if( r < w ) return r + snd->size - w - 1;
  else return r - w - 1;
}

/* ----------------------------------------
  SCI (USART) get character from buffer.
---------------------------------------- */
int SCI_Getc( int ch )
{
  int c;
  char *buf;
  SCI_BUFFER *rcv;

  rcv = sci_rcv[ ch ];
  if( RcvSizeOfBuf( ch ) == 0 ) return (-1);
  buf = (char *)rcv->buf;
  c = buf[ rcv->rptr ];
  if( ++rcv->rptr >= rcv->size ) rcv->rptr = 0;

  return c & 0x00ff;
}

/* ----------------------------------------
  SCI (USART) get sci status register value.
---------------------------------------- */
unsigned long SCI_Status( int ch )
{
  USART_TypeDef *SCI = Select_SCI( ch );
  return SCI->SR;
}

/* ----------------------------------------
  SCI (USART) direct read from data register.
---------------------------------------- */
char SCI_DirectRead( int ch )
{
  USART_TypeDef *SCI = Select_SCI( ch );
  return SCI->DR;
}

/* ----------------------------------------
  SCI (USART) purge recieve data.
---------------------------------------- */
void SCI_Purge( int ch )
{
  SCI_BUFFER *rcv;

  rcv = sci_rcv[ ch ];
  rcv->wptr = rcv->rptr = 0;
}

/* ----------------------------------------
  SCI (USART) recieve one data from buffer.
---------------------------------------- */
int Rcv1Char( int handle )
{
  while( RcvSizeOfBuf( handle ) == 0) rot_rdq();
  return SCI_Getc( handle );
}

/* ----------------------------------------
  SCI (USART) get line from recieve buffer.
---------------------------------------- */
#define  BACKSPACE   0x08
#define  USER_BREAK  0x03
#define  CTRL_C      0x03
#define  CTRL_Z      0x1a
#define  IWDG_Clr()
char *GetLine( char *buf, int ch, int BufSz )
{
  char c,*str;

  str = buf;
  while( 1 )
  {
    SYSTIM tmout = systim;
    while( RcvSizeOfBuf( ch ) <= 0 )
    {
      if( (systim - tmout) >= 1000UL ) return NULL;  // original is 1000UL
      rot_rdq();
    }
    c = SCI_Getc( ch );
    if( c == USER_BREAK ) return 0;

//    SCI_Write( ch, &c, sizeof(char) );
//    SCI_Putc( ch, c );

    if( c == '\r' ) c = '\0';
    else if( c == '\n' ) break;
    else if( c == BACKSPACE )
    {
      if( str != buf ) str--;
      continue;
    }
    else {}

    if( str == &buf[ BufSz ] )
    {
      SCI_Purge( ch );
      *(--str) = '\0';
      return buf;
    }
    *str++ = c;
  }

//  c = '\n';
//  SCI_Write( ch, &c, sizeof(char) );  // if you need echo back then remove comment out.
  *str = '\0';

  return buf;
}

/* ----------------------------------------
  One-line fetching from the specified interface.
  buf is the destination, ch is the handle, and BufSz is the size of the buffer
  If the return value is a destination address,
   the function returns a null pointer if the return value is user aborted.
---------------------------------------- */
char *SCI_Gets( char *buf, int ch, int BufSz )
{
  char c,*str;

  str = buf;
  while( 1 )
  {
    while( RcvSizeOfBuf( ch ) <= 0 ) rot_rdq();  /* Waiting to get a character */
    c = SCI_Getc( ch );

    if( c == '\n' ) break;  /* I found a line break, so we'll move on to the next process */
    if( c == '\r' ) continue;  /* without doing anything */
    if( str == &buf[ BufSz ] )  /* Now that the buffer is full, I'll stop grabbing more. */
    {
      SCI_Purge( ch );  /* Discard the incoming data and be done with it. */
      *(--str) = '\0';  /* Forced conversion to string */
      return buf;
    }
    *str++ = c;
  }
  *str = '\0';  /* Convert line by line to string */

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
      rot_rdq();  /* Waiting to get a character */
    }
    int c = SCI_Getc( ch );

    if( c == '\n' ) break;  /* I found a line break, so we'll move on to the next process */
    if( c == '\r' ) continue;  /* without doing anything */
    if( str == &buf[ BufSz ] )  /* Now that the buffer is full, I'll stop grabbing more. */
    {
      SCI_Purge( ch );  /* Discard the incoming data and be done with it. */
      *(--str) = '\0';  /* Forced conversion to string */
      return 0;
    }
    *str++ = c;
  }
  *str = '\0';  /* Convert line by line to string */

  return 0;
}



/* ----------------------------------------
  SCI (USART) get error informations.
---------------------------------------- */
void GetErrorInfo( int ch, unsigned long *fe, unsigned long *pe, unsigned long *ne, unsigned long *ore )
{
  SCI_ERROR_COUNT *err;

  if( (err = sci_err[ ch ]) == 0 ) return;

  *fe = err->FE_cnt;
  *pe = err->PE_cnt;
  *ne = err->NE_cnt;
  *ore = err->ORE_cnt;
}

/* ----------------------------------------
  SCI (USART) interrupt handler.
---------------------------------------- */
void SCI_ISR( int ch )
{
  unsigned short next;
  char data,*ptr;
  USART_TypeDef *SCI = Select_SCI( ch );
  SCI_BUFFER *rcv;
  SCI_BUFFER *snd;
  SCI_ERROR_COUNT *err;

  if( USART_GetFlagStatus( SCI, USART_FLAG_FE ) == SET )
  {
    data = (char)USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_IT_FE );
    if( (err = sci_err[ ch ]) != 0 )
      err->FE_cnt++;
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_PE ) == SET )
  {
    data = (char)USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_FLAG_PE );
    if( (err = sci_err[ ch ]) != 0 )
      err->PE_cnt++;
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_NE ) == SET )
  {
    data = (char)USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_FLAG_NE );
    if( (err = sci_err[ ch ]) != 0 )
      err->NE_cnt++;
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_ORE ) == SET )
  {
    data = (char)USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_FLAG_ORE );
    if( (err = sci_err[ ch ]) != 0 )
      err->ORE_cnt++;
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_RXNE ) == SET )
  {
    data = (char)USART_ReceiveData( SCI );
    rcv = sci_rcv[ ch ];
    ptr = (char *)rcv->buf;
    next = rcv->wptr + 1;
    if( next >= rcv->size ) next = 0;
    if( next != rcv->rptr )
    {
      *(ptr + rcv->wptr) = data;
      rcv->wptr = next;
    }
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_TXE ) == SET )
  {
    snd = sci_snd[ ch ];
    if( snd->rptr == snd->wptr )
    {
      USART_ITConfig( SCI, USART_IT_TXE, DISABLE );
      USART_ClearITPendingBit( SCI, USART_FLAG_TXE );
      return;
    }
    ptr = (char *)snd->buf;
    USART_SendData( SCI, (uint16_t)*(ptr + snd->rptr) );
    if( ++snd->rptr >= snd->size ) snd->rptr = 0;
  }
}

/* ----------------------------------------
  SCI 1 (USART1) interrupt handler.
---------------------------------------- */
void USART1_IRQHandler( void )
{
  SCI_ISR( SCI_1 );
}

/* ----------------------------------------
  SCI 2 (USART2) interrupt handler.
---------------------------------------- */
void USART2_IRQHandler( void )
{
  SCI_ISR( SCI_2 );
}

/* ----------------------------------------
  SCI 3 (USART3) interrupt handler.
---------------------------------------- */
void USART3_IRQHandler( void )
{
  SCI_ISR( SCI_3 );
}

#if defined (STM32F407ZE)
/* ----------------------------------------
  SCI 4 (UART4) interrupt handler.
---------------------------------------- */
void UART4_IRQHandler( void )
{
  SCI_ISR( SCI_4 );
}

/* ----------------------------------------
  SCI 5 (UART5) interrupt handler.
---------------------------------------- */
void UART5_IRQHandler( void )
{
  SCI_ISR( SCI_5 );
}

/* ----------------------------------------
  SCI 6 (USART6) interrupt handler.
---------------------------------------- */
void USART6_IRQHandler( void )
{
  SCI_ISR( SCI_6 );
}
#endif /*defined (STM32F407ZE)*/

/* ----------------------------------------
  end of file
  designed by hamayan since 2008/11/06
---------------------------------------- */
