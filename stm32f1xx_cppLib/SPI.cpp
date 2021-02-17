/* ----------------------------------------
  spi utilities
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
#include  "spi.h"

#define  TIMEOUT  5UL  /* 5ms */

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
extern SYSTIM systim;
static ID semaIDv[3];

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    begin and end
    w5100 is only supported in mode 0 or mode 3???.
---------------------------------------- */
int SPI::begin( SPI_TypeDef *spi, ID id,
  uint8_t _sck, uint8_t _mosi, uint8_t _miso )
{
  SPIx = spi;
  semaID = id;
  sck = _sck;
  mosi = _mosi;
  miso = _miso;

  /* spi interface case by case. */
  if( SPIx == SPI1 )
  {
    rccClockEnable( RCC_SPI1 );
    if( sck == PB3 && mosi == PB5 && miso == PB4 )  // need remap.
    {
      rccClockEnable( RCC_GPIOB );
      remap( REMAP_SPI1 );
    }
    else if( sck == PA5 && mosi == PA7 && miso == PA6 )  // not need remap.
    {
      rccClockEnable( RCC_GPIOA );
    }
    else return SPI_DEFINITION_ERROR;
  }
  else if( SPIx == SPI2 )
  {
    if( sck == PB13 && mosi == PB15 && miso == PB14 )  // not need remap.
    {
      rccClockEnable( RCC_SPI2 );
      rccClockEnable( RCC_GPIOB );
    }
    else return SPI_DEFINITION_ERROR;
  }
  else if( SPIx == SPI3 )
  {
    if( sck == PB3 && mosi == PB5 && miso == PB4 )  // not need remap.
    {
      rccClockEnable( RCC_SPI3 );
      rccClockEnable( RCC_GPIOB );
    }
    else return SPI_DEFINITION_ERROR;
  }
  else return SPI_IF_ERROR;

  /* spi gpio configuration. */
  /* alternate functions. */
  pinMode( sck, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( mosi, ALTERNATE_PP, GPIO_SPEED_FAST );
  pinMode( miso, INPUT, GPIO_SPEED_FAST );

  /* spi configuration. */
  SPI_InitTypeDef   SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // full duplex
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // mode is master.
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // data length
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  /* mode 0 */
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  /* APB1:36MHz PB2:72MHz */
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // 72000kHz/4=18MHz This frequency is the limit.
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 72000kHz/8=9MHz
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init( SPIx, &SPI_InitStructure );
  SPI_CalculateCRC( SPIx, DISABLE );
  SPI_Cmd( SPIx, ENABLE );

  /* drain SPI */
  while( SPI_I2S_GetFlagStatus( SPIx, SPI_I2S_FLAG_TXE ) == RESET ) {}
  volatile uint8_t dummyread = SPI_I2S_ReceiveData( SPIx );

  return SPI_SUCCESS;
}

void SPI::end()
{
  if( SPIx == SPI1 )
  {
    SPI_I2S_DeInit( SPIx );
    RCC_APB2PeriphClockCmd( rcc, DISABLE );
    RCC_APB2PeriphResetCmd( rcc, ENABLE );
  }
  else if( SPIx == SPI2 || SPIx == SPI3 )
  {
    SPI_I2S_DeInit( SPIx );
    RCC_APB1PeriphClockCmd( rcc, DISABLE );
    RCC_APB1PeriphResetCmd( rcc, ENABLE );
  }
  else return;

  pinMode( sck, INPUT_PULLUP );
  pinMode( mosi, INPUT_PULLUP );
}

/* ----------------------------------------
    wait semaphore
---------------------------------------- */
int SPI::waiSema()
{
  if( semaID )
  {
#if 0
    /* Wait for the spi semaphore to be used. */
    SYSTIM baseTim = systim;
    while( pol_sem( semaID ) != E_OK )
    {
      if( (systim - baseTim) >= TIMEOUT ) return SPI_SEMAPHORE_BUSY;
      rot_rdq();
    }
#else
    wai_sem( semaID );
#endif
    return SPI_SUCCESS;
  }
  else return SPI_SUCCESS;
}

void SPI::sigSema()
{
  /* end of process. */
  if( semaID ) sig_sem( semaID );
}


/* ----------------------------------------
    read and write
---------------------------------------- */
int SPI::readWrite( uint8_t wData )
{
  int ret;
  /* Wait for the spi semaphore. */
  if( (ret = waiSema()) != SPI_SUCCESS ) return ret;

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData( SPIx, wData );

  /* Wait to receive a byte */
  SYSTIM baseTim = systim;
  while( SPI_I2S_GetFlagStatus( SPIx, SPI_I2S_FLAG_RXNE ) == RESET )
  {
    if( (systim - baseTim) >= TIMEOUT ) { sigSema(); return SPI_RECIEVE_TIMEOUT; }
    rot_rdq();
  }

  /* Return the byte read from the SPI bus */
  uint8_t rcv = SPI_I2S_ReceiveData( SPIx );

  /* end of process. */
  sigSema();
  return rcv & 0x00FF;
}


extern "C"
{

/* ----------------------------------------
    spi read/write
---------------------------------------- */
uint8_t spi_rw_SPI1( uint8_t out )
{
  /* Loop while DR register in not empty */
  /// not needed: while (SPI_I2S_GetFlagStatus( SPI2, SPI_I2S_FLAG_TXE) == RESET) { ; }

  /* Send byte through the SPIx peripheral */
  SPI_I2S_SendData( SPI1, out );

  /* Wait to receive a byte */
  while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE ) == RESET ) rot_rdq();

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData( SPI1 );
}

