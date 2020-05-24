/* ----------------------------------------
 not preempt multi task service call routines.

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

#ifndef _MULTITASK_H_
#define _MULTITASK_H_

#include "derivertive.h"

/* ----------------------------------------
 defines service call
---------------------------------------- */
typedef  int            ID;
typedef  int            ER;
typedef  unsigned char  STAT;
typedef  unsigned long long  SYSTIM;
//typedef  unsigned long  SYSTIM;
typedef  unsigned long  RELTIM;
typedef  unsigned long  TMO;
typedef  unsigned int   UINT;

typedef  long           VP_INT;  /* VP or INT */
typedef struct
{
  void *sp;
  VP_INT exinf[4];
  unsigned char rdy_flg;
} TASK_CONTROL_BLOCK;

enum TSK_ID {TASK1,TASK2,TASK3,TASK4,TASK5,TASK6,TASK7,TASK8,TASK9,TASK10,};

/* ----------------------------------------
 manege semapfore
---------------------------------------- */
typedef struct
{
  unsigned int sigCount;
  unsigned int waiCount;
} SEM_OBJECT;

typedef struct t_rsem
{
  ID    wtskid;  // First task ID of semaphore queue
  UINT  semcnt;  // Current number of semaphore resources
} T_RSEM;

typedef  struct t_rtst{ STAT tskstat; STAT tskwait; } T_RTST;

/* ----------------------------------------
 constants and macros
---------------------------------------- */
#define  E_OK        0
#define  TMO_POL     0
#define  TMO_FEVR    (-1)
#define  TMO_NBLK    (-2)

#define  TTS_RUN     0x01
#define  TTS_RDY     0x02
#define  TTS_WAI     0x04
#define  TTS_SUS     0x08
#define  TTS_WAS     0x0C
#define  TTS_DMT     0x10

#define  TSK_SELF    0
#define  TSK_NONE    0

/****************************************************************************/
/* エラーコード                                                             */
/****************************************************************************/
#define  E_SYS    (-5)
#define  E_NOSPT  (-9)
#define  E_RSFN   (-10)
#define  E_RSATR  (-11)
#define  E_PAR    (-17)
#define  E_ID     (-18)
#define  E_CTX    (-25)
#define  E_MACV   (-26)
#define  E_OACV   (-27)
#define  E_ILUSE  (-28)
#define  E_NOMEM  (-33)
#define  E_NOID   (-34)
#define  E_OBJ    (-41)
#define  E_NOEXS  (-42)
#define  E_QOVR   (-43)
#define  E_RLWAI  (-49)
#define  E_TMOUT  (-50)
#define  E_DLT    (-51)
#define  E_CLS    (-52)
#define  E_WBLK   (-57)
#define  E_BOVR   (-58)

/* ----------------------------------------
 prototype
---------------------------------------- */
//ER   reg_tsk( ID tid, void (*task)(void), void *stk, unsigned int sz );
ER reg_tsk( ID tid, void *tsk, void *stk, int stk_sz, VP_INT exinf1, VP_INT exinf2, VP_INT exinf3, VP_INT exinf4 );
ER   sta_tsk( ID tid );
void ext_tsk( void );
ER   sus_tsk( ID tid );
ER   rsm_tsk( ID tid );
ER   dly_tsk( RELTIM dly );
ER   rot_rdq( void );
ER   sta_rdq( ID tid );
ER   ref_tst( ID tid, T_RTST *pk_rtst );
ER   rel_wai( ID tid );
ER   get_par( VP_INT *exinf, int size );
ER   dis_dsp( void );
ER   ena_dsp( void );
ER   isig_sem( ID semid );
ER   sig_sem( ID semid );
ER   wai_sem( ID semid );
ER   pol_sem( ID semid );
ER   ref_sem( ID semid, T_RSEM *pk_rsem );

#if   defined(RLDUINO78_H)
  #define  loc_cpu  noInterrupts
  #define  unl_cpu  interrupts
#elif defined (__GNUC__)
  //#define  loc_cpu  __disable_irq
  #define  loc_cpu()  __asm__("cpsid i\n\t")
  //#define  unl_cpu  __enable_irq
  #define  unl_cpu()  __asm__("cpsie i\n\t")
#endif

/* ----------------------------------------
 disatcher prototype
---------------------------------------- */
void sta_ctx( void *exe );  // load context and task start.
void swi_ctx( void *pre, void *post);  // switch context.

#endif  /*_MULTITASK_H_*/
