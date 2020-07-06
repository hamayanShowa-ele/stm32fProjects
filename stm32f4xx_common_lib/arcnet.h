/*********************************************************************************/
/* ARCNETコントローラー(COM2002213V-HT)に関する処理のヘッダー                    */
/* バスは16bitアクセスとして設定する事。                                         */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
//#define  ARC_BASE_ADR   0x68000000

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/
//#define  ARC_RST_ENB    GPIO_Pin_3       /*ARCNETコントローラーリセット。アクティブLO*/
//#define  ARC_RST_LO     GPIOC->BSRR = (ARC_RST_ENB << 16)
//#define  ARC_RST_HI     GPIOC->BSRR = (ARC_RST_ENB)

/*************************************************************************/
/* コマンド及びステータス                                                */
/*************************************************************************/
#define  ARC_ADR_STATUS *((volatile short *)(ARC_BASE_ADR + (0 * 2)))  /*ステータスレジスタ*/
#define  ARC_ADR_IMASK  *((volatile short *)(ARC_BASE_ADR + (0 * 2)))  /*割り込みマスクレジスタ*/
#define  ARC_ADR_DIAG   *((volatile short *)(ARC_BASE_ADR + (1 * 2)))  /*ダイアグノスティックレジスタ*/
#define  ARC_ADR_CMD    *((volatile short *)(ARC_BASE_ADR + (1 * 2)))  /*コマンドレジスタ*/
#define  ARC_ADR_ADR_U  *((volatile short *)(ARC_BASE_ADR + (2 * 2)))  /*アドレスポインタ 上位*/
#define  ARC_ADR_ADR_D  *((volatile short *)(ARC_BASE_ADR + (3 * 2)))  /*アドレスポインタ 下位*/
#define  ARC_ADR_DATA   *((volatile short *)(ARC_BASE_ADR + (4 * 2)))  /*データレジスタ*/
#define  ARC_ADR_SUB_ADR *((volatile short *)(ARC_BASE_ADR + (5 * 2)))  /*サブアドレスレジスタ*/
#define  ARC_ADR_CONFIG *((volatile short *)(ARC_BASE_ADR + (6 * 2)))  /*コンフィグレーションレジスタ*/
#define  ARC_ADR_TENTID *((volatile short *)(ARC_BASE_ADR + (7 * 2)))  /*TENT-IDレジスタ*/
#define  ARC_ADR_NODEID *((volatile short *)(ARC_BASE_ADR + (7 * 2)))  /*NODE-IDレジスタ*/
#define  ARC_ADR_SETUP1 *((volatile short *)(ARC_BASE_ADR + (7 * 2)))  /*SETUP1レジスタ*/
#define  ARC_ADR_SETUP2 *((volatile short *)(ARC_BASE_ADR + (7 * 2)))  /*SETUP2レジスタ*/
#define  ARC_ADR_NEXTID *((volatile short *)(ARC_BASE_ADR + (7 * 2)))  /*NEXT-IDレジスタ*/
#define  ARC_ADR_BUSCTRL *((volatile short *)(ARC_BASE_ADR + (7 * 2)))  /*BUS CONTROLレジスタ*/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
void arcInit( void );
int arcBufferCheck( void );


/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
