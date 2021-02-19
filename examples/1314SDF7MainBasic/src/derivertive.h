/* ----------------------------------------
  derivertive.h
    designed by hamayan since 2009/05/21
---------------------------------------- */
#ifndef _DERIVERTIVE_H_
#define _DERIVERTIVE_H_

#include  <stm32fPeripheral.h>

#define  __BOARD_1314_USE__
//#define  __BOARD_1314A_USE__
//#define  __BOARD_1314B_USE__
//#define  __KANJI_USE__

#define  TIMEZONE_JST  (9 * 3600UL)

/*
  defines for interrupt
*/
#define  BASE_PRIORITY      2
#define  BASE_SUB_PRIORITY  0

/*
  defines for w5100.
*/
#define  _WIZCHIP_  5100
#define  _WIZCHIP_IO_MODE_  _WIZCHIP_IO_MODE_SPI_  // _WIZCHIP_IO_MODE_SPI_VDM_
#define  _WIZCHIP_IF_NUMBERS_  2  /* Number of wiznet chips. */

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
#define  USART3_TXD_PORT      GPIOC
#define  USART3_RXD_PORT      GPIOC
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

/* I2C1,I2C2 */
#define  SCL1       PB8  /* remap */
#define  SDA1       PB9  /* remap */

/* SPI1,SPI2,SPI3 */
#define  SPI1_PORT      GPIOB  /* remap */
#define  SPI1_SCK_PIN   GPIO_Pin_3  /* remap */
#define  SPI1_MISO_PIN  GPIO_Pin_4  /* remap */
#define  SPI1_MOSI_PIN  GPIO_Pin_5  /* remap */
#define  SPI2_PORT      GPIOB
#define  SPI2_SCK_PIN   GPIO_Pin_13
#define  SPI2_MISO_PIN  GPIO_Pin_14
#define  SPI2_MOSI_PIN  GPIO_Pin_15
#define  SPI3_PORT      GPIOB
#define  SPI3_SCK_PIN   GPIO_Pin_3
#define  SPI3_MISO_PIN  GPIO_Pin_4
#define  SPI3_MOSI_PIN  GPIO_Pin_5

/* PULSE IN */
#define  PULSE_IN_PORT  GPIOD
#define  PULSE0_PIN     GPIO_Pin_8
#define  PULSE1_PIN     GPIO_Pin_9
#define  PULSE2_PIN     GPIO_Pin_10
#define  PULSE3_PIN     GPIO_Pin_11
#define  PULSE4_PIN     GPIO_Pin_12
#define  PULSE5_PIN     GPIO_Pin_13
#define  PULSE6_PIN     GPIO_Pin_14
#define  PULSE7_PIN     GPIO_Pin_15

/* ETHERNET */
#define  ETH1_INT   PD3  /* input */
#define  ETH2_INT   PD2  /* input */
#define  ETH_RST    PD5   /* output */
#define  ETH1_RST   PD4   /* output */
#define  ETH2_RST   PD5   /* output */
#define  ETH1_CS    PD7  /* output */
#define  ETH1_CS_PORT  GPIOD
#define  ETH1_CS_PIN   GPIO_Pin_7
#define  ETH1_CS_IS_0  ETH1_CS_PORT->BSRR = (ETH1_CS_PIN << 16)
#define  ETH1_CS_IS_1  ETH1_CS_PORT->BSRR = (ETH1_CS_PIN << 0)
#define  ETH2_CS    PD6  /* output */
#define  ETH2_CS_PORT  GPIOD
#define  ETH2_CS_PIN   GPIO_Pin_6
#define  ETH2_CS_IS_0  ETH2_CS_PORT->BSRR = (ETH2_CS_PIN << 16)
#define  ETH2_CS_IS_1  ETH2_CS_PORT->BSRR = (ETH2_CS_PIN << 0)

/* ANOTHER */
#define  TAMPER     PC13  /* input */
#define  ACTLED     PE0   /* output */
#define  USBON      PE1   /* output */
#define  RAM_CS     PB12  /* output */
#define  RAM_CS_PORT GPIOB  /* output */
#define  RAM_CS_PIN  GPIO_Pin_12  /* output */


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
#define  MAX_TASK_NUMBER  2  //
enum GR_SAKURA_TSK_ID
{
  ID_stackMonitor,
  ID_ntpClient,
};

#define  MAX_MBX_NUMBER    1
enum GR_SAKURA_MBX_ID
{
  MBXID_dummy,
};

#define  MAX_SEM_NUMBER    3
enum GR_SAKURA_SEM_ID
{
  SEMID_GCLD = 1,
  SEMID_SPI2,
  SEMID_SPI3,
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
