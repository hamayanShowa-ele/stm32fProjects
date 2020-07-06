/*********************************************************************************/
/* 周辺IOの初期化                                                                */
/*                                         designed by hamayan since 2019/09/19  */
/*********************************************************************************/
#include  "dmaInt.h"

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
unsigned char update_dma1_st5_ht;   /*dma1 stream5 half transmit*/
unsigned char update_dma1_st5_ft;   /*dma1 stream5 full transmit*/
unsigned long update_dma1_st5_dme;  /*dma1 stream5 dma error*/
unsigned long update_dma1_st5_te;   /*dma1 stream5 transmit error*/

unsigned char update_dma1_st6_ht;   /*dma1 stream6 half transmit*/
unsigned char update_dma1_st6_ft;   /*dma1 stream6 full transmit*/
unsigned long update_dma1_st6_dme;  /*dma1 stream6 dma error*/
unsigned long update_dma1_st6_te;   /*dma1 stream6 transmit error*/

unsigned char update_dma2_st1_ht;   /*dma2 stream1 half transmit*/
unsigned char update_dma2_st1_ft;   /*dma2 stream1 full transmit*/
unsigned long update_dma2_st1_dme;  /*dma2 stream1 dma error*/
unsigned long update_dma2_st1_te;   /*dma2 stream1 transmit error*/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

/*************************************************************************/
/* DMA1 stream5割り込みハンドラ                                          */
/*************************************************************************/
void DMA1_Stream5_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA1_Stream5, DMA_IT_TCIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream5, DMA_IT_TCIF0 );
    update_dma1_st5_ft++;  /*dma1 stream5 full transmit*/
  }
  if( DMA_GetITStatus( DMA1_Stream5, DMA_IT_HTIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream5, DMA_IT_HTIF0 );
    update_dma1_st5_ht++;  /*dma1 stream5 half transmit*/
  }
  if( DMA_GetITStatus( DMA1_Stream5, DMA_IT_TEIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream5, DMA_IT_TEIF0 );
    update_dma1_st5_te++;  /*dma1 stream5 transmit error*/
  }
  if( DMA_GetITStatus( DMA1_Stream5, DMA_IT_DMEIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream5, DMA_IT_DMEIF0 );
    update_dma1_st5_dme++;  /*dma1 stream5 dma error*/
  }
  if( DMA_GetITStatus( DMA1_Stream5, DMA_IT_FEIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream5, DMA_IT_FEIF0 );
  }
}

/*************************************************************************/
/* DMA1 stream6割り込みハンドラ                                          */
/*************************************************************************/
void DMA1_Stream6_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA1_Stream6, DMA_IT_TCIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream6, DMA_IT_TCIF0 );
    update_dma1_st6_ft++;  /*dma1 stream6 full transmit*/
  }
  if( DMA_GetITStatus( DMA1_Stream6, DMA_IT_HTIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream6, DMA_IT_HTIF0 );
    update_dma1_st6_ht++;  /*dma1 stream6 half transmit*/
  }
  if( DMA_GetITStatus( DMA1_Stream6, DMA_IT_TEIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream6, DMA_IT_TEIF0 );
    update_dma1_st6_te++;  /*dma1 stream6 transmit error*/
  }
  if( DMA_GetITStatus( DMA1_Stream6, DMA_IT_DMEIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream6, DMA_IT_DMEIF0 );
    update_dma1_st6_dme++;  /*dma1 stream6 dma error*/
  }
  if( DMA_GetITStatus( DMA1_Stream6, DMA_IT_FEIF0 ) == SET )
  {
    DMA_ClearITPendingBit( DMA1_Stream6, DMA_IT_FEIF0 );
  }
}


/*************************************************************************/
/* DMA2 stream1割り込みハンドラ                                          */
/*************************************************************************/
void DMA2_Stream1_IRQHandler( void )
{
  if( DMA_GetITStatus( DMA2_Stream1, DMA_IT_TCIF1 ) == SET )
  {
    DMA_ClearITPendingBit( DMA2_Stream1, DMA_IT_TCIF1 );
    update_dma2_st1_ft++;  /*dma2 stream1 full transmit*/
  }
  if( DMA_GetITStatus( DMA2_Stream1, DMA_IT_HTIF1 ) == SET )
  {
    DMA_ClearITPendingBit( DMA2_Stream1, DMA_IT_HTIF1 );
    update_dma2_st1_ht++;  /*dma2 stream1 half transmit*/
  }
  if( DMA_GetITStatus( DMA2_Stream1, DMA_IT_TEIF1 ) == SET )
  {
    DMA_ClearITPendingBit( DMA2_Stream1, DMA_IT_TEIF1 );
    update_dma2_st1_te++;  /*dma2 stream1 transmit error*/
  }
  if( DMA_GetITStatus( DMA2_Stream1, DMA_IT_DMEIF1 ) == SET )
  {
    DMA_ClearITPendingBit( DMA2_Stream1, DMA_IT_DMEIF1 );
    update_dma2_st1_dme++;  /*dma2 stream1 dma error*/
  }
  if( DMA_GetITStatus( DMA2_Stream1, DMA_IT_FEIF1 ) == SET )
  {
    DMA_ClearITPendingBit( DMA2_Stream1, DMA_IT_FEIF1 );
  }
}


/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2019/09/19  */
/*********************************************************************************/
