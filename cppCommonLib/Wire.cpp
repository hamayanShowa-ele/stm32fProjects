/* ----------------------------------------
  stm32f1xx and stm32f4xx i2c utilities code
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
#include  <Wire.h>

#define  TIMEOUT  5UL  /* 10ms */
/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern SYSTIM systim;

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
STM32F_I2C::STM32F_I2C()
{
}

STM32F_I2C::STM32F_I2C( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed )
{
  (void)begin( i2c, sda, scl, speed );
}

STM32F_I2C::~STM32F_I2C()
{
  end();
}

/* ----------------------------------------
    pins alternate
---------------------------------------- */
int STM32F_I2C::pinsAlternate()
{
  /* gpio set. */
  pinMode( sdaPort, sdaPin, OUTPUT_OPEN_DRAIN, GPIO_SPEED_FAST );
  pinMode( sclPort, sclPin, OUTPUT_OPEN_DRAIN, GPIO_SPEED_FAST );

  /* start condition. */
  digitalWrite( sdaPin, HIGH );  /* sda set 1 */
  digitalWrite( sclPin, HIGH );  /* scl set 1 */
  dly10us( 1UL );
  digitalWrite( sdaPin, LOW );  /* sda set 0 */
  dly10us( 1UL );
  for( int i = 0; i < 8; i++ )
  {
    digitalWrite( sclPin, LOW );  /* scl set 0 */
    dly10us( 1UL );
    digitalWrite( sclPin, HIGH );  /* scl set 1 */
    dly10us( 1UL );
  }
  /* stop condition. */
  digitalWrite( sdaPin, HIGH );  /* sda set 1 */

  /* check bus busy. */
  dly10us(5UL);
  if( !(sdaPort->IDR & sdaPin) || !(sclPort->IDR & sclPin) ) return I2C_ERROR_WIRE_OTHER_ERROR;

  /* alternate function set. */
//  GPIO_PinRemapConfig( GPIO_Remap_I2C1 , ENABLE );  // If you are using PB8 and PB9 in I2C1, then you will need a REMAP.
  pinMode( sdaPort, sdaPin, ALTERNATE_OD, GPIO_SPEED_FAST );
  pinMode( sclPort, sclPin, ALTERNATE_OD, GPIO_SPEED_FAST );

  return I2C_SUCCESS;
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
int STM32F_I2C::begin( I2C_TypeDef *i2c, int sda, int scl, uint32_t speed )
{
  /* initialize i2c bus. */
  if( i2c == I2C1 )
  {
//    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );
//    RCC_APB1PeriphResetCmd( RCC_APB1Periph_I2C1, DISABLE );
  }
  else if( i2c == I2C2 )
  {
//    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE );
//    RCC_APB1PeriphResetCmd( RCC_APB1Periph_I2C2, DISABLE );
  }
  else return I2C_ERROR_WIRE_OTHER_ERROR;

  /* save scl and sda pin numbers. */
  wire = i2c;
  sdaPin = whatPin( sda );
  sclPin =  whatPin( scl );
  sdaPort = whatGPIOType( sda );
  sclPort = whatGPIOType( scl );

  /* clear i2c bus. */
//  if( pinsAlternate() < 0 ) return I2C_ERROR_WIRE_OTHER_ERROR;

  I2C_DeInit( wire );
  I2C_InitTypeDef I2C_InitStruct;
  I2C_StructInit( &I2C_InitStruct );
  I2C_InitStruct.I2C_ClockSpeed = speed;
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init( wire, &I2C_InitStruct );

  /* start i2c */
  I2C_Cmd( wire, ENABLE );

  return I2C_SUCCESS;
}

void STM32F_I2C::end()
{
  pinMode( sdaPort, sdaPin, INPUT_PULLUP );
  pinMode( sclPort, sclPin, INPUT_PULLUP );
}


/* ----------------------------------------
    set clock
---------------------------------------- */
void STM32F_I2C::setClock( uint32_t speed )
{
  I2C_InitTypeDef I2C_InitStruct;
  I2C_StructInit( &I2C_InitStruct );
  I2C_InitStruct.I2C_ClockSpeed = speed;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_Init( wire, &I2C_InitStruct );
}

/* ----------------------------------------
    begin transmission
---------------------------------------- */
int STM32F_I2C::beginTransmission( uint8_t adr )
{
  adr <<= 1;
  SYSTIM baseTim;

  /* While the bus is busy */
  baseTim = systim;
  while( I2C_GetFlagStatus(wire, I2C_FLAG_BUSY) == SET )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_BUS_BUSY;
    }
    rot_rdq();
  }

  /* initiate start sequence */
  I2C_GenerateSTART( wire, ENABLE );
  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  baseTim = systim;
  while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_MODE_SELECT) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_BUS_BUSY;
    }
    rot_rdq();
  }

  //Send address for write
  I2C_Send7bitAddress( wire, adr, I2C_Direction_Transmitter );
  //EV6 and clear it：アドレス送信確認、送信データレジスタ空き、データbyte(アドレスのR/Wbit)送信、
  //通信中(BUS BUSY)、マスター
  baseTim = systim;
  while( I2C_CheckEvent(wire,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_ERROR_WIRE_RECEIVED_NACK_OF_ADDRESS;
    }
    rot_rdq();
  }

  return I2C_SUCCESS;
}

