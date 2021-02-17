/* ----------------------------------------
 USART,UART utility header
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
#ifndef  __USART_UART_H__
#define  __USART_UART_H__

#include  <stm32fPeripheral.h>
#include  <stdio.h>
#include  <gpio.h>
#include  <derivertive.h>

extern "C"
{
  #include  <mul_tsk.h>
  #include  <stdarg.h>
  #include  <stdio.h>
  #include  <string.h>

  void serialByteOutput( int ifNumber, uint8_t c );
  void serialStringOutput( int ifNumber, const char *str );
}

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    defines
---------------------------------------- */
#define  USART_SUCCESS         (0)
#define  USART_IF_ERROR        (-1)
#define  USART_DEFINITION_ERROR (-2)


typedef struct
{
  uint8_t *txBuf;
  uint8_t *rxBuf;

  uint16_t txSize;
  uint16_t rxSize;

  uint16_t txWptr;
  uint16_t txRptr;
  uint16_t rxWptr;
  uint16_t rxRptr;

  uint32_t FE_cnt;  /* Cumulative number of framing errors. */
  uint32_t PE_cnt;  /* Cumulative number of parity errors. */
  uint32_t NE_cnt;  /* Cumulative number of noise errors. */
  uint32_t ORE_cnt; /* Cumulative number of overrun errors. */

  bool echo;
} T_SCI_BUFFER;

enum SCI_IF_NUMBERS
{
  USART1_IF_NUMBER = 0,
  USART2_IF_NUMBER,
  USART3_IF_NUMBER,
  UART4_IF_NUMBER,
  UART5_IF_NUMBER,
  USART6_IF_NUMBER,
  UART7_IF_NUMBER,
  UART8_IF_NUMBER,
};

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
class USART_UART : public GPIO
{
private:
  int ifNumber;
  USART_TypeDef *USARTx;
  uint8_t txPin,rxPin;
  uint32_t brr;

  uint16_t txAvailable();
  uint16_t rxAvailable();
//  #define  available()  rxAvailable()
  void isrCallBack( void );

public:
  USART_UART();
  ~USART_UART();

  int begin(
    USART_TypeDef *_usart, uint32_t brr,
    uint8_t _tx, uint8_t _rx,
    uint8_t basePri = BASE_PRIORITY, uint8_t subPri = BASE_SUB_PRIORITY );
  void end();

  int write( uint8_t c );
  int write( const uint8_t *str, int size );
  int print( const char *str );
  int printf( const char *fmt, ... );
  int read();
  int read( uint8_t *dst, int size, TMO tmout = 100UL );

  char *getLine( char *dst, int size );
  void purge();
  void echo( bool onOff = true );
  void loopBack();
};


#endif  /* __USART_UART_H__ */

