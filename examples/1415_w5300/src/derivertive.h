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
#define  BASE_PRIORITY      15  // The priorities range from 0 to 15, with 0 being the highest priority.
#define  BASE_SUB_PRIORITY  0

/*
  memory address.
*/
#define  INT_SRAM_BASE_ADDRESS      0x60000000
#define  INT_ETHERNET_BASE_ADDRESS  0x64000000
#define  INT_ARCNET_BASE_ADDRESS    0x68000000
#define  CBUS_BASE_ADR   0x6C000000
#define  CBUS_IO_ADR     (CBUS_BASE_ADR + 0x00000000)
#define  CBUS_MEM_ADR    (CBUS_BASE_ADR + 0x00800000)

//#define  CBUS_DUMMY_MEM_ADR  (CBUS_MEM_ADR + 0x074000)
#define  CBUS_DUMMY_MEM_ADR  (CBUS_MEM_ADR + 0x080000)
//#define  CBUS_DUMMY_MEM_ADR  (CBUS_MEM_ADR + 0x072000)  // semaphore address
#define  CBUS_DUMMY_IO_ADR   (CBUS_IO_ADR + 0x000000)

/*
  sockets
*/
#define  SNTP_CLIENT_SOCKET  7
#define  ECHO_CLIENT_SOCKET  6
#define  ECHO_SERVER_SOCKET  5

/*
  defines for w5300.
*/
#define  ETN_RST   PC2
#define _WIZCHIP_  5300
//#define  ETH_BASE_ADDRESS    INT_ETHERNET_BASE_ADDRESS
//#define  _WIZCHIP_IO_BASE_   INT_ETHERNET_BASE_ADDRESS
#define  _WIZCHIP_IO_BUS_WIDTH_  16
//#define  IDM_AR  ((_WIZCHIP_IO_BASE_ + 0x0002))  ///< Indirect mode address register
//#define  IDM_DR  ((_WIZCHIP_IO_BASE_ + 0x0004))  ///< Indirect mode data register
#define  IDM_AR  ((INT_ETHERNET_BASE_ADDRESS + (0x0002 * 2)))  ///< Indirect mode address register
#define  IDM_DR  ((INT_ETHERNET_BASE_ADDRESS + (0x0004 * 2)))  ///< Indirect mode data register

/*
  time zone.
*/
#define  TIMEZONE_JST  (9 * 60 * 60)

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

/* c-bus enable. */
//#define  BUS_ENB     PF11
#define  BUS_ENB_PORT  GPIOF
#define  BUS_ENB_PIN   GPIO_Pin_11
#define  BUS_ENB_IS_0  BUS_ENB_PORT->BSRR = (BUS_ENB_PIN << 16)
#define  BUS_ENB_IS_1  BUS_ENB_PORT->BSRR = (BUS_ENB_PIN << 0)

/* ACT LED */
//#define  ACTLED     PB9
#define  ACTLED_PORT  GPIOB
#define  ACTLED_PIN   GPIO_Pin_9
#define  ACTLED_IS_0  ACTLED_PORT->BSRR = (ACTLED_PIN << 16)
#define  ACTLED_IS_1  ACTLED_PORT->BSRR = (ACTLED_PIN << 0)

/* I2C1,I2C2 */
#define  SCL1       PB7
#define  SDA1       PB6

/* CPU BUS */
#define  EXT_BUS_D0  PD14
#define  EXT_BUS_D1  PD15
#define  EXT_BUS_D2  PD0
#define  EXT_BUS_D3  PD1
#define  EXT_BUS_D4  PE7
#define  EXT_BUS_D5  PE8
#define  EXT_BUS_D6  PE9
#define  EXT_BUS_D7  PE10
#define  EXT_BUS_D8  PE11
#define  EXT_BUS_D9  PE12
#define  EXT_BUS_D10  PE13
#define  EXT_BUS_D11  PE14
#define  EXT_BUS_D12  PE15
#define  EXT_BUS_D13  PD8
#define  EXT_BUS_D14  PD9
#define  EXT_BUS_D15  PD10

