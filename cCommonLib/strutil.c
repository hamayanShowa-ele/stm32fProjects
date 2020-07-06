/*************************************************************************/
/* 文字列処理ユーティリティ集                                            */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
#include  <strutil.h>

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/

/*************************************************************************/
/* その他定義                                                            */
/*************************************************************************/

/*************************************************************************/
/* 大域変数宣言                                                          */
/*************************************************************************/
static const char hex[][3] =
{
  "0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F",
  "10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
  "20",
};

//extern unsigned short BGColor;     /*?w?i?F*/
//extern unsigned short STRColor;    /*?????F*/

/*************************************************************************/
/* uintToAscii                                                           */
/* 引数numをASCII変換してdestに書き込む                                  */
/* destには充分なサイズが必要                                            */
/*************************************************************************/
char *uint2Ascii( char *dest, unsigned int num )
{
  signed char value[ 10 ];
  int i,j;
  static const char dec[][3] =
  {
    "0","1","2","3","4","5","6","7","8","9",
    "10","11","12","13","14","15","16","17","18","19",
    "20","21","22","23","24",
  };

  if( num <= 24 )
  {
    StrCpy( dest, dec[ num ] );
    return dest;
  }
  else dest[0] = ' ';

  value[ 0 ] =  num / 1000000000L;
  value[ 1 ] = (num % 1000000000L) / 100000000L;
  value[ 2 ] = (num % 100000000L)  / 10000000L;
  value[ 3 ] = (num % 10000000L)   / 1000000L;
  value[ 4 ] = (num % 1000000L)    / 100000L;
  value[ 5 ] = (num % 100000L)     / 10000L;
  value[ 6 ] = (num % 10000L)      / 1000L;
  value[ 7 ] = (num % 1000L)       / 100L;
  value[ 8 ] = (num % 100L)        / 10L;
  value[ 9 ] =  num % 10L;

  for( i = 0; i < 10; i++ )  /*左詰処理*/
  {
    if( value[ i ] == 0 ) continue;
    else break;
  }

  for( j = 0; i < 10; )
  {
    dest[ j++ ] = value[ i++ ] + '0';
  }

  dest[ j ] = '\0';

  return dest;
}

/*************************************************************************/
/* intToAscii                                                            */
/* 引数numをASCII変換してdestに書き込む                                  */
/* destには充分なサイズが必要                                            */
/*************************************************************************/
char *int2Ascii( char *dest, int num )
{
  signed char value[ 10 ];
  int i,j;
  static const char dec[][3] =
  {
    "0","1","2","3","4","5","6","7","8","9",
    "10","11","12","13","14","15","16","17","18","19",
    "20","21","22","23","24",
  };

  if( num >= 0 && num <= 24 )
  {
    StrCpy( dest, dec[ num ] );
    return dest;
  }
  else if( (unsigned long)num == 0x80000000 )
  {
    return StrCpy( dest, "-2147483648" );
  }
  else if( num < 0 )
  {
    dest[0] = '-';
    num = -num;
  }
  else dest[0] = ' ';

  value[ 0 ] =  num / 1000000000L;
  value[ 1 ] = (num % 1000000000L) / 100000000L;
  value[ 2 ] = (num % 100000000L)  / 10000000L;
  value[ 3 ] = (num % 10000000L)   / 1000000L;
  value[ 4 ] = (num % 1000000L)    / 100000L;
  value[ 5 ] = (num % 100000L)     / 10000L;
  value[ 6 ] = (num % 10000L)      / 1000L;
  value[ 7 ] = (num % 1000L)       / 100L;
  value[ 8 ] = (num % 100L)        / 10L;
  value[ 9 ] =  num % 10L;

  for( i = 0; i < 10; i++ )  /*左詰処理*/
  {
    if( value[ i ] == 0 ) continue;
    else break;
  }

  if( dest[0] == '-' )  /*符号を付ける場合*/
  {
    for( j = 1; i < 10; )
    {
      dest[ j++ ] = value[ i++ ] + '0';
    }
  }
  else
  {
    for( j = 0; i < 10; )
    {
      dest[ j++ ] = value[ i++ ] + '0';
    }
  }

  dest[ j ] = '\0';

  return dest;
}

