/* ----------------------------------------
  GNSS utilities
  for STMicroelectronics SPL library

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
#include  "gnss.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */
//extern Serial Serial1;  /* hardware serial 1 */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */
GNSS::GNSS()
{
}

GNSS::GNSS( USART_UART *seri )
{
  begin( seri );
}

GNSS::~GNSS()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void GNSS::begin( USART_UART *seri )
{
  gpsSerial = seri;
}


/* ----------------------------------------
    generate check sum.
  ---------------------------------------- */
uint8_t GNSS::checkSum( const char *str, int size )
{
  uint8_t sum = 0;
  for( int i = 0; i < size; i++ )
  {
    sum ^= *str++;
  }

  return sum;
}

/* ----------------------------------------
    latitude.
  ---------------------------------------- */
float GNSS::latitude( const char *str )
{
  if( !*str ) return 0.0F;
  int tempI = (str[0] - '0') * 10;
  tempI += (str[1] - '0');
  float tempF = char2Float( &str[2] );
  return tempI + (tempF / 60.0F);
}

/* ----------------------------------------
    longitude.
  ---------------------------------------- */
float GNSS::longitude( const char *str )
{
  if( !*str ) return 0.0F;
  int tempI = (str[0] - '0') * 100;
  tempI += (str[1] - '0') * 10;
  tempI += (str[2] - '0') * 1;
  float tempF = char2Float( (const char *)&str[3] );
  return tempI + (tempF / 60.0F);
}

/* ----------------------------------------
    get one line from GPS serial.
  ---------------------------------------- */
int GNSS::line( char *rcv, int size )
{
  int count = 0;
  /* get line. */
  *rcv = '\0';
  #define  LOOP_LIMIT  36
  int loop;
  /* Sentences are output with a maximum delay of 75 ms from the PPS. */
  for( loop = 0; loop < LOOP_LIMIT; loop++ )
  {
    if( gpsSerial->available() )
    {
      char c = gpsSerial->read();
//      Serial1.write( c );
      if( c == '$' )
      {
        *rcv++ = c;
        count++;
        dly_tsk( 2UL );  /* 0.2ms * 10 */
        break;
      }
    }
    dly_tsk( 2UL );  /* 0.2ms * 10 */
  }
  if( loop == LOOP_LIMIT ) return 0;

  while( gpsSerial->available() )
  {
    char c = gpsSerial->read();
//    Serial1.write( c );
    if( c == '\n' )
    {
      *rcv = '\0';
      break;
    }
    else if( c == '\r' ) *rcv = '\0';
    else if( c == '$' || !isgraph( c ) ) return 0;
    else if( !isalnum( c ) &&
      c == ',' && c == '.' && c == '+' && c == '-' ) return 0;
    else
    {
      *rcv++ = c;
      if( ++count >= (size - 1) ) return 0;
    }
    if( !gpsSerial->available() ) dly_tsk( 10UL );  /* 0.2ms * 10 */
    else rot_rdq();
  }

  return count;
}

/* ----------------------------------------
    output the sentence to GPS serial.
  ---------------------------------------- */
void GNSS::output( const char *str )
{
  gpsSerial->print( str );
  gpsSerial->print( "\r\n" );
}


/* ----------------------------------------
    xxZDA format.
  ---------------------------------------- */
int GNSS::gpzda( const char *sentence, GPS_GPZDA *zda )
{
  if( strncmp( sentence, "$GPZDA", sizeof("$GPZDA") - 1 ) != 0 &&
      strncmp( sentence, "$GLZDA", sizeof("$GLZDA") - 1 ) != 0 &&  // $GLZDA ???
      strncmp( sentence, "$GNZDA", sizeof("$GNZDA") - 1 ) != 0 )   // $GNZDA ???
  {
    return 0;
  }

//  Serial.println( sentence );
  int argc;
  char *argv[7];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "argc=" + (String)argc );
  if( argc < 5 || argc > 7 ) return 0;

  /* utc time */
  zda->hhmmss = char2Float( (const char *)argv[1] );
  /* utc day */
  zda->day = (uint8_t)char2Int( (const char *)argv[2] );
//  zda->day = atoi( (const char *)argv[2] );
  /* utc month */
  zda->month = (uint8_t)char2Int( (const char *)argv[3] );
//  zda->month = atoi( (const char *)argv[3] );
  /* utc year */
  zda->year = (uint16_t)char2Int( (const char *)argv[4] );
//  zda->year = atoi( (const char *)argv[4] );
  /* local time zone hour */
  if( argc >= 6 ) zda->tzHour = char2Int( (const char *)argv[5] );
//  if( argc >= 6 ) zda->tzHour = atoi( (const char *)argv[5] );
  /* local time zone minute */
  if( argc >= 7 ) zda->tzMinute = (uint8_t)char2Int( (const char *)argv[6] );
//  if( argc >= 7 ) zda->tzMinute = atoi( (const char *)argv[6] );

  return 1;
}

/* ----------------------------------------
    xxGGA format.
  ---------------------------------------- */
int GNSS::gpgga( const char *sentence, GPS_GPGGA *gga )
{
  if( strncmp( sentence, "$GPGGA", sizeof("$GPGGA") - 1 ) != 0 &&
      strncmp( sentence, "$GNGGA", sizeof("$GNGGA") - 1 ) != 0 &&  // $GNGGA ???
      strncmp( sentence, "$GLGGA", sizeof("$GLGGA") - 1 ) != 0 )   // $GLGGA ???
  {
    return 0;
  }

//  Serial.println( sentence );
  int argc;
  char *argv[15];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  int sat = 0;
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "argc=" + (String)argc );
  if( argc < 13 || argc > 15 ) return 0;

  /* utc time */
  gga->hhmmss = char2Float( (const char *)argv[1] );
  /* latitude */
  gga->latitude = latitude( (const char *)argv[2] );
  /* north or south */
  gga->ns = char2Char( (const char *)argv[3] );
  /* longitude */
  gga->longitude = longitude( (const char *)argv[4] );
  /* east or west */
  gga->ew = char2Char( (const char *)argv[5] );
  /* GNSS positioning status */
  gga->quality = (uint8_t)char2Int( (const char *)argv[6] );
  /* Location using satellites */
  sat = char2Int( (const char *)argv[7] );
  gga->satellites = sat;
  /* HDOP value */
  gga->hdop = char2Float( (const char *)argv[8] );
  /* altitude above sea level */
  gga->altitude = char2Float( (const char *)argv[9] );
  /* Unit of altitude above sea level */
  gga->altiUnit = char2Char( (const char *)argv[10] );
  /* Geoid Height */
  gga->qeoidElevation = char2Float( (const char *)argv[11] );
  /* Unit of Geoid Height */
  gga->qeoidUnit = char2Char( (const char *)argv[12] );

  return sat;
}