#define  EXT_BUS_A0  PF0
#define  EXT_BUS_A1  PF1
#define  EXT_BUS_A2  PF2
#define  EXT_BUS_A3  PF3
#define  EXT_BUS_A4  PF4
#define  EXT_BUS_A5  PF5
#define  EXT_BUS_A6  PF12
#define  EXT_BUS_A7  PF13
#define  EXT_BUS_A8  PF14
#define  EXT_BUS_A9  PF15
#define  EXT_BUS_A10  PG0
#define  EXT_BUS_A11  PG1
#define  EXT_BUS_A12  PG2
#define  EXT_BUS_A13  PG3
#define  EXT_BUS_A14  PG4
#define  EXT_BUS_A15  PG5
#define  EXT_BUS_A16  PD11
#define  EXT_BUS_A17  PD12
#define  EXT_BUS_A18  PD13
#define  EXT_BUS_A21  PE6

#define  EXT_BUS_OE  PD4
#define  EXT_BUS_WE  PD5
#define  EXT_BUS_NE1  PD7
#define  EXT_BUS_NE2  PG9
#define  EXT_BUS_NE3  PG10
#define  EXT_BUS_NE4  PG12
#define  EXT_BUS_BLE  PE0
#define  EXT_BUS_BHE  PE1
#define  EXT_BUS_WAIT PD6

/* ETHERNET */

/* AT93C46 (SPI1) */
#define  AT93C46_CS  PB0
#define  AT93C46_CS_PORT  GPIOB
#define  AT93C46_CS_PIN   GPIO_Pin_0
#define  AT93C46_CS_IS_0  AT93C46_CS_PORT->BSRR = (AT93C46_CS_PIN << 16)
#define  AT93C46_CS_IS_1  AT93C46_CS_PORT->BSRR = (AT93C46_CS_PIN << 0)

#define  AT93C46_SK  PA5
#define  AT93C46_SK_PORT  GPIOA
#define  AT93C46_SK_PIN   GPIO_Pin_5
#define  AT93C46_SK_IS_0  AT93C46_SK_PORT->BSRR = (AT93C46_SK_PIN << 16)
#define  AT93C46_SK_IS_1  AT93C46_SK_PORT->BSRR = (AT93C46_SK_PIN << 0)

#define  AT93C46_DI  PA7
#define  AT93C46_DI_PORT  GPIOA
#define  AT93C46_DI_PIN   GPIO_Pin_7
#define  AT93C46_DI_IS_0  AT93C46_DI_PORT->BSRR = (AT93C46_DI_PIN << 16)
#define  AT93C46_DI_IS_1  AT93C46_DI_PORT->BSRR = (AT93C46_DI_PIN << 0)

#define  AT93C46_DO  PA6
#define  AT93C46_DO_PORT  GPIOA
#define  AT93C46_DO_PIN   GPIO_Pin_6
#define  AT93C46_DO_IS_0  AT93C46_DO_PORT->BSRR = (AT93C46_DO_PIN << 16)
#define  AT93C46_DO_IS_1  AT93C46_DO_PORT->BSRR = (AT93C46_DO_PIN << 0)
#define  AT93C46_DO_IS    (AT93C46_DO_PORT->IDR & AT93C46_DO_PIN)

/* ANOTHER */
#define  ACTLED     PB9
#define  ARC_RST    PC3
#define  C_RST      PD3
#define  BUS_ENB    PF11

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
#define  MAX_TASK_NUMBER  3
enum GR_SAKURA_TSK_ID
{
  ID_stackMonitor,
  ID_echoClient,
  ID_echoServer,
};

#define  MAX_MBX_NUMBER    1
enum GR_SAKURA_MBX_ID
{
  MBXID_dummy,
};

#define  MAX_SEM_NUMBER    1
enum GR_SAKURA_SEM_ID
{
  SEMID_W5300 = 1,
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