/*************************************************************************/
/* int2Hex                                                              */
/* 引数numを16進文字に変換してdestに書き込む                             */
/* destには充分なサイズが必要                                            */
/*************************************************************************/
char *int2Hex( char *dest, unsigned int num )
{
  unsigned char value[ 8 ];
  int i,j;

  if( num <= 0x20 )
  {
    StrCpy( dest, hex[ num ] );
    return dest;
  }

  value[ 0 ] =  num / 0x10000000;
  value[ 1 ] = (num % 0x10000000) / 0x1000000;
  value[ 2 ] = (num % 0x1000000)  / 0x100000;
  value[ 3 ] = (num % 0x100000)   / 0x10000;
  value[ 4 ] = (num % 0x10000)    / 0x1000;
  value[ 5 ] = (num % 0x1000)     / 0x100;
  value[ 6 ] = (num % 0x100)      / 0x10;
  value[ 7 ] = num % 0x10;

  for( i = 0; i < 8; i++ )  /*左詰処理*/
  {
    if( value[ i ] == 0 ) continue;
    else break;
  }

  for( j = 0; i < 8; )
  {
    dest[ j++ ] = *(hex[ value[ i++ ] ]);
  }
  dest[ j ] = '\0';

  return dest;
}

/*************************************************************************/
/* int2Hex08                                                             */
/* 引数numを16進文字に変換してdestに書き込む                             */
/* destには充分なサイズが必要                                            */
/*************************************************************************/
char *int2Hex08( char *dest, unsigned long num )
{
  unsigned char value[ 8 ];
  int i;
  static const char Zero[] = "00000000";

  if( num == 0 )
  {
    StrCpy( dest, Zero );
    return dest;
  }

  value[ 0 ] =  num / 0x10000000;
  value[ 1 ] = (num % 0x10000000) / 0x1000000;
  value[ 2 ] = (num % 0x1000000)  / 0x100000;
  value[ 3 ] = (num % 0x100000)   / 0x10000;
  value[ 4 ] = (num % 0x10000)    / 0x1000;
  value[ 5 ] = (num % 0x1000)     / 0x100;
  value[ 6 ] = (num % 0x100)      / 0x10;
  value[ 7 ] = num % 0x10;

  for( i = 0; i < 8; i++ )
  {
    dest[ i ] = *(hex[ value[ i ] ]);
  }
  dest[ i ] = '\0';

  return dest;
}

/*************************************************************************/
/* int2Hex04                                                             */
/* 引数numを16進文字に変換してdestに書き込む                             */
/* destには充分なサイズが必要                                            */
/*************************************************************************/
char *int2Hex04( char *dest, unsigned short num )
{
  unsigned char value[ 4 ];
  int i;
  static const char Zero[] = "0000";

  if( num == 0 )
  {
    StrCpy( dest, Zero );
    return dest;
  }

  value[ 0 ] =  num / 0x1000;
  value[ 1 ] = (num % 0x1000) / 0x100;
  value[ 2 ] = (num % 0x100)  / 0x10;
  value[ 3 ] = num % 0x10;

  for( i = 0; i < 4; i++ )
  {
    dest[ i ] = *(hex[ value[ i ] ]);
  }
  dest[ i ] = '\0';

  return dest;
}

/*************************************************************************/
/* int2Hex02                                                             */
/* 引数numを16進文字に変換してdestに書き込む                             */
/* destには充分なサイズが必要                                            */
/*************************************************************************/
char *int2Hex02( char *dest, unsigned char num )
{
  unsigned char value[ 2 ];
  int i;
  static const char Zero[] = "00";

  if( num == 0 )
  {
    StrCpy( dest, Zero );
    return dest;
  }

  value[ 0 ] =  num / 0x10;
  value[ 1 ] = num % 0x10;

  for( i = 0; i < 2; i++ )
  {
    dest[ i ] = *(hex[ value[ i ] ]);
  }
  dest[ i ] = '\0';

  return dest;
}

