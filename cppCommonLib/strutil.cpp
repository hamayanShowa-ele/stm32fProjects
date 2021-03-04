/* ----------------------------------------
  string utilities code

  Copyright (c) 2020 hamayan (hamayan@showa-ele.jp).
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Created 2020 by hamayan (hamayan@showa-ele.jp)
---------------------------------------- */
#include  <strutil.h>

/* ----------------------------------------
    defines
---------------------------------------- */
#if !defined( TIMEZONE_JST )
#define  TIMEZONE_JST  (9 * 3600UL)
#endif

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
static const char hex[][3] =
{
  "0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F",
  "10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
  "20",
};

/* ----------------------------------------
    prototypes
---------------------------------------- */


/* ----------------------------------------
  uintToAscii
  Converts num to ASCII and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
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
    strcpy( dest, dec[ num ] );
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

  for( i = 0; i < 10; i++ )  /* left justification */
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

/* ----------------------------------------
  intToAscii
  Converts num to ASCII and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
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
    strcpy( dest, dec[ num ] );
    return dest;
  }
  else if( (unsigned long)num == 0x80000000 )
  {
    return strcpy( dest, "-2147483648" );
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

  for( i = 0; i < 10; i++ )  /* left justification */
  {
    if( value[ i ] == 0 ) continue;
    else break;
  }

  if( dest[0] == '-' )  /* When signing. */
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

/* ----------------------------------------
  longLong2Ascii
  Converts num to ASCII and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
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

  for( i = 0; i < 10; i++ )  /* left justification */
  {
    if( value[ i ] == 0 ) continue;
    else break;
  }

  if( dest[0] == '-' )  /* When signing. */
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

/* ----------------------------------------
  int2Hex
  Converts the argument num to hexadecimal characters
    and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
char *int2Hex( char *dest, unsigned int num )
{
  unsigned char value[ 8 ];
  int i,j;

  if( num <= 0x20 )
  {
    strcpy( dest, hex[ num ] );
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

  for( i = 0; i < 8; i++ )  /* left justification */
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

/* ----------------------------------------
  int2Hex08
  Converts the argument num to hexadecimal characters
    and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
char *int2Hex08( char *dest, unsigned long num )
{
  unsigned char value[ 8 ];
  int i;
  static const char Zero[] = "00000000";

  if( num == 0 )
  {
    strcpy( dest, Zero );
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

/* ----------------------------------------
  int2Hex04
  Converts the argument num to hexadecimal characters
    and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
char *int2Hex04( char *dest, unsigned short num )
{
  unsigned char value[ 4 ];
  int i;
  static const char Zero[] = "0000";

  if( num == 0 )
  {
    strcpy( dest, Zero );
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

/* ----------------------------------------
  int2Hex02
  Converts the argument num to hexadecimal characters
    and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
char *int2Hex02( char *dest, unsigned char num )
{
  unsigned char value[ 2 ];
  int i;
  static const char Zero[] = "00";

  if( num == 0 )
  {
    strcpy( dest, Zero );
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

/* ----------------------------------------
  split
  String Splitting Utility
---------------------------------------- */
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

/* ----------------------------------------
  split
  Character-specified string splitting utility
---------------------------------------- */
int split( char comp, char *str , char *argv[], int sz )
{
  int argc = 0;
  char *origin = str;

  while( *str != '\0' && argc < sz )
  {
    if( *str != comp && isgraph( (int)*str ) != 0 )
    {
      argv[ argc++ ] = str;
      while( *str != '\0' && *str != comp && isgraph( (int)*str ) != 0 ) str++;
    }
    else if( *str == comp )
    {
      *str = '\0';
      if( str != origin && *(str - 1) == '\0' )
      {
        argv[ argc++ ] = str;
      }
      str++;
    }
    else *str++ = '\0';
  }

  return argc;
}

/* ----------------------------------------
  isDigitString
  Determine if the string consists of numbers.
  Returns 0 for any non-numeric character and 1 for a number.
---------------------------------------- */
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

