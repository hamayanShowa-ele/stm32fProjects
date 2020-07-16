/*********************************************************************************/
/* 文字列操作に関する定義                                                        */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
#ifndef  __STRUTIL_H__
#define  __STRUTIL_H__

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>
#include  <time.h>
#include  <math.h>

/*************************************************************************/
/* 定義                                                                  */
/*************************************************************************/

/*************************************************************************/
/* プロトタイプ宣言                                                      */
/*************************************************************************/
char *uint2Ascii( char *dest, unsigned int num );
char *int2Ascii( char *dest, int num );
char *int2Hex( char *dest, unsigned int num );
char *StrCpy( char *dest, const char *src );
char *StrCat( char *dest, const char *src );
char *StrChr( char *src, int c );
char *StrNChr( const char *src, int c, unsigned int size );
void *MemSet( void *s, int c, unsigned int n );
void *MemCpy( void *d, const void *s, unsigned int n );
char *int2Hex08( char *dest, unsigned long num );
char *int2Hex04( char *dest, unsigned short num );
char *int2Hex02( char *dest, unsigned char num );
char *longLong2Ascii( char *dest, long long num );
int  htoi( const char *s );

int  split( char *str , char *argv[], int sz );
int  split_char( char comp, char *str , char *argv[], int sz );
int  isDigitString( const char *digit );
char *small2large( char *str );

char *fixed_little_number( char *dst, long num, int point );

void int2byteBig( char *dst, int d );
void int2byteLittle( char *dst, int d );
void double2byteLittle( char *dst, double d );
void double2byteBig( char *dst, double d );

unsigned short utf8to16( const unsigned char utf8[], int sz );

void compileDate(int *year, int *month, int *day);
void compileTime(int *hour, int *minute, int *second);

int patternMatch( const char *compA, const char *compB );

time_t iso8601ToUnixtime( const char *iso8601 );

char *dtostrf(double number, signed char width, unsigned char prec, char *s);

#endif  /*__STRUTIL_H__*/

/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
