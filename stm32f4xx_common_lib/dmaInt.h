/*************************************************************************/
/* STM32F4のDAC用の割り込み処理                                          */
/*                                 designed by hamayan since 2019/09/19  */
/*************************************************************************/
#ifndef  _DMA_INT_H_
#define  _DMA_INT_H_

#include  "stm32f4xx.h"

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
extern unsigned char update_dma1_st5_ht;   /*dma1 stream5 half transmit*/
extern unsigned char update_dma1_st5_ft;   /*dma1 stream5 full transmit*/
extern unsigned long update_dma1_st5_dme;  /*dma1 stream5 dma error*/
extern unsigned long update_dma1_st5_te;   /*dma1 stream5 transmit error*/

extern unsigned char update_dma1_st6_ht;   /*dma1 stream6 half transmit*/
extern unsigned char update_dma1_st6_ft;   /*dma1 stream6 full transmit*/
extern unsigned long update_dma1_st6_dme;  /*dma1 stream6 dma error*/
extern unsigned long update_dma1_st6_te;   /*dma1 stream6 transmit error*/

extern unsigned char update_dma2_st1_ht;   /*dma2 stream1 half transmit*/
extern unsigned char update_dma2_st1_ft;   /*dma2 stream1 full transmit*/
extern unsigned long update_dma2_st1_dme;  /*dma2 stream1 dma error*/
extern unsigned long update_dma2_st1_te;   /*dma2 stream1 transmit error*/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

#endif  /*_DMA_INT_H_*/

/*************************************************************************/
/* end of file                                                           */
/*                                 designed by hamayan since 2019/09/19  */
/*************************************************************************/