/* ----------------------------------------
  isHexadecimalString
  Determine if the string consists of hexadecimal characters.
  Returns 0 for any non-hexadecimal character and 1 for a hexadecimal character.
---------------------------------------- */
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

/* ----------------------------------------
  fixed_little_number
  Converts num to a string with a decimal point.
---------------------------------------- */
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

/* ----------------------------------------
  small2large
  Conversion of uppercase and lowercase letters.
---------------------------------------- */
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

/* ----------------------------------------
  double2byteLittle
  Convert a double type to little endian from big endian.
---------------------------------------- */
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

/* ----------------------------------------
  int2byteLittle
  Convert a int type to little endian from big endian.
---------------------------------------- */
void int2byteLittle( char *dst, int d )
{
  char *ptr = (char *)&d;

  dst[0] = *ptr++;
  dst[1] = *ptr++;
  dst[2] = *ptr++;
  dst[3] = *ptr;
}

/* ----------------------------------------
  double2byteBig
  Convert a double type to big endian from little endian.
---------------------------------------- */
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

/* ----------------------------------------
  int2byteBig
  Convert a int type to big endian from little endian.
---------------------------------------- */
void int2byteBig( char *dst, int d )
{
  char *ptr = (char *)&d;

  dst[3] = *ptr++;
  dst[2] = *ptr++;
  dst[1] = *ptr++;
  dst[0] = *ptr;
}

/* ----------------------------------------
  utf8to16
  utf8 convert to utf16.
---------------------------------------- */
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

/* ----------------------------------------
  htoi
  hexadecimal to int.
---------------------------------------- */
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

/* ----------------------------------------
  patternMatch
  pattern match.
---------------------------------------- */
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

/* ----------------------------------------
  compileDate
  Returns the compile date in a manageable int type.
  Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
---------------------------------------- */
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

/* ----------------------------------------
  compileTime
  Returns the compile time in a manageable int type.
---------------------------------------- */
void compileTime(int *hour, int *minute, int *second)
{
  char *argv[4];
  char Time[] = __TIME__;

  split( ':', Time, argv, 3 );
  *hour   = atoi(argv[0]);
  *minute = atoi(argv[1]);
  *second = atoi(argv[2]);
}

/* ----------------------------------------
  utcDateTimeToUnixtime
  convert to utc date and time to unix time
  Returns the seconds from unix time.
---------------------------------------- */
time_t utcDateTimeToUnixtime( uint16_t y, uint8_t mo, uint8_t d,
  uint8_t h, uint8_t mi, uint8_t s )
{
  struct tm localTime;
  localTime.tm_year = y - 1900;
  localTime.tm_mon = mo - 1;
  localTime.tm_mday = d;
  localTime.tm_hour = h;
  localTime.tm_min = mi;
  localTime.tm_sec = s;
  time_t t = mktime( &localTime );
  return t;
}

/* ----------------------------------------
  localDateTimeToUnixtime
  convert to local date and time to unix time
  Returns the seconds from unix time.
---------------------------------------- */
time_t localDateTimeToUnixtime( uint16_t y, uint8_t mo, uint8_t d,
  uint8_t h, uint8_t mi, uint8_t s )
{
  time_t ut = utcDateTimeToUnixtime( y, mo, d, h, mi, s );
  return ut - TIMEZONE_JST;
}

/* ----------------------------------------
  utcDateTime
  unix time(ut) converted to utc date and time strings.
  no returns.
---------------------------------------- */
void utcDateTime( time_t ut,
  uint16_t *year, uint8_t *month, uint8_t *day,
  uint8_t *hour, uint8_t *minute, uint8_t *second )
{
  struct tm *t = localtime( (const time_t *)&ut );
  *year = t->tm_year + 1900;
  *month = t->tm_mon + 1;
  *day = t->tm_mday;
  *hour = t->tm_hour;
  *minute = t->tm_min;
  *second = t->tm_sec;
}