/* ----------------------------------------
    xxGSA format.
  ---------------------------------------- */
int GNSS::gpgsa( const char *sentence, GPS_GPGSA *gsa )
{
  if( strncmp( sentence, "$GNGSA", sizeof("$GNGSA") - 1 ) != 0 &&
      strncmp( sentence, "$GPGSA", sizeof("$GPGSA") - 1 ) != 0 &&
      strncmp( sentence, "$GLGSA", sizeof("$GLGSA") - 1 ) != 0 )
  {
    return 0;
  }

//  Serial.println( sentence );
  int argc;
  char *argv[19];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "argc=" + (String)argc );
  if( argc < 18 || argc > 19  ) return 0;

  gsa->oMode = char2Char( (const char *)argv[1] );  // operating mode
  if( gsa->oMode != 'A' && gsa->oMode != 'M' ) return 0;
  gsa->pMode = char2Char( (const char *)argv[2] );  // Positioning Mode
  if( gsa->pMode != '1' && gsa->pMode != '2' && gsa->pMode != '3' ) return 0;

  gsa->pdop = char2Float( (const char *)argv[15] );  // PDOP
  gsa->hdop = char2Float( (const char *)argv[16] );  // HDOP
  gsa->vdop = char2Float( (const char *)argv[17] );  // VDOP

  /* clear gps numbers.  */
  for( int i = 0; i < 12; i++ ) gsa->sNumber[ i ] = 0;
  int index = 0;
  for( int i = 3; i <= 14; i++ )
  {
    if( *((char *)argv[i]) != '\0' )
    {
      gsa->sNumber[ index++ ] = char2Int( (const char *)argv[i] );  // satellite numbers
    }
    else
    {
      gsa->sNumber[ index++ ] = 0;  // satellite numbers = 0 is no satellitee.
    }
  }

  return argc - 7;
}


/* ----------------------------------------
    xxGSV format.
  ---------------------------------------- */
int GNSS::gpgsv( const char *sentence, GPS_GPGSV *gsv, int *type )
{
  if( strncmp( sentence, "$GPGSV", sizeof("$GPGSV") - 1 ) == 0 )  // GPS
    *type = TYPE_GPS;
  else if( strncmp( sentence, "$QZGSV", sizeof("$QZGSV") - 1 ) == 0 )  // QZSS
    *type = TYPE_QZSS;
  else if( strncmp( sentence, "$GLGSV", sizeof("$GLGSV") - 1 ) == 0 )  // GLONASS
    *type = TYPE_GLONASS;
  else return 0;

//  Serial.println( sentence );
  int argc;
  char *argv[21];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "argc=" + (String)argc );
  if( argc < 5 ) return 0;

  int total = char2Int( (const char *)argv[1] );  // Total number of messages
  int seq = char2Int( (const char *)argv[2] );  // Sequence number of the message
  int view = char2Int( (const char *)argv[3] );  // Number of in-field satellites
  if( view == 0 || total == 0 || seq == 0 ) return 0;
  /* signa ID. */
  if( argc >= 21 )
  {
    gsv->id = char2Char( (const char *)argv[20] );  // signal ID.
  }
  else gsv->id = '0';  // signal ID.

  gsv->total = total;  // Total number of messages
  gsv->seq = seq;  // Sequence number of the message
  gsv->inField = view;  // Number of in-field satellites

  if( (view - ((seq - 1) * 4)) >= 4 ) view = 4;
  else view %= 4;

  /* clear Satellite information */
  for( int i = 0; i < 4; i++ )
  {
    gsv->info[ i ].number = 0;
    gsv->info[ i ].elevation = 0;
    gsv->info[ i ].azimuth = 0;
    gsv->info[ i ].snr = 0;
  }

  /* Satellite information */
  argc -= 3;
  for( int i = 0; i < view; i++ )
  {
    if( --argc && *argv[(i * 4) + 4] )
    {
      gsv->info[ i ].number = char2Int( (const char *)argv[(i * 4) + 4] );  // satellite number
    }
    else gsv->info[ i ].number = 0;  // satellite number

    if( --argc && *argv[(i * 4) + 5] )
    {
      gsv->info[ i ].elevation = (uint8_t)char2Int( (const char *)argv[(i * 4) + 5] );  // angle of elevation
    }
    else gsv->info[ i ].elevation = 0;

    if( --argc && *argv[(i * 4) + 6] )
    {
      gsv->info[ i ].azimuth = (uint16_t)char2Int( (const char *)argv[(i * 4) + 6] );  // azimuth
    }
    else gsv->info[ i ].azimuth = 0;

    if( --argc && *argv[(i * 4) + 7] )
    {
      gsv->info[ i ].snr = (uint8_t)char2Int( (const char *)argv[(i * 4) + 7] );  // SNR(dB)
    }
    else gsv->info[ i ].snr = 0;
  }

  return view;
}


/* ----------------------------------------
    xxGLL format.
  ---------------------------------------- */
int GNSS::gpgll( const char *sentence, GPS_GPGLL *gll )
{
  if( strncmp( sentence, "$GPGLL", sizeof("$GPGLL") - 1 ) != 0 &&
      strncmp( sentence, "$GLGLL", sizeof("$GLGLL") - 1 ) != 0 &&  // $GLGLL ???
      strncmp( sentence, "$GNGLL", sizeof("$GNGLL") - 1 ) != 0 )   // $GNGLL ???
  {
    return 0;
  }

//  Serial.println( sentence );
  int argc;
  char *argv[8];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "argc=" + (String)argc );
  if( argc != 8 ) return 0;

  /* latitude */
  gll->latitude = latitude( (const char *)argv[1] );
  /* north or south */
  gll->ns = char2Char( (const char *)argv[2] );
  /* longitude */
  gll->longitude = longitude( (const char *)argv[3] );
  /* east or west */
  gll->ew = char2Char( (const char *)argv[4] );
  /* utc time */
  gll->hhmmss = char2Float( (const char *)argv[5] );
  /* positional status */
  gll->pStatus = char2Char( (const char *)argv[6] );
  /* positional mode */
  gll->pMode = char2Char( (const char *)argv[7] );

  return 1;
}


