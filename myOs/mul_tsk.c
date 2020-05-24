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

#include "mul_tsk.h"

/* ----------------------------------------
 global variables
---------------------------------------- */
extern SYSTIM systim;  // system timer was increment by 1 at 1 milli second.
ID cur_tid;  // current task ID
static unsigned char disable_dispatch;  // Dispatch prohibited except 0
static unsigned char startTaskContext;  // 0 means OS has not started

/* ----------------------------------------
 task control block
---------------------------------------- */
TASK_CONTROL_BLOCK tcb[MAX_TASK_NUMBER];   // Area for saving environment variables (context) of tasks

/* ----------------------------------------
 regist task
  You can register task IDs, entry points, stack areas, stack sizes, and startup arguments.
  Startup arguments are up to four variables.
---------------------------------------- */
ER reg_tsk( ID tid, void *tsk, void *stk, int stk_sz,
  VP_INT exinf1, VP_INT exinf2, VP_INT exinf3, VP_INT exinf4 )
{
  VP_INT *ptr;

  if( tid < 0 || tid >= MAX_TASK_NUMBER ) return E_ID;

  // Initialize task stack
  tcb[tid].sp = (void *)((char *)stk + stk_sz - (4 * (9 + 2)));  // Register r6 to register r14 are stored in this area
  ptr = (VP_INT *)tcb[tid].sp;
  ptr[9] = (VP_INT)tsk;  /*pc*/
  //*((unsigned long *)(stk + stk_sz - (4 * 2))) = (unsigned long)tsk;
  tcb[tid].exinf[0] = exinf1;
  tcb[tid].exinf[1] = exinf2;
  tcb[tid].exinf[2] = exinf3;
  tcb[tid].exinf[3] = exinf4;

  tcb[tid].rdy_flg = TTS_DMT;

  return E_OK;
}

/* ----------------------------------------
 start task (Actually register to scheduler)
---------------------------------------- */
ER sta_tsk( ID tid )
{
  if( tid < 0 || tid >= MAX_TASK_NUMBER ) return E_ID;

  tcb[tid].rdy_flg = TTS_RDY;  // Put a task in a ready state

  return E_OK;
}

/* ----------------------------------------
 Round robin start
---------------------------------------- */
ER sta_rdq( ID tid )
{
  if( tid < 0 || tid >= MAX_TASK_NUMBER ) return E_ID;
  if( tcb[tid].rdy_flg != TTS_RDY ) return E_CTX;
  startTaskContext = 1;

  cur_tid = tid;
  sta_ctx( (void *)&tcb[cur_tid].sp );

  return E_OK;
}

/* ----------------------------------------
 Rotate Round Que
---------------------------------------- */
ER rot_rdq( void )
{
  int i;
  volatile ID tid = cur_tid;
  volatile ID old_tid = cur_tid;

  if( startTaskContext == 0 || disable_dispatch != 0 ) return E_OK;

  for( i = 0; i < MAX_TASK_NUMBER; i++ )
  {
    if( ++tid >= MAX_TASK_NUMBER ) tid = 0;  // Switch to next task
    if( tcb[tid].rdy_flg & TTS_SUS ) continue;
    loc_cpu();  // Temporarily disable interrupts
    if( tcb[tid].rdy_flg & (TTS_RDY | TTS_WAI) )
    {
      cur_tid = tid;  // Saving task ID after context switch
      swi_ctx( &tcb[old_tid].sp, &tcb[tid].sp );
      unl_cpu();  // Resume interrupt
      return E_OK;
    }
    unl_cpu();  // Resume interrupt
  }

  return E_PAR;
}

/* ----------------------------------------
 Rotate Round Que in interrupt
---------------------------------------- */
void irot_rdq( void )
{
  int i;
  ID tid = cur_tid;
  ID old_tid = cur_tid;

  if( startTaskContext == 0 || disable_dispatch != 0 ) return;

  for( i = 0; i < MAX_TASK_NUMBER; i++ )
  {
    if( ++tid >= MAX_TASK_NUMBER ) tid = 0;  // Switch to next task
    if( tcb[tid].rdy_flg & TTS_SUS ) continue;
    if( tcb[tid].rdy_flg & (TTS_RDY | TTS_WAI) )
    {
      cur_tid = tid;  // Saving task ID after context switch
      swi_ctx( &tcb[old_tid].sp, &tcb[tid].sp );
      return;
    }
  }
}