/*************************************************************************/
/* longLong2Ascii                                                        */
/* 引数numをASCII変換してdestに書き込む                                  */
/* destには充分なサイズが必要                                            */
/*************************************************************************/
char *longLong2Ascii( char *dest, long long num )
{
  signed char value[ 16 ];
  int i,j;

  if( num < 0 )
  {
    dest[0] = '-';
    num = -num;
  }
  else dest[0] = ' ';

  value[ 0 ] =  num / 1000000000000000LL;
  value[ 1 ] = (num % 1000000000000000LL) / 100000000000000LL;
  value[ 2 ] = (num % 100000000000000LL) / 10000000000000LL;
  value[ 3 ] = (num % 10000000000000LL) / 1000000000000LL;
  value[ 4 ] = (num % 1000000000000LL) / 100000000000LL;
  value[ 5 ] = (num % 100000000000LL) / 10000000000LL;
  value[ 6 ] = (num % 10000000000LL) / 1000000000LL;
  value[ 7 ] = (num % 1000000000LL) / 100000000LL;
  value[ 8 ] = (num % 100000000LL) / 10000000LL;
  value[ 9 ] = (num % 10000000LL) / 1000000LL;
  value[ 10] = (num % 1000000LL) / 100000LL;
  value[ 11] = (num % 100000LL) / 10000LL;
  value[ 12] = (num % 10000LL) / 1000LL;
  value[ 13] = (num % 1000LL) / 100LL;
  value[ 14] = (num % 100LL) / 10LL;
  value[ 15] =  num % 10L;

  for( i = 0; i < 10; i++ )  /*左詰*/
  {
    if( value[ i ] == 0 ) continue;
    else break;
  }

  if( dest[0] == '-' )  /*符号判定*/
  {
    for( j = 1; i < 16; )
    {
      dest[ j++ ] = value[ i++ ] + '0';
    }
  }
  else
  {
    for( j = 0; i < 16; )
    {
      dest[ j++ ] = value[ i++ ] + '0';
    }
  }

  dest[ j ] = '\0';

  return dest;
}

/*************************************************************************/
/* strcpy                                                                */
/*************************************************************************/
char *StrCpy( char *dest, const char *src )
{
  char *ptr = dest;

  while( *src ) *ptr++ = *src++;
  *ptr = '\0';

  return dest;
}

/*************************************************************************/
/* strcat                                                                */
/*************************************************************************/
char *StrCat( char *dest, const char *src )
{
  StrCpy( StrChr( dest, '\0' ), src );

  return dest;
}

/*************************************************************************/
/* strchr                                                                */
/*************************************************************************/
char *StrChr( char *src, int c )
{
  while( *src && *src != c ) src++;

  if( *src == '\0' && c != '\0' ) return 0;  /**/
  else return src;
}

/*************************************************************************/
/* strnchr                                                              */
/*************************************************************************/
char *StrNChr( const char *src, int c, unsigned int size )
{
  while( size > 0 && *src && *src != c ){ src++; size--; }

  if( size == 0 || (*src == '\0' && c != '\0') ) return 0;  /**/
  else return (char *)src;
}

/*************************************************************************/
/* memset                                                                */
/*************************************************************************/
void *MemSet( void *s, int c, unsigned int n )
{
  const unsigned char uc = c;
  unsigned char       *p = (unsigned char *)s;

  while( n-- > 0 ) *p++ = uc;

  return s;
}

/*************************************************************************/
/* memcpy                                                                */
/*************************************************************************/
void *MemCpy( void *d, const void *s, unsigned int n )
{
  char *p1 = (char *)d;
  const char *p2 = (const char *)s;

  while( n-- > 0 )
  {
    *p1++ = *p2++;
  }
  return d;
}

