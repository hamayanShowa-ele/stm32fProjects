/* ----------------------------------------
  USART,UART utilities
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
#include  "USART_UART.h"

#define  TIMEOUT  5UL  /* 5ms */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern SYSTIM systim;

static uint8_t sci1TxBuffer[ SCI_1_SND_BUFFER_SIZE ];
static uint8_t sci1RxBuffer[ SCI_1_RCV_BUFFER_SIZE ];
static uint8_t sci2TxBuffer[ SCI_2_SND_BUFFER_SIZE ];
static uint8_t sci2RxBuffer[ SCI_2_RCV_BUFFER_SIZE ];
static uint8_t sci3TxBuffer[ SCI_3_SND_BUFFER_SIZE ];
static uint8_t sci3RxBuffer[ SCI_3_RCV_BUFFER_SIZE ];
static uint8_t sci4TxBuffer[ SCI_4_SND_BUFFER_SIZE ];
static uint8_t sci4RxBuffer[ SCI_4_RCV_BUFFER_SIZE ];
static uint8_t sci5TxBuffer[ SCI_5_SND_BUFFER_SIZE ];
static uint8_t sci5RxBuffer[ SCI_5_RCV_BUFFER_SIZE ];
static uint8_t sci6TxBuffer[ SCI_6_SND_BUFFER_SIZE ];
static uint8_t sci6RxBuffer[ SCI_6_RCV_BUFFER_SIZE ];
#if 0
static uint8_t sci7TxBuffer[ SCI_7_SND_BUFFER_SIZE ];
static uint8_t sci7RxBuffer[ SCI_7_RCV_BUFFER_SIZE ];
static uint8_t sci8TxBuffer[ SCI_8_SND_BUFFER_SIZE ];
static uint8_t sci8RxBuffer[ SCI_8_RCV_BUFFER_SIZE ];
#endif