/* ----------------------------------------
 Delay processing
  Continue task switch until time comes
---------------------------------------- */
ER dly_tsk( RELTIM dly )
{
  RELTIM lastTim = systim;
  ID tid = cur_tid;

  if( dly == 0UL )
  {
    rot_rdq();
    return E_OK;
  }
  if( startTaskContext == 0 )
  {
    while( (RELTIM)(systim - lastTim) < dly ) {}  // Waiting for time to elapse
    return E_OK;
  }

  tcb[tid].rdy_flg = TTS_WAI;  // Transition to wait state
  for( ;; )
  {
    if( (systim - lastTim) >= dly )  // Waiting for time to elapse
    {
      tcb[tid].rdy_flg &= ~TTS_WAI;  // Release waiting state
      tcb[tid].rdy_flg |= TTS_RDY;  // Transition to READY
      return E_OK; 
    }
    (void)rot_rdq();  // Round robin execution
    if( !(tcb[tid].rdy_flg & TTS_WAI) ) break;  // Loop ends when wait state is released
  }

  tcb[tid].rdy_flg |= TTS_RDY;
  return E_RLWAI;
}

/* ----------------------------------------
 Forced release of waiting state
  Target is applicable task
---------------------------------------- */
ER rel_wai( ID tid )
{
  if( tid < 0 || tid >= MAX_TASK_NUMBER ) return E_ID;     // Incorrect ID
  if( tcb[tid].rdy_flg == 0 )             return E_NOEXS;  // Object not created
  if( !(tcb[tid].rdy_flg & TTS_WAI) )     return E_OBJ;    // Target is not waiting

  tcb[tid].rdy_flg &= ~TTS_WAI;  // Release waiting state

  return E_OK;
}

/* ----------------------------------------
 Get task status (Abridged edition)
---------------------------------------- */
ER ref_tst( ID tid, T_RTST *pk_rtst )
{
  if( tid < 0 || tid >= MAX_TASK_NUMBER ) return E_NOEXS;

  if(tid == TSK_SELF)
  {
    pk_rtst->tskstat = tcb[cur_tid].rdy_flg;
  }
  else
  {
    pk_rtst->tskstat = tcb[tid].rdy_flg;
  }

  return E_OK;
}

/* ----------------------------------------
 Transition to forced task wait state
---------------------------------------- */
ER sus_tsk( ID tid )
{
  if( tid < 0 || tid >= MAX_TASK_NUMBER ) return E_ID;
  if( tcb[tid].rdy_flg == 0 ) return E_NOEXS;  // Task disabled

  tcb[tid].rdy_flg |= TTS_SUS;  // Task disabled

  if( tid == cur_tid ) rot_rdq();  // round QUE

  return E_OK;
}

/* ----------------------------------------
 Canceling the forced wait state
  Target is applicable task
---------------------------------------- */
ER rsm_tsk( ID tid )
{
  if( tid < 0 || tid >= MAX_TASK_NUMBER ) return E_ID;     // Incorrect ID
  if( tcb[tid].rdy_flg == 0 )             return E_NOEXS;  // Object not created
  if( !(tcb[tid].rdy_flg & (TTS_WAI | TTS_SUS)) ) return E_OBJ; // Target is not waiting

  tcb[tid].rdy_flg &= ~TTS_SUS;  // Release waiting state

  rot_rdq();  // round QUE

  return E_OK;
}

/* ----------------------------------------
 Terminate own task
  Move the status of the scheduler table to DORMANT
---------------------------------------- */
void ext_tsk( void )
{
  tcb[cur_tid].rdy_flg = TTS_DMT;  // Make the task DORMANT

  for( ;; )  // Note that if some task is not RDY, it will lock here
  {
    (void)rot_rdq();
  }
}