/*************************************************************************/
/* 文字列分割ユーティリティ                                             */
/*************************************************************************/
int split( char *str , char *argv[], int sz )
{
  int argc = 0;

  while( *str != '\0' && argc < sz )
  {
    if( isgraph( (int)*str ) != 0 )
    {
      argv[ argc++ ] = str;
      while( *str != '\0' && isgraph( (int)*str ) != 0 ) str++;
    }
    else *str++ = '\0';
  }

  return argc;
}

/*************************************************************************/
/* 文字指定型文字列分割ユーティリティ                                   */
/*************************************************************************/
int split_char( char comp, char *str , char *argv[], int sz )
{
  int argc = 0;

  while( *str != '\0' && argc < sz )
  {
    if( *str != comp && isgraph( (int)*str ) != 0 )
    {
      argv[ argc++ ] = str;
      while( *str != '\0' && *str != comp && isgraph( (int)*str ) != 0 ) str++;
    }
//    else if( *str == comp )
//    {
//      argv[ argc++ ] = str++;
//    }
    else *str++ = '\0';
  }

  return argc;
}

/*************************************************************************/
/* 文字列が数字で構成されているのかを判定                                */
/* 数字以外の文字があれば0、数字であれば1を返す。                        */
/*************************************************************************/
int isDigitString( const char *digit )
{
  if( *digit == '\0' ) return 0;  /**/

  while( *digit )
  {
    if( isdigit( (int)*digit ) == 0 ) return 0;  /**/
    digit++;
  }

  return 1;
}

/*************************************************************************/
/* 文字列が16進文字で構成されているのかを判定                            */
/* 16進文字以外の文字があれば0、16進文字であれば1を返す。                */
/*************************************************************************/
int isHexadecimalString( const char *str )
{
  if( *str == '\0' ) return 0;  /**/

  while( *str )
  {
    if( !isdigit( (int)*str ) )  /**/
    {
      if( (*str & ~0x20) < 'A' || (*str & ~0x20) > 'F' )
        return 0;  /**/
    }
    str++;
  }

  return 1;
}

/*************************************************************************/
/* 小数点位置を指定してnumを文字列に変換する                             */
/*************************************************************************/
char *fixed_little_number( char *dst, long num, int point )
{
  char sign[2],buf1[10],buf2[5];
  int i,dot;
  long large;
  unsigned long little;

  if( num != 0 )
  {
    for( i = 0, dot = 1; i < point; i++ ) dot *= 10;  /**/
    large = num / dot;  /**/
    little = ( num < 0 ) ? (num * (-1)) % dot : num % dot;  /**/
    if( num < 0 && large == 0 ) (void)strcpy( sign, "-" );
    else (void)strcpy( sign, "" );

    (void)int2Ascii( buf1, large );  /**/
    if( little != 0 )
    {
      (void)uint2Ascii( buf2, little );  /**/
      for( i = strlen( buf2 ); i < point; i++ )
      {
        buf2[4] = buf2[3];  /**/
        buf2[3] = buf2[2];
        buf2[2] = buf2[1];
        buf2[1] = buf2[0];
        buf2[0] = '0';
      }
    }
	else
    {
      (void)strcpy( buf2, "0" );
    }

    (void)strcpy( dst, sign );
    (void)strcat( dst, buf1 );
    (void)strcat( dst, "." );
    (void)strcat( dst, buf2 );
  }
  else
  {
    (void)strcpy( dst, "0.0" );
  }

  return dst;
}

/*************************************************************************/
/* 大文字/小文字変換                                                     */
/*************************************************************************/
char *small2large( char *str )
{
  char *ptr = str;

  while( *ptr != '\0' )
  {
    *ptr = (char)toupper((int)*ptr);
    ptr++;
  }

  return str;
}

