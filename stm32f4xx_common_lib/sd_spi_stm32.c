///*-----------------------------------------------------------------------*/
/* MMC/SDSC/SDHC (in SPI mode) control module for STM32 Version 1.1.0    */
/* (C) Martin Thomas, 2009 - based on the AVR MMC module (C)ChaN, 2007   */
/*-----------------------------------------------------------------------*/

/* Copyright (c) 2009, Martin Thomas, ChaN
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */


#include  "stm32f4xx.h"
#include  "diskio.h"
#include  "derivertive.h"
#include  "peripheral.h"
#include  "mul_tsk.h"

// demo uses a command line option for this (see Makefile):
#define STM32_USE_DMA

#ifdef STM32_USE_DMA
//#warning Information only - using DMA
#endif


//  #define CARD_SUPPLY_SWITCHABLE   0
  #define SOCKET_WP_CP_CONNECTED   1 /* */
//  #define GPIO_WP_CP               GPIOB
//  #define RCC_APB2Periph_GPIO_WP_CP  RCC_APB2Periph_GPIOB
//  #define GPIO_Pin_WP              GPIO_Pin_11
//  #define GPIO_Pin_CP              GPIO_Pin_10
//  #define GPIO_Mode_WP_CP          GPIO_Mode_IPU /* pull up resistors */
//  #define SPI_SD                   SPI2
//  #define GPIO_CS                  GPIOB
//  #define RCC_APB2Periph_GPIO_CS   RCC_APB2Periph_GPIOB
//  #define GPIO_Pin_CS              GPIO_Pin_12
//  #define DMA_Channel_SPI_SD_RX    DMA1_Channel4  //dma1 stream3 channel0
//  #define DMA_Channel_SPI_SD_TX    DMA1_Channel5  //dma1 stream4 channel0
//  #define DMA_FLAG_SPI_SD_TC_RX    DMA1_FLAG_TC4  //dma1 stream3 channel0
//  #define DMA_FLAG_SPI_SD_TC_TX    DMA1_FLAG_TC5  //dma1 stream4 channel0
//  #define GPIO_SPI_SD              GPIOB
//  #define GPIO_Pin_SPI_SD_SCK      GPIO_Pin_13
//  #define GPIO_Pin_SPI_SD_MISO     GPIO_Pin_14
//  #define GPIO_Pin_SPI_SD_MOSI     GPIO_Pin_15
//  #define RCC_APBPeriphClockCmd_SPI_SD  RCC_APB1PeriphClockCmd
//  #define RCC_APBPeriph_SPI_SD     RCC_APB1Periph_SPI2
 /* for SPI2 and full-speed APB1: 42MHz/2 */
 /* !! PRESCALE 4 used here - 2 does not work, maybe because 
       of the poor wireing on the HELI_V1 prototype hardware */
//  #define SPI_BaudRatePrescaler_SPI_SD  SPI_BaudRatePrescaler_4  /**/

/* Definitions for MMC/SDC command */
#define CMD0  (0x40+0)  /* GO_IDLE_STATE */
#define CMD1  (0x40+1)  /* SEND_OP_COND (MMC) */
#define ACMD41  (0xC0+41)  /* SEND_OP_COND (SDC) */
#define CMD8  (0x40+8)  /* SEND_IF_COND */
#define CMD9  (0x40+9)  /* SEND_CSD */
#define CMD10  (0x40+10)  /* SEND_CID */
#define CMD12  (0x40+12)  /* STOP_TRANSMISSION */
#define ACMD13  (0xC0+13)  /* SD_STATUS (SDC) */
#define CMD16  (0x40+16)  /* SET_BLOCKLEN */
#define CMD17  (0x40+17)  /* READ_SINGLE_BLOCK */
#define CMD18  (0x40+18)  /* READ_MULTIPLE_BLOCK */
#define CMD23  (0x40+23)  /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0xC0+23)  /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24  (0x40+24)  /* WRITE_BLOCK */
#define CMD25  (0x40+25)  /* WRITE_MULTIPLE_BLOCK */
#define CMD55  (0x40+55)  /* APP_CMD */
#define CMD58  (0x40+58)  /* READ_OCR */

