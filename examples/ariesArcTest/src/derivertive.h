/* ----------------------------------------
  derivertive.h
    designed by hamayan since 2009/05/21
---------------------------------------- */
#ifndef _DERIVERTIVE_H_
#define _DERIVERTIVE_H_

#include  <stm32fPeripheral.h>

#define  __BOARD_0900_USE__
#define  __BOARD_ARC_TEST_USE__
#define  __KANJI_USE__

/*
  defines for interrupt
*/
#define  BASE_PRIORITY      2
#define  BASE_SUB_PRIORITY  0

/*
 defines for external cpu bus.
*/
#define  GLCD_DATA_BYTE_ADR   *((volatile uint8_t *)0x60020000)
#define  GLCD_DATA_WORD_ADR   *((volatile uint16_t *)0x60020000)
#define  GLCD_ADDRESS_SET     *((volatile uint8_t *)0x60000000)

/*
 defines for usart and uart.
*/
#define  SCI_1_SND_BUFFER_SIZE  256
#define  SCI_1_RCV_BUFFER_SIZE  256
#define  SCI_2_SND_BUFFER_SIZE  0
#define  SCI_2_RCV_BUFFER_SIZE  0
#define  SCI_3_SND_BUFFER_SIZE  0
#define  SCI_3_RCV_BUFFER_SIZE  0
#define  SCI_4_SND_BUFFER_SIZE  0
#define  SCI_4_RCV_BUFFER_SIZE  0
#define  SCI_5_SND_BUFFER_SIZE  0
#define  SCI_5_RCV_BUFFER_SIZE  0

/*
 defines for gpio pins.
*/
#if  defined( __BOARD_0900_USE__ )
/* USART1 TXD RXD */
#define  USART1_TXD_PORT      GPIOA
#define  USART1_RXD_PORT      GPIOA
#define  USART1_TXD_PIN       GPIO_Pin_9
#define  USART1_RXD_PIN       GPIO_Pin_10

/* USART2 TXD RXD */
#define  USART2_TXD_PORT      GPIOA
#define  USART2_RXD_PORT      GPIOA
#define  USART2_TXD_PIN       GPIO_Pin_2
#define  USART2_RXD_PIN       GPIO_Pin_3

/* USART3 TXD RXD */
#define  USART3_TXD_PORT      GPIOB
#define  USART3_RXD_PORT      GPIOB
#define  USART3_TXD_PIN       GPIO_Pin_10
#define  USART3_RXD_PIN       GPIO_Pin_11

/* UART4 TXD RXD */
#define  UART4_TXD_PORT      GPIOC
#define  UART4_RXD_PORT      GPIOC
#define  UART4_TXD_PIN       GPIO_Pin_10
#define  UART4_RXD_PIN       GPIO_Pin_11

/* UART5 TXD RXD */
#define  UART5_TXD_PORT      GPIOC
#define  UART5_RXD_PORT      GPIOD
#define  UART5_TXD_PIN       GPIO_Pin_12
#define  UART5_RXD_PIN       GPIO_Pin_2

/* I2C1 */
#define  SDA1  PB7
#define  SCL1  PB6
/* I2C2 */
#define  SDA2  PB11
#define  SCL2  PB10


#define  TOUCH_PANEL_UP    PC0  /* analog input and digital output */
#define  TOUCH_PANEL_DOWN  PC1  /* analog input and digital output */
#define  TOUCH_PANEL_RIGHT PC2  /* analog input and digital output */
#define  TOUCH_PANEL_LEFT  PC3  /* analog input and digital output */
#define  AXIS_Z     PA6  /* analog input */
#define  AXIS_Y     PC4  /* analog input */
#define  AXIS_X     PC5  /* analog input */
#define  SD_CD      PB10  /* input */
#define  SD_WP      PB11  /* input */
#define  SD_SS      PB12  /* output */
#define  LCD_INT    PD12  /* input */
#define  PWRSVE     PD13  /* output */
#define  LCD_TE     PD3   /* input */
#define  LCD_RST    PD6   /* output */
#define  BRIGHT     PB8   /* output */
#define  BUZZER     PB9   /* output */
#define  USBON      PA7   /* output */