/* ----------------------------------------
    xxRMC format.
  ---------------------------------------- */
int GNSS::gprmc( const char *sentence, GPS_GPRMC *rmc )
{
  if( strncmp( sentence, "$GPRMC", sizeof("$GPRMC") - 1 ) != 0 &&
      strncmp( sentence, "$GLRMC", sizeof("$GLRMC") - 1 ) != 0 &&  // $GLRMC ???
      strncmp( sentence, "$GNRMC", sizeof("$GNRMC") - 1 ) != 0 )   // $GNRMC ???
  {
    return 0;
  }

//  Serial.println( sentence );
  int argc;
  char *argv[14];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "gprmc argc=" + (String)argc );
  if( argc < 13 || argc > 14 ) return 0;

  /* utc time */
  rmc->hhmmss = char2Float( (const char *)argv[1] );
  /* positional status */
  rmc->pStatus = char2Char( (const char *)argv[2] );;
  /* latitude */
  rmc->latitude = latitude( (const char *)argv[3] );
  /* north or south */
  rmc->ns = char2Char( (const char *)argv[4] );
  /* longitude */
  rmc->longitude = longitude( (const char *)argv[5] );
  /* east or west */
  rmc->ew = char2Char( (const char *)argv[6] );
  /* knot */
  rmc->knot = char2Float( (const char *)argv[7] );
  /* azimuth */
  rmc->azimuth = char2Float( (const char *)argv[8] );
  /* day,month,year */
  rmc->ddmmyy = (uint32_t)char2Int( (const char *)argv[9] );
  /* magnetic deviation */
  rmc->magneticDeviation = char2Float( (const char *)argv[10] );
  /* positional mode */
  rmc->pMode = char2Char( (const char *)argv[12] );
  /* navigation status */
  rmc->nStatus = char2Char( (const char *)argv[13] );

  return 1;
}


/* ----------------------------------------
    xxVTG format.
  ---------------------------------------- */
int GNSS::gpvtg( const char *sentence, GPS_GPVTG *vtg )
{
  if( strncmp( sentence, "$GPVTG", sizeof("$GPVTG") - 1 ) != 0 &&
      strncmp( sentence, "$GLVTG", sizeof("$GLVTG") - 1 ) != 0 &&  // $GLVTG ???
      strncmp( sentence, "$GNVTG", sizeof("$GNVTG") - 1 ) != 0 )   // $GNVTG ???
  {
    return 0;
  }

//  Serial.println( sentence );
  int argc;
  char *argv[10];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "argc=" + (String)argc );
  if( argc != 10 ) return 0;

  /* azimuth */
  vtg->azimuth = char2Float( (const char *)argv[1] );
  /* magnetic orientation */
  vtg->orientation = char2Float( (const char *)argv[3] );
  /* knot */
  vtg->knot = char2Float( (const char *)argv[5] );
  /* km/h */
  vtg->kmh = char2Float( (const char *)argv[7] );
  /* positional mode */
  vtg->pMode = char2Char( (const char *)argv[8] );

  return 1;
}


/* ----------------------------------------
    xxGNS format.
  ---------------------------------------- */
