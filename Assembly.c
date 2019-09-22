And_Gate():
  mov x0, 16388
  movk x0, 0x4000, lsl 16
  mov x1, 20484
  movk x1, 0x4002, lsl 16
  ldr x2, [x1]
  mov x1, 20544
  movk x1, 0x4002, lsl 16
  ldr x1, [x1]
  and x1, x2, x1
  str x1, [x0]
  nop
  ret
Or_Gate():
  mov x0, 16392
  movk x0, 0x4000, lsl 16
  mov x1, 20484
  movk x1, 0x4002, lsl 16
  ldr x2, [x1]
  mov x1, 20544
  movk x1, 0x4002, lsl 16
  ldr x1, [x1]
  orr x1, x2, x1
  str x1, [x0]
  nop
  ret
Xor_Gate():
  mov x0, 16400
  movk x0, 0x4000, lsl 16
  mov x1, 20484
  movk x1, 0x4002, lsl 16
  ldr x2, [x1]
  mov x1, 20544
  movk x1, 0x4002, lsl 16
  ldr x1, [x1]
  eor x1, x2, x1
  str x1, [x0]
  nop
  ret
main:
  stp x29, x30, [sp, -16]!
  add x29, sp, 0
  mov x0, 57608
  movk x0, 0x400f, lsl 16
  mov x1, 57608
  movk x1, 0x400f, lsl 16
  ldr x2, [x1]
  mov x1, 33
  orr x1, x2, x1
  str x1, [x0]
  mov x0, 21504
  movk x0, 0x4002, lsl 16
  str xzr, [x0]
  mov x0, 17408
  movk x0, 0x4000, lsl 16
  mov x1, 17408
  movk x1, 0x4000, lsl 16
  ldr x1, [x1]
  orr x1, x1, 7
  str x1, [x0]
  mov x0, 17440
  movk x0, 0x4000, lsl 16
  str xzr, [x0]
  mov x0, 21536
  movk x0, 0x4002, lsl 16
  str xzr, [x0]
  mov x0, 17696
  movk x0, 0x4000, lsl 16
  mov x1, 17227
  movk x1, 0x4c4f, lsl 16
  str x1, [x0]
  mov x0, 21792
  movk x0, 0x4002, lsl 16
  mov x1, 17227
  movk x1, 0x4c4f, lsl 16
  str x1, [x0]
  mov x0, 17700
  movk x0, 0x4000, lsl 16
  mov x1, 17700
  movk x1, 0x4000, lsl 16
  ldr x1, [x1]
  orr x1, x1, 7
  str x1, [x0]
  mov x0, 21796
  movk x0, 0x4002, lsl 16
  mov x1, 21796
  movk x1, 0x4002, lsl 16
  ldr x2, [x1]
  mov x1, 17
  orr x1, x2, x1
  str x1, [x0]
  mov x0, 17692
  movk x0, 0x4000, lsl 16
  mov x1, 17692
  movk x1, 0x4000, lsl 16
  ldr x1, [x1]
  orr x1, x1, 7
  str x1, [x0]
  mov x0, 21788
  movk x0, 0x4002, lsl 16
  mov x1, 21788
  movk x1, 0x4002, lsl 16
  ldr x2, [x1]
  mov x1, 17
  orr x1, x2, x1
  str x1, [x0]
  mov x0, 17704
  movk x0, 0x4000, lsl 16
  str xzr, [x0]
  mov x0, 21800
  movk x0, 0x4002, lsl 16
  str xzr, [x0]
  mov x0, 21776
  movk x0, 0x4002, lsl 16
  mov x1, 17
  str x1, [x0]
.L5:
  bl And_Gate()
  bl Or_Gate()
  bl Xor_Gate()
  b .L5