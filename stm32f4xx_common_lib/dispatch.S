  .cpu cortex-m4
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
 