T_SCI_BUFFER t_sci[] =
{
  { sci1TxBuffer, sci1RxBuffer, SCI_1_SND_BUFFER_SIZE, SCI_1_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
  { sci2TxBuffer, sci2RxBuffer, SCI_2_SND_BUFFER_SIZE, SCI_2_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
  { sci3TxBuffer, sci3RxBuffer, SCI_3_SND_BUFFER_SIZE, SCI_3_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
  { sci4TxBuffer, sci4RxBuffer, SCI_4_SND_BUFFER_SIZE, SCI_4_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
  { sci5TxBuffer, sci5RxBuffer, SCI_5_SND_BUFFER_SIZE, SCI_5_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
  { sci6TxBuffer, sci6RxBuffer, SCI_6_SND_BUFFER_SIZE, SCI_6_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
#if 0
  { sci7TxBuffer, sci7RxBuffer, SCI_7_SND_BUFFER_SIZE, SCI_7_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
  { sci8TxBuffer, sci8RxBuffer, SCI_8_SND_BUFFER_SIZE, SCI_8_RCV_BUFFER_SIZE, 0,0,0,0, 0UL,0UL,0UL,0UL, false },
#endif
};

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
USART_UART::USART_UART()
{
}

USART_UART::~USART_UART()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
int USART_UART::begin(
  USART_TypeDef *_usart, uint32_t _brr,
  uint8_t _tx, uint8_t _rx,
  bool remap, uint8_t basePri, uint8_t subPri )
{
  USARTx = _usart;
  txPin = _tx; rxPin = _rx;
  brr = _brr;
  uint8_t irq;
  ifNumber = USART1_IF_NUMBER;

  if( USARTx == USART1 )
  {
    ifNumber = USART1_IF_NUMBER;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );  /* usart clock enable. */
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_USART1, DISABLE );
    if( txPin == PB6 && rxPin == PB7 )  /* PB6 : tx, PB7 : rx */
    {
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
      pinMode( PB6, ALTERNATE_PP );
      pinMode( PB7, ALTERNATE_PP );
      pinAFConfig( PB6, GPIO_AF_USART1 );
      pinAFConfig( PB7, GPIO_AF_USART1 );
    }
    else if( txPin == PA9 && rxPin == PA10 )  /* PA9 : tx, PA10 : rx */
    {
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
      pinMode( PA9, ALTERNATE_PP );
      pinMode( PA10, ALTERNATE_PP );
      pinAFConfig( PA9, GPIO_AF_USART1 );
      pinAFConfig( PA10, GPIO_AF_USART1 );
    }
    else return USART_DEFINITION_ERROR;
    irq = USART1_IRQn;
  }
  else if( USARTx == USART2 )
  {
    ifNumber = USART2_IF_NUMBER;
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );  /* usart clock enable. */
    RCC_APB1PeriphResetCmd( RCC_APB1Periph_USART2, DISABLE );
    if( txPin == PD5 && rxPin == PD6 )  /* PD5 : tx, PD6 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
      pinMode( PD5, ALTERNATE_PP );
      pinMode( PD6, ALTERNATE_PP );
      pinAFConfig( PD5, GPIO_AF_USART2 );
      pinAFConfig( PD6, GPIO_AF_USART2 );
    }
    else if( txPin == PA2 && rxPin == PA3 )  /* PA2 : tx, PA3 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
      pinMode( PA2, ALTERNATE_PP );
      pinMode( PA3, ALTERNATE_PP );
      pinAFConfig( PA2, GPIO_AF_USART2 );
      pinAFConfig( PA3, GPIO_AF_USART2 );
    }
    else return USART_DEFINITION_ERROR;
    irq = USART2_IRQn;
  }
  else if( USARTx == USART3 )
  {
    ifNumber = USART3_IF_NUMBER;
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );  /* usart clock enable. */
    RCC_APB1PeriphResetCmd( RCC_APB1Periph_USART3, DISABLE );
    if( txPin == PD8 && rxPin == PD9 )  /* PD8 : tx, PD9 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
      pinMode( PD8, ALTERNATE_PP );
      pinMode( PD9, ALTERNATE_PP );
      pinAFConfig( PD8, GPIO_AF_USART3 );
      pinAFConfig( PD9, GPIO_AF_USART3 );
    }
    else if( txPin == PB10 && rxPin == PB11 )  /* PB10 : tx, PB11 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
      pinMode( PB10, ALTERNATE_PP );
      pinMode( PB11, ALTERNATE_PP );
      pinAFConfig( PB10, GPIO_AF_USART3 );
      pinAFConfig( PB11, GPIO_AF_USART3 );
    }
    else return USART_DEFINITION_ERROR;
    irq = USART3_IRQn;
  }
  else if( USARTx == UART4 )  /* PC10 : tx, PC11 : rx */
  {
    ifNumber = UART4_IF_NUMBER;
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE );  /* usart clock enable. */
    RCC_APB1PeriphResetCmd( RCC_APB1Periph_UART4, DISABLE );
    if( txPin == PA0 && rxPin == PA1 )  /* PA0 : tx, PA1 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
      pinMode( PA0, ALTERNATE_PP );
      pinMode( PA1, ALTERNATE_PP );
      pinAFConfig( PA0, GPIO_AF_UART4 );
      pinAFConfig( PA1, GPIO_AF_UART4 );
    }
    else if( txPin == PC10 && rxPin == PC11 )  /* PC10 : tx, PC11 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
      pinMode( PC10, ALTERNATE_PP );
      pinMode( PC11, ALTERNATE_PP );
      pinAFConfig( PC10, GPIO_AF_UART4 );
      pinAFConfig( PC11, GPIO_AF_UART4 );
    }
    else return USART_DEFINITION_ERROR;
    irq = UART4_IRQn;
  }
  else if( USARTx == UART5 )
  {
    ifNumber = UART5_IF_NUMBER;
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE );  /* usart clock enable. */
    RCC_APB1PeriphResetCmd( RCC_APB1Periph_UART5, DISABLE );
    if( txPin == PC12 && rxPin == PD2 )  /* PC12 : tx, PD2 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
      pinMode( PC12, ALTERNATE_PP );
      pinMode( PD2, ALTERNATE_PP );
      pinAFConfig( PC12, GPIO_AF_UART5 );
      pinAFConfig( PD2, GPIO_AF_UART5 );
    }
    else return USART_DEFINITION_ERROR;
    irq = UART5_IRQn;
  }
  else if( USARTx == USART6 )  /* PC12 : tx, PD2 : rx */
  {
    ifNumber = USART6_IF_NUMBER;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART6, ENABLE );  /* usart clock enable. */
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_USART6, DISABLE );
    if( txPin == PG14 && rxPin == PG9 )  /* PG14 : tx, PG9 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE );
      pinMode( PG14, ALTERNATE_PP );
      pinMode( PG9, ALTERNATE_PP );
      pinAFConfig( PG14, GPIO_AF_USART6 );
      pinAFConfig( PG9, GPIO_AF_USART6 );
    }
    else if( txPin == PC6 && rxPin == PC7 )  /* PC6 : tx, PC7 : rx */
    {
      /* usart gpio configuration. */
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
      pinMode( PC6, ALTERNATE_PP );
      pinMode( PC7, ALTERNATE_PP );
      pinAFConfig( PC6, GPIO_AF_USART6 );
      pinAFConfig( PC7, GPIO_AF_USART6 );
    }
    else return USART_DEFINITION_ERROR;
    irq = USART6_IRQn;
  }
  else return USART_DEFINITION_ERROR;

  /* buffer and error counter clear. */
  t_sci[ifNumber].txWptr = t_sci[ifNumber].txRptr = 0;
  t_sci[ifNumber].rxWptr = t_sci[ifNumber].rxRptr = 0;
  t_sci[ifNumber].FE_cnt = t_sci[ifNumber].PE_cnt = 0UL;
  t_sci[ifNumber].NE_cnt = t_sci[ifNumber].ORE_cnt = 0UL;
  t_sci[ifNumber].echo = false;

  /* usart and uart configuraton. */
  USART_InitTypeDef USART_InitStruct;
  USART_StructInit( &USART_InitStruct );
  USART_InitStruct.USART_BaudRate = brr;
  USART_Init( USARTx, &USART_InitStruct );
  USART_ITConfig( USARTx, USART_IT_TXE, ENABLE );
  USART_ITConfig( USARTx, USART_IT_RXNE, ENABLE );
  USART_ITConfig( USARTx, USART_IT_ERR, ENABLE );
  USART_Cmd( USARTx, ENABLE );

  /* nvic interrupt configuraton. */
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = irq;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = basePri;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPri;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  return USART_SUCCESS;
}

void USART_UART::end()
{
}

/* ----------------------------------------
    tx available, rx available
---------------------------------------- */
uint16_t USART_UART::txAvailable()
{
  if( t_sci[ifNumber].txWptr == t_sci[ifNumber].txRptr )
    return t_sci[ifNumber].txSize - 1;
  else if( t_sci[ifNumber].txRptr < t_sci[ifNumber].txWptr )
    return t_sci[ifNumber].txRptr + t_sci[ifNumber].txSize - t_sci[ifNumber].txWptr - 1;
  else return t_sci[ifNumber].txRptr - t_sci[ifNumber].txWptr - 1;
}

uint16_t USART_UART::rxAvailable()
{
  if( t_sci[ifNumber].rxWptr == t_sci[ifNumber].rxRptr )
    return 0;
  else if( t_sci[ifNumber].rxWptr < t_sci[ifNumber].rxRptr )
    return t_sci[ifNumber].rxWptr + t_sci[ifNumber].rxSize - t_sci[ifNumber].rxRptr;
  else return t_sci[ifNumber].rxWptr - t_sci[ifNumber].rxRptr;
}


/* ----------------------------------------
    write
---------------------------------------- */
int USART_UART::write( uint8_t c )
{
#if 0
  while( USART_GetFlagStatus( USARTx, USART_FLAG_TXE ) == RESET ) rot_rdq();
  USART_SendData( USARTx, (uint16_t)c );
  return 1;
#else
  return write( (const uint8_t *)&c, 1 );
#endif
}

int USART_UART::write( const uint8_t *str, int size )
{
  if( txAvailable() == 0 ) return (-1);  /* If there was no time to write. */

  int count = 0;
  for( int i = 0; i < size; i++ )
  {
    uint16_t next = t_sci[ifNumber].txWptr + 1;  /* the next pointer. */
    if( next >= t_sci[ifNumber].txSize ) next = 0;  /* round the next pointer. */
    if( next == t_sci[ifNumber].txRptr ) break;  /* When it cannot be assigned. */
    t_sci[ifNumber].txBuf[ t_sci[ifNumber].txWptr ] = *str++;  /* Assign to the buffer. */
    __disable_irq();
    t_sci[ifNumber].txWptr = next;  /* Update the write pointer. */
    __enable_irq();
    USART_ITConfig( USARTx, USART_IT_TXE, ENABLE );  /* Permit transmit interrupt. */
    count++;  /* Update the written count value. */
  }

  return count;
}

/* ----------------------------------------
    read
---------------------------------------- */
int USART_UART::read()
{
  if( rxAvailable() == 0 ) return (-1);
  int c = t_sci[ifNumber].rxBuf[ t_sci[ifNumber].rxRptr ];
  uint16_t next = t_sci[ifNumber].rxRptr + 1;
  if( next >= t_sci[ifNumber].rxSize ) next = 0;
  __disable_irq();
  t_sci[ifNumber].rxRptr = next;
  __enable_irq();

  return c & 0x00FF;
}

int USART_UART::read( uint8_t *dst, int size, TMO tmout )
{
  SYSTIM baseTim = systim;
  for( int i = 0; i < size; )
  {
    if( (systim - baseTim) > tmout ) return (-1);
    int c = read();
    if( c < 0 ){ dly_tsk( 2UL ); continue; }
    baseTim = systim;
    *dst++ = (uint8_t)c;
    i++;
  }
  return 0;
}

/* ----------------------------------------
    print and printf
---------------------------------------- */
int USART_UART::print( const char *str )
{
  return write( (const uint8_t *)str, strlen(str) );
}

int USART_UART::printf( const char *fmt, ... )
{
  char buf[256];
  va_list args;
  va_start( args, fmt );
  vsnprintf( buf, sizeof(buf), fmt, args );
  va_end( args );
  int ret = print( (const char *)buf );

  return ret;
}

/* ----------------------------------------
    get line.
---------------------------------------- */
char *USART_UART::getLine( char *dst, int size )
{
  #define  BACKSPACE   0x08
  #define  USER_BREAK  0x03
  #define  CTRL_C      0x03
  #define  CTRL_Z      0x1a

  char *str = dst;
  while( 1 )
  {
    while( rxAvailable() < 0 ) rot_rdq();  /* Waiting to get a character. */
    char c = (char)read();
    if( c == USER_BREAK ) return 0;  /* End when user break. */
    write( c );  /* echo back */
    if( c == '\r' ) break;  /* I found a line break, so next process. */
    if( str == &dst[ size ] )  /* The buffer was full, so I stopped taking in any more. */
    {
      purge();  /* Discard the input and exit. */
      if( str > dst ) str--;  /* Force conversion to string. */
      *str = '\0';  /* Force conversion to string. */
      return dst;
    }
    if( c == BACKSPACE )  /* Correspondence to BACK SPACE. */
    {
      if( str != dst ) str--;
      continue;
    }
    *str++ = c;
  }
  *str = '\0';  /* Convert to string in lines. */

  return dst;
}

/* ----------------------------------------
    rx buffer purge.
---------------------------------------- */
void USART_UART::purge()
{
  t_sci[ifNumber].rxWptr = t_sci[ifNumber].rxRptr = 0;
}

/* ----------------------------------------
    echo on/off.
---------------------------------------- */
void USART_UART::echo( bool onOff )
{
  t_sci[ifNumber].echo = onOff;
}

/* ----------------------------------------
    loop back.
---------------------------------------- */
void USART_UART::loopBack()
{
  while( true )
  {
    int c = read();
    if( c >= 0 ) write( (uint8_t)c );
    rot_rdq();
  }
}


extern "C"
{
/* ----------------------------------------
    c source putc for debug message.
---------------------------------------- */
void serialByteOutput( int ifNumber, uint8_t c )
{
  USART_TypeDef *USARTx;
  if( ifNumber == USART1_IF_NUMBER ) USARTx = USART1;
  else if( ifNumber == USART2_IF_NUMBER ) USARTx = USART2;
  else if( ifNumber == USART3_IF_NUMBER ) USARTx = USART3;
  else if( ifNumber == UART4_IF_NUMBER ) USARTx = UART4;
  else if( ifNumber == UART5_IF_NUMBER ) USARTx = UART5;
  else if( ifNumber == USART6_IF_NUMBER ) USARTx = USART6;
  else return;

  uint16_t next = t_sci[ifNumber].txWptr + 1;  /* the next pointer. */
  if( next >= t_sci[ifNumber].txSize ) next = 0;  /* round the next pointer. */
  if( next == t_sci[ifNumber].txRptr ) return;  /* When it cannot be assigned. */
  t_sci[ifNumber].txBuf[ t_sci[ifNumber].txWptr ] = c;  /* Assign to the buffer. */
  __disable_irq();
  t_sci[ifNumber].txWptr = next;  /* Update the write pointer. */
  __enable_irq();
  USART_ITConfig( USARTx, USART_IT_TXE, ENABLE );  /* Permit transmit interrupt. */
}

void serialStringOutput( int ifNumber, const char *str )
{
  while( *str ) { serialByteOutput( ifNumber, (uint8_t)*str++ ); }
}


/* ----------------------------------------
    interrupt operations.
---------------------------------------- */
static void sciCallBack( int ifNum )
{
  USART_TypeDef *SCI;
  if( ifNum == USART1_IF_NUMBER ) SCI = USART1;
  else if( ifNum == USART2_IF_NUMBER ) SCI = USART2;
  else if( ifNum == USART3_IF_NUMBER ) SCI = USART3;
  else if( ifNum == UART4_IF_NUMBER ) SCI = UART4;
  else if( ifNum == UART5_IF_NUMBER ) SCI = UART5;
  else if( ifNum == USART6_IF_NUMBER ) SCI = USART6;
  else return;

  volatile int data;
  /* Check for various errors. */
  if( USART_GetFlagStatus( SCI, USART_FLAG_FE ) == SET )
  {
    data = USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_IT_FE );
    t_sci[ifNum].FE_cnt++;  /* Cumulative number of framing errors. */
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_PE ) == SET )
  {
    data = USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_FLAG_PE );
    t_sci[ifNum].PE_cnt++;  /* Cumulative number of parity errors. */
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_NE ) == SET )
  {
    data = USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_FLAG_NE );
    t_sci[ifNum].NE_cnt++;  /* Cumulative number of noise errors. */
  }

  if( USART_GetFlagStatus( SCI, USART_FLAG_ORE ) == SET )
  {
    data = USART_ReceiveData( SCI );
    USART_ClearITPendingBit( SCI, USART_FLAG_ORE );
    t_sci[ifNum].ORE_cnt++;  /* Cumulative number of overrun errors. */
  }

  uint16_t next;
  /* Determination of receive interrupt. */
  if( USART_GetFlagStatus( SCI, USART_FLAG_RXNE ) == SET )
  {
    data = USART_ReceiveData( SCI );
    if( t_sci[ifNum].echo ) USART_SendData( SCI, data );
    next = t_sci[ifNum].rxWptr + 1;
    if( next >= t_sci[ifNum].rxSize ) next = 0;
    if( next != t_sci[ifNum].rxRptr )
    {
      t_sci[ifNum].rxBuf[ t_sci[ifNum].rxWptr ] = (uint8_t)data;
      t_sci[ifNum].rxWptr = next;
    }
    else { /* flow control. */ }
  }

  /* Determination of transmit interrupt. */
  if( USART_GetFlagStatus( SCI, USART_FLAG_TXE ) == SET )
  {
    if( t_sci[ifNum].txRptr == t_sci[ifNum].txWptr )  /* When there is no data to be sent. */
    {
      USART_ITConfig( SCI, USART_IT_TXE, DISABLE );  /* Disable transmit interrupt. */
      USART_ClearITPendingBit( SCI, USART_FLAG_TXE );
      return;
    }
    data = t_sci[ifNum].txBuf[ t_sci[ifNum].txRptr ];
    USART_SendData( SCI, data );
    if( ++t_sci[ifNum].txRptr >= t_sci[ifNum].txSize ) t_sci[ifNum].txRptr = 0;
  }
}

