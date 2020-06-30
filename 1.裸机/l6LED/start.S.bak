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
	
set_svc:
	mrs r0, cpsr
	bic r0, r0,#0x1f
	orr r0, r0,#0xd3
	msr cpsr, r0
	mov pc, lr
	
#define pWTCON 0X53000000
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
	

	
	
