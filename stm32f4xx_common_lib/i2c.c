/*********************************************************************************/
/* STM32F407ZET6のI2Cを何とかする                                                */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
#include  "stm32f4xx.h"
#include  "derivertive.h"
#include  "peripheral.h"
//#include  "mul_tsk.h"
#include  "i2c.h"

#if  !defined(_MULTITASK_H_)
  #define  rot_rdq()
#endif

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/

/*************************************************************************/
/* IOレジスタ定義                                                        */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* I2C1のポート及びそれ自身の初期化                                      */
/*************************************************************************/
void I2C1_Init( void )
{
  int i;
  I2C_InitTypeDef I2C_InitStruct;

  /*不定になっているデバイスの解消*/
  GPIO_Terminal_Init( I2C1_PORT, OUT25M_PP, I2C1_SCL_PIN );
  for(i = 0; i < 10; i++)
  {
    I2C1_SCL_IS_0;
    Dly10us( 1 );
    I2C1_SCL_IS_1;
    Dly10us( 1 );
  }

  RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );
  RCC_APB1PeriphResetCmd( RCC_APB1Periph_I2C1, DISABLE );
  GPIO_Terminal_Init( I2C1_PORT, AF_OD, I2C1_SCL_PIN );
  GPIO_Terminal_Init( I2C1_PORT, AF_OD, I2C1_SDA_PIN );
  GPIO_PinAFConfig( I2C1_PORT, I2C1_SCL_SOURCE, GPIO_AF_I2C1 );
  GPIO_PinAFConfig( I2C1_PORT, I2C1_SDA_SOURCE, GPIO_AF_I2C1 );

  I2C_DeInit(I2C1);
  I2C_InitStruct.I2C_ClockSpeed = 100 * 1000UL;  //100khz
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  I2C_Init(I2C1, &I2C_InitStruct);

  //I2C起動
  I2C_Cmd(I2C1, ENABLE);
}

/*************************************************************************/
/* I2C1のポート及びそれ自身の初期化解除                                  */
/*************************************************************************/
void I2C1_Deinit( void )
{
  //I2C起動解除
  I2C_Cmd(I2C1, DISABLE);
  I2C_DeInit(I2C1);

  RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, DISABLE );
  RCC_APB1PeriphResetCmd( RCC_APB1Periph_I2C1, ENABLE );

  GPIO_Terminal_Init( I2C1_PORT, IN_FLOAT, I2C1_SCL_PIN );
  GPIO_Terminal_Init( I2C1_PORT, IN_FLOAT, I2C1_SDA_PIN );
}

/*************************************************************************/
/* データ送信                                                            */
/* 成功すれば1、失敗で-1を返す                                           */
/*************************************************************************/
int I2C_Write( I2C_TypeDef * i2c, unsigned char adr, unsigned char data )
{
  // While the bus is busy
  while(I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY)) rot_rdq();

  //start condition
  I2C_GenerateSTART(i2c, ENABLE);

  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) rot_rdq();

  //Send address for write
  I2C_Send7bitAddress(i2c,adr << 1,I2C_Direction_Transmitter);

  //EV6 and clear it：アドレス送信確認、送信データレジスタ空き、データbyte(アドレスのR/Wbit)送信、通信中(BUS BUSY)、マスター
  while(I2C_CheckEvent(i2c,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) rot_rdq();

  //Send the slave’s address to write to : MSB of the address first
  I2C_SendData(i2c,data);

  //EV8 and clear it：送信データレジスタ空き、データバイト(アドレスのR/Wbit)送信、通信中(BUS BUSY)、マスター
  //EV8_2 and clear it：データbyte送信完了、送信データレジスタ空き、データバイト(アドレスのR/Wbit)送信、通信中(BUS BUSY)、マスター
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != ERROR) rot_rdq();

  //stop condition
  I2C_GenerateSTOP(i2c, ENABLE);

  return 1;
}

/*************************************************************************/
/* データ受信                                                            */
/* 成功すれば1、失敗で-1を返す                                           */
/*************************************************************************/
int I2C_Read( I2C_TypeDef * i2c, unsigned char adr, unsigned char *data )
{
  unsigned char c;

  /* While the bus is busy */
  while(I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY)) rot_rdq();

  //start condition
  I2C_GenerateSTART(i2c, ENABLE);

  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) rot_rdq();

  /* Send module address and behave as receiver */
  I2C_Send7bitAddress(i2c, adr << 1, I2C_Direction_Receiver);
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) rot_rdq();

  while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_RECEIVED)) rot_rdq();
  /* Read a byte from the module */
  c = I2C_ReceiveData(i2c);

  /* Disable Acknowledgment */
  I2C_AcknowledgeConfig(i2c, DISABLE);

  /* Send STOP Condition */
  I2C_GenerateSTOP(i2c, ENABLE);
  *data = c;

  return 1;
}

/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
