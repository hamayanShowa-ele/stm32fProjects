/*************************************************************************/
/* STM32F4のNVICを何とかする                                             */
/*                                 designed by hamayan since 2019/09/19  */
/*************************************************************************/
#include  "configNVIC.h"

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

/*************************************************************************/
/* 割り込み有効                                                          */
/*************************************************************************/
void NVIC_IRQEnable( uint8_t ch, uint8_t basePri, uint8_t subPri )
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /*割り込み設定 */
  NVIC_InitStructure.NVIC_IRQChannel = ch;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = basePri;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPri;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  DI();
  NVIC_Init( &NVIC_InitStructure );
  EI();
}


/*************************************************************************/
/* 割り込み無効                                                          */
/*************************************************************************/
void NVIC_IRQDisable( uint8_t ch )
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /*割り込み設定 */
  NVIC_InitStructure.NVIC_IRQChannel = ch;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  DI();
  NVIC_Init( &NVIC_InitStructure );
  EI();
}

/*************************************************************************/
/* end of file                                                           */
/*                                 designed by hamayan since 2019/09/19  */
/*************************************************************************/