//#define WEAK __attribute__ ((weak))
#define WEAK

/* ----------------------------------------
    USART1 interrupt.
---------------------------------------- */
void WEAK USART1_IRQHandler( void )
{
  sciCallBack( USART1_IF_NUMBER );
}
/* ----------------------------------------
    USART2 interrupt.
---------------------------------------- */
void WEAK USART2_IRQHandler( void )
{
  sciCallBack( USART2_IF_NUMBER );
}

/* ----------------------------------------
    USART3 interrupt.
---------------------------------------- */
void WEAK USART3_IRQHandler( void )
{
  sciCallBack( USART3_IF_NUMBER );
}

/* ----------------------------------------
    UART4 interrupt.
---------------------------------------- */
void WEAK UART4_IRQHandler( void )
{
  sciCallBack( UART4_IF_NUMBER );
}

/* ----------------------------------------
    UART5 interrupt.
---------------------------------------- */
void WEAK UART5_IRQHandler( void )
{
  sciCallBack( UART5_IF_NUMBER );
}

/* ----------------------------------------
    USART6 interrupt.
---------------------------------------- */
void WEAK USART6_IRQHandler( void )
{
  sciCallBack( USART6_IF_NUMBER );
}

#if 0
/* ----------------------------------------
    UART7 interrupt.
---------------------------------------- */
void WEAK UART7_IRQHandler( void )
{
  sciCallBack( UART7_IF_NUMBER );
}

/* ----------------------------------------
    UART8 interrupt.
---------------------------------------- */
void WEAK UART8_IRQHandler( void )
{
  sciCallBack( UART8_IF_NUMBER );
}
#endif

}  /* extern "C" */