/*************************************************************************/
/* doubleをビッグエンディアンからリトルエンディアンに変換                */
/*************************************************************************/
void double2byteLittle( char *dst, double d )
{
  char *ptr = (char *)&d;

  dst[0] = *ptr++;
  dst[1] = *ptr++;
  dst[2] = *ptr++;
  dst[3] = *ptr++;
  dst[4] = *ptr++;
  dst[5] = *ptr++;
  dst[6] = *ptr++;
  dst[7] = *ptr;
}

/*************************************************************************/
/* intをビッグエンディアンからリトルエンディアンに変換                   */
/*************************************************************************/
void int2byteLittle( char *dst, int d )
{
  char *ptr = (char *)&d;

  dst[0] = *ptr++;
  dst[1] = *ptr++;
  dst[2] = *ptr++;
  dst[3] = *ptr;
}

/*************************************************************************/
/* doubleをリトルエンディアンからビッグエンディアンに変換                */
/*************************************************************************/
void double2byteBig( char *dst, double d )
{
  char *ptr = (char *)&d;

  dst[7] = *ptr++;
  dst[6] = *ptr++;
  dst[5] = *ptr++;
  dst[4] = *ptr++;
  dst[3] = *ptr++;
  dst[2] = *ptr++;
  dst[1] = *ptr++;
  dst[0] = *ptr;
}

/*************************************************************************/
/* intをリトルエンディアンからビッグエンディアンに変換                   */
/*************************************************************************/
void int2byteBig( char *dst, int d )
{
  char *ptr = (char *)&d;

  dst[3] = *ptr++;
  dst[2] = *ptr++;
  dst[1] = *ptr++;
  dst[0] = *ptr;
}

/*************************************************************************/
/* utf8 convert to utf16                                                 */
/*************************************************************************/
unsigned short utf8to16( const unsigned char utf8[], int sz )
{
  unsigned short a,b,c;

  if( sz > 3 || sz <= 0 ) return 0;

  if(sz == 3)
  {
    a = utf8[0] & 0x0f;
    b = utf8[1] & 0x3f;
    c = utf8[2] & 0x3f;
    a <<= 12;
    b <<= 6;
  }
  else if(sz == 2)
  {
    a = utf8[0] & 0x1f;
    b = utf8[1] & 0x3f;
    c = 0;
    a <<= 6;
  }
  else
  {
    a = utf8[0] & 0x7f;
    b = 0;
    c = 0;
  }

  return a | b | c;
}

/*************************************************************************/
/* hexadecimal to int                                                    */
/*************************************************************************/
int htoi( const char *s )
{
  int n;
//  if ( *s != '0' || !(*(s+1) != 'x' || *(s+1) != 'X') )
//  {
//    return 0;
//  }

//  for( n = 0, s += 2; *s; s++ )
  for( n = 0; *s; s++ )
  {
    if( *s >= '0' && *s <= '9' )
    {
      n = 16 * n + (*s - '0');
    }
    else if( *s >= 'a' && *s <= 'f' )
    {
      n = 16 * n + ((*s - 'a') + 10);
    }
    else if( *s >= 'A' && *s <= 'F' )
    {
      n = 16 * n + ((*s - 'A') + 10);
    }
  }

  return n;
}

/*************************************************************************/
/* コンパイル日を扱いやすいint型で返す                                   */
/* Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec                       */
/*************************************************************************/
void compileDate(int *year, int *month, int *day)
{
  char *argv[4],*str;
  char date[] = __DATE__;

  split( date, argv, 3 );

  *year = atoi(argv[2]);
  *day = atoi(argv[1]);
  str = argv[0];
  *month =
    (*str == 'F') ? 2 :
    (*str == 'S') ? 9 :
    (*str == 'O') ? 10 :
    (*str == 'N') ? 11 :
    (*str == 'D') ? 12 : 0;
  if( *month == 0 )  //jan,jun,jul,
  {
    if(*str == 'J')
    {
      if(str[1] == 'a')
      {
        *month = 1;
      }
      else
      {
        if(str[2] == 'n')
          *month = 6;
        else
          *month = 7;
      }
    }
    else if(*str == 'M')  //mar,may,
    {
      if(str[2] == 'r')
        *month = 3;
      else
        *month = 5;
    }
    else  //apr,aug,
    {
      if(str[1] == 'p')
        *month = 4;
      else
        *month = 8;
    }
  }
}

