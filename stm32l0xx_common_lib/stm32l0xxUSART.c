/* ----------------------------------------
  USART utilities
  for STMicroelectronics HAL library

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
#include  "stm32l0xxUSART.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */
void HAL_UARTEx_RxEventCallback( UART_HandleTypeDef *huart, uint16_t Size );

static void ireqByteRcv( UART_HandleTypeDef *huart );
static int  startRcvInterrupt( UART_HandleTypeDef *huart );

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
UART_HandleTypeDef UartHandle[2];
uint32_t rcvOverflowCount[2];

static uint8_t rcvTemporaryBuffer[2][1];
int sciWptr[2],sciRptr[2];
// static uint8_t sci1TxBuffer[ SCI_1_SND_BUFFER_SIZE ];
uint8_t sci1RxBuffer[ SCI_1_RCV_BUFFER_SIZE ];
// static uint8_t sci2TxBuffer[ SCI_2_SND_BUFFER_SIZE ];
uint8_t sci2RxBuffer[ SCI_2_RCV_BUFFER_SIZE ];

/* ----------------------------------------
    start recieve interrupt.
---------------------------------------- */
static int startRcvInterrupt( UART_HandleTypeDef *huart )
{
  /* Computation of UART mask to apply to RDR register */
  UART_MASK_COMPUTATION( huart );

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

  huart->RxISR = ireqByteRcv;

  __HAL_UNLOCK( huart );

  /* Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
  SET_BIT(huart->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);

  return HAL_OK;
}

/* ----------------------------------------
    usart begin
---------------------------------------- */
int usartBegin( USART_TypeDef *USARTx, uint32_t brr, uint8_t txPin, uint8_t rxPin )
{
  int handle,irqNumber;

  if( USARTx == USART1 )
  {
    if( txPin == PA9 && rxPin == PA10 )
    {
      pinAlternateMode( txPin, GPIO_MODE_AF_PP, GPIO_AF4_USART1 );
      pinAlternateMode( rxPin, GPIO_MODE_AF_PP, GPIO_AF4_USART1 );
    }
    else if( txPin == PB6 && rxPin == PB7 )
    {
      pinAlternateMode( txPin, GPIO_MODE_AF_PP, GPIO_AF0_USART1 );
      pinAlternateMode( rxPin, GPIO_MODE_AF_PP, GPIO_AF0_USART1 );
    }
    else return USART_ERR;
    __HAL_RCC_USART1_CLK_ENABLE();
    handle = 0;
    irqNumber = USART1_IRQn;
  }
  else if( USARTx == USART2 )
  {
    if( txPin == PA2 && rxPin == PA3 )
    {
      pinAlternateMode( txPin, GPIO_MODE_AF_PP, GPIO_AF4_USART2 );
      pinAlternateMode( rxPin, GPIO_MODE_AF_PP, GPIO_AF4_USART2 );
    }
    else if( txPin == PA14 && rxPin == PA15 )
    {
      pinAlternateMode( txPin, GPIO_MODE_AF_PP, GPIO_AF4_USART2 );
      pinAlternateMode( rxPin, GPIO_MODE_AF_PP, GPIO_AF4_USART2 );
    }
    else return USART_ERR;
    __HAL_RCC_USART2_CLK_ENABLE();
    handle = 1;
    irqNumber = USART2_IRQn;
  }
  else return USART_ERR;

  UartHandle[handle].Instance        = USARTx;
  UartHandle[handle].Init.BaudRate   = brr;
  UartHandle[handle].Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle[handle].Init.StopBits   = UART_STOPBITS_1;
  UartHandle[handle].Init.Parity     = UART_PARITY_NONE;
  UartHandle[handle].Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle[handle].Init.Mode       = UART_MODE_TX_RX;

  if( HAL_UART_Init( &UartHandle[handle] ) != HAL_OK )
  {
    return USART_ERR;
  }

  nvicSetIRQ( irqNumber, BASE_PRIORITY, BASE_SUB_PRIORITY );

  return handle;
}

/* ----------------------------------------
    usart block write
---------------------------------------- */
int usartBlockWrite( int handle, const uint8_t *data, int size )
{
  int result = HAL_UART_Transmit_IT(
    &UartHandle[handle], (uint8_t*)data, size );

  if( result != HAL_OK ) return USART_ERR;
  return USART_OK;
}

/* ----------------------------------------
    usart block read
---------------------------------------- */
int usartBlockRead( int handle, uint8_t *data, int size )
{
  int result = HAL_UART_Receive_IT(
    &UartHandle[handle], data, size );

  if( result != HAL_OK ) return USART_ERR;
  return USART_OK;
}

/* ----------------------------------------
    usart read start
---------------------------------------- */
int usartReadStart( int handle )
{
#if 0
  int result = HAL_UART_Receive_IT(
    &UartHandle[handle], rcvTemporaryBuffer[handle], 1 );
#else
  int result = startRcvInterrupt( &UartHandle[handle] );
#endif

  if( result != HAL_OK ) return USART_ERR;
  return USART_OK;
}

/* ----------------------------------------
    available for recieve buffer.
---------------------------------------- */
int usartAvailable( int handle )
{
  if( sciWptr[handle] == sciRptr[handle] ) return 0;
  uint32_t tempUL = sciWptr[handle] - sciRptr[handle];
  return tempUL % ((handle == 0) ? SCI_1_RCV_BUFFER_SIZE : SCI_2_RCV_BUFFER_SIZE);
}

/* ----------------------------------------
    1byte read from recieve buffer.
---------------------------------------- */
int usartByteRead( int handle )
{
  if( usartAvailable( handle ) == 0 ) return (-1);
  uint32_t next = sciRptr[handle] + 1;
  if( next >= ((handle == 0) ? SCI_1_RCV_BUFFER_SIZE : SCI_2_RCV_BUFFER_SIZE) )
  {
    next = 0;
  }
  uint8_t *buf;
  if( handle == 0 ) buf = sci1RxBuffer;
  else buf = sci2RxBuffer;
  int tempI = buf[ sciRptr[handle] ];
  sciRptr[handle] = next;

  return tempI & 0x00FF;
}

/* ----------------------------------------
    1byte write.
---------------------------------------- */
int usartByteWrite( int handle, uint8_t data )
{
  uint8_t buf[1];
  buf[0] = data;
  return usartBlockWrite( handle, (const uint8_t *)buf, 1 );
}

/* ----------------------------------------
    print.
---------------------------------------- */
int usartPrint( int handle, const char *str )
{
  return usartBlockWrite( handle, (const uint8_t *)str, strlen(str) );
}

/* ----------------------------------------
    printf.
---------------------------------------- */
int usartPrintf( int handle, const char *fmt, ... )
{
  char buf[128];
  va_list args;
  va_start( args, fmt );
  vsnprintf( buf, sizeof(buf), fmt, args );
  va_end( args );

  return usartBlockWrite( handle, (const uint8_t *)buf, strlen(buf) );
}


/* ----------------------------------------
    call back functions.
---------------------------------------- */
static void cbUSARTx_Recieved(
  UART_HandleTypeDef *huart,
  int handle, uint8_t *buf, uint32_t bufferSize )
{
//  UART_Start_Receive_IT( huart, rcvTemporaryBuffer[handle], 1 );
//  HAL_UART_Receive_IT( huart, rcvTemporaryBuffer[handle], 1 );
  uint8_t rcv = rcvTemporaryBuffer[handle][0];
  uint32_t next = sciWptr[handle] + 1;
  if( next >= bufferSize ) next = 0;
  if( next == sciRptr[handle] )
  {
    rcvOverflowCount[handle]++;
    return;
  }
  buf[ sciWptr[handle] ] = rcv;
  sciWptr[handle] = next;
}

void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart )
{
  if( huart->Instance == USART1 )
  {
    cbUSARTx_Recieved( huart, 0, sci1RxBuffer, SCI_1_RCV_BUFFER_SIZE );
  }
  else if( huart->Instance == USART2 )
  {
    cbUSARTx_Recieved( huart, 1, sci2RxBuffer, SCI_2_RCV_BUFFER_SIZE );
  }
}


/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles UART interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream
  *         used for USART data transmission
  */
void USART1_IRQHandler( void )
{
  HAL_UART_IRQHandler( &UartHandle[0] );
}

void USART2_IRQHandler( void )
{
  HAL_UART_IRQHandler( &UartHandle[1] );
}

/* ----------------------------------------
    recieve 8 bit data at interrupt handler.
---------------------------------------- */
static void ireqByteRcv( UART_HandleTypeDef *huart )
{
  uint16_t uhdata = (uint16_t) READ_REG(huart->Instance->RDR);

  /* Clear RXNE interrupt flag */
  __HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);

  int handle;
  uint32_t bufferSize;
  if(huart->Instance == USART1)
  {
    handle = 0;
    bufferSize = SCI_1_RCV_BUFFER_SIZE;
  }
  else
  {
    handle = 1;
    bufferSize = SCI_2_RCV_BUFFER_SIZE;
  }

  uint32_t next = sciWptr[handle] + 1;
  if( next >= bufferSize ) next = 0;
  if( next == sciRptr[handle] ) return;

  sci1RxBuffer[ sciWptr[handle] ] = (uint8_t)uhdata;
  sciWptr[handle] = next;
}