/* ----------------------------------------
 Receiving startup arguments
  exinf is the receiving destination, size is the number of receiving, up to 4
---------------------------------------- */
ER get_par( VP_INT *exinf, int size )
{
  int i;

  if( size <= 0 || size > 4 ) return E_PAR;

  for( i = 0; i < size; i++ )
  {
    *exinf++ = tcb[cur_tid].exinf[i];
  }
  return E_OK;
}

/* ----------------------------------------
 Dispatch prohibited
---------------------------------------- */
ER dis_dsp( void )
{
  loc_cpu();
  disable_dispatch = 1;  // Dispatching is prohibited when it is not 0
  unl_cpu();
  return E_OK;
}

/* ----------------------------------------
 Dispatch permission
---------------------------------------- */
ER ena_dsp( void )
{
  loc_cpu();
  disable_dispatch = 0;  // Dispatching is prohibited when it is not 0
  unl_cpu();
  return E_OK;
}

extern volatile SEM_OBJECT sem_obj[];
/* ----------------------------------------
 Waiting for semaphore
  After obtaining the current waiCount, count up waiCount
  Wait until the current sigCount matches the acquired waiCount
  It's not preemptive, so loc_cpu doesn't need to be unnecessary
---------------------------------------- */
ER wai_sem( ID semid )
{
  if( semid <= 0 ) return E_ID;
  ID tid = cur_tid;

  tcb[tid].rdy_flg = TTS_WAI;  // Transition to wait state
  for( ;; )
  {
    if( sem_obj[ semid - 1 ].waiCount == sem_obj[ semid - 1 ].sigCount )
    {
      tcb[tid].rdy_flg &= ~TTS_WAI;  // Release waiting state
      tcb[tid].rdy_flg |= TTS_RDY;  // Transition to READY
      loc_cpu();
      sem_obj[ semid - 1 ].waiCount++;
      unl_cpu();
      return E_OK;
    }
    (void)rot_rdq();  // Round robin execution
    if( !(tcb[tid].rdy_flg & TTS_WAI) ) break;  // Loop ends when wait state is released
  }

  return E_RLWAI;
}

/* ----------------------------------------
 Semaphore polling
  After obtaining the current waiCount, count up waiCount
  Wait until the current sigCount matches the acquired waiCount
  It's not preemptive, so loc_cpu doesn't need to be unnecessary
---------------------------------------- */
ER pol_sem( ID semid )
{
  if( semid <= 0 ) return E_ID;

  if( sem_obj[ semid - 1 ].waiCount == sem_obj[ semid - 1 ].sigCount )
  {
    loc_cpu();
    sem_obj[ semid - 1 ].waiCount++;
    unl_cpu();
    return E_OK;
  }

  return E_TMOUT;
}

/* ----------------------------------------
 Return semaphore
  Count up the current sigCount
  It's not preemptive, so loc_cpu doesn't need to be unnecessary
---------------------------------------- */
ER sig_sem( ID semid )
{
  if( semid <= 0 ) return E_ID;
  loc_cpu();
  sem_obj[ semid - 1 ].sigCount++;
  //sem_obj[ semid - 1 ].sigCount = sem_obj[ semid - 1 ].waiCount;
  unl_cpu();
  rot_rdq();

  return E_OK;
}

ER isig_sem( ID semid )
{
  if( semid <= 0 ) return E_ID;
  sem_obj[ semid - 1 ].sigCount++;

  return E_OK;
}

/* ----------------------------------------
 Refer to semaphore
---------------------------------------- */
ER ref_sem( ID semid, T_RSEM *pk_rsem )
{
  if( semid <= 0 ) return E_ID;
//  loc_cpu();
  pk_rsem->wtskid = cur_tid;
  pk_rsem->semcnt = sem_obj[ semid - 1 ].waiCount - sem_obj[ semid - 1 ].sigCount;
//  unl_cpu();
  return E_OK;
}

