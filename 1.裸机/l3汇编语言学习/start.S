.data
hello:
.ascii "helloworld"
.align 4
bh:
.byte 0x1
.align 4
ADD:
.word 0xff
.text
.global _start
_start:
@伪指令
nop
.equ DA, 0X55
mov r1, #DA
mov r2, #0xff
ldr r2, =0xfff
@b分支
	mov r0, #1
	mov r1, #2
	cmp r0, r1
	bgt branch1
	add r3, r1, r0
	b end
	branch1:
	sub r3, r1, r0
	end:
	nop
@bl分支
   mov r0, #1
   mov r1, #2
   bl func1
   func1:
   add r3, r0, r1
   mov pc, lr
@cmp比较，不储存结果，但是会影响cpsr寄存器 <0N置1 =0Z置1
    mov r0, #0x2
    mov r1, #0x5
    cmp r0, r1
@tst按位与，如果结果为0，Z置1
	mov r1, r1, lsr #2
	tst r0, r1
@lsl逻辑左移,lsr逻辑右移
	mov r0, #0b1
	mov r1, #2
	mov r0, r0, lsl #1
	mov r0, r0, lsr #1
	orr r0, r0, r1
@ror循环右移
	mov r0, r0, ror  r1

@mvn取反赋值
	mvn r1, #5
	mvn r2, #0b10
    mvn r3, #0x8
@orr逻辑或
	mov r1, #0b1
	mvn r2, r2
	orr r1, r1, r2
@sub减法
	mov r1, #5
	mov r2, #4
	sub r1, r1, r2

















	mov r1,#1
	mov r2,#2
	mov r3,#3