int GNSS::gpgns( const char *sentence, GPS_GPGNS *gns )
{
  if( strncmp( sentence, "$GPGNS", sizeof("$GPGNS") - 1 ) != 0 &&
      strncmp( sentence, "$GLGNS", sizeof("$GLGNS") - 1 ) != 0 &&  // $GLGNS ???
      strncmp( sentence, "$GNGNS", sizeof("$GNGNS") - 1 ) != 0 )   // $GNGNS ???
  {
    return 0;
  }

//  Serial.println( sentence );
  int argc;
  char *argv[14];
  char buffer[ strlen(sentence) + 1 ];
  strcpy( buffer, sentence );

  /* Isolate Checksums */
  char checkSumStr[3];
  argc = split( '*', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
  if( argc == 2 )
  {
    strncpy( checkSumStr, argv[1], 2 );
    checkSumStr[2] = '\0';
  }

  /* Split the string by commas. */
  argc = split( ',', buffer, argv, sizeof(argv) / sizeof(argv[0]) );
//  Serial.println( "argc=" + (String)argc );
  if( argc != 14 ) return 0;

  /* utc time */
  gns->hhmmss = char2Float( (const char *)argv[1] );
  /* latitude */
  gns->latitude = latitude( (const char *)argv[2] );
  /* north or south */
  gns->ns = char2Char( (const char *)argv[3] );
  /* longitude */
  gns->longitude = longitude( (const char *)argv[4] );
  /* east or west */
  gns->ew = char2Char( (const char *)argv[5] );

  return 1;
}


/* ----------------------------------------
    xxGNS format.
  ---------------------------------------- */
int GNSS::gpsGMTtime( const GPS_GPZDA *zda, struct tm *t )
{
  int tempI = (int)zda->hhmmss;
  t->tm_hour = tempI / 10000;
  t->tm_min = (tempI % 10000) / 100;
  t->tm_sec = tempI % 100;
  t->tm_year = zda->year - 1900;
  t->tm_mon = zda->month - 1;
  t->tm_mday = zda->day;
  return 0;
}


/* ----------------------------------------
    UTC time string.
  ---------------------------------------- */
char *GNSS::utcTimeString( char *dest, float utc )
{
  if( utc >= (24.0F * 3600.0F) ) return 0;
  int32_t tempI = (int32_t)utc;

  int index = 0;
  int value = tempI / (10L * 3600L);
  dest[index++] = '0' + value;
  tempI %= 10L * 3600L;

  value = tempI / (1L * 3600L);
  dest[index++] = '0' + value;
  tempI %= 1L * 3600L;

  value = tempI / (10L * 60L);
  dest[index++] = '0' + value;
  tempI %= 10L * 60L;

  value = tempI / (1L * 60L);
  dest[index++] = '0' + value;
  tempI %= 1L * 60L;

  value = tempI / 10L;
  dest[index++] = '0' + value;

  value = tempI % 10L;
  dest[index++] = '0' + value;

  dest[index++] = '.';

  tempI = (int32_t)(utc * 1000.0F) % 1000L;

  value = tempI / 100L;
  dest[index++] = '0' + value;
  tempI %= 100L;

  value = tempI / 10L;
  dest[index++] = '0' + value;

  value = tempI % 10L;
  dest[index++] = '0' + value;

  dest[index] = '\0';

  return dest;
}

/* ----------------------------------------
    latitude string.
  ---------------------------------------- */
char *GNSS::latitudeString( char *dest, double lat )
{
  if( lat > 90.0 ) return 0;
  int index = 0;

  /* degree */
  int32_t tempI = (int32_t)lat;
  int value = tempI / 10L;
  dest[index++] = '0' + value;
  value = tempI % 10L;
  dest[index++] = '0' + value;

  /* minute */
  lat -= tempI;
  lat *= (60.0 * 10000.0);
  lat += 0.5;
  tempI = (int32_t)lat;

  value = tempI / 100000L;
  dest[index++] = '0' + value;
  tempI %= 100000L;
  value = tempI / 10000L;
  dest[index++] = '0' + value;
  tempI %= 10000L;

  dest[index++] = '.';

  value = tempI / 1000L;
  dest[index++] = '0' + value;
  tempI %= 1000L;
  value = tempI / 100L;
  dest[index++] = '0' + value;
  tempI %= 100L;
  value = tempI / 10L;
  dest[index++] = '0' + value;
  value = tempI % 10L;
  dest[index++] = '0' + value;

  dest[index] = '\0';

  return dest;
}

/* ----------------------------------------
    longitude string.
  ---------------------------------------- */
char *GNSS::longitudeString( char *dest, double lon )
{
  if( lon > 180.0 ) return 0;
  int index = 0;

  /* degree */
  int32_t tempI = (int32_t)lon;
  int value = tempI / 100L;
  dest[index++] = '0' + value;
  value = (tempI % 100L) / 10L;
  dest[index++] = '0' + value;
  value = tempI % 10L;
  dest[index++] = '0' + value;

  /* minute */
  lon -= tempI;
  lon *= (60.0 * 10000.0);
  lon += 0.5;
  tempI = (int32_t)lon;

  value = tempI / 100000L;
  dest[index++] = '0' + value;
  tempI %= 100000L;
  value = tempI / 10000L;
  dest[index++] = '0' + value;
  tempI %= 10000L;

  dest[index++] = '.';

  value = tempI / 1000L;
  dest[index++] = '0' + value;
  tempI %= 1000L;
  value = tempI / 100L;
  dest[index++] = '0' + value;
  tempI %= 100L;
  value = tempI / 10L;
  dest[index++] = '0' + value;
  value = tempI % 10L;
  dest[index++] = '0' + value;

  dest[index] = '\0';

  return dest;
}

/* ----------------------------------------
    number of satellites string.
  ---------------------------------------- */
char *GNSS::numberOfSatellitesString( char *dest, int sat )
{
  if( sat > 32 ) return 0;
  dest[0] = '0' + (sat / 10L);
  dest[1] = '0' + (sat % 10L);
  dest[2] = '\0';

  return dest;
}

/* ----------------------------------------
    hdop string.
  ---------------------------------------- */
char *GNSS::hdopString( char *dest, float hdop )
{
  if( hdop > 50.0F ) return 0;
  int index = 0;
  int value;

  int32_t tempI = (int32_t)hdop;
  if( tempI >= 10L )
  {
    value = tempI / 10L;
    dest[index++] = '0' + value;
  }
  value = tempI % 10L;
  dest[index++] = '0' + value;

  dest[index++] = '.';

  value = (int)(hdop * 10.0F) % 10;
  dest[index++] = '0' + value;

  dest[index] = '\0';

  return dest;
}

/* ----------------------------------------
    altitude string.
  ---------------------------------------- */
char *GNSS::altitudeString( char *dest, float alt )
{
  if( alt > (1000.0F * 1000.0F) ) return 0;
  int index = 0;
  int value;

  int32_t tempI = (int32_t)alt;
  if( tempI >= 100L * 1000L )
  {
    value = tempI / (100L * 1000L);
    dest[index++] = '0' + value;
    tempI %= 100L * 1000L;
  }

  if( tempI >= 10L * 1000L )
  {
    value = tempI / (10L * 1000L);
    dest[index++] = '0' + value;
    tempI %= 10L * 1000L;
  }

  if( tempI >= 1L * 1000L )
  {
    value = tempI / (1L * 1000L);
    dest[index++] = '0' + value;
    tempI %= 1L * 1000L;
  }

  if( tempI >= 100L )
  {
    value = tempI / 100L;
    dest[index++] = '0' + value;
    tempI %= 100L;
  }

  if( tempI >= 10L )
  {
    value = tempI / 10L;
    dest[index++] = '0' + value;
    tempI %= 10L;
  }

  value = tempI % 10L;
  dest[index++] = '0' + value;

  dest[index++] = '.';

  tempI = (int32_t)(alt * 10.0F);
  value = tempI % 10L;
  dest[index++] = '0' + value;

  dest[index++] = '\0';

  return dest;
}





/* ----------------------------------------
    nmea0183 xxGGA sentence.
  ---------------------------------------- */
int GNSS::nmeaGGA( GPS_GPGGA *gga, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPGGA) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPGGA," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLGGA," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GAGGA," );
  else if( satType == 0 ) strcpy( sentence, "$GNGGA," );
  else return GNSS_SENTENCE_ERROR;

  /* UTC time. */
  strcat( sentence, utcTimeString( buffer, gga->hhmmss ) );
  strcat( sentence, "," );

  /* latitude. */
  strcat( sentence, latitudeString( buffer, gga->latitude ) );
  if( gga->ns == 'N' ) strcat( sentence, ",N," );
  else if( gga->ns == 'S' ) strcat( sentence, ",S," );
  else return GNSS_SENTENCE_ERROR;

  /* longitude. */
  strcat( sentence, longitudeString( buffer, gga->longitude ) );
  if( gga->ew == 'E' ) strcat( sentence, ",E," );
  else if( gga->ew == 'W' ) strcat( sentence, ",W," );
  else return GNSS_SENTENCE_ERROR;

  /* positioning status */
  if( gga->quality == 0 ) strcat( sentence, "0," );
  else if( gga->quality == 1 ) strcat( sentence, "1," );
  else if( gga->quality == 2 ) strcat( sentence, "2," );
  else return GNSS_SENTENCE_ERROR;

  /* number of satellites */
  strcat( sentence, numberOfSatellitesString( buffer, gga->satellites ) );
  strcat( sentence, "," );

  /* hdop */
  strcat( sentence, hdopString( buffer, gga->hdop ) );
  strcat( sentence, "," );

  /* altitude */
  strcat( sentence, altitudeString( buffer, gga->altitude ) );
  strcat( sentence, ",M," );

  /* geoid */
  strcat( sentence, altitudeString( buffer, gga->qeoidElevation ) );
  strcat( sentence, ",M," );

  /* Time of DGPS data and Identification number of the DGPS station. */
  strcat( sentence, "," );

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}

