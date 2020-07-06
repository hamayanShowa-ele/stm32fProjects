/*********************************************************************************/
/* Low Level 入出力用ライブラリ                                                  */
/*                                         designed by hamayan since 2013/01/21  */
/*********************************************************************************/

#include  <stdio.h>
#include  <string.h>
#include  "usart.h"
#include  "peripheral.h"
#include  "derivertive.h"
//#include  "strutil.h"

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
#if 1
/* sbrk で管理する領域サイズ */
static union
{
  long dummy;             /* 4 バイト境界にするためのダミー*/
  char heap[ HEAPSIZE ];  /* sbrk で管理する領域の宣言*/
} heap_area;

static char *brk = (char *)&heap_area; /* sbrk で割り付けた領域の最終アドレス */
#endif

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

/*************************************************************************/
/* printfなどから呼ばれる。                                              */
/*************************************************************************/
int _xwrite(int fd, char *str, int len)
{
  int ret;

  switch( fd )
  {
#if (SCI_1_SND_BUFFER_SIZE > 0)
    case STDOUT :  /*usart1*/
      ret = SCI_Write( SCI_1, (const void *)str, len );
      break;
#endif

#if (SCI_2_SND_BUFFER_SIZE > 0)
    case STDOUT2 :  /*usart2*/
      ret = SCI_Write( SCI_2, (const void *)str, len );
      break;
#endif

#if (SCI_3_SND_BUFFER_SIZE > 0)
    case STDOUT3 :  /*usart3*/
      ret = SCI_Write( SCI_3, (const void *)str, len );
      break;
#endif

#if (SCI_4_SND_BUFFER_SIZE > 0)
    case STDOUT4 :  /*uart4*/
      ret = SCI_Write( SCI_4, (const void *)str, len );
      break;
#endif

#if (SCI_5_SND_BUFFER_SIZE > 0)
    case STDOUT5 :  /*uart5*/
      ret = SCI_Write( SCI_5, (const void *)str, len );
      break;
#endif

#if (SCI_6_SND_BUFFER_SIZE > 0)
    case STDOUT6 :  /*uart6*/
      ret = SCI_Write( SCI_6, (const void *)str, len );
      break;
#endif

    case STDNULL :  /*ターゲット無し*/
    default :
      ret = len;
      break;
  }

  return ret;
}

/*************************************************************************/
/* ?????fなどから呼ばれる。                                              */
/* とりあえず一文字ずつ                                                  */
/*************************************************************************/
int _xread(int fd, char *str, int len)
{
  int c;

  switch( fd )
  {
#if (SCI_1_RCV_BUFFER_SIZE > 0)
    case STDIN :  /*usart1*/
      c = SCI_Getc( SCI_1 );
      break;
#endif /**/

#if (SCI_2_RCV_BUFFER_SIZE > 0)
    case STDOUT2 :  /*usart2*/
      c = SCI_Getc( SCI_2 );
      break;
#endif /**/

#if (SCI_3_RCV_BUFFER_SIZE > 0)
    case STDOUT3 :  /*usart3*/
      c = SCI_Getc( SCI_3 );
      break;
#endif /**/

#if (SCI_4_RCV_BUFFER_SIZE > 0)
    case STDOUT4 :  /*uart4*/
      c = SCI_Getc( SCI_4 );
      break;
#endif /**/

#if (SCI_5_RCV_BUFFER_SIZE > 0)
    case STDOUT5 :  /*uart5*/
      c = SCI_Getc( SCI_5 );
      break;
#endif /**/

#if (SCI_6_RCV_BUFFER_SIZE > 0)
    case STDOUT6 :  /*uart6*/
      c = SCI_Getc( SCI_6 );
      break;
#endif /**/

    default :
      return 0;
      break;
  }

  if( c < 0 ) return 0;
  *str = (char)c;
  return 1;
}

#if 0
/*************************************************************************/
/*  sbrk:メモリ領域の割り付け                                            */
/*  リターン値：割り付けた領域の先頭アドレス(成功)                       */
/*  -1 (失敗)                                                            */
/*************************************************************************/
char *_sbrk( unsigned long size )
{
#if (HEAPSIZE > 0)
  char  *p;

  /* 空き領域のチェック */
  if( brk + size > heap_area.heap + HEAPSIZE ) return (char *)-1;
  p = brk;    /* 領域の割り付け*/
  brk += size;  /* 最終アドレスの更新*/
  return p;
#else
  return (char *)(-1);
#endif
}

/*************************************************************************/
/*  HEAPの残りサイズを報告                                               */
/*************************************************************************/
int LastHeap( void )
{
  int ret;

  ret = HEAPSIZE - (int)((unsigned long)brk - (unsigned long)&heap_area);

  return ret;
}

/*************************************************************************/
/*  HEAPチェックとメッセージ                                             */
/*  問題が無ければ0を返し、あれば0以外を返す。                           */
/*************************************************************************/
int HeapCheck( void *h )
{
  if( h == 0 )
  {
    (void)_xwrite(1, "HEAP確保失敗\r\n", sizeof("HEAP確保失敗\r\n") - 1 );
    return (-1);
  }

  return 0;
}
#endif

/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2013/01/21  */
/*********************************************************************************/