/* Card-Select Controls  (Platform dependent) */
//#define SELECT()        SD_CS_IS_0    /* MMC CS = L */
//#define DESELECT()      SD_CS_IS_1      /* MMC CS = H */

/* Manley EK-STM32F board does not offer socket contacts -> dummy values: */
#define SOCKPORT  1      /* Socket contact port */
#define SOCKWP    1      /* Write protect switch (PB5) */
#define SOCKINS   1      /* Card detect switch (PB4) */

/*--------------------------------------------------------------------------

   Module Private Functions and Variables

---------------------------------------------------------------------------*/

typedef DWORD socket_state_t;

static volatile
DSTATUS Stat = STA_NOINIT;  /* Disk status */

static volatile
DWORD Timer1, Timer2;  /* 100Hz decrement timers */

static
BYTE CardType;      /* Card type flags */

enum speed_setting { INTERFACE_SLOW, INTERFACE_FAST };

static void interface_speed( enum speed_setting speed )
{
  DWORD tmp;

  tmp = SPI_SD->CR1;
  if ( speed == INTERFACE_SLOW )
  {
    /* Set slow clock (100k-400k) */
    tmp = ( tmp | SPI_BaudRatePrescaler_256 );  //164khz
  }
  else
  {
    /* Set fast clock (depends on the CSD) */
    tmp = ( tmp & ~SPI_BaudRatePrescaler_256 ) | SPI_BaudRatePrescaler_SPI_SD;
  }
  SPI_SD->CR1 = tmp;
}

#if SOCKET_WP_CP_CONNECTED

/* Socket's Write-Protection Pin: high = write-protected, low = writeable */
/* Socket's Card-Present Pin: high = socket empty, low = card inserted */

static void socket_wp_cp_init(void)
{
  GPIO_Terminal_Init( SD_CD_PORT, IN_UP, SD_CD_PIN );  //カード挿入で0、またはカード挿入無しで1
  GPIO_Terminal_Init( SD_WP_PORT, IN_UP, SD_WP_PIN );  //write protectで1、またはカード挿入無しで1
}

static inline socket_state_t socket_wp_cp_state(void)
{
  return (socket_state_t)(SD_CD_IS | SD_WP_IS);
}

static inline BOOL socket_is_empty( socket_state_t st )
{
  return ( st & SD_CD_PIN ) ? TRUE : FALSE;  //カード挿入で0、またはカード挿入無しで1
}

static inline BOOL socket_is_write_protected( socket_state_t st )
{
  return ( st & SD_WP_PIN ) ? TRUE : FALSE;  //write protectで1、またはカード挿入無しで1
}

#else

static void socket_wp_cp_init(void)
{
  return;
}

static inline socket_state_t socket_wp_cp_state(void)
{
  return 0;
}

static inline BOOL socket_is_empty( socket_state_t st )
{
  st = st;
  return FALSE; /* fake inserted */
}

static inline BOOL socket_is_write_protected( socket_state_t st )
{
  st = st;
  return FALSE; /* fake not protected */
}

#endif /* SOCKET_WP_CP_CONNECTED */


static void card_power(BYTE on)
{
  on=on;
}

static int chk_power(void) 
{
  return 1;
}


/*-----------------------------------------------------------------------*/
/* Transmit/Receive a byte to MMC via SPI  (Platform dependent)          */
/*-----------------------------------------------------------------------*/
static BYTE stm32_spi_rw( BYTE out )
{
  /* Loop while DR register in not empty */
  /// not needed: while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_TXE) == RESET) { ; }

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI_SD, out);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_RXNE) == RESET) { rot_rdq(); }

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI_SD);
}



/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

#define xmit_spi(dat)  stm32_spi_rw(dat)

/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/

static
BYTE rcvr_spi (void)
{
  return stm32_spi_rw(0xff);
}

/* Alternative macro to receive data fast */
#define rcvr_spi_m(dst)  *(dst)=stm32_spi_rw(0xff)



