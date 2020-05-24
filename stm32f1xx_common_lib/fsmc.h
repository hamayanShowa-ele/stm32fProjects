/*************************************************************************/
/* STM32F103VET6のFSMCの定義                                             */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
#ifndef  __FSMC_H__
#define  __FSMC_H__

//#define  RCC_AHBPeriph_FSMC   ((uint32_t)0x00000100)

//#define  FSMC_Bank1_R_BASE    (FSMC_R_BASE + 0x0000) /*!< FSMC Bank1 registers base address */
//#define  FSMC_Bank1E_R_BASE   (FSMC_R_BASE + 0x0104) /*!< FSMC Bank1E registers base address */
//#define  FSMC_Bank2_R_BASE    (FSMC_R_BASE + 0x0008) /*!< FSMC Bank2 registers base address */
//#define  FSMC_Bank3_R_BASE    (FSMC_R_BASE + 0x0010) /*!< FSMC Bank3 registers base address */
//#define  FSMC_Bank4_R_BASE    (FSMC_R_BASE + 0x0018) /*!< FSMC Bank4 registers base address */

//#define  FSMC_Bank1           ((FSMC_Bank1_TypeDef *) FSMC_Bank1_R_BASE)
//#define  FSMC_Bank1E          ((FSMC_Bank1E_TypeDef *) FSMC_Bank1E_R_BASE)
//#define  FSMC_Bank2           ((FSMC_Bank2_TypeDef *) FSMC_Bank2_R_BASE)
//#define  FSMC_Bank3           ((FSMC_Bank3_TypeDef *) FSMC_Bank3_R_BASE)
//#define  FSMC_Bank4           ((FSMC_Bank4_TypeDef *) FSMC_Bank4_R_BASE)

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
#define  __IO      volatile
#define  uint32_t  unsigned long
#define  uint8_t   unsigned char

/*************************************************************************/
/* IOレジスタ定義                                                        */
/*************************************************************************/
typedef union
{
  __IO uint32_t DWORD;
  struct
  {
    __IO uint32_t MBKEN : 1;
    __IO uint32_t MUXEN : 1;
    __IO uint32_t MTYP : 2;
    __IO uint32_t MWID : 2;
    __IO uint32_t FACCEN : 1;
    __IO uint32_t rsv3 : 1;
    __IO uint32_t BURSTEN : 1;
    __IO uint32_t WAITPOL : 1;
    __IO uint32_t WRAPMOD : 1;
    __IO uint32_t WAITCFG : 1;
    __IO uint32_t WREN : 1;
    __IO uint32_t WAITEN : 1;
    __IO uint32_t EXTMOD : 1;
    __IO uint32_t ASYNCWAIT : 1;
    __IO uint32_t rsv1 : 16;
  } BIT;
} FSMC_BCRx;

typedef union
{
  __IO uint32_t DWORD;
  struct
  {
    __IO uint32_t ADDSET : 4;
    __IO uint32_t ADDHLD : 4;
    __IO uint32_t DATAST : 8;
    __IO uint32_t BUSTURN : 4;
    __IO uint32_t CLKDIV : 4;
    __IO uint32_t DATLAT : 4;
    __IO uint32_t ACCMOD : 2;
    __IO uint32_t rsv1 : 2;
  } BIT;
} FSMC_TCRx;

typedef union
{
  __IO uint32_t DWORD;
  struct
  {
    __IO uint32_t ADDSET : 4;
    __IO uint32_t ADDHLD : 4;
    __IO uint32_t DATAST : 8;
    __IO uint32_t BUSTURN : 4;
    __IO uint32_t CLKDIV : 4;
    __IO uint32_t DATLAT : 4;
    __IO uint32_t ACCMOD : 2;
    __IO uint32_t rsv1 : 2;
  } BIT;
} FSMC_BWTRx;

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

#endif  /*__FSMC_H__*/
/*************************************************************************/
/* end of file                                                           */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
