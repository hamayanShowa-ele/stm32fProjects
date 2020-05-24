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
#ifndef _USART_H_
#define _USART_H_

#include  <stm32f10x.h>
#include  <peripheral.h>
#include  <derivertive.h>
#include  <mul_tsk.h>

/*************************************************************************/
/* ボーレート定義                                                        */
/*************************************************************************/
#define  BAUD_921600  (0x00000270UL / 8)
#define  BAUD_460800  (0x00000270UL / 4)
#define  BAUD_230400  (0x00000270UL / 2)
#define  BAUD_115200  (0x00000270UL)
#define  BAUD_57600   (0x000004E2UL)
#define  BAUD_38400   (0x00000753UL)
#define  BAUD_19200   (0x00000EA6UL)
#define  BAUD_9600    (0x00001D4CUL)

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* IOレジスタ定義                                                        */
/*************************************************************************/

/*************************************************************************/
/* レジスタ bit定義                                                      */
/*************************************************************************/
/*SR*/
#define  PE_Bit     0x00000001
#define  FE_Bit     0x00000002
#define  NE_Bit     0x00000004
#define  ORE_Bit    0x00000008
#define  IDLE_Bit   0x00000010
#define  RXNE_Bit   0x00000020
#define  TC_Bit     0x00000040
#define  TXE_Bit    0x00000080
#define  LBD_Bit    0x00000100
#define  CTS_Bit    0x00000200

/*CR1*/
#define  SBK_Bit    0x00000001
#define  RWU_Bit    0x00000002
#define  RE_Bit     0x00000004
#define  TE_Bit     0x00000008
#define  IDLEIE_Bit 0x00000010
#define  RXNEIE_Bit 0x00000020
#define  TCIE_Bit   0x00000040
#define  TXEIE_Bit  0x00000080
#define  PEIE_Bit   0x00000100
#define  PS_Bit     0x00000200
#define  PCE_Bit    0x00000400
#define  WAKE_Bit   0x00000800
#define  M_Bit      0x00001000
#define  UE_Bit     0x00002000

/*CR2*/
#define  LBDL_Bit   0x00000020
#define  LBDIE_Bit  0x00000040
#define  LBCL_Bit   0x00000100
#define  CPHA_Bit   0x00000200
#define  CPOL_Bit   0x00000400
#define  CLKEN_Bit  0x00000800
#define  STOP0_Bit  0x00001000
#define  STOP1_Bit  0x00002000
#define  LINEN_Bit  0x00004000

/*CR3*/
#define  EIE_Bit    0x00000001
#define  IREN_Bit   0x00000002
#define  IRLP_Bit   0x00000004
#define  HDSEL_Bit  0x00000008
#define  NACK_Bit   0x00000010
#define  SCEN_Bit   0x00000020
#define  DMAR_Bit   0x00000040
#define  DMAT_Bit   0x00000080
#define  RTSE_Bit   0x00000100
#define  CTSE_Bit   0x00000200
#define  CTSIE_Bit  0x00000400

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
typedef enum
{
  SCI_1 = 0, SCI_2, SCI_3, SCI_4, SCI_5, SCI_6,
} SciNum_t;

typedef struct
{
  unsigned long sr;
  unsigned long dr;
  unsigned long brr;
  unsigned long cr1;
  unsigned long cr2;
  unsigned long cr3;
  unsigned long gtpr;
} SCI_REG;

typedef struct
{
  void *buf; 
  unsigned short wptr,rptr; 
  unsigned short size;
} SCI_BUFFER;

typedef struct
{
  unsigned long FE_cnt;  /*フレーミングエラー数の累積*/
  unsigned long PE_cnt;  /*パリティエラー数の累積*/
  unsigned long NE_cnt;  /*ノイズエラー数の累積*/
  unsigned long ORE_cnt; /*オーバーランエラー数の累積*/
} SCI_ERROR_COUNT;

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
//int SCI_Init( int ch, SCI_BUFFER *rcv, SCI_BUFFER *snd, unsigned long brr );
int  SCI_Init( int ch, SCI_BUFFER *rcv, SCI_BUFFER *snd, SCI_ERROR_COUNT *err, unsigned long brr );
int  SCI_Deinit( int ch );
int  SCI_Putc( int ch, char c );
int  SCI_Puts( int ch, const char *str );
int  SCI_Getc( int ch );
char *SCI_Gets( char *buf, int ch, int BufSz );
unsigned short RcvSizeOfBuf( int ch );
unsigned short SndSizeOfBuf( int ch );
void SCI_ISR( int ch );
unsigned long SCI_Status( int ch );
char SCI_DirectRead( int ch );
void SCI_Purge( int ch );

int  SCI_Write( int ch, const void *src, int size );
char *GetLine( char *buf, int ch, int BufSz );
int  Rcv1Char( int handle );

void GetErrorInfo( int ch, unsigned long *fe, unsigned long *pe, unsigned long *ne, unsigned long *ore );
int  SCI_GetsWithTmout( char *buf, int ch, int BufSz, unsigned long tmout );


#endif /*_USART_H_*/
