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

GNSS::GNSS( Serial *seri )
{
  begin( seri );
}

GNSS::~GNSS()
{
}

/* ----------------------------------------
    begin and end
---------------------------------------- */
void GNSS::begin( Serial *seri )
{
  gpsSerial = seri;
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
