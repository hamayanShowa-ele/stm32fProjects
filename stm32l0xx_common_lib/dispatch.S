/* ----------------------------------------
 not pre-empt multi task system dispatcher for cortex M3.

  Copyright (c) 2020 hamayan (hamayan.contact@gmail.com).
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

  Created 2020 by hamayan (hamayan.contact@gmail.com)
---------------------------------------- */

  .syntax unified
//  .cpu cortex-m0
  .cpu cortex-m0plus
  .fpu softvfp
  .thumb

  .text

  .thumb_func
  .global swi_ctx /* Switch execution context */
  .global sta_ctx /* Transition the task to READY. */
  .type	swi_ctx,%function	@LSB=1
  .type	sta_ctx,%function	@LSB=1

/*
  void sta_ctx( void *exe )
  Transition the task to READY.
  The argument exe(R0) is the destination of the execution context.
*/
sta_ctx:
  // push {lr}
  PUSH {LR}

  // ldr r13,[r0]
  LDR  R2,[R0]  // ldr r13,[r0]
  MOV  R13,R2  // ldr r13,[r0]

//  pop {r4-r12,pc}
  POP  {R4,R5,R6,R7}
  POP  {R2}
  MOV  R8,R2  // pop r8
  POP  {R2}
  MOV  R9,R2  // pop r9
  POP  {R2}
  MOV  R10,R2  // pop r10
  POP  {R2}
  MOV  R11,R2  // pop r11
  POP  {R2}
  MOV  R12,R2  // pop r12
  POP  {PC}  // pop pc

/*
  void swi_ctx( void *pre, void *post)
  Switch execution context.
  The argument pre(R0) is the destination of the current context.
  The argument post(R1) is the source of the context to switch to.
*/
swi_ctx:
  // push {r4-r12,lr}
  PUSH {LR}  // push lr
  MOV  R2,R12  // push r12
  PUSH {R2}
  MOV  R2,R11  // push r11
  PUSH {R2}
  MOV  R2,R10  // push r10
  PUSH {R2}
  MOV  R2,R9  // push r9
  PUSH {R2}
  MOV  R2,R8  // push r8
  PUSH {R2}  // push r8
  PUSH  {R7,R6,R5,R4}

  //  str r13,[r0]
  MOV  R2,R13  // ldr r13,[r0]
  STR  R2,[R0]  // str r13,[r0]

  //  ldr r13,[r1]
  LDR  R2,[R1]  // ldr r13,[r1]
  MOV  R13,R2  // ldr r13,[r0]

  //  pop {r4-r12,pc}
  POP  {R4,R5,R6,R7}
  POP  {R2}  // pop r8
  MOV  R8,R2  // pop r8
  POP  {R2}  //
  MOV  R9,R2  // pop r9
  POP  {R2}  //
  MOV  R10,R2  // pop r10
  POP  {R2}  //
  MOV  R11,R2  // pop r11
  POP  {R2}  //
  MOV  R12,R2  // pop r12
  POP  {PC}  // pop pc

  .end
 
