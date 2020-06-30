.text 
.global _start
_start:

b   reset
ldr pc, _undefined_instruction
ldr pc, _software_interrupt
ldr pc, _prefetch_abort
ldr pc, _date_abort
ldr pc, _not_used
ldr pc, _IRQ
ldr pc, _FIQ


_undefined_instruction :.word undefined_instruction
_software_interrupt :.word software_interrupt
_prefetch_abort :.word prefetch_abort
_date_abort :.word date_abort
_not_used :.word not_used
_IRQ :.word IRQ
_FIQ :.word FIQ


undefined_instruction:
	nop	
software_interrupt:
	nop
prefetch_abort:
	nop
date_abort:
	nop
not_used:
	nop
IRQ:
	nop
FIQ:
	nop
reset:
	bl set_svc
	bl disable_watchdog
	bl disable_interrupt
	bl disable_mmu
	bl clock_init
	bl init_sdram
	bl copy_to_ram
	bl init_stack
	bl clean_bss
	ldr pc, =gboot_main
@	bl light_led
	
set_svc:
	mrs r0, cpsr
	bic r0, r0,#0x1f
	orr r0, r0,#0xd3
	msr cpsr, r0
	mov pc, lr
	
#define pWTCON 0x53000000
disable_watchdog:
	ldr r0, =pWTCON	
	mov r1, #0x0
	str r1, [r0]
	mov pc, lr
	
disable_interrupt:
	mvn r1, #0x0
	ldr r0, =0x4a000008
	str r1, [r0]
	mov pc, lr
	
disable_mmu:
	mcr p15,0,r0,c7,c7,0	
	mrc p15,0,r0,c1,c0,0
	bic r0, r0,#0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc, lr

#define CLKDIVN 0x4C000014
#define MPLLCON 0x4C000004
#define MPLL_405MHZ ((127<<12)|(2<<4)|(1<<0)) 
clock_init:
	ldr r0, =CLKDIVN
	mov r1, #0x5
	str r1, [r0]
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	ldr r0, =MPLLCON
	ldr r1, =MPLL_405MHZ
	str r1, [r0]
	mov pc,lr

#define mem_contrl 0x48000000
init_sdram:
	ldr  r0, = mem_contrl
	add  r3, r0, #4*13
   	adrl r1, mem_data
0:
	ldr r2, [r1],#4
	str r2, [r0],#4 
	cmp r0, r3
	bne 0b
	mov pc, lr

mem_data:
  .long	0x22000000
  .long	0x00000700
  .long	0x00000700
  .long	0x00000700
  .long	0x00000700
  .long	0x00000700
  .long	0x00000700
  .long	0x00018001
  .long	0x00018001
  .long	0x008c04f5
  .long	0x000000b1
  .long	0x00000030
  .long	0x00000030

copy_to_ram:
	ldr r0, =0x0
	ldr r1, =0x30008000
	add r3,r0,#1024*4
copy_loop:
	ldr r2, [r0],#4
	str r2, [r1],#4
	cmp r0, r3
	bne copy_loop
	mov pc, lr
	
init_stack:
	ldr sp, =0x34000000
 	mov pc, lr
 	
clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	cmp r0, r1
	moveq pc, lr
loop_clean:
	mov r2, #0x0
	str r2,[r0],#4
	cmp r0, r1
	bne loop_clean
	mov pc, lr
	
	
	

	
	
