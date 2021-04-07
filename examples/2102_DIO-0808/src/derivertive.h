/* ----------------------------------------
  derivertive.h
    designed by hamayan since 2009/05/21
---------------------------------------- */
#ifndef _DERIVERTIVE_H_
#define _DERIVERTIVE_H_

#include  <stm32fPeripheral.h>

/*
  defines for interrupt
*/
#define  BASE_PRIORITY      2
#define  BASE_SUB_PRIORITY  0

/*
  defines for w5500.
*/
#define  _WIZCHIP_  5500
#define  _WIZCHIP_IO_MODE_  _WIZCHIP_IO_MODE_SPI_VDM_
#define  _WIZCHIP_CS_  PC7

/*
  time zone.
*/
#define TIMEZONE_JST    (9 * 60 * 60)

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
#define  USART1_TXD           PA9
#define  USART1_RXD           PA10

/* USART2 TXD RXD */
#define  USART2_TXD_PORT      GPIOA
#define  USART2_RXD_PORT      GPIOA
#define  USART2_TXD_PIN       GPIO_Pin_2
#define  USART2_RXD_PIN       GPIO_Pin_3
#define  USART2_TXD           PA2
#define  USART2_RXD           PA3

/* USART3 TXD RXD */
#define  USART3_TXD_PORT      GPIOB
#define  USART3_RXD_PORT      GPIOB
#define  USART3_TXD_PIN       GPIO_Pin_10
#define  USART3_RXD_PIN       GPIO_Pin_11
#define  USART3_TXD           PB10
#define  USART3_RXD           PB11

/* UART4 TXD RXD */
#define  UART4_TXD_PORT      GPIOC
#define  UART4_RXD_PORT      GPIOC
#define  UART4_TXD_PIN       GPIO_Pin_10
#define  UART4_RXD_PIN       GPIO_Pin_11
#define  UART4_TXD           PC10
#define  UART4_RXD           PC11

/* UART5 TXD RXD */
#define  UART5_TXD_PORT      GPIOC
#define  UART5_RXD_PORT      GPIOD
#define  UART5_TXD_PIN       GPIO_Pin_12
#define  UART5_RXD_PIN       GPIO_Pin_2
#define  UART5_TXD           PC12
#define  UART5_RXD           PD2

/* I2C1,I2C2 */
#define  SCL1       PB6
#define  SDA1       PB7

/* ETHERNET */
/*
#define  ETH_SPI    SPI2
#define  ETH_RCC    RCC_APB1Periph_SPI2
#define  ETH_GPIO   GPIOB
#define  ETH_SCK_PIN  GPIO_Pin_13
#define  ETH_MOSI_PIN GPIO_Pin_15
#define  ETH_MISO_PIN GPIO_Pin_14
#define  ETH_CS_PORT  GPIOB
#define  ETH_CS_PIN   GPIO_Pin_12
*/

#define  W5500_INT       PC6   /* input */
#define  W5500_CS        PB12   /* output */
#define  W5500_RESET     PC7   /* output */
#define  SPI2_SCK_PORT   GPIOB
#define  SPI2_SCK_PIN    GPIO_Pin_13
#define  SPI2_MISO_PORT  GPIOB
#define  SPI2_MISO_PIN   GPIO_Pin_14
#define  SPI2_MOSI_PORT  GPIOB
#define  SPI2_MOSI_PIN   GPIO_Pin_15
#define  SPI2_SCK        PB13
#define  SPI2_MISO       PB14
#define  SPI2_MOSI       PB15
#define  W5500_CS_PORT   GPIOB
#define  W5500_CS_PIN    GPIO_Pin_12
#define  W5500_CS_IS_0   W5500_CS_PORT->BSRR = (W5500_CS_PIN << 16)
#define  W5500_CS_IS_1   W5500_CS_PORT->BSRR = (W5500_CS_PIN)

/* SNTP */
#define  SNTP_CLIENT_SOCKET  1
#define  TIME_ZONE           40  /* Korea, East Timor, Russia (Irkutsk Oblast), Japan */

/* ANOTHER */
#define  ACTLED     PB4   /* output */


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
#define  MAX_TASK_NUMBER  2
enum GR_SAKURA_TSK_ID
{
  ID_stackMonitor,
  ID_pulseOut,
};

#define  MAX_MBX_NUMBER    1
enum GR_SAKURA_MBX_ID
{
  MBXID_dummy,
};

#define  MAX_SEM_NUMBER    1
enum GR_SAKURA_SEM_ID
{
  SEMID_SPI2 = 1,
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