/* ----------------------------------------
    nmea0183 xxGLL sentence.
  ---------------------------------------- */
int GNSS::nmeaGLL( GPS_GPGLL *gll, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPGLL) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPGLL," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLGLL," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GAGLL," );
  else if( satType == 0 ) strcpy( sentence, "$GNGLL," );
  else return GNSS_SENTENCE_ERROR;

  /* latitude. */
  strcat( sentence, latitudeString( buffer, gll->latitude ) );
  if( gll->ns == 'N' ) strcat( sentence, ",N," );
  else if( gll->ns == 'S' ) strcat( sentence, ",S," );
  else return GNSS_SENTENCE_ERROR;

  /* longitude. */
  strcat( sentence, longitudeString( buffer, gll->longitude ) );
  if( gll->ew == 'E' ) strcat( sentence, ",E," );
  else if( gll->ew == 'W' ) strcat( sentence, ",W," );
  else return GNSS_SENTENCE_ERROR;

  /* UTC time. */
  strcat( sentence, utcTimeString( buffer, gll->hhmmss ) );
  strcat( sentence, "," );

  /* positioning status */
  if( gll->pStatus == 'A' ) strcat( sentence, "A," );
  else if( gll->pStatus == 'N' ) strcat( sentence, "N," );
  else return GNSS_SENTENCE_ERROR;

  /* Positioning mode */
  if( gll->pMode == 'A' ) strcat( sentence, "A" );
  else if( gll->pMode == 'D' ) strcat( sentence, "D" );
  else if( gll->pMode == 'N' ) strcat( sentence, "N" );
  else return GNSS_SENTENCE_ERROR;

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}


/* ----------------------------------------
    nmea0183 xxGNS sentence.
  ---------------------------------------- */
int GNSS::nmeaGNS( GPS_GPGNS *gns, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPGNS) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPGNS," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLGNS," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GAGNS," );
  else if( satType == 0 ) strcpy( sentence, "$GNGNS," );
  else return GNSS_SENTENCE_ERROR;

  /* UTC time. */
  strcat( sentence, utcTimeString( buffer, gns->hhmmss ) );
  strcat( sentence, "," );

  /* latitude. */
  strcat( sentence, latitudeString( buffer, gns->latitude ) );
  if( gns->ns == 'N' ) strcat( sentence, ",N," );
  else if( gns->ns == 'S' ) strcat( sentence, ",S," );
  else return GNSS_SENTENCE_ERROR;

  /* longitude. */
  strcat( sentence, longitudeString( buffer, gns->longitude ) );
  if( gns->ew == 'E' ) strcat( sentence, ",E," );
  else if( gns->ew == 'W' ) strcat( sentence, ",W," );
  else return GNSS_SENTENCE_ERROR;

  /* Positioning mode */
  len = strlen( sentence );
  sentence[ len++ ] = gns->pStatus[0];
  sentence[ len++ ] = gns->pStatus[1];
  sentence[ len++ ] = gns->pStatus[2];
  sentence[ len++ ] = ',';
  sentence[ len ] = '\0';

  /* number of satellites */
  strcat( sentence, numberOfSatellitesString( buffer, gns->satellites ) );
  strcat( sentence, "," );

  /* hdop */
  strcat( sentence, hdopString( buffer, gns->hdop ) );
  strcat( sentence, "," );

  /* altitude */
  strcat( sentence, altitudeString( buffer, gns->altitude ) );
  strcat( sentence, "," );

  /* geoid */
  strcat( sentence, altitudeString( buffer, gns->qeoidElevation ) );
  strcat( sentence, "," );

  /* DGPS data time and DGPS station identification number */
  strcat( sentence, ",," );

  /* navigation status */
  len = strlen( sentence );
  sentence[ len++ ] = gns->nStatus;
  sentence[ len ] = '\0';

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}

/* ----------------------------------------
    nmea0183 xxGSA sentence.
  ---------------------------------------- */
int GNSS::nmeaGSA( GPS_GPGSA *gsa, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPGSA) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPGSA," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLGSA," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GAGSA," );
  else if( satType == 0 ) strcpy( sentence, "$GNGSA," );
  else return GNSS_SENTENCE_ERROR;

  /* operating mode */
  if( gsa->oMode == 'M' ) strcat( sentence, "M," );
  else if( gsa->oMode == 'A' ) strcat( sentence, "A," );
  else return GNSS_SENTENCE_ERROR;

  /* Positioning Mode */
  if( gsa->pMode == '1' ) strcat( sentence, "1," );
  else if( gsa->pMode == '2' ) strcat( sentence, "2," );
  else if( gsa->pMode == '3' ) strcat( sentence, "3," );
  else return GNSS_SENTENCE_ERROR;

  /* satellite informations */
  len = strlen(sentence);
  for( int i = 0; i < (int)(sizeof(gsa->sNumber) / sizeof(gsa->sNumber[0])); i++ )
  {
    int tempI = gsa->sNumber[i];
    if( tempI > 0 && tempI < 100 )
    {
      sentence[ len++ ] = '0' + (tempI / 10);
      sentence[ len++ ] = '0' + (tempI % 10);
    }
    sentence[ len++ ] = ',';
  }
  sentence[ len ] = '\0';

  /* pdop */
  strcat( sentence, hdopString( buffer, gsa->pdop ) );
  strcat( sentence, "," );

  /* hdop */
  strcat( sentence, hdopString( buffer, gsa->hdop ) );
  strcat( sentence, "," );

  /* vdop */
  strcat( sentence, hdopString( buffer, gsa->vdop ) );

