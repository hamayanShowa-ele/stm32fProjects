/*********************************************************************************/
/* FatFsユーティリティ                                                           */
/*                                         designed by hamayan since 2014/09/09  */
/*********************************************************************************/
#include  <string.h>
#include  <time.h>
#include  "derivertive.h"
#include  "strutil.h"
#include  "ff.h"
#include  "rtc.h"
//#include  "FatFsUtil.h"

/*************************************************************************/
/* その他の定義                                                          */
/*************************************************************************/
#define  DrvNum2DrvName(name,drv)  do{ name[0] = drv + '0'; name[1] = ':'; name[2] = '\0'; }while(0)

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
//static char DriveString[ _DRIVES ][ 16 ];
static FATFS Fatfs[ _DRIVES ];  /* File system object for each logical drive */

  /*************************************************************************/
  /* エラーコード                                                          */
  /*************************************************************************/
  static const char *fr_result_msgs[] =
  {
    "FR OK",              /* 0 */
    "FR DISK ERR",        /* 1 */
    "FR INT ERR",         /* 2 */
    "FR NOT READY",       /* 3 */
    "FR NO FILE",         /* 4 */
    "FR NO PATH",         /* 5 */
    "FR INVALID NAME",    /* 6 */
    "FR DENIED",          /* 7 */
    "FR EXIST",           /* 8 */
    "FR INVALID OBJECT",  /* 9 */
    "FR WRITE PROTECTED", /* 10 */
    "FR INVALID DRIVE",   /* 11 */
    "FR NOT ENABLED",     /* 12 */
    "FR NO FILESYSTEM",   /* 13 */
    "FR MKFS ABORTED",    /* 14 */
    "FR TIMEOUT",         /* 15 */
  };


/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
extern void localTime_Get( struct tm *localTime );

/*************************************************************************/
/* 端子定義                                                              */
/*************************************************************************/

/*************************************************************************/
/* ドライブのマウント                                                    */
/* 引数はドライブ番号                                                    */
/*************************************************************************/
FRESULT FatFsMount( int drv )
{
  if(drv >= _DRIVES) return  FR_INVALID_DRIVE;

  return f_mount( drv, &Fatfs[ drv ] );
}

/*************************************************************************/
/* ドライブのアンマウント                                                    */
/* 引数はドライブ番号                                                    */
/*************************************************************************/
FRESULT FatFsUnMount( int drv )
{
  if(drv >= _DRIVES) return  FR_INVALID_DRIVE;

  return f_mount( drv, 0 );
}

/*************************************************************************/
/* ドライブのディレクトリを取る                                          */
/* とりあえずルートディレクトリ                                          */
/*************************************************************************/
void FatFsDir( int drv )
{
  char path[8],tmpbuf[16],buf[128];
#if _USE_LFN
  char *Lfname;
#endif /*_USE_LFN*/
  UINT file_nums,dir_nums;
  long total_file_size;
  FRESULT result;
  DIR LocalDir;  /*ディレクトリオブジェクト構造体*/
  FATFS *fs;  /* Pointer to file system object */
  FILINFO Finfo;

  /*ドライブ文字からドライブ番号を得る*/
  DrvNum2DrvName( path, drv );

  if( (result = f_opendir( &LocalDir, path )) != FR_OK )  /*現状、Rootのみ検索*/
  {
    xprintf( STDOUT,"%s\r\n", fr_result_msgs[ result ] );
  }

  /**/
  total_file_size = 0;
  file_nums = dir_nums = 0;

  for(;;)
  {
#if _USE_LFN
    char Lfname[512];
    Finfo.lfname = Lfname;
    Finfo.lfsize = sizeof(Lfname);
#endif /*_USE_LFN*/
    result = f_readdir( &LocalDir, &Finfo );
    if( (result != FR_OK) || !Finfo.fname[0] )
    {
      break;
    }

    if( Finfo.fattrib & AM_DIR )
    {
      dir_nums++;
    }
    else
    {
      file_nums++;
      total_file_size += Finfo.fsize;
    }
    strcpy( buf, (Finfo.fattrib & AM_DIR) ? "D" : "-" );
    strcat( buf, (Finfo.fattrib & AM_RDO) ? "R" : "-" );
    strcat( buf, (Finfo.fattrib & AM_HID) ? "H" : "-" );
    strcat( buf, (Finfo.fattrib & AM_SYS) ? "S" : "-" );
    strcat( buf, (Finfo.fattrib & AM_ARC) ? "A " : "- " );
    strcat( buf, int2Ascii( tmpbuf, (Finfo.fdate >> 9) + 1980 ) );
    strcat( buf, "/" );
    strcat( buf, int2Ascii( tmpbuf, (Finfo.fdate >> 5) & 15 ) );
    strcat( buf, "/" );
    strcat( buf, int2Ascii( tmpbuf, Finfo.fdate & 31 ) );
    strcat( buf, " " );
    strcat( buf, int2Ascii( tmpbuf, (Finfo.ftime >> 11) ) );
    strcat( buf, ":" );
    strcat( buf, int2Ascii( tmpbuf, (Finfo.ftime >> 5) & 63 ) );
    strcat( buf, " " );
    strcat( buf, int2Ascii( tmpbuf, Finfo.fsize ) );
    strcat( buf, "  " );
    strcat( buf, &(Finfo.fname[0]) );

#if _USE_LFN
    strcat( buf, "  " );
    strcat( buf, Lfname );
    strcat( buf, "\r\n" );
    xprintf( STDOUT, buf );
#else
    strcat( buf, "\r\n" );
    xprintf( STDOUT, buf );
#endif
  }

  strcpy( buf, uint2Ascii( tmpbuf, file_nums ) );
  strcat( buf, " File(s)," );
  strcat( buf, uint2Ascii( tmpbuf, total_file_size ) );
  strcat( buf, " bytes total\r\n" );
  strcat( buf, uint2Ascii( tmpbuf, dir_nums ) );
  strcat( buf, " Dir(s)" );
  xprintf( STDOUT, buf );

  if( f_getfree( path, (DWORD*)&total_file_size, &fs ) == FR_OK )
  {
    strcpy( buf, ", " );
    strcat( buf, uint2Ascii( tmpbuf, total_file_size * fs->csize * 512 ) );
    strcat( buf, " bytes free\r\n" );
    xprintf( STDOUT, buf );
  }
}


