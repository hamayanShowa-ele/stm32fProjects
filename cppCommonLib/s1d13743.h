/* ----------------------------------------
  S1D13743 utilities
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

#ifndef  __S1D13743_H__
#define  __S1D13743_H__

#include  <derivertive.h>
#include  <Dma.h>
#include  <0900.h>

extern "C"
{
  #include  <mul_tsk.h>
}

/* ----------------------------------------
    defines for graphic controller IC (S1D13743) register
---------------------------------------- */
#define  GLCD_ADR_REVISION_CODE                        0x00  /*通常0x98が返る*/
#define  GLCD_ADR_CONFIGULATION_READBACK               0x02  /*CNF0..2の状態*/
#define  GLCD_ADR_PLL_M_DIVEIDER                       0x04  /*CLKIの分周比を設定*/
#define  GLCD_ADR_PLL_SETTING_0                        0x06  /*0xF8を代入する*/
#define  GLCD_ADR_PLL_SETTING_1                        0x08  /*0x80を代入する*/
#define  GLCD_ADR_PLL_SETTING_2                        0x0A  /*0x28を代入する*/
#define  GLCD_ADR_PLL_SETTING_3                        0x0C  /*0x00を代入する*/
#define  GLCD_ADR_PLL_SETTING_4                        0x0E  /*目標の周波数を45MHzとし、5分周で9MHzのPCLKとすると、45となる*/
#define  GLCD_ADR_DUMMY_01                             0x10  /**/
#define  GLCD_ADR_CLOCK_SOURCE_SELECT                  0x12  /*5分周でPLLの選択 (4 <<* 3) | 0x01/*/
#define  GLCD_ADR_PANEL_TYPE                           0x14  /*パネルタイプの設定*/
#define  GLCD_ADR_HORIZONTAL_DISPLAY_WIDTH             0x16  /*HDISPサイズ*/
#define  GLCD_ADR_HORIZONTAL_NON_DISPLAY_WIDTH         0x18  /*HNDISPサイズ(非表示期間)*/
#define  GLCD_ADR_VARTICAL_DISPLAY_HEIGHT_0            0x1A  /*VDISPの下位8bit*/
#define  GLCD_ADR_VARTICAL_DISPLAY_HEIGHT_1            0x1C  /*VDISPの上位2bit*/
#define  GLCD_ADR_VARTICAL_NON_DISPLAY_PERIOD          0x1E  /*VNDISPサイズ(非表示期間)*/
#define  GLCD_ADR_HS_PULSE_WIDTH                       0x20  /*HSYNCパルスの極性と幅*/
#define  GLCD_ADR_HS_PULSE_START_POSITION              0x22  /*HSYNCパルスの開始位置*/
#define  GLCD_ADR_VS_PULSE_WIDTH                       0x24  /*VSYNCパルスの極性と幅*/
#define  GLCD_ADR_VS_PULSE_START_POSITION              0x26  /*VSYNCパルスの開始位置*/
#define  GLCD_ADR_PCLK_POLARITY                        0x28  /*PCLKの極性*/
#define  GLCD_ADR_INPUT_MODE                           0x2A  /*入力モード*/
#define  GLCD_ADR_INPUT_YUV_RGB_TRANSLATE_MODE_0       0x2C  /*入力モード*/
#define  GLCD_ADR_INPUT_YUV_RGB_TRANSLATE_MODE_1       0x2E  /*入力モード*/
#define  GLCD_ADR_U_DATA_FIX                           0x30  /**/
#define  GLCD_ADR_V_DATA_FIX                           0x32  /**/
#define  GLCD_ADR_DISPLAY_MODE                         0x34  /**/
#define  GLCD_ADR_SPECIAL_EFFECT                       0x36  /**/
#define  GLCD_ADR_WINDOW_X_START_POSITION_0            0x38  /**/
#define  GLCD_ADR_WINDOW_X_START_POSITION_1            0x3A  /**/
#define  GLCD_ADR_WINDOW_Y_START_POSITION_0            0x3C  /**/
#define  GLCD_ADR_WINDOW_Y_START_POSITION_1            0x3E  /**/
#define  GLCD_ADR_WINDOW_X_END_POSITION_0              0x40  /**/
#define  GLCD_ADR_WINDOW_X_END_POSITION_1              0x42  /**/
#define  GLCD_ADR_WINDOW_Y_END_POSITION_0              0x44  /**/
#define  GLCD_ADR_WINDOW_Y_END_POSITION_1              0x46  /**/
#define  GLCD_ADR_MEMORY_DATA_PORT_0                   0x48  /**/
#define  GLCD_ADR_MEMORY_DATA_PORT_1                   0x49  /**/
#define  GLCD_ADR_MEMORY_READ_ADDRESS_0                0x4A  /**/
#define  GLCD_ADR_MEMORY_READ_ADDRESS_1                0x4C  /**/
#define  GLCD_ADR_MEMORY_READ_ADDRESS_2                0x4E  /**/
#define  GLCD_ADR_GAMMA_CORRECTION_ENABLE              0x50  /**/
#define  GLCD_ADR_GAMMA_CORRECTION_TABLE_INDEX         0x52  /**/
#define  GLCD_ADR_GAMMA_CORRECTION_TABLE_DATA          0x54  /**/
#define  GLCD_ADR_POWER_SAVE                           0x56  /**/
#define  GLCD_ADR_NON_DISPLAY_PERIOD_CONTROL_STATUS    0x58  /**/
#define  GLCD_ADR_GPIO_PINS_CONFIGURATION_0            0x5A  /**/
#define  GLCD_ADR_GPIO_PINS_STATUS_CONTROL_0           0x5C  /**/
#define  GLCD_ADR_GPIO_POSITIVE_EDGE_INTERRUPT_TRIGGER 0x5E  /**/
#define  GLCD_ADR_GPIO_NEGATIVE_EDGE_INTERRUPT_TRIGGER 0x60  /**/
#define  GLCD_ADR_GPIO_INTERRUPT_STATUS                0x62  /**/
#define  GLCD_ADR_GPIO_PULL_DOWN_CONTROL               0x64  /**/