uint8_t spi_rw_SPI2( uint8_t out )
{
  /* Send byte through the SPIx peripheral */
  SPI_I2S_SendData( SPI2, out );
  /* Wait to receive a byte */
  while( SPI_I2S_GetFlagStatus( SPI2, SPI_I2S_FLAG_RXNE ) == RESET ) rot_rdq();
  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData( SPI2 );
}

uint8_t spi_rw_SPI3( uint8_t out )
{
  /* Send byte through the SPIx peripheral */
  SPI_I2S_SendData( SPI3, out );
  /* Wait to receive a byte */
  while( SPI_I2S_GetFlagStatus( SPI3, SPI_I2S_FLAG_RXNE ) == RESET ) rot_rdq();
  /* Return the byte read from the SPI bus */
  uint8_t tempUC = SPI_I2S_ReceiveData( SPI3 );
  return tempUC;
}

/* ----------------------------------------
    wait semaphore
---------------------------------------- */
void waiSema_SPI1( void ) { wai_sem( semaIDv[0] ); }
void waiSema_SPI2( void ) { wai_sem( semaIDv[1] ); }
void waiSema_SPI3( void ) { wai_sem( semaIDv[2] ); }

void sigSema_SPI1( void ) { sig_sem( semaIDv[0] ); }
void sigSema_SPI2( void ) { sig_sem( semaIDv[1] ); }
void sigSema_SPI3( void ) { sig_sem( semaIDv[2] ); }

/* ----------------------------------------
    SPI1 interrupt handler
---------------------------------------- */
void SPI1_IRQHandler( void )
{
  ITStatus flag;

  /* Examine the interruption factors. */
  flag = SPI_I2S_GetITStatus( SPI1, SPI_I2S_IT_TXE );
  if(flag == SET)  // Transmission Empty
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI1, SPI_I2S_IT_RXNE );
  if(flag == SET)  // full reception
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI1, SPI_I2S_IT_OVR | SPI_IT_MODF );
  if(flag == SET)  // error
  {
    ;
  }
}

/* ----------------------------------------
    SPI2 interrupt handler
---------------------------------------- */
void SPI2_IRQHandler( void )
{
  ITStatus flag;

  /* Examine the interruption factors. */
  flag = SPI_I2S_GetITStatus( SPI2, SPI_I2S_IT_TXE );
  if(flag == SET)  // Transmission Empty
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI2, SPI_I2S_IT_RXNE );
  if(flag == SET)  // full reception
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI2, SPI_I2S_IT_OVR | SPI_IT_MODF );
  if(flag == SET)  // error
  {
    ;
  }
}

/* ----------------------------------------
    SPI3 interrupt handler
---------------------------------------- */
void SPI3_IRQHandler( void )
{
  ITStatus flag;

  /* Examine the interruption factors. */
  flag = SPI_I2S_GetITStatus( SPI3, SPI_I2S_IT_TXE );
  if(flag == SET)  // Transmission Empty
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI3, SPI_I2S_IT_RXNE );
  if(flag == SET)  // full reception
  {
    ;
  }

  flag = SPI_I2S_GetITStatus( SPI3, SPI_I2S_IT_OVR | SPI_IT_MODF );
  if(flag == SET)  // error
  {
    ;
  }
}
}  /* extern "C" */