int STM32F_I2C::addressTransmitOnly( uint8_t adr )
{
  adr <<= 1;
  SYSTIM baseTim;
  /* While the bus is busy */
  baseTim = systim;
  while( I2C_GetFlagStatus(wire, I2C_FLAG_BUSY) == SET )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_BUS_BUSY;
    }
    rot_rdq();
  }

  /* initiate start sequence */
  I2C_GenerateSTART( wire, ENABLE );
  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  baseTim = systim;
  while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_MODE_SELECT) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_BUS_BUSY;
    }
    rot_rdq();
  }

  //Send address for write
  I2C_Send7bitAddress( wire, adr, I2C_Direction_Transmitter );

  return I2C_SUCCESS;
}

/* ----------------------------------------
    end transmission
---------------------------------------- */
void STM32F_I2C::endTransmission()
{
  //stop condition
  I2C_GenerateSTOP( wire, ENABLE );

  /*stop bit flag*/
  SYSTIM baseTim = systim;
  while( I2C_GetFlagStatus( wire, I2C_FLAG_STOPF ) == SET )
  {
    if( (systim - baseTim) >= TIMEOUT ) return;
    rot_rdq();
  }
}

/* ----------------------------------------
    request from
---------------------------------------- */
int STM32F_I2C::requestFrom( uint8_t adr )
{
  adr <<= 1;
  SYSTIM baseTim;

  /* While the bus is busy */
  baseTim = systim;
  while( I2C_GetFlagStatus(wire, I2C_FLAG_BUSY) == SET )
  {
    if( (systim - baseTim) >= TIMEOUT ) return I2C_BUS_BUSY;
    rot_rdq();
  }

  /* initiate start sequence */
//  I2C_AcknowledgeConfig( wire, ENABLE );  //ACK
  I2C_GenerateSTART( wire, ENABLE );

  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  baseTim = systim;
  while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_MODE_SELECT) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_BUS_BUSY;
    }
    rot_rdq();
  }

  /* Send module address and behave as receiver */
  I2C_Send7bitAddress( wire, adr, I2C_Direction_Receiver );
  /* Test on EV6 and clear it */
  baseTim = systim;
  while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT ) return I2C_ERROR_WIRE_RECEIVED_NACK_OF_ADDRESS;
    rot_rdq();
  }

  return I2C_SUCCESS;
}

/* ----------------------------------------
    write
---------------------------------------- */
int STM32F_I2C::write( uint8_t c )
{
  SYSTIM baseTim;

  //Send the data to write to
  I2C_SendData( wire, c );
  /* Test on EV8 and clear it */
  baseTim = systim;
  while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT )
    {
      return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;
    }
    rot_rdq();
  }

  return 1;
}