//  strcat( sentence, ",1" );

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}


/* ----------------------------------------
    nmea0183 xxGSV sentence.
  ---------------------------------------- */
int GNSS::nmeaGSV( GPS_GPGSV *gsv, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPGSV) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPGSV," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLGSV," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GAGSV," );
  else if( satType == 0 ) strcpy( sentence, "$GNGSV," );
  else return GNSS_SENTENCE_ERROR;

  /* Total number of messages */
  if( gsv->total < 1 || gsv->total > 4 ) return GNSS_SENTENCE_ERROR;
  len = strlen(sentence);
  sentence[ len++ ] = '0' + gsv->total;
  sentence[ len++ ] = ',';

  /* Sequence number of the message */
  if( gsv->seq < 1 || gsv->seq > 4 ) return GNSS_SENTENCE_ERROR;
  sentence[ len++ ] = '0' + gsv->seq;
  sentence[ len++ ] = ',';

  /* Number of in-field satellites */
  if( gsv->inField < 0 || gsv->inField > 14 ) return GNSS_SENTENCE_ERROR;
  sentence[ len++ ] = '0' + (gsv->inField / 10);
  sentence[ len++ ] = '0' + (gsv->inField % 10);
  sentence[ len++ ] = ',';

  /* satellite informations */
  for( int i = 0; i < 4; i++ )
  {
    if( gsv->info[i].number > 0 && gsv->info[i].number < 100 )
    {
      sentence[ len++ ] = '0' + (gsv->info[i].number / 10);
      sentence[ len++ ] = '0' + (gsv->info[i].number % 10);
      sentence[ len++ ] = ',';

      if( gsv->info[i].elevation >= 0 && gsv->info[i].elevation <= 99 )
      {
        sentence[ len++ ] = '0' + (gsv->info[i].elevation / 10);
        sentence[ len++ ] = '0' + (gsv->info[i].elevation % 10);
      }
      sentence[ len++ ] = ',';

      if( gsv->info[i].azimuth >= 0 && gsv->info[i].azimuth <= 359 )
      {
        sentence[ len++ ] = '0' + (gsv->info[i].azimuth / 100);
        sentence[ len++ ] = '0' + ((gsv->info[i].azimuth % 100) / 10);
        sentence[ len++ ] = '0' + (gsv->info[i].azimuth % 10);
      }
      sentence[ len++ ] = ',';

      if( gsv->info[i].snr >= 0 && gsv->info[i].snr <= 99 )
      {
        sentence[ len++ ] = '0' + (gsv->info[i].snr / 10);
        sentence[ len++ ] = '0' + (gsv->info[i].snr % 10);
      }
      sentence[ len++ ] = ',';
    }
  }

  /* signal ID */
  sentence[ len++ ] = gsv->id;
  sentence[ len++ ] = '\0';

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}

/* ----------------------------------------
    nmea0183 xxRMC sentence.
  ---------------------------------------- */
int GNSS::nmeaRMC( GPS_GPRMC *rmc, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPRMC) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPRMC," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLRMC," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GARMC," );
  else if( satType == 0 ) strcpy( sentence, "$GNRMC," );
  else return GNSS_SENTENCE_ERROR;

  /* UTC time. */
  strcat( sentence, utcTimeString( buffer, rmc->hhmmss ) );
  strcat( sentence, "," );

  /* positional status */
  if( rmc->pStatus == 'A' ) strcat( sentence, "A," );
  else if( rmc->pStatus == 'V' ) strcat( sentence, "V," );
  else return GNSS_SENTENCE_ERROR;

  /* latitude. */
  strcat( sentence, latitudeString( buffer, rmc->latitude ) );
  if( rmc->ns == 'N' ) strcat( sentence, ",N," );
  else if( rmc->ns == 'S' ) strcat( sentence, ",S," );
  else return GNSS_SENTENCE_ERROR;

  /* longitude. */
  strcat( sentence, longitudeString( buffer, rmc->longitude ) );
  if( rmc->ew == 'E' ) strcat( sentence, ",E," );
  else if( rmc->ew == 'W' ) strcat( sentence, ",W," );
  else return GNSS_SENTENCE_ERROR;

  /* speed (knot). */
  if( rmc->knot >= 0.0F && rmc->knot < 10000.0F )
  {
    len = (rmc->knot > 1000.0F) ? 6 + 1 :
          (rmc->knot > 100.0F) ? 5 + 1 :
          (rmc->knot > 10.0F) ? 4 + 1 : 3 + 1;
    dtostrf( rmc->knot, len, 2, buffer );
    strcat( sentence, buffer );
  }
  strcat( sentence, "," );

  /* true orientation. */
  if( rmc->azimuth >= 0.0F && rmc->azimuth < 360.0F )
  {
    len = (rmc->azimuth > 100.0F) ? 5 + 1 :
          (rmc->azimuth > 10.0F) ? 4 + 1 : 3 + 1;
    dtostrf( rmc->azimuth, len, 2, buffer );
    strcat( sentence, buffer );
  }
  strcat( sentence, "," );

  /* day,month,year */
  time_t dmy = (time_t)rmc->ddmmyy;
  struct tm *t = localtime( (const time_t *)&dmy );
  int yy = t->tm_year + 1900;
  int mm = t->tm_mon + 1;
  int dd = t->tm_mday;
  len = strlen( sentence );
  sentence[ len++ ] = '0' + ((dd % 100) / 10);
  sentence[ len++ ] = '0' + (dd % 10);
  sentence[ len++ ] = '0' + ((mm % 100) / 10);
  sentence[ len++ ] = '0' + (mm % 10);
  sentence[ len++ ] = '0' + ((yy % 100) / 10);
  sentence[ len++ ] = '0' + (yy % 10);
  sentence[ len++ ] = ',';
  sentence[ len ] = '\0';

  /* magnetic deviation and Direction of magnetic deviation correction */
  strcat( sentence, ",," );

  /* positional mode */
  if( rmc->pMode == 'A' ) strcat( sentence, "A," );
  else if( rmc->pMode == 'D' ) strcat( sentence, "D," );
  else if( rmc->pMode == 'N' ) strcat( sentence, "N," );
  else return GNSS_SENTENCE_ERROR;

  /* navigation status */
  if( rmc->nStatus == 'S' ) strcat( sentence, "S" );
  else if( rmc->nStatus == 'C' ) strcat( sentence, "C" );
  else if( rmc->nStatus == 'U' ) strcat( sentence, "U" );
  else if( rmc->nStatus == 'V' ) strcat( sentence, "V" );
  else return GNSS_SENTENCE_ERROR;

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}


