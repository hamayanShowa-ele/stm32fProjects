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
  #include  <mul_tsk.h>
  #include  <time.h>
}

/* ----------------------------------------
    defines
---------------------------------------- */
#define  char2Int  atoi
#define  char2Char(str)  (*str)

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
  float latitude;  // 2 ido
  char  ns;  // 3 N or S
  float longitude;  // 4 keido
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
  float latitude;  // 1 ido
  char  ns;  // 2 N or S
  float longitude;  // 3 keido
  char  ew;  // 4 E or W
  float hhmmss;  // 5
  char  pStatus;  // 6 positional status
  char  pMode;  // 7 positional mode
} GPS_GPGLL;

typedef struct
{
  float hhmmss;  // 1
  char  pStatus;  // 2 positional status
  float latitude;  // 3 ido
  char  ns;  // 4 N or S
  float longitude;  // 5 keido
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
  float latitude;  // 2 ido
  char  ns;  // 3 N or S
  float longitude;  // 4 keido
  char  ew;  // 5 E or W
  char  pStatus;  // 6 positional status
  int   satellites;  // 7
  float hdop;  // 8
  float altitude;  // 9
  float qeoidElevation;  // 10
  char  DGPS_data_time;  // 11 DGPS data time
  char  DGPS_station_id;  // 12 DGPS station identification number
  char  nStatus;  // 13 navigation status
} GPS_GPGNS;

enum SATELLITE_TYPE { TYPE_GPS, TYPE_QZSS, TYPE_GLONASS, TYPE_GALILEO, TYPE_BEIDOU, TYPE_SBAS,  };


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
  float latitude( const char *str );
  float longitude( const char *str );

public:
  GNSS();
  GNSS( USART_UART *seri );
  ~GNSS();
  void begin( USART_UART *seri );
  int  line( char *rcv, int size );

  int  gpzda( const char *sentence, GPS_GPZDA *zda );
  int  gpgga( const char *sentence, GPS_GPGGA *gga );
  int  gpgsa( const char *sentence, GPS_GPGSA *gsa );
  int  gpgsv( const char *sentence, GPS_GPGSV *gsv, int *type );
  int  gpgll( const char *sentence, GPS_GPGLL *gll );
  int  gprmc( const char *sentence, GPS_GPRMC *rmc );
  int  gpvtg( const char *sentence, GPS_GPVTG *vtg );
  int  gpgns( const char *sentence, GPS_GPGNS *gns );
  int  gpsGMTtime( const GPS_GPZDA *zda, struct tm *t );
};


#endif  /* __GNSS_H__ */