/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
BYTE wait_ready (void)
{
  BYTE res;


  Timer2 = 50;  /* Wait for ready in timeout of 500ms */
  rcvr_spi();
  do
  {
    rot_rdq();
    res = rcvr_spi();
  } while ((res != 0xFF) && Timer2) ;

  return res;
}



/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void release_spi (void)
{
  DESELECT();
  rcvr_spi();
}

#ifdef STM32_USE_DMA
/*-----------------------------------------------------------------------*/
/* Transmit/Receive Block using DMA (Platform dependent. STM32 here)     */
/*-----------------------------------------------------------------------*/
static
void stm32_dma_transfer(
  BOOL receive,      /* FALSE for buff->SPI, TRUE for SPI->buff               */
  const BYTE *buff,  /* receive TRUE  : 512 byte data block to be transmitted
                        receive FALSE : Data buffer to store received data    */
  UINT btr           /* receive TRUE  : Byte count (must be multiple of 2)
                        receive FALSE : Byte count (must be 512)              */
)
{
  DMA_InitTypeDef DMA_InitStructure;
  WORD rw_workbyte[] = { 0xffff };

  /* shared DMA configuration values */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (DWORD)(&(SPI_SD->DR));
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_BufferSize = btr;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

//  DMA_DeInit(DMA2_Stream0);
//  while( DMA_GetCmdStatus(DMA2_Stream0) == SET ) ;
  DMA_StructInit( &DMA_InitStructure );
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (DWORD)(&(SPI_SD->DR));
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_BufferSize = btr;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  //DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  //DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  //DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_DeInit(DMA_Channel_SPI_SD_RX);
  DMA_DeInit(DMA_Channel_SPI_SD_TX);

  if ( receive ) {

    /* DMA1 channel2 configuration SPI1 RX ---------------------------------------------*/
    /* DMA1 channel4 configuration SPI2 RX ---------------------------------------------*/
//    DMA_InitStructure.DMA_MemoryBaseAddr = (DWORD)buff;
    DMA_InitStructure.DMA_Memory0BaseAddr = (DWORD)buff;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_Init(DMA_Channel_SPI_SD_RX, &DMA_InitStructure);

    /* DMA1 channel3 configuration SPI1 TX ---------------------------------------------*/
    /* DMA1 channel5 configuration SPI2 TX ---------------------------------------------*/
//    DMA_InitStructure.DMA_MemoryBaseAddr = (DWORD)rw_workbyte;
    DMA_InitStructure.DMA_Memory0BaseAddr = (DWORD)rw_workbyte;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_Init(DMA_Channel_SPI_SD_TX, &DMA_InitStructure);

  }
  else
  {
    /* DMA1 channel2 configuration SPI1 RX ---------------------------------------------*/
    /* DMA1 channel4 configuration SPI2 RX ---------------------------------------------*/
//    DMA_InitStructure.DMA_MemoryBaseAddr = (DWORD)rw_workbyte;
    DMA_InitStructure.DMA_Memory0BaseAddr = (DWORD)rw_workbyte;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_Init(DMA_Channel_SPI_SD_RX, &DMA_InitStructure);

    /* DMA1 channel3 configuration SPI1 TX ---------------------------------------------*/
    /* DMA1 channel5 configuration SPI2 TX ---------------------------------------------*/
//    DMA_InitStructure.DMA_MemoryBaseAddr = (DWORD)buff;
    DMA_InitStructure.DMA_Memory0BaseAddr = (DWORD)buff;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_Init(DMA_Channel_SPI_SD_TX, &DMA_InitStructure);

  }

  /* Enable DMA RX Channel */
  DMA_Cmd(DMA_Channel_SPI_SD_RX, ENABLE);
  /* Enable DMA TX Channel */
  DMA_Cmd(DMA_Channel_SPI_SD_TX, ENABLE);

  /* Enable SPI TX/RX request */
  SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);

  /* Wait until DMA1_Channel 3 Transfer Complete */
  /// not needed: while (DMA_GetFlagStatus(DMA_FLAG_SPI_SD_TC_TX) == RESET) { ; }
  /* Wait until DMA1_Channel 2 Receive Complete */
  while (DMA_GetFlagStatus(DMA_Channel_SPI_SD_RX,DMA_FLAG_SPI_SD_TC_RX) == RESET) { rot_rdq(); }
  // same w/o function-call:
  // while ( ( ( DMA1->ISR ) & DMA_FLAG_SPI_SD_TC_RX ) == RESET ) { ; }

  /* Disable DMA RX Channel */
  DMA_Cmd(DMA_Channel_SPI_SD_RX, DISABLE);
  /* Disable DMA TX Channel */
  DMA_Cmd(DMA_Channel_SPI_SD_TX, DISABLE);

  /* Disable SPI1 RX/TX request */
  SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, DISABLE);
}
#endif /* STM32_USE_DMA */


