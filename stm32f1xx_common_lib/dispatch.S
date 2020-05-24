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


/*  .cpu cortex-m4*/
  .cpu cortex-m3
  .syntax unified
  .thumb
  .text

  .thumb_func
  .global swi_ctx /*実行コンテキストの切替*/
  .global sta_ctx /*コンテキストをロードし、タスクを開始*/
  .type	swi_ctx,%function	@LSB=1
  .type	sta_ctx,%function	@LSB=1

/************************************************
 コンテキストをロードし、タスクを開始
 void sta_ctx(
 void *exe ) //実行コンテキストの保存先
************************************************/
sta_ctx:
  push {lr} /*lrを退避*/
  ldr r13,[r0] /*スタックポインタ設定*/
  pop {r4-r12,pc} /*r4-r12,pcを復帰*/

/************************************************
 実行コンテキストの切替
 void swi_ctx(
 void *pre, //現在のコンテキストの保存先
 void *post) //切り替えるコンテキスト
************************************************/
swi_ctx:
  push {r4-r12,lr} /*r4-r12,lrを退避。r4が最も若いアドレスになる*/
  str r13,[r0] /*スタックポインタ保存*/
  ldr r13,[r1] /*スタックポインタ復帰*/
  pop {r4-r12,pc} /*r4-r12,pcを復帰*/

  .end
 