/*************************************************************************/
/* ドライブのファイルを読み出す                                          */
/*************************************************************************/
FRESULT FatFsRead( int drv, const char *fn )
{
  FIL fp;
  char line[128];
  FRESULT fr;

  if( (fr = f_open(&fp,fn,FA_READ)) != FR_OK ) return fr;

  /* 1行ずつ読み出して表示 */
  while (f_gets(line, sizeof(line), &fp))
  {
    xprintf( STDOUT, line );
    xprintf( STDOUT, "\r" );
  }
  /* ファイルを閉じる */
  f_close(&fp);

  return FR_OK;
}

/*************************************************************************/
/* ファイルに追記モードで書き込みを行う。ファイルが無い時は新たに生成する。                                         */
/*************************************************************************/
FRESULT FatFsAppendWrite( int drv, const char *fn, const void *data, size_t sz )
{
  FIL fp;
  FRESULT fr;
  FILINFO info;

  /* 既存ファイルを書き込みで開く */
  if( (fr = f_open(&fp,fn,FA_OPEN_ALWAYS | FA_WRITE)) != FR_OK ) return fr;

  /* ファイルの最後に移動する */
  fr = f_stat(fn,&info);
  fr = f_lseek(&fp,info.fsize);
  if(fr == FR_OK)
  {
    /* データの書き込み */
    size_t bw;
    fr = f_write(&fp, data, sz, &bw);
  }

  /* ファイルを閉じる */
  f_close(&fp);

  return fr;
}

/*************************************************************************/
/* ファイルに上書きモードで書き込みを行う。ファイルが無い時は新たに生成する。                                         */
/*************************************************************************/
FRESULT FatFsOverWrite( int drv, const char *fn, const void *data, size_t sz )
{
  FIL fp;
  FRESULT fr;

  /* 既存ファイルを書き込みで開く */
  if( (fr = f_open(&fp,fn,FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK ) return fr;

  /* データの書き込み */
  size_t bw;
  fr = f_write(&fp, data, sz, &bw);

  /* ファイルを閉じる */
  f_close(&fp);

  return fr;
}

/********************************************************************/
/* FatFsの時刻データ変換                                            */
/********************************************************************/
bool rtc_gettime( RTC_t *rtc )
{
  struct tm localTime;

  localTime_Get( &localTime );
  rtc->year = localTime.tm_year + 1900 - 1980;
  rtc->month = localTime.tm_mon + 1;
  rtc->mday = localTime.tm_mday;
  rtc->hour = localTime.tm_hour;
  rtc->min = localTime.tm_min;
  rtc->sec = localTime.tm_sec;
  rtc->wday = localTime.tm_wday;

  return true;
}


/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2014/09/09  */
/*********************************************************************************/