/* ----------------------------------------
  localDateTime
  unix time(ut) converted to local date and time strings.
  no returns.
---------------------------------------- */
void localDateTime( time_t ut,
  uint16_t *year, uint8_t *month, uint8_t *day,
  uint8_t *hour, uint8_t *minute, uint8_t *second )
{
  utcDateTime( ut + TIMEZONE_JST, year, month, day, hour, minute, second );
}

/* ----------------------------------------
  utcDateString
  Returns the utc date string from unix time.
---------------------------------------- */
char *utcDateString( char *dst, time_t ut )
{
  struct tm *t = localtime( (const time_t *)&ut );
  uint16_t year  = t->tm_year + 1900;
  uint8_t  month = t->tm_mon + 1;
  uint8_t  day   = t->tm_mday;

  sprintf( dst, "%d/%02d/%02d",
    year, month, day );
  return dst;
}

/* ----------------------------------------
  local date string
  Returns the local date string from unix time.
---------------------------------------- */
char *localDateString( char *dst, time_t ut )
{
  return utcDateString( dst, ut + TIMEZONE_JST );
}

/* ----------------------------------------
  utcTimeString
  Returns the utc time string from unix time.
---------------------------------------- */
char *utcTimeString( char *dst, time_t ut )
{
  struct tm *t = localtime( (const time_t *)&ut );
  uint8_t  hour  = t->tm_hour;
  uint8_t  minute = t->tm_min;
  uint8_t  second = t->tm_sec;

  sprintf( dst, "%02d:%02d:%02d",
    hour, minute, second );
  return dst;
}

/* ----------------------------------------
  local time string
  Returns the local time string from unix time.
---------------------------------------- */
char *localTimeString( char *dst, time_t ut )
{
  return utcTimeString( dst, ut + TIMEZONE_JST );
}

/* ----------------------------------------
  utcDateTimeString
  Returns the utc date and time string from unix time.
---------------------------------------- */
char *utcDateTimeString( char *dst, time_t ut )
{
  utcDateString( dst, ut );
  strcat( dst, " " );
  int len = strlen( dst );
  utcTimeString( dst + len, ut );

  return dst;
}

/* ----------------------------------------
  local date and time string
  Returns the local date and time string from unix time.
---------------------------------------- */
char *localDateTimeString( char *dst, time_t ut )
{
  localDateString( dst, ut );
  strcat( dst, " " );
  int len = strlen( dst );
  localTimeString( dst + len, ut );

  return dst;
}

/* ----------------------------------------
  weekDay
  Returns the local date and time string from unix time.
---------------------------------------- */
int weekDay( time_t ut )
{
  struct tm *t = localtime( (const time_t *)&ut );
  return t->tm_wday;
}

/* ----------------------------------------
  weekDayFromYMD
  Returns the local date and time string from unix time.
---------------------------------------- */
int weekDayFromYMD( uint16_t y, uint8_t mo, uint8_t d )
{
  time_t ut = utcDateTimeToUnixtime( y, mo, d, 0, 0, 0 );
  return weekDay( ut );
}