/*
 defines for lcd(s1d13743).
*/
#define  LCD_TE_PORT_SOURCE  GPIO_PortSourceGPIOD
#define  LCD_TE_PIN_SOURCE   GPIO_PinSource3
#define  LCD_TE_ETXI_LINE    EXTI_Line3
#define  LCD_TE_IREQn        EXTI3_IRQn
#define  LCD_DMA_IREQn       DMA2_Channel4_5_IRQn
#define  LCD_DMA_Channel     DMA2_Channel4

/* S1D13743のGPIO7に接続されているLCD個別の定義 */
#define  LCD_DISP_PIN  0x80
/* S1D13743のCLKIの定義 */
//#define  __CLKI_IS_1M__  /*CLKIが1MHzの時*/
#define  __CLKI_IS_8M__  /*CLKIが8MHzの時*/
//#define  __CLKI_IS_36M__  /*CLKIが36MHzの時*/
#endif  /* __BOARD_0900_USE__ */


/* gpio common. */
#define  EXT_BUS_D0  PD14   /* input/output */
#define  EXT_BUS_D1  PD15   /* input/output */
#define  EXT_BUS_D2  PD0    /* input/output */
#define  EXT_BUS_D3  PD1    /* input/output */
#define  EXT_BUS_D4  PE7    /* input/output */
#define  EXT_BUS_D5  PE8    /* input/output */
#define  EXT_BUS_D6  PE9    /* input/output */
#define  EXT_BUS_D7  PE10   /* input/output */
#define  EXT_BUS_D8  PE11   /* input/output */
#define  EXT_BUS_D9  PE12   /* input/output */
#define  EXT_BUS_D10 PE13   /* input/output */
#define  EXT_BUS_D11 PE14   /* input/output */
#define  EXT_BUS_D12 PE15   /* input/output */
#define  EXT_BUS_D13 PD8    /* input/output */
#define  EXT_BUS_D14 PD9    /* input/output */
#define  EXT_BUS_D15 PD10   /* input/output */

#define  EXT_BUS_A16 PD11   /* output */
#define  EXT_BUS_OE  PD4    /* output */
#define  EXT_BUS_WE  PD5    /* output */
#define  EXT_BUS_NE1 PD7    /* output */


/*
 defines for another types.
*/
#if 0
typedef enum
{
  STDNULL=0,
  STDOUT=1,
  STDOUT2,
  STDOUT3,
  STDOUT4,
  STDOUT5,
  STDEND,
} HANDLES;
#define  STDIN                    STDOUT

#define  Cpu_Delay100US(delay)    Dly100us(delay)
#define  HEAPSIZE                 (10000)  /* look at ld file. */

typedef struct
{
  const char *fn;
  const char *dateTime;
  unsigned long dataOffset;
  void *data;
  const char *auth;
  void *exec;
  unsigned long dataSize;
} FILE_PROPERTIES;
#endif


/*
 global variables.
*/

/*
 prototypes.
*/
#if 0
//#include  "mul_tsk.h"
int xprintf(int fd,const char *fmt, ...);
int _xwrite(int fd, char *str, int len);
int _xread(int fd, char *str, int len);
void gpioInit( void );
void Dly10us( int delay );
void Dly50us( int delay );
void Dly100us( int delay );
void cris_en( void );  // stop he interrupt.
void cris_ex( void );  // start he interrupt.
#endif

/*
 configures for multi tasks.
*/
#define  MAX_TASK_NUMBER  4  //
enum GR_SAKURA_TSK_ID
{
  ID_stackMonitor,
  ID_lcdDemo01,
  ID_actLed,
  ID_temperature,
};

#define  MAX_MBX_NUMBER    1
enum GR_SAKURA_MBX_ID
{
  MBXID_dummy,
};

#define  MAX_SEM_NUMBER    1
enum GR_SAKURA_SEM_ID
{
  SEMID_GCLD = 1,
};

/*
 debug messages.
*/
#define _DBG_passedFileAndLine() {strncpy(_file, __FILE__, sizeof(_file)); _line = __LINE__;}
extern char _file[];
extern int _line;


#endif /*_DERIVERTIVE_H_*/


/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2009/05/21  */
/*********************************************************************************/
