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

#ifndef  __GNSS_H__
#define  __GNSS_H__

#include  <USART_UART.h>
#include  <strutil.h>

extern "C"
{
  #include  <stdlib.h>
  #include  <string.h>
  #include  <mul_tsk.h>
  #include  <time.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  char2Int  atoi
#define  char2Char(str)  (*str)

#define  GNSS_OK             (0)
#define  GNSS_SENTENCE_ERROR (-1)

typedef struct
{
  int      number;        // satellite number
  uint8_t  elevation;  // angle of elevation
  uint16_t azimuth;  // azimuth
  uint8_t  snr;       // SNR(dB)
} Satellite_INFO;

typedef struct
{
  float hhmmss;  // 1
  uint8_t day;   // 2
  uint8_t month; // 3
  uint16_t year;  // 4
  int     tzHour;  // 5 local time zone hour.
  uint8_t tzMinute; // 6 local time zone minute.
} GPS_GPZDA;

typedef struct
{
  float hhmmss;  // 1
  double latitude;  // 2 ido
  char  ns;  // 3 N or S
  double longitude;  // 4 keido
  char  ew;  // 5 E or W
  uint8_t quality;  // 6
  int   satellites;  // 7
  float hdop;  // 8
  float altitude;  // 9
  char  altiUnit;  // 10 unit
  float qeoidElevation;  // 11
  char  qeoidUnit;  // 12 unit
} GPS_GPGGA;

typedef struct
{
  char  oMode;  // 1 operating mode
  char  pMode;  // 2 Positioning Mode
  int   sNumber[12];  // 3~14 satellite numbers
  float pdop;  // 15
  float hdop;  // 16
  float vdop;  // 17
  char  id;    // 18 GNSS system id
} GPS_GPGSA;

typedef struct
{
  int  total;    // 1 Total number of messages
  int  seq;      // 2 Sequence number of the message
  int  inField;  // 3 Number of in-field satellites
  Satellite_INFO info[4];
  char  id;      // 20 signal ID.
} GPS_GPGSV;

typedef struct
{
  double latitude;  // 1 ido
  char  ns;  // 2 N or S
  double longitude;  // 3 keido
  char  ew;  // 4 E or W
  float hhmmss;  // 5
  char  pStatus;  // 6 positional status
  char  pMode;  // 7 positional mode
} GPS_GPGLL;

typedef struct
{
  float hhmmss;  // 1
  char  pStatus;  // 2 positional status
  double latitude;  // 3 ido
  char  ns;  // 4 N or S
  double longitude;  // 5 keido
  char  ew;  // 6 E or W
  float knot;  // 7 knot
  float azimuth;  // 8 azimuth
  uint32_t ddmmyy;  // 9 day,month,year
  float magneticDeviation;  // 10 magnetic deviation
  char  direct;  // 11 Direction of magnetic deviation correction
  char  pMode;  // 12 positional mode
  char  nStatus;  // 13 navigation status
} GPS_GPRMC;

typedef struct
{
  float azimuth;  // 1 azimuth
  char  T;  // 2
  float orientation;  // 3 magnetic orientation
  char  M;  // 4
  float knot;  // 5 knot
  char  N;  // 6
  float kmh;  // 7 km/h
  char  K;  // 8
  char  pMode;  // 9 positional mode
} GPS_GPVTG;

typedef struct
{
  float hhmmss;  // 1
  double latitude;  // 2 ido
  char  ns;  // 3 'N' or 'S'
  double longitude;  // 4 keido
  char  ew;  // 5 'E' or 'W'
  char  pStatus[3];  // 6 positional status
  int   satellites;  // 7
  float hdop;  // 8
  float altitude;  // 9
  float qeoidElevation;  // 10
  char  DGPS_data_time;  // 11 DGPS data time
  char  DGPS_station_id;  // 12 DGPS station identification number
  char  nStatus;  // 13 navigation status
} GPS_GPGNS;


typedef struct  /* 74byte */
{
  char  message[6];  // "$GPGGA"
  char  comma01;  // ','
  char  utc[10];  // 1:"hhmmss.sss"
  char  comma02;  // ','
  char  latitude[9];  // 2:"ddmm.mmmm"
  char  comma03;  // ','
  char  NS;  // 3:'N' or 'S'
  char  comma04;  // ','
  char  longitude[10];  // 4:"dddmm.mmmm"
  char  comma05;  // ','
  char  EW;  // 5:'E' or 'W'
  char  comma06;  // ','
  char  status;  // 6:'0' or '1' or '2' positioning status
  char  comma07;  // ','
  char  satellites[2];  // 7:"09" number of satellites.
  char  comma08;  // ','
  char  hdop[4];  // 8:"01.0" !variable length
  char  comma09;  // ','
  char  altitude[5];  // 9:"123.4" sea level altitude. !variable length
  char  comma10;  // ','
  char  unitOfAltitude;  // 10:'M'
  char  comma11;  // ','
  char  geoid[5];  // 11:"123.4" geoid altitude. !variable length
  char  comma12;  // ','
  char  unitOfGeoid;  // 12:'M'
  char  comma13;  // ','
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPGGA;

typedef struct  /* 52byte */
{
  char  message[6];  // "$GPGLL"
  char  comma01;  // ','
  char  latitude[9];  // 1:"ddmm.mmmm"
  char  comma02;  // ','
  char  NS;  // 2:'N' or 'S'
  char  comma03;  // ','
  char  longitude[10];  // 3:"dddmm.mmmm"
  char  comma04;  // ','
  char  EW;  // 4:'E' or 'W'
  char  comma05;  // ','
  char  utc[10];  // 5:"hhmmss.sss"
  char  comma06;  // ','
  char  status;  // 6:'A' or 'N' positioning status
  char  comma07;  // ','
  char  mode;  // 7:'A' or 'D' or 'N' Positioning mode.
  char  comma08;  // ','
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPGLL;

typedef struct  /* 74byte */
{
  char  message[6];  // "$GPGNS"
  char  comma01;  // ','
  char  utc[10];  // 1:"hhmmss.sss"
  char  comma02;  // ','
  char  latitude[9];  // 2:"ddmm.mmmm"
  char  comma03;  // ','
  char  NS;  // 3:'N' or 'S'
  char  comma04;  // ','
  char  longitude[10];  // 4:"dddmm.mmmm"
  char  comma05;  // ','
  char  EW;  // 5:'E' or 'W'
  char  comma06;  // ','
  char  status[3];  // 6:'A' or 'D' or 'N' Status of each satellite. !variable length
  char  comma07;  // ','
  char  satellites[2];  // 7:"09" number of satellites.
  char  comma08;  // ','
  char  hdop[4];  // 8:"01.0" !variable length
  char  comma09;  // ','
  char  altitude[5];  // 9:"123.4" sea level altitude. !variable length
  char  comma10;  // ','
  char  geoid[5];  // 11:"123.4" geoid altitude. !variable length
  char  comma11;  // ','
  char  navStatus;  // 12:'S' or 'C' or 'U' or 'V' Status of navigation.
  char  comma12;  // ','
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPGNS;

typedef struct  /* 68byte */
{
  char  message[6];  // "$GPGSA"
  char  comma01;  // ','
  char  mode1;  // 1:'M' or 'A'
  char  comma02;  // ','
  char  mode2;  // 2:'1' or '2' or '3'
  char  comma03;  // ','

  // !variable fields.
  struct
  {
    char  number[2];  // from 3 to 14 :"01"
    char  comma04;  // ','
  } satelliteNumbers[12];

  char  pdop[4];  // 15:"50.0" !variable length
  char  comma05;  // ','
  char  hdop[4];  // 16:"50.0" !variable length
  char  comma06;  // ','
  char  vdop[4];  // 17:"50.0" !variable length
  char  comma07;  // ','
  char  systemID;  // 18:'1'
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPGSA;

typedef struct  /* 72byte */
{
  char  message[6];  // "$GPGSV"
  char  comma01;  // ','
  char  numberOfMessage;  // 1:'1' or '2' or '3' or '4'
  char  comma02;  // ','
  char  sequenceOfMessage;  // 2:'1' or '2' or '3' or '4'
  char  comma03;  // ','
  char  view[2];  // 3:"12"
  char  comma04;  // ','

  // !variable fields.
  struct
  {
    char  number[2];  // 4:"01"
    char  comma05;  // ','
    char  angleOfElevation[2];  // 5:"90"
    char  comma06;  // ','
    char  azimuth[3];  // 6:"012"
    char  comma07;  // ','
    char  snr[2];  // 7:"12"
    char  comma08;  // ','
  } satelliteInformations[4];

  char  signalID;  // 20:'1'
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPGSV;

typedef struct  /* 74byte */
{
  char  message[6];  // "$GPRMC"
  char  comma01;  // ','
  char  utc[10];  // 1:"hhmmss.sss"
  char  comma02;  // ','
  char  status;  // 2:'A' or 'V'
  char  comma03;  // ','
  char  latitude[9];  // 3:"ddmm.mmmm"
  char  comma04;  // ','
  char  NS;  // 4:'N' or 'S'
  char  comma05;  // ','
  char  longitude[10];  // 5:"dddmm.mmmm"
  char  comma06;  // ','
  char  EW;  // 6:'E' or 'W'
  char  comma07;  // ','
  char  knots[5];  // 7:"123.4" knots !variable length
  char  comma08;  // ','
  char  trueOrientation[5];  // 8:"123.4" !variable length
  char  comma09;  // ','
  char  date[6];  // 9:"ddmmyy"
  char  comma10;  // ','
  char  comma11;  // ',' 10:
  char  comma12;  // ',' 11:
  char  mode;  // 12:'A' or 'D' or 'N'
  char  comma13;  // ','
  char  navStatus;  // 13:'S' or 'C' or 'U' or 'V'
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPRMC;

typedef struct  /* 45byte */
{
  char  message[6];  // "$GPVTG"
  char  comma01;  // ','
  char  trueOrientation[5];  // 1:"123.4" !variable length
  char  comma02;  // ','
  char  T;  // 2:'T'
  char  comma03;  // ','
  char  magneticOrientation[5];  // 3:"123.4" !variable length
  char  comma04;  // ','
  char  M;  // 4:'M'
  char  comma05;  // ','
  char  knots[5];  // 5:"123.4" knots !variable length
  char  comma06;  // ','
  char  N;  // 6:'N'
  char  comma07;  // ','
  char  kmh[5];  // 7:"123.4" km/h !variable length
  char  comma08;  // ','
  char  K;  // 8:'K'
  char  comma09;  // ','
  char  mode;  // 9:'A' or 'D' or 'N'
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPVTG;

typedef struct  /* 40byte */
{
  char  message[6];  // "$GPZDA"
  char  comma01;  // ','
  char  utc[10];  // 1:"hhmmss.sss"
  char  comma02;  // ','
  char  day[2];  // 2:"dd"
  char  comma03;  // ','
  char  month[2];  // 3:"mm"
  char  comma04;  // ','
  char  year[4];  // 4:"yyyy"
  char  comma05;  // ','
  char  localTimeZoneHour[3];  // 5:"+hh"
  char  comma06;  // ','
  char  localTimeZoneMinute[2];  // 4:"mm"
  char  checkSum[3];  // "*XX"
  char  CR;  // '\r'
  char  LF;  // '\n'
} NMEA0183_GPZDA;

enum SATELLITE_TYPE { TYPE_GPS = 1, TYPE_QZSS, TYPE_GLONASS, TYPE_GALILEO, TYPE_BEIDOU, TYPE_SBAS,  };


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class GNSS // : public hoge
{
private:
  USART_UART *gpsSerial;

  uint8_t checkSum( const char *str, int size );
  float latitude( const char *str );
  float longitude( const char *str );

  char *utcTimeString( char *dest, float utc );
  char *latitudeString( char *dest, double lat );
  char *longitudeString( char *dest, double lon );
  char *numberOfSatellitesString( char *dest, int sat );
  char *hdopString( char *dest, float hdop );
  char *altitudeString( char *dest, float alt );

public:
  GNSS();
  GNSS( USART_UART *seri );
  ~GNSS();
  void begin( USART_UART *seri );
  int  line( char *rcv, int size );
  void output( const char *str );

  int  gpzda( const char *sentence, GPS_GPZDA *zda );
  int  gpgga( const char *sentence, GPS_GPGGA *gga );
  int  gpgsa( const char *sentence, GPS_GPGSA *gsa );
  int  gpgsv( const char *sentence, GPS_GPGSV *gsv, int *type );
  int  gpgll( const char *sentence, GPS_GPGLL *gll );
  int  gprmc( const char *sentence, GPS_GPRMC *rmc );
  int  gpvtg( const char *sentence, GPS_GPVTG *vtg );
  int  gpgns( const char *sentence, GPS_GPGNS *gns );
  int  gpsGMTtime( const GPS_GPZDA *zda, struct tm *t );

  int  nmeaGGA( GPS_GPGGA *gga, int satType, char *sentence, size_t size );
  int  nmeaGLL( GPS_GPGLL *gll, int satType, char *sentence, size_t size );
  int  nmeaGNS( GPS_GPGNS *gns, int satType, char *sentence, size_t size );
  int  nmeaGSA( GPS_GPGSA *gsa, int satType, char *sentence, size_t size );
  int  nmeaGSV( GPS_GPGSV *gsv, int satType, char *sentence, size_t size );
  int  nmeaRMC( GPS_GPRMC *rmc, int satType, char *sentence, size_t size );
  int  nmeaVTG( GPS_GPVTG *vtg, int satType, char *sentence, size_t size );
  int  nmeaZDA( GPS_GPZDA *zda, int satType, char *sentence, size_t size );
};


#endif  /* __GNSS_H__ */

