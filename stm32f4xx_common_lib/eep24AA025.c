/*********************************************************************************/
/* I2C EEPROM 24AA025の初期化                                                    */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
#include  "stm32f4xx.h"
#include  "derivertive.h"
#include  "peripheral.h"
#include  "mul_tsk.h"
#include  "eep24aa025.h"

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
/* 24AA025E write                                          */
/*************************************************************************/
void EEP24AA025Ewrite( I2C_TypeDef * i2c, unsigned char adr, unsigned char wordAdr, unsigned char data )
{
  /* While the bus is busy */
  while(I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY)) rot_rdq();

  //start condition
  I2C_GenerateSTART(i2c, ENABLE);

  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) rot_rdq();

  //Send address for write
  I2C_Send7bitAddress(i2c,adr << 1,I2C_Direction_Transmitter);
  while(I2C_CheckEvent(i2c,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) rot_rdq();

  //Send the slave’s address to write to : MSB of the address first
  I2C_SendData(i2c,wordAdr);
  while(I2C_CheckEvent(i2c,I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) rot_rdq();

  I2C_SendData(i2c,data);
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != ERROR) rot_rdq();

  //stop condition
  I2C_GenerateSTOP(i2c, ENABLE);
  dly_tsk( 5UL );  /* Write cycle time (byte or page) */
}

/*************************************************************************/
/* 24AA025Eシーケンシャルwrite                                          */
/* 16byte毎の境界で書き込みが可能                           */
/*************************************************************************/
void EEP24AA025EseqWrite( I2C_TypeDef * i2c, unsigned char adr, unsigned char wordAdr, const unsigned char *data, int sz )
{
  /* While the bus is busy */
  while(I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY)) rot_rdq();

  //start condition
  I2C_GenerateSTART(i2c, ENABLE);
  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) rot_rdq();

  //Send address for write
  I2C_Send7bitAddress(i2c,adr << 1,I2C_Direction_Transmitter);
  while(I2C_CheckEvent(i2c,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) rot_rdq();

  //Send the slave’s address to write to : MSB of the address first
  I2C_SendData(i2c,wordAdr);
  while(I2C_CheckEvent(i2c,I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) rot_rdq();

  /* Write to 24aa025e until data size */
  for(int cnt = 0; cnt < sz; cnt++)
  {
    I2C_SendData( i2c, *data++ );
    while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != ERROR) rot_rdq();
    dly_tsk( 1UL );  /* amazing */
  }

  //stop condition
  I2C_GenerateSTOP(i2c, ENABLE);
  dly_tsk( 5UL );  /* Write cycle time (byte or page) */
}

/*************************************************************************/
/* 24AA025Eシーケンシャルリード                                          */
/* 成功すれば受信数、失敗で-1を返す                                      */
/*************************************************************************/
int EEP24AA025EseqRead( I2C_TypeDef * i2c, unsigned char adr, unsigned char wordAdr ,unsigned char *data, int sz )
{
  int cnt;
  unsigned char c,*ptr;

  /* While the bus is busy */
  while(I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY)) rot_rdq();

  //start condition
  I2C_GenerateSTART(i2c, ENABLE);
  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) rot_rdq();

  /* Send module address and behave as receiver */
  I2C_Send7bitAddress(i2c, adr << 1,I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) rot_rdq();

  /* Send word address for 24aa025e */
  I2C_SendData(i2c,wordAdr);
  //EV8 and clear it：送信データレジスタ空き、データバイト(アドレスのR/Wbit)送信、通信中(BUS BUSY)、マスター
  //EV8_2 and clear it：データbyte送信完了、送信データレジスタ空き、データバイト(アドレスのR/Wbit)送信、通信中(BUS BUSY)、マスター
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != ERROR) rot_rdq();

  //restart condition
  I2C_GenerateSTART(i2c, DISABLE);
  I2C_GenerateSTART(i2c, ENABLE);
  //EV5 and clear it：マスターモードでstartコンディション完了後、通信を開始している事を確認している。
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) rot_rdq();

  /* Send control byte for 24aa025e */
  I2C_Send7bitAddress(i2c, adr << 1, I2C_Direction_Receiver);
  //EV8 and clear it：送信データレジスタ空き、データバイト(アドレスのR/Wbit)送信、通信中(BUS BUSY)、マスター
  //EV8_2 and clear it：データbyte送信完了、送信データレジスタ空き、データバイト(アドレスのR/Wbit)送信、通信中(BUS BUSY)、マスター
  while(I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != ERROR) rot_rdq();

  /* Get from 24aa025e until data size */
  for(cnt = 0, ptr = data; cnt < sz; cnt++)
  {
    while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_RECEIVED)) rot_rdq();
    /* Read a byte from the module */
    c = I2C_ReceiveData(i2c);
    *ptr++ = c;
  }

  /* Disable Acknowledgment */
  I2C_AcknowledgeConfig(i2c, DISABLE);

  /* Send STOP Condition */
  I2C_GenerateSTOP(i2c, ENABLE);

  return cnt;
}

/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
