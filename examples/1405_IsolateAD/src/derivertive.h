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

#define  ADC123_IN10  PC0   /* analog input */
#define  ADC123_IN11  PC1   /* analog input */
#define  ADC123_IN12  PC2   /* analog input */
#define  ADC123_IN13  PC3   /* analog input */
#define  ADC123_IN0   PA0   /* analog input */
#define  ADC123_IN1   PA1   /* analog input */
#define  ADC123_IN2   PA2   /* analog input */
#define  ADC123_IN3   PA3   /* analog input */
#define  ADC12_IN4    PA4   /* analog input */
#define  ADC12_IN5    PA5   /* analog input */
#define  V2R5         PA6   /* analog input (ADC12_IN6) */

#define  FIFO_D0      PB0   /* fifo data bus bit 0 */
#define  FIFO_D1      PB1   /* fifo data bus bit 1 */
#define  FIFO_D2      PB2   /* fifo data bus bit 2 */

#define  INT2HOST     PC10   /* interrupt request to the host. */
#define  CONVERT      PC11   /* adc convert timing input. */
#define  FIFOWR1      PC12   /* fifo write timing 1. */
#define  FIFOWR0      PD2    /* fifo write timing 0. */

#define  FIFO_D3      PB3   /* fifo data bus bit 3 */
#define  FIFO_D4      PB4   /* fifo data bus bit 4 */
#define  FIFO_D5      PB5   /* fifo data bus bit 5 */
#define  FIFO_D6      PB6   /* fifo data bus bit 6 */
#define  FIFO_D7      PB7   /* fifo data bus bit 7 */

/* PA11 :ACTLED */
#define  BD1405_ACTLED     PA11   /* output */
#define  ACTLED_PORT   GPIOA
#define  ACTLED_PIN    GPIO_Pin_11
#define  ACTLED_IS_0   ACTLED_PORT->BSRR = (ACTLED_PIN << 16)
#define  ACTLED_IS_1   ACTLED_PORT->BSRR = (ACTLED_PIN)

/* PC10 :INT2HOST */
#define  INT2HOST_PORT  GPIOC
#define  INT2HOST_PIN   GPIO_Pin_10
#define  INT2HOST_IS_0  INT2HOST_PORT->BSRR = (INT2HOST_PIN << 16)
#define  INT2HOST_IS_1  INT2HOST_PORT->BSRR = (INT2HOST_PIN)

/* PD2 :FIFOWR0 */
#define  FIFOWR0_PORT  GPIOD
#define  FIFOWR0_PIN   GPIO_Pin_2
#define  FIFOWR0_IS_0  FIFOWR0_PORT->BSRR = (FIFOWR0_PIN << 16)
#define  FIFOWR0_IS_1  FIFOWR0_PORT->BSRR = (FIFOWR0_PIN)

/* PC12 :FIFOWR1 */
#define  FIFOWR1_PORT  GPIOC
#define  FIFOWR1_PIN   GPIO_Pin_12
#define  FIFOWR1_IS_0  FIFOWR1_PORT->BSRR = (FIFOWR1_PIN << 16)
#define  FIFOWR1_IS_1  FIFOWR1_PORT->BSRR = (FIFOWR1_PIN)

/*  PC11 : convert timing for ADC. */
#define  CONVERT_PORT  GPIOC
#define  CONVERT_PIN   GPIO_Pin_11
#define  CONVERT_IS    (CONVERT_PORT->IDR & CONVERT_PIN)

/* DATA BUS */
#define  BUS_PORT  GPIOB
#define  BUS_PIN   (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)

/* ID SW */
#define  IDSW_PORT  GPIOC
#define  IDSW_0     (IDSW_PORT->IDR & GPIO_Pin_13)
#define  IDSW_1     (IDSW_PORT->IDR & GPIO_Pin_14)
#define  IDSW_2     (IDSW_PORT->IDR & GPIO_Pin_15)
#define  IDSW       (((IDSW_PORT->IDR & (GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)) >> 13) & 0x0007)

/* ANOTHER */
#define  EXTI_TO_INT_NUMBER(n)  (n)
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
#define  MAX_TASK_NUMBER  1
enum GR_SAKURA_TSK_ID
{
  ID_stackMonitor,
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