/* ----------------------------------------
    nmea0183 xxVTG sentence.
  ---------------------------------------- */
int GNSS::nmeaVTG( GPS_GPVTG *vtg, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPVTG) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPVTG," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLVTG," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GAVTG," );
  else if( satType == 0 ) strcpy( sentence, "$GNVTG," );
  else return GNSS_SENTENCE_ERROR;

  /* true orientation. */
  if( vtg->azimuth >= 0.0F && vtg->azimuth < 360.0F )
  {
    len = (vtg->azimuth > 100.0F) ? 5 + 1 :
          (vtg->azimuth > 10.0F) ? 4 + 1 : 3 + 1;
    dtostrf( vtg->azimuth, len, 2, buffer );
    strcat( sentence, buffer );
  }
  strcat( sentence, ",T," );

  /* magnetic orientation. */
  if( vtg->orientation >= 0.0F && vtg->orientation < 360.0F )
  {
    len = (vtg->orientation > 100.0F) ? 5 + 1 :
          (vtg->orientation > 10.0F) ? 4 + 1 : 3 + 1;
    dtostrf( vtg->orientation, len, 2, buffer );
    strcat( sentence, buffer );
  }
  strcat( sentence, ",M," );

  /* speed (knot). */
  if( vtg->knot >= 0.0F && vtg->knot < 10000.0F )
  {
    len = (vtg->knot > 1000.0F) ? 6 + 1 :
          (vtg->knot > 100.0F) ? 5 + 1 :
          (vtg->knot > 10.0F) ? 4 + 1 : 3 + 1;
    dtostrf( vtg->knot, len, 2, buffer );
    strcat( sentence, buffer );
  }
  strcat( sentence, ",N," );

  /* speed (km/h). */
  if( vtg->kmh >= 0.0F && vtg->kmh < 10000.0F )
  {
    len = (vtg->kmh > 1000.0F) ? 6 + 1 :
          (vtg->kmh > 100.0F) ? 5 + 1 :
          (vtg->kmh > 10.0F) ? 4 + 1 : 3 + 1;
    dtostrf( vtg->kmh, len, 2, buffer );
    strcat( sentence, buffer );
  }
  strcat( sentence, ",K," );

  /* positional mode */
  if( vtg->pMode == 'A' ) strcat( sentence, "A" );
  else if( vtg->pMode == 'D' ) strcat( sentence, "D" );
  else if( vtg->pMode == 'N' ) strcat( sentence, "N" );
  else return GNSS_SENTENCE_ERROR;

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}


/* ----------------------------------------
    nmea0183 xxZDA sentence.
  ---------------------------------------- */
int GNSS::nmeaZDA( GPS_GPZDA *zda, int satType, char *sentence, size_t size )
{
  if( size < sizeof(NMEA0183_GPZDA) ) return GNSS_SENTENCE_ERROR;
  int len;
  char buffer[32];

  /* satellite type. */
  if( satType == TYPE_GPS || satType == TYPE_QZSS ) strcpy( sentence, "$GPZDA," );
  else if( satType == TYPE_GLONASS ) strcpy( sentence, "$GLZDA," );
  else if( satType == TYPE_GALILEO ) strcpy( sentence, "$GAZDA," );
  else if( satType == 0 ) strcpy( sentence, "$GNZDA," );
  else return GNSS_SENTENCE_ERROR;

  /* UTC time. */
  strcat( sentence, utcTimeString( buffer, zda->hhmmss ) );
  strcat( sentence, "," );

  len = strlen( sentence );
  /* day. */
  if( zda->day < 1 || zda->day > 31 ) return GNSS_SENTENCE_ERROR;
  sentence[ len++ ] = '0' + (zda->day / 10);
  sentence[ len++ ] = '0' + (zda->day % 10);
  sentence[ len++ ] = ',';
  /* month. */
  if( zda->month < 1 || zda->month > 12 ) return GNSS_SENTENCE_ERROR;
  sentence[ len++ ] = '0' + (zda->month / 10);
  sentence[ len++ ] = '0' + (zda->month % 10);
  sentence[ len++ ] = ',';
  /* year. */
  if( zda->year < 1999 || zda->year > 2099 ) return GNSS_SENTENCE_ERROR;
  int tempI = zda->year;
  sentence[ len++ ] = '0' + (tempI / 1000);
  tempI %= 1000;
  sentence[ len++ ] = '0' + (tempI / 100);
  tempI %= 100;
  sentence[ len++ ] = '0' + (tempI / 10);
  sentence[ len++ ] = '0' + (tempI % 10);
  sentence[ len++ ] = ',';

  /* local time zone hour. */
  if( zda->tzHour < -23 || zda->tzHour > 23 ) return GNSS_SENTENCE_ERROR;
  tempI = zda->tzHour;
  if( tempI >= 0 ) sentence[ len++ ] = '+';
  else sentence[ len++ ] = '-';
  tempI = abs( tempI );
  sentence[ len++ ] = '0' + (tempI / 10);
  sentence[ len++ ] = '0' + (tempI % 10);
  sentence[ len++ ] = ',';

  /* local time zone minute. */
  if( zda->tzMinute > 59 ) return GNSS_SENTENCE_ERROR;
  sentence[ len++ ] = '0' + (zda->tzMinute / 10);
  sentence[ len++ ] = '0' + (zda->tzMinute % 10);
  sentence[ len++ ] = '\0';

  /* check sum. */
  uint8_t sum = checkSum( (const char *)sentence + 1, strlen(sentence) - 1 );
  len = strlen( sentence );
  sentence[ len++ ] = '*';
  int2Hex02( &sentence[len], sum );

  return GNSS_OK;
}