/*************************************************************************/
/* コンパイル時を扱いやすいint型で返す                                   */
/*************************************************************************/
void compileTime(int *hour, int *minute, int *second)
{
  char *argv[4];
  char Time[] = __TIME__;

  split_char( ':', Time, argv, 3 );
  *hour   = atoi(argv[0]);
  *minute = atoi(argv[1]);
  *second = atoi(argv[2]);
}


/*************************************************************************/
/* pattern match.                                 */
/*************************************************************************/
int patternMatch( const char *compA, const char *compB )
{
  if( !*compA || !*compB ) return (-1);
  if( strcmp( compA ,compB ) == 0 ) return 0;

  int lenA = strlen( compA );
  int lenB = strlen( compB );
  int loop = (lenA >= lenB) ? lenA - lenB : lenB - lenA;
  loop += 1;

  const char *a,*b;
  int n;
  if(lenA >= lenB) { a = compA; b = compB; n = lenB; }
  else { a = compB; b = compA; n = lenA; }

  int i;
  for( i = 0; i < loop; i++ )
  {
    if( strncmp(a,b,n) == 0 ) break;
    a++;
  }
  if( i == loop ) return (-1);

  return 0;
}

/*************************************************************************/
/* ISO 8601形式の時刻表記をunixtime型(UTC)に変換                         */
/*                                                                       */
/* ISO 8601が定めるところの表記方法は二種類あります。                    */
/* 一つは基本表記（標準表記）、もう一つは拡張表記です。                  */
/*                                                                       */
/* ●基本表記                                                            */
/* 基本表記は以下の書式をとります。                                      */
/*                                                                       */
/*   YYYYMMDDThhmmssZ                                                    */
/*                                                                       */
/*   YYYYMMDDThhmmss+0900                                                */
/*                                                                       */
/* YYYYは西暦の4桁の数字、MMは2桁の月、DDは2桁の日、                     */
/* Tは日付と時刻の区切り、                                               */
/* hhは2桁の時、mmは2桁の分、ssは2桁の秒です。                           */
/*                                                                       */
/* 最後のZは時刻が協定世界時(UTC)である事を示します。                    */
/*                                                                       */
/* UTCではないローカルな時間はUTCからの進みまたは遅れを+記号(進み)       */
/* または-記号(遅れ)に続けてその時間を4桁で記載します。                  */
/*                                                                       */
/* 例えば日本の場合日本標準時(JST)はUTCから9時間進んでい                 */
/* ますので、2014年6月21日12時ちょうどは                                 */
/*                                                                       */
/*   20140621T120000+0900                                                */
/* となります。                                                          */
/*                                                                       */
/*                                                                       */
/* ●拡張表記                                                            */
/* 拡張表記は以下の書式をとります。                                      */
/*                                                                       */
/*   YYYY-MM-DDThh:mm:ssZ                                                */
/*                                                                       */
/*   YYYY-MM-DDThh:mm:ss+09:00                                           */
/*                                                                       */
/* 日付では区切り文字としてハイフン(-)、時刻では区切り文字として         */
/* コロン(:)を使用し読み易くしています。                                 */
/* ※ss、つまり秒に関しては小数点以下の表示まで存在するので、ここを固定ととらえない事。     */
/*************************************************************************/
time_t iso8601ToUnixtime( const char *iso8601 )
{
  time_t RagLead;
  char zone,*dst,temp[8];
  struct tm tim;
  const char *src = iso8601;

  /*基本表記と拡張表記の判定*/
  if( iso8601[8] == 'T' )  /* 基本表記 YYYYMMDDThhmmssZまたはYYYYMMDDThhmmss+0900 */
  {                        /*          0123456789012345      01234567890123456789 */
    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_year = atoi( temp ) - 1900;

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_mon = atoi( temp ) - 1;

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_mday = atoi( temp );

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_hour = atoi( temp );

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_min = atoi( temp );

    float secF;
    char offset[8];
    sscanf( &iso8601[13], "%f%c%s", &secF, &zone, offset );
    tim.tm_sec = (int)(secF + 0.5f);
    if(zone != 'Z')
    {
      temp[0] = offset[0];
      temp[1] = offset[1];
      temp[2] = offset[2];
      temp[3] = offset[3];
      temp[4] = '\0';
      RagLead = (time_t)(atoi( temp ) * 3600L);
    }
  }
  else  /* 拡張表記 YYYY-MM-DDThh:mm:ssZまたはYYYY-MM-DDThh:mm:ss+09:00 */
  {     /*          01234567890123456789      0123456789012345678901234 */
    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_year = atoi( temp ) - 1900;
    src++;  /* - */

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_mon = atoi( temp ) - 1;
    src++;  /* - */

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_mday = atoi( temp );
    src++;  /* T */

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_hour = atoi( temp );
    src++;  /* : */

    dst = temp;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = '\0';
    tim.tm_min = atoi( temp );
    //src++;  /* : */

    float secF;
    char offset[8];
    sscanf( &iso8601[17], "%f%c%s", &secF, &zone, offset );
    tim.tm_sec = (int)(secF + 0.5f);
    if(zone != 'Z')
    {
      temp[0] = offset[0];
      temp[1] = offset[1];
      temp[2] = offset[3];
      temp[3] = offset[4];
      temp[4] = '\0';
      RagLead = (time_t)(atoi( temp ) * 3600L);
    }
  }


  time_t unix = mktime( &tim );

  if( zone == '+' )  /*ローカルが進みとなっている*/
  {
    unix -= RagLead;
  }
  else if( zone == '-' )  /*ローカルが遅れとなっている*/
  {
    unix += RagLead;
  }
  else {}  /*UTC*/

  return unix;
}


