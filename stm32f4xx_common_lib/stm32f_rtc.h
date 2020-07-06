/*************************************************************************/
/* STM32F103VBT6のRTCを使う                                              */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* IOレジスタ定義                                                        */
/*************************************************************************/

/*************************************************************************/
/* レジスタ bit定義                                                      */
/*************************************************************************/
/*CRH*/
#define  SECIE_Bit      0x00000001
#define  ALRIE_Bit      0x00000002
#define  OWIE_Bit       0x00000004

/*CRL*/
#define  SECF_Bit       SECIE_Bit
#define  ALRF_Bit       ALRIE_Bit
#define  OWF_Bit        OWIE_Bit
#define  RSF_Bit        0x00000008
#define  CNF_Bit        0x00000010
#define  RTOFF_Bit      0x00000020

/*************************************************************************/
/* ビットフィールド定義                                                  */
/*************************************************************************/
/*CRH*/
typedef union
{
  unsigned long DWORD;
  struct
  {
    unsigned long SECIE : 1;  /*bit0*/
    unsigned long ALRIE : 1;  /*bit1*/
    unsigned long OWIE : 1;  /*bit2*/
    unsigned long rsv1 : 29;  /*bit31~3*/
  } BIT;
} RTC_CRH_REG;

/*CRL*/
typedef union
{
  unsigned long DWORD;
  struct
  {
    unsigned long SECF : 1;  /*bit0*/
    unsigned long ALRF : 1;  /*bit1*/
    unsigned long OWF : 1;  /*bit2*/
    unsigned long RSF : 1;  /*bit3*/
    unsigned long CNF : 1;  /*bit4*/
    unsigned long RTOFF : 1;  /*bit5*/
    unsigned long rsv1 : 26;  /*bit31~6*/
  } BIT;
} RTC_CRL_REG;

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
enum DATE_AND_TIME { YEAR = 0, MONTH, DAY, HOUR, MINUTE, SECOND };

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
//int RTC_Init( void );
void RTC_Alarm_Init( void );
void RTC_Int( void );
void RTC_Alarm_Int( void );
void Date_Time_Set( const signed char str[] );
char *DateTime2Strings( char *dest, const signed char str[] );

void unixTime_Set( time_t unixTime );


/*************************************************************************/
/* end of file                                                           */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