/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */

static
void power_on (void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  volatile BYTE dummyread;

  /* Enable GPIO clock for CS */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_CS, ENABLE);
  /* Enable SPI clock, SPI1-APB2, SPI2-APB1 */
  RCC_APBPeriphClockCmd_SPI_SD(RCC_APBPeriph_SPI_SD, ENABLE);

  card_power(1);
  socket_wp_cp_init();

//  for (Timer1 = 25; Timer1; ) ;  /* Wait for 250ms */
  dly_tsk(250);

  /* Configure I/O for Flash Chip select */
  GPIO_Terminal_Init( SD_CS_PORT, OUT50M_PP, SD_CS_PIN );

  /* Deselect the Card: Chip Select high */
  DESELECT();

  /* Configure SPI pins: SCK and MOSI with default alternate function (not remapped) push-pull */
  GPIO_Terminal_Init( SD_SPI_PORT, AF_PP, SD_SCK_PIN | SD_MOSI_PIN );
  GPIO_PinAFConfig( SD_SPI_PORT, GPIO_PinSource13, GPIO_AF_SPI2 );
  GPIO_PinAFConfig( SD_SPI_PORT, GPIO_PinSource15, GPIO_AF_SPI2 );
  /* Configure MISO as Input with internal pull-up */
  //GPIO_Terminal_Init( SD_SPI_PORT, IN_FLOAT, SD_MISO_PIN );
  GPIO_Terminal_Init( SD_SPI_PORT, AF_PP, SD_MISO_PIN );
  GPIO_PinAFConfig( SD_SPI_PORT, GPIO_PinSource14, GPIO_AF_SPI2 );

  /* SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // 42000kHz/256=281kHz < 400Hz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_SD, &SPI_InitStructure);

  SPI_CalculateCRC(SPI_SD, DISABLE);

  /* Enable SPIx  */
  SPI_Cmd(SPI_SD, ENABLE);

  /* drain SPI */
  while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_TXE) == RESET) { rot_rdq(); }
  dummyread = SPI_I2S_ReceiveData(SPI_SD);

#ifdef STM32_USE_DMA
  /* enable DMA clock */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#endif
}

static
void power_off (void)
{
  if (!(Stat & STA_NOINIT))
  {
    SELECT();
    wait_ready();
    release_spi();
  }

  SPI_Cmd(SPI_SD, DISABLE);
  SPI_I2S_DeInit(SPI_SD);

  RCC_APBPeriphClockCmd_SPI_SD(RCC_APBPeriph_SPI_SD, DISABLE);

  /* All SPI-Pins to input with weak internal pull-downs */
  GPIO_Terminal_Init( SD_SPI_PORT, IN_UP, SD_SCK_PIN | SD_MISO_PIN | SD_MOSI_PIN );

  card_power(0);

  Stat |= STA_NOINIT;  /* Set STA_NOINIT */
}