/*****************************************************/
/* dtostrf                                           */
/*****************************************************/
char *dtostrf(double number, signed char width, unsigned char prec, char *s)
{
  typedef  unsigned char bool;
  #define  FALSE  0
  #define  TRUE   !FALSE
  bool negative = FALSE;
  if( isnan(number) )
  {
    strcpy(s, "nan");
    return s;
  }
  if( isinf(number) )
  {
    strcpy(s, "inf");
    return s;
  }

  char *out = s;
  int fillme = width; // how many cells to fill for the integer part
  if( prec > 0 ) fillme -= (prec+1);

  // Handle negative numbers
  if( number < 0.0 )
  {
    negative = TRUE;
    fillme--;
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  // I optimized out most of the divisions
  double rounding = 2.0;
  for( uint8_t i = 0; i < prec; ++i )
  {
    rounding *= 10.0;
  }
  rounding = 1.0 / rounding;
  number += rounding;

  // Figure out how big our number really is
  double tenpow = 1.0;
  int digitcount = 1;
  while (number >= 10.0 * tenpow)
  {
    tenpow *= 10.0;
    digitcount++;
  }

  number /= tenpow;
  fillme -= digitcount;

  // Pad unused cells with spaces
  while( fillme-- > 0 ) *out++ = ' ';

  // Handle negative sign
  if( negative ) *out++ = '-';

  // Print the digits, and if necessary, the decimal point
  digitcount += prec;
  int8_t digit = 0;
  while( digitcount-- > 0 )
  {
    digit = (int8_t)number;
    if (digit > 9) digit = 9; // insurance
    *out++ = (char)('0' | digit);
    if( (digitcount == prec) && (prec > 0) )
    {
      *out++ = '.';
    }
    number -= digit;
    number *= 10.0;
  }

  // make sure the string is terminated
  *out = 0;
  return s;
}

/*************************************************************************/
/* end of file                                                           */
/*                                 designed by hamayan since 2008/11/06  */
/*************************************************************************/