int STM32F_I2C::write( const uint8_t *data, size_t size )
{
  SYSTIM baseTim;

  //Send the data to write to
  int count;
  for( count = 0; count < (int)size; count++ )
  {
    I2C_SendData( wire, *data++ );
    /* Test on EV8 and clear it */
    baseTim = systim;
    while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR )
    {
      if( (systim - baseTim) >= TIMEOUT )
      {
        return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;
      }
      rot_rdq();
    }
  }

  return count;
}

/* ----------------------------------------
    read
---------------------------------------- */
int STM32F_I2C::read( bool ack )
{
  SYSTIM baseTim = systim;
  while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT ) return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;
    rot_rdq();
  }
//  while( !(wire->SR1 & I2C_SR1_RXNE) ) {}
  /* Read a byte from the module */
  int c = I2C_ReceiveData( wire );
  /* Disable Acknowledgment */
  I2C_AcknowledgeConfig( wire, (ack) ? DISABLE : ENABLE );

  return c & 0x00FF;
}

int STM32F_I2C::read( uint8_t *data, size_t size )
{
  /* Enable Acknowledgment */
  I2C_AcknowledgeConfig( wire, ENABLE );
  /* Read a byte from the module */
  int count = 0;
  for( ; count < (int)size; count++ )
  {
    SYSTIM baseTim = systim;
    while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR )
//    while( !(wire->SR1 & I2C_SR1_RXNE) )
    {
   	  if( (systim - baseTim) >= TIMEOUT )
      {
        I2C_AcknowledgeConfig( wire, DISABLE );
        return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;
      }
      rot_rdq();
    }
    *data++ = I2C_ReceiveData( wire );
  }

  /* Disable Acknowledgment */
  I2C_AcknowledgeConfig( wire, DISABLE );

  return count;
}

int STM32F_I2C::read( const uint8_t *snd, size_t sndSize, uint8_t *rcv, size_t rcvSize )
{
  SYSTIM baseTim;
  int count;

  //Send the data to write to
  for( count = 0; count < (int)sndSize; count++ )
  {
    I2C_SendData( wire, *snd++ );
    /* Test on EV8 and clear it */
    baseTim = systim;
    while( I2C_CheckEvent(wire, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR )
    {
      if( (systim - baseTim) >= TIMEOUT )
      {
        return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;
      }
      rot_rdq();
    }
  }

  //restart condition
  I2C_GenerateSTART( wire, DISABLE );
  I2C_GenerateSTART( wire, ENABLE );
  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  baseTim = systim;
  while(I2C_CheckEvent( wire, I2C_EVENT_MASTER_MODE_SELECT) == ERROR )
  {
    if( (systim - baseTim) >= TIMEOUT ) return I2C_BUS_BUSY;
    rot_rdq();
  }

  /* Enable Acknowledgment */
  I2C_AcknowledgeConfig( wire, ENABLE );
  /* Read a byte from the module */
  for( count = 0; count < (int)rcvSize; count++ )
  {
    baseTim = systim;
    while( !I2C_CheckEvent(wire, I2C_EVENT_MASTER_BYTE_RECEIVED) )
    {
      if( (systim - baseTim) >= TIMEOUT ) return I2C_ERROR_WIRE_RECEIVED_NACK_OF_DATA;
      rot_rdq();
    }
    *rcv++ = I2C_ReceiveData( wire );
  }

  /* Disable Acknowledgment */
  I2C_AcknowledgeConfig( wire, DISABLE );

  return count;
}

/* ----------------------------------------
    i2c general call reset.
---------------------------------------- */
void STM32F_I2C::generalCallReset()
{
  /* general call reset. */
  beginTransmission( 0x00 );
  write( 0x06 );
  endTransmission();
}

/* ----------------------------------------
    i2c software reset.
---------------------------------------- */
void STM32F_I2C::softReset()
{
  I2C_SoftwareResetCmd( wire, ENABLE );
}