/* ----------------------------------------
    defines for another types.
---------------------------------------- */
#define  X_MAX_PIXEL    480
#define  Y_MAX_PIXEL    272
#define  HSYNC_PERIOD   525
#define  VSYNC_PERIOD   286

#define  SWIVEL_VIEW_000 0
#define  SWIVEL_VIEW_090 1
#define  SWIVEL_VIEW_180 2
#define  SWIVEL_VIEW_270 3


#if 0
#define  PIXEL_SIZING(s)  do{ while( GLCD_Semafor & 0x00000001 ) ; GLCD_ADDRESS_SET = GLCD_ADR_SPECIAL_EFFECT; GLCD_DATA_BYTE_ADR = (s & 0x03);} while(0)
enum PIXEL_RESIZING { PIXEL_NOT_RESIZING = 0, PIXEL_DOUBLING, PIXEL_HALVING, PIXEL_RESERVE };

#define  SWIVEL_VIEW(s)   do{ while( GLCD_Semafor & 0x00000001 ) ; GLCD_ADDRESS_SET = GLCD_ADR_DISPLAY_MODE; GLCD_DATA_BYTE_ADR = (s & 0x03);} while(0)
enum SWIVEL_ANGLE { ANGLE_0 = 0, ANGLE_90, ANGLE_180, ANGLE_270 };
#endif


/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class S1D13743 : public STM32F_DMA
{
public:
  S1D13743();
  ~S1D13743();

  uint8_t revision();
  int begin();
  int vaConfig( uint16_t l = 0, uint16_t u = 0, uint16_t r = X_MAX_PIXEL - 1, uint16_t d = Y_MAX_PIXEL - 1 );
  uint16_t left();
  uint16_t right();
  uint16_t up();
  uint16_t down();
  uint16_t width();
  uint16_t height();

  void coordinate( uint16_t x, uint16_t y );
  void window( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end );
  void write( uint16_t data );
  void write( const uint16_t *data, uint32_t size );
  void write( uint16_t data, uint32_t size );
  void read( uint32_t adr, uint16_t *dst, uint32_t size = 1 );

  void swivelView( uint8_t r );

private:
  uint16_t va_width,va_height;
  uint16_t va_left,va_right;
  uint16_t va_up,va_down;

  int redGreenRead( uint32_t adr, uint16_t *dst, uint32_t size );
  int blueRead( uint32_t adr, uint16_t *dst, uint32_t size );
};


#endif  /* __S1D13743_H__ */