/* ----------------------------------------
  iso8601ToUnixtime
  Convert ISO 8601 time format to unixtime (UTC).

  There are two types of notation as defined by ISO 8601.
  One is the basic notation (standard notation) and the other is the extended notation.

  Basic notation
  The basic notation format is as follows
    YYYYMMDDThhmmssZ
    YYYYMMDDThhmmss+0900

    YYYY is a 4-digit number in the western calendar,
    MM is a 2-digit month, and DD is a 2-digit day.
    T is for date and time separation.
    hh is for two digits hours,
    mm is for two-digit minutes, and ss is for two-digit seconds.
    The last Z indicates that the time is in Coordinated Universal Time (UTC).

    For local times that are not in UTC,
     the advance or delay from UTC is indicated by a + sign (advance)
     or a - sign (delay) followed by the time in 4 digits.

    For example, Japan Standard Time (JST) is 9 hours ahead of UTC in Japan,
     so exactly 12:00 pm on June 21, 2014 would be 20140621T120000+0900.

  Extended notation
  The extended notation has the following format.
    YYYY-MM-DDThh:mm:ssZ
    YYYY-MM-DDThh:mm:ss+09:00

    Dates are separated by a hyphen (-)
     and times are separated by a colon (:) to make them easier to read.

    In the case of ss, there is even a fractional number of seconds,
     so don't take this as a fixed value.
---------------------------------------- */
time_t iso8601ToUnixtime( const char *iso8601 )
{
  time_t RagLead;
  char zone,*dst,temp[8];
  struct tm tim;
  const char *src = iso8601;

  /* Determination of basic notation and extended notation */
  if( iso8601[8] == 'T' )  /* Basic notation YYYYMMDDThhmmssZ or YYYYMMDDThhmmss+0900 */
  {                        /*                0123456789012345    01234567890123456789 */
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
  else  /* Extended notation YYYY-MM-DDThh:mm:ssZ or YYYY-MM-DDThh:mm:ss+09:00 */
  {     /*                   01234567890123456789    0123456789012345678901234 */
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

  if( zone == '+' )  /* Local time has become an advance. */
  {
    unix -= RagLead;
  }
  else if( zone == '-' )  /* The local time has been delayed. */
  {
    unix += RagLead;
  }
  else {}  /*UTC*/

  return unix;
}


/* ----------------------------------------
  dtostrf
---------------------------------------- */
char *dtostrf(double number, signed char width, unsigned char prec, char *s)
{
  bool negative = false;
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
    negative = true;
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

/* ----------------------------------------
  swap
---------------------------------------- */
uint8_t swap( uint8_t c )
{
  uint8_t upper = c >> 4;
  uint8_t lower = (c & 0x0F) << 4;
  return upper | lower;
}

uint16_t swap( uint16_t w )
{
  uint16_t upper = w >> 8;
  uint16_t lower = (w & 0xFF) << 8;
  return upper | lower;
}

uint32_t swap( uint32_t dw )
{
  uint32_t uu = (dw >> 24) & 0x000000FF;
  uint32_t ul = (dw >> 8)  & 0x0000FF00;
  uint32_t lu = (dw << 8)  & 0x00FF0000;
  uint32_t ll = (dw << 24) & 0xFF000000;
  return uu | ul | lu | ll;
}

/* ----------------------------------------
  byte dump.
---------------------------------------- */
static char *dumpByteLine( char *dst, const uint8_t *data )
{
  int2Hex08( dst, (uint32_t)data );
  strcat( dst, " : " );
  const uint8_t *ptr = data;
  for( int col = 0; col < 16; col++ )
  {
    char asc[4];
    strcat( dst, int2Hex02( asc, *ptr++ ) );
    if( col == 7 ) { strcat( dst, ", " ); }
    else { strcat( dst, "," ); }
  }

  strcat( dst, "  // " );
  int len = strlen( dst );
  for( int col = 0; col < 16; col++ )
  {
    dst[ len++ ] = ( isprint(*data) ) ? *data : '.';
    if( col == 15 )
    {
      dst[ len ] = '\0';
      strcat( dst, "\r\n" );
    }
    data++;
  }

  return dst;
}

#if 0
void dump( const uint8_t *data, uint32_t sz, Serial *s )
{
  int rawLimit = ((sz - 1) / 16) + 1;
  for( int raw = 0; raw < rawLimit; raw++ )
  {
    char buffer[128];
    dumpByteLine( buffer, data );
    s->puts( (const char *)buffer );
    data += 16;
    dly_tsk( 20UL );
  }
  s->puts( "\r\n" );
}
#else
void dump( const uint8_t *data, uint32_t sz, USART_UART *s )
{
  int rawLimit = ((sz - 1) / 16) + 1;
  for( int raw = 0; raw < rawLimit; raw++ )
  {
    char buffer[128];
    dumpByteLine( buffer, data );
    s->print( (const char *)buffer );
    data += 16;
    dly_tsk( 20UL );
  }
  s->print( "\r\n" );
}
#endif

/* ----------------------------------------
  word dump.
---------------------------------------- */
static char *dumpWordLine( char *dst, const uint16_t *data )
{
  int2Hex08( dst, (uint32_t)data );
  strcat( dst, " : " );
  const uint16_t *ptr = data;
  for( int col = 0; col < 8; col++ )
  {
    char asc[8];
    strcat( dst, int2Hex04( asc, *ptr++ ) );
    if( col == 3 ) { strcat( dst, ", " ); }
    else { strcat( dst, "," ); }
  }
  strcat( dst, "\r\n" );

  return dst;
}

#if 0
void dump( const uint16_t *data, uint32_t sz, Serial *s )
{
  sz /= sizeof(uint16_t);
  int rawLimit = ((sz - 1) / 8) + 1;
  for( int raw = 0; raw < rawLimit; raw++ )
  {
    char buffer[128];
    dumpWordLine( buffer, data );
    s->puts( (const char *)buffer );
    data += 8;
    dly_tsk( 20UL );
  }
  s->puts( "\r\n" );
}
#else
void dump( const uint16_t *data, uint32_t sz, USART_UART *s )
{
  sz /= sizeof(uint16_t);
  int rawLimit = ((sz - 1) / 8) + 1;
  for( int raw = 0; raw < rawLimit; raw++ )
  {
    char buffer[128];
    dumpWordLine( buffer, data );
    s->print( (const char *)buffer );
    data += 8;
    dly_tsk( 20UL );
  }
  s->print( "\r\n" );
}
#endif

/* ----------------------------------------
  double word dump.
---------------------------------------- */
static char *dumpDWordLine( char *dst, const uint32_t *data )
{
  int2Hex08( dst, (uint32_t)data );
  strcat( dst, " : " );
  const uint32_t *ptr = data;
  for( int col = 0; col < 4; col++ )
  {
    char asc[12];
    strcat( dst, int2Hex08( asc, *ptr++ ) );
    if( col == 1 ) { strcat( dst, ", " ); }
    else { strcat( dst, "," ); }
  }
  strcat( dst, "\r\n" );

  return dst;
}

#if 0
void dump( const uint32_t *data, uint32_t sz, Serial *s )
{
  sz /= sizeof(uint32_t);
  int rawLimit = ((sz - 1) / 4) + 1;
  for( int raw = 0; raw < rawLimit; raw++ )
  {
    char buffer[128];
    dumpDWordLine( buffer, data );
    s->puts( (const char *)buffer );
    data += 4;
    dly_tsk( 20UL );
  }
  s->puts( "\r\n" );
}
#else
void dump( const uint32_t *data, uint32_t sz, USART_UART *s )
{
  sz /= sizeof(uint32_t);
  int rawLimit = ((sz - 1) / 4) + 1;
  for( int raw = 0; raw < rawLimit; raw++ )
  {
    char buffer[128];
    dumpDWordLine( buffer, data );
    s->print( (const char *)buffer );
    data += 4;
    dly_tsk( 20UL );
  }
  s->print( "\r\n" );
}
#endif


/* ----------------------------------------
  characters convert to float value..
---------------------------------------- */
float char2Float( const char *str )
{
  int tempI = 0;
  float tempF1 = 0.0F;
  float tempF2 = 10.0F;
  bool isLarge = true;
  while( *str )
  {
    if( *str == '.' ) isLarge = false;
    else if( !isdigit( *str ) ) break;
    else
    {
      if( isLarge == true )
      {
        tempI *= 10;
        tempI += *str - '0';
      }
      else
      {
        tempF1 += (float)(*str - '0') / tempF2;
        tempF2 *= 10.0F;
      }
    }
    str++;
  }
  return tempI + tempF1;
}
