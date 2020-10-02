/* ----------------------------------------
  derivertive.h
    designed by hamayan since 2009/05/21
---------------------------------------- */
#ifndef _DERIVERTIVE_H_
#define _DERIVERTIVE_H_

#include  <stm32fPeripheral.h>

//#define  __BOARD_1303_1304_USE__
//#define  __BOARD_0900_USE__
//#define  __KANJI_USE__

/*
  defines for interrupt
*/
#define  BASE_PRIORITY      2
#define  BASE_SUB_PRIORITY  0

/*
 defines for external cpu bus.
*/
//#define  GLCD_DATA_BYTE_ADR   *((volatile uint8_t *)0x60020000)
//#define  GLCD_DATA_WORD_ADR   *((volatile uint16_t *)0x60020000)
//#define  GLCD_ADDRESS_SET     *((volatile uint8_t *)0x60000000)

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

/* SD CARD */
#define  SD_CD      PC6  /* input */
#define  SD_WP           /* input */
#define  SD_SS      PB12  /* output */

/* I2C1,I2C2 */
#define  SCL2       PB10
#define  SDA2       PB11
#define  BOARD_1703_IC2_ADR  0x20  /* PCA8574PW */
#define  BOARD_1703_IC3_ADR  0x21  /* PCA8574PW */
#define  BOARD_1703_IC4_ADR  0x22  /* PCA8574PW */
#define  BOARD_1703_U1_ADR   0x76  /* BME280 */
#define  BOARD_1703_IC7_ADR  0x50  /* 24AA025E48 mac address i2c eeprom */

/* ANOTHER */
#define  W5500_INT  PC5   /* input */
#define  W5500_CS   PC7   /* output */
#define  W5500_RESET PC8   /* output */
#define  PPS        PC9   /* input/output */
#define  GCLK_RESET PA8   /* output */
#define  DIVCLK     PA11   /* input/output */
#define  GCLK       PA12   /* input/output */
#define  SEG_A      PB3   /* output */
#define  SEG_B      PB4   /* output */
#define  SEG_C      PB5   /* output */
#define  SEG_D      PB6   /* output */
#define  SEG_E      PB7   /* output */
#define  SEG_F      PB8   /* output */
#define  SEG_G      PB9   /* output */
#define  ACTLED     PC13   /* output */


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
  ID_ppsGenerator,
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