/* examples. */
#if 0
/* ----------------------------------------
    example for nmeaGGA.
  ---------------------------------------- */
static void exampleNmeaGGA()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPGGA gga;

  gga.hhmmss = (2 * 3600UL) + (54 * 60UL) + 11 + 0.516F;
  gga.latitude = 34.71357666;
  gga.ns = 'N';
  gga.longitude = 135.33515;
  gga.ew = 'E';
  gga.quality = 1;
  gga.satellites = 11;
  gga.hdop = 0.8F;
  gga.altitude = 24.0F;
  gga.qeoidElevation = 36.7F;
  gnss.nmeaGGA( &gga, TYPE_GPS, gnssBuffer, sizeof(gnssBuffer) );
}

/* ----------------------------------------
    example for nmeaGLL.
  ---------------------------------------- */
static void exampleNmeaGLL()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPGLL gll;

  gll.latitude = 34.71357666;
  gll.ns = 'N';
  gll.longitude = 135.33515;
  gll.ew = 'E';
  gll.hhmmss = (2 * 3600UL) + (54 * 60UL) + 11 + 0.516F;
  gll.pStatus = 'A';
  gll.pMode = 'A';
  gnss.nmeaGLL( &gll, TYPE_GPS, gnssBuffer, sizeof(gnssBuffer) );
}


/* ----------------------------------------
    example for nmeaGNS.
  ---------------------------------------- */
static void exampleNmeaGNS()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPGNS gns;

  gns.hhmmss = (0 * 3600UL) + (44 * 60UL) + 57 + 0.0F;
  gns.latitude = 34.713776666;
  gns.ns = 'N';
  gns.longitude = 135.335391666;
  gns.ew = 'E';
  gns.pStatus[0] = 'D';
  gns.pStatus[1] = 'D';
  gns.pStatus[2] = 'N';
  gns.satellites = 22;
  gns.hdop = 0.5F;
  gns.altitude = 40.6;
  gns.qeoidElevation = 36.7;
  gns.nStatus = 'V';
  gnss.nmeaGNS( &gns, 0, gnssBuffer, sizeof(gnssBuffer) );
}

/* ----------------------------------------
    example for nmeaGSA.
  ---------------------------------------- */
static void exampleNmeaGSA()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPGSA gsa;

  gsa.oMode = 'A';
  gsa.pMode = '3';
  gsa.sNumber[0] = 9;
  gsa.sNumber[1] = 15;
  gsa.sNumber[2] = 26;
  gsa.sNumber[3] = 5;
  gsa.sNumber[4] = 24;
  gsa.sNumber[5] = 21;
  gsa.sNumber[6] = 8;
  gsa.sNumber[7] = 2;
  gsa.sNumber[8] = 29;
  gsa.sNumber[9] = 28;
  gsa.sNumber[10] = 18;
  gsa.sNumber[11] = 10;
  gsa.pdop = 0.8;
  gsa.hdop = 0.5;
  gsa.vdop = 0.5;
  gnss.nmeaGSA( &gsa, 0, gnssBuffer, sizeof(gnssBuffer) );
}

/* ----------------------------------------
    example for nmeaGSV.
  ---------------------------------------- */
static void exampleNmeaGSV()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPGSV gsv;

  gsv.total = 4;
  gsv.seq = 1;
  gsv.inField = 14;
  gsv.info[0].number = 15;
  gsv.info[0].elevation = 67;
  gsv.info[0].azimuth = 319;
  gsv.info[0].snr = 52;
  gsv.info[1].number = 9;
  gsv.info[1].elevation = 63;
  gsv.info[1].azimuth = 68;
  gsv.info[1].snr = 53;
  gsv.info[2].number = 26;
  gsv.info[2].elevation = 45;
  gsv.info[2].azimuth = 39;
  gsv.info[2].snr = 50;
  gsv.info[3].number = 5;
  gsv.info[3].elevation = 44;
  gsv.info[3].azimuth = 104;
  gsv.info[3].snr = 49;
  gsv.id = '1';
  gnss.nmeaGSV( &gsv, TYPE_GPS, gnssBuffer, sizeof(gnssBuffer) );
}

/* ----------------------------------------
    example for nmeaRMC.
  ---------------------------------------- */
static void exampleNmeaRMC()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPRMC rmc;

  rmc.hhmmss = (1 * 3600UL) + (23 * 60UL) + 44 + 0.0F;
  rmc.pStatus = 'A';
  rmc.latitude = 34.713776666;
  rmc.ns = 'N';
  rmc.longitude = 135.335388333;
  rmc.ew = 'E';
  rmc.knot = 0.0F;
  rmc.azimuth = 0.0F;
  struct tm tt = {0,0,0, 19,11 - 1,2032 - 1900, 0,0,0 };
  time_t ttt = mktime( &tt );
  rmc.ddmmyy = ttt;
  rmc.pMode = 'D';
  rmc.nStatus = 'V';
  gnss.nmeaRMC( &rmc, 0, gnssBuffer, sizeof(gnssBuffer) );
}


/* ----------------------------------------
    example for nmeaVTG.
  ---------------------------------------- */
static void exampleNmeaVTG()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPVTG vtg;

  vtg.azimuth = 0.0F;
  vtg.orientation = 360.0F;
  vtg.knot = 0.0F;
  vtg.kmh = 0.0F;
  vtg.pMode = 'D';
  gnss.nmeaVTG( &vtg, 0, gnssBuffer, sizeof(gnssBuffer) );
}


/* ----------------------------------------
    example for nmeaZDA.
  ---------------------------------------- */
static void exampleNmeaZDA()
{
  char gnssBuffer[128];
  GNSS gnss;
  GPS_GPZDA zda;

  zda.hhmmss = (1 * 3600UL) + (48 * 60UL) + 11 + 0.0F;
  zda.day = 13;
  zda.month = 9;
  zda.year = 2013;
  zda.tzHour = 0;
  zda.tzMinute = 0;
  gnss.nmeaZDA( &zda, TYPE_GPS, gnssBuffer, sizeof(gnssBuffer) );
}
#endif