/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
BOOL rcvr_datablock (
  BYTE *buff,    /* Data buffer to store received data */
  UINT btr       /* Byte count (must be multiple of 4) */
)
{
  BYTE token;


  Timer1 = 10;
  do
  {  /* Wait for data packet in timeout of 100ms */
    rot_rdq();
    token = rcvr_spi();
  } while ((token == 0xFF) && Timer1);

  if(token != 0xFE) return FALSE;  /* If not valid data token, return with error */

#ifdef STM32_USE_DMA
  stm32_dma_transfer( TRUE, buff, btr );
#else
  do
  {  /* Receive the data block into buffer */
    rcvr_spi_m(buff++);
    rcvr_spi_m(buff++);
    rcvr_spi_m(buff++);
    rcvr_spi_m(buff++);
  } while (btr -= 4);
#endif /* STM32_USE_DMA */

  rcvr_spi();            /* Discard CRC */
  rcvr_spi();

  return TRUE;          /* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
static
BOOL xmit_datablock
(
  const BYTE *buff,  /* 512 byte data block to be transmitted */
  BYTE token      /* Data/Stop token */
)
{
  BYTE resp;
#ifndef STM32_USE_DMA
  BYTE wc;
#endif

  if (wait_ready() != 0xFF) return FALSE;

  xmit_spi(token);          /* Xmit data token */
  if (token != 0xFD)
  {  /* Is data token */

#ifdef STM32_USE_DMA
    stm32_dma_transfer( FALSE, buff, 512 );
#else
    wc = 0;
    do
    {              /* Xmit the 512 byte data block to MMC */
      xmit_spi(*buff++);
      xmit_spi(*buff++);
    } while (--wc);
#endif /* STM32_USE_DMA */

    xmit_spi(0xFF);          /* CRC (Dummy) */
    xmit_spi(0xFF);
    resp = rcvr_spi();        /* Receive data response */
    if ((resp & 0x1F) != 0x05)    /* If not accepted, return with error */
      return FALSE;
  }

  return TRUE;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (
  BYTE cmd,    /* Command byte */
  DWORD arg    /* Argument */
)
{
  BYTE n, res;


  if (cmd & 0x80)
  {  /* ACMD<n> is the command sequence of CMD55-CMD<n> */
    cmd &= 0x7F;
    res = send_cmd(CMD55, 0);
    if (res > 1) return res;
  }

  /* Select the card and wait for ready */
  DESELECT();
  SELECT();
  if (wait_ready() != 0xFF)
  {
    return 0xFF;
  }

  /* Send command packet */
  xmit_spi(cmd);            /* Start + Command index */
  xmit_spi((BYTE)(arg >> 24));    /* Argument[31..24] */
  xmit_spi((BYTE)(arg >> 16));    /* Argument[23..16] */
  xmit_spi((BYTE)(arg >> 8));      /* Argument[15..8] */
  xmit_spi((BYTE)arg);        /* Argument[7..0] */
  n = 0x01;              /* Dummy CRC + Stop */
  if (cmd == CMD0) n = 0x95;      /* Valid CRC for CMD0(0) */
  if (cmd == CMD8) n = 0x87;      /* Valid CRC for CMD8(0x1AA) */
  xmit_spi(n);

  /* Receive command response */
  if (cmd == CMD12) rcvr_spi();    /* Skip a stuff byte when stop reading */

  n = 10;                /* Wait for a valid response in timeout of 10 attempts */
  do
  {
    rot_rdq();
    res = rcvr_spi();
  }while ((res & 0x80) && --n);

  return res;      /* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
  BYTE drv    /* Physical drive number (0) */
)
{
  BYTE n, cmd, ty, ocr[4];


  if (drv) return STA_NOINIT;      /* Supports only single drive */
  if (Stat & STA_NODISK) return Stat;  /* No card in the socket */

  power_on();              /* Force socket power on */
  interface_speed(INTERFACE_SLOW);
  for (n = 10; n; n--) rcvr_spi();  /* 80 dummy clocks */

  ty = 0;
  if (send_cmd(CMD0, 0) == 1)
  {      /* Enter Idle state */
    Timer1 = 100;            /* Initialization timeout of 1000 msec */
    if (send_cmd(CMD8, 0x1AA) == 1)
    {  /* SDHC */
      for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();    /* Get trailing return value of R7 resp */
      if (ocr[2] == 0x01 && ocr[3] == 0xAA)
      {      /* The card can work at vdd range of 2.7-3.6V */
        while (Timer1 && send_cmd(ACMD41, 1UL << 30)) rot_rdq();  /* Wait for leaving idle state (ACMD41 with HCS bit) */
        if (Timer1 && send_cmd(CMD58, 0) == 0)
        {    /* Check CCS bit in the OCR */
          for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
          ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
        }
      }
    }
    else
    {              /* SDSC or MMC */
      if (send_cmd(ACMD41, 0) <= 1)
      {
        ty = CT_SD1; cmd = ACMD41;  /* SDSC */
      }
      else
      {
        ty = CT_MMC; cmd = CMD1;  /* MMC */
      }
      while (Timer1 && send_cmd(cmd, 0)) rot_rdq();      /* Wait for leaving idle state */
      if (!Timer1 || send_cmd(CMD16, 512) != 0)  /* Set R/W block length to 512 */
        ty = 0;
    }
  }
  CardType = ty;
  release_spi();

  if (ty)
  {      /* Initialization succeeded */
    Stat &= ~STA_NOINIT;    /* Clear STA_NOINIT */
    interface_speed(INTERFACE_FAST);
  }
  else
  {      /* Initialization failed */
    power_off();
  }

  return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
  BYTE drv    /* Physical drive number (0) */
)
{
  if (drv) return STA_NOINIT;    /* Supports only single drive */
  return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
  BYTE drv,      /* Physical drive number (0) */
  BYTE *buff,      /* Pointer to the data buffer to store read data */
  DWORD sector,    /* Start sector number (LBA) */
  BYTE count      /* Sector count (1..255) */
)
{
  if (drv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;

  if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to byte address if needed */

  if (count == 1)
  {  /* Single block read */
    if (send_cmd(CMD17, sector) == 0)
    { /* READ_SINGLE_BLOCK */
      if (rcvr_datablock(buff, 512))
      {
        count = 0;
      }
    }
  }
  else
  {        /* Multiple block read */
    if (send_cmd(CMD18, sector) == 0)
    {  /* READ_MULTIPLE_BLOCK */
      do
      {
        if (!rcvr_datablock(buff, 512))
        {
          break;
        }
        buff += 512;
        rot_rdq();
      } while (--count);
      send_cmd(CMD12, 0);        /* STOP_TRANSMISSION */
    }
  }
  release_spi();
  
  return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
  BYTE drv,      /* Physical drive number (0) */
  const BYTE *buff,  /* Pointer to the data to be written */
  DWORD sector,    /* Start sector number (LBA) */
  BYTE count      /* Sector count (1..255) */
)
{
  if (drv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  if (Stat & STA_PROTECT) return RES_WRPRT;

  if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to byte address if needed */

  if (count == 1)
  {  /* Single block write */
    if ((send_cmd(CMD24, sector) == 0)  /* WRITE_BLOCK */
      && xmit_datablock(buff, 0xFE))
      count = 0;
  }
  else
  {        /* Multiple block write */
    if (CardType & CT_SDC) send_cmd(ACMD23, count);
    if (send_cmd(CMD25, sector) == 0) {  /* WRITE_MULTIPLE_BLOCK */
      do
      {
        if (!xmit_datablock(buff, 0xFC)) break;
        buff += 512;
        rot_rdq();
      } while (--count);
      if (!xmit_datablock(0, 0xFD))  /* STOP_TRAN token */
        count = 1;
    }
  }
  release_spi();

  return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT disk_ioctl (
  BYTE drv,    /* Physical drive number (0) */
  BYTE ctrl,    /* Control code */
  void *buff    /* Buffer to send/receive control data */
)
{
  DRESULT res;
  BYTE n, csd[16], *ptr = buff;
  WORD csize;


  if (drv) return RES_PARERR;

  res = RES_ERROR;

  if (ctrl == CTRL_POWER)
  {
    switch (*ptr)
    {
      case 0:    /* Sub control code == 0 (POWER_OFF) */
        if (chk_power())
          power_off();    /* Power off */
        res = RES_OK;
        break;
      case 1:    /* Sub control code == 1 (POWER_ON) */
        power_on();        /* Power on */
        res = RES_OK;
        break;
      case 2:    /* Sub control code == 2 (POWER_GET) */
        *(ptr+1) = (BYTE)chk_power();
        res = RES_OK;
        break;
      default :
        res = RES_PARERR;
    }
  }
  else
  {
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl)
    {
      case CTRL_SYNC :    /* Make sure that no pending write process */
        SELECT();
        if (wait_ready() == 0xFF)
          res = RES_OK;
        break;

      case GET_SECTOR_COUNT :  /* Get number of sectors on the disk (DWORD) */
        if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16))
        {
          if ((csd[0] >> 6) == 1)
          {  /* SDC ver 2.00 */
            csize = csd[9] + ((WORD)csd[8] << 8) + 1;
            *(DWORD*)buff = (DWORD)csize << 10;
          }
          else
          {          /* SDC ver 1.XX or MMC*/
            n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
            csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
            *(DWORD*)buff = (DWORD)csize << (n - 9);
          }
          res = RES_OK;
        }
        break;

      case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
        *(WORD*)buff = 512;
        res = RES_OK;
        break;

      case GET_BLOCK_SIZE :  /* Get erase block size in unit of sector (DWORD) */
        if (CardType & CT_SD2)
        {  /* SDC ver 2.00 */
          if (send_cmd(ACMD13, 0) == 0)
          {  /* Read SD status */
            rcvr_spi();
            if (rcvr_datablock(csd, 16))
            {        /* Read partial block */
              for (n = 64 - 16; n; n--) rcvr_spi();  /* Purge trailing data */
              *(DWORD*)buff = 16UL << (csd[10] >> 4);
              res = RES_OK;
            }
          }
        }
        else
        {          /* SDC ver 1.XX or MMC */
          if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16))
          {  /* Read CSD */
            if (CardType & CT_SD1)
            {  /* SDC ver 1.XX */
              *(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
            }
            else
            {          /* MMC */
              *(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
            }
            res = RES_OK;
          }
        }
        break;

      case MMC_GET_TYPE :    /* Get card type flags (1 byte) */
        *ptr = CardType;
        res = RES_OK;
        break;

      case MMC_GET_CSD :    /* Receive CSD as a data block (16 bytes) */
        if (send_cmd(CMD9, 0) == 0    /* READ_CSD */
          && rcvr_datablock(ptr, 16))
          res = RES_OK;
        break;

      case MMC_GET_CID :    /* Receive CID as a data block (16 bytes) */
        if (send_cmd(CMD10, 0) == 0    /* READ_CID */
          && rcvr_datablock(ptr, 16))
          res = RES_OK;
        break;

      case MMC_GET_OCR :    /* Receive OCR as an R3 resp (4 bytes) */
        if (send_cmd(CMD58, 0) == 0)
        {  /* READ_OCR */
          for (n = 4; n; n--) *ptr++ = rcvr_spi();
          res = RES_OK;
        }
        break;

      case MMC_GET_SDSTAT :  /* Receive SD status as a data block (64 bytes) */
        if (send_cmd(ACMD13, 0) == 0)
        {  /* SD_STATUS */
          rcvr_spi();
          if (rcvr_datablock(ptr, 64))
            res = RES_OK;
        }
        break;

      default:
        res = RES_PARERR;
    }

    release_spi();
  }

  return res;
}
#endif /* _USE_IOCTL != 0 */


/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */

RAMFUNC void disk_timerproc (void)
{
  static socket_state_t pv;
  socket_state_t ns;
  BYTE n, s;


  n = Timer1;            /* 100Hz decrement timer */
  if (n) Timer1 = --n;
  n = Timer2;
  if (n) Timer2 = --n;

  ns = pv;
  pv = socket_wp_cp_state();  /* Sample socket switch */

  if (ns == pv)
  {        /* Have contacts stabled? */
    s = Stat;

    if (socket_is_write_protected(pv))  /* WP is H (write protected) */
      s |= STA_PROTECT;
    else                /* WP is L (write enabled) */
      s &= ~STA_PROTECT;

    if (socket_is_empty(pv))      /* INS = H (Socket empty) */
      s |= (STA_NODISK | STA_NOINIT);
    else                /* INS = L (Card inserted) */
      s &= ~STA_NODISK;

    Stat = s;
  }
}

