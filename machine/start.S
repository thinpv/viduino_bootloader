
USR_MODE		=	0x10
FIQ_MODE		=	0x11
IRQ_MODE		=	0x12
SVC_MODE		=	0x13
ABT_MODE		=	0x17
UDF_MODE		=	0x1B
SYS_MODE		=	0x1F
       
I_BIT				=	0x80        /* Disables IRQ when I bit is set               */
F_BIT				=	0x40        /* Disables FIQ when F bit is set               */

RAM_Limit				=	0x8000            // For unexpanded hardware board

USR_Stack				= RAM_Limit
SVC_Stack				= (USR_Stack-1024*1)      /* SVC stack at top of memory */
FIQ_Stack				= (SVC_Stack-1024*1)      /* followed by IRQ stack */
IRQ_Stack				= (FIQ_Stack-1024*1)
Abort_Stack			= (IRQ_Stack-1024*1)
UND_Stack				= (Abort_Stack-1024*1)

.text
	.arm

	.global	_start
_start:
	/* Boot head information for BROM */
	.long 0xea000016
	.byte 'e', 'G', 'O', 'N', '.', 'B', 'T', '0'
	.long 0, __bootloader_size
	.byte 'S', 'P', 'L', 2
	.long 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0	/* 0x40 - boot params, 0x58 - fel boot type, 0x5c - dram size */

_vector:
	b reset
	ldr pc, _undefined_instruction
	ldr pc, _software_interrupt
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	ldr pc, _not_used
	ldr pc, _irq
	ldr pc, _fiq

_undefined_instruction:
	.word undefined_instruction
_software_interrupt:
	.word software_interrupt
_prefetch_abort:
	.word prefetch_abort
_data_abort:
	.word data_abort
_not_used:
	.word not_used
_irq:
	.word irq
_fiq:
	.word fiq

/*
 * The actual reset code
 */
reset:
	/* --------------------------------
	Initial Stack Pointer register
	--------------------------------
	INIT_STACK */
	MSR    CPSR_c, #UDF_MODE | I_BIT | F_BIT
	LDR    SP, =UND_Stack

	MSR    CPSR_c, #ABT_MODE | I_BIT | F_BIT
	LDR    SP, =Abort_Stack

	MSR    CPSR_c, #IRQ_MODE | I_BIT | F_BIT
	LDR    SP, =IRQ_Stack

	MSR    CPSR_c, #FIQ_MODE | I_BIT | F_BIT
	LDR    SP, =FIQ_Stack

	MSR    CPSR_c, #SYS_MODE | I_BIT | F_BIT
	LDR    SP, =USR_Stack

	MSR    CPSR_c, #SVC_MODE | I_BIT | F_BIT
	LDR    SP, =SVC_Stack

	/* ------------------------------------------------------
	Set the normal exception vector of CP15 control bit
	------------------------------------------------------ */
	MRC p15, 0, r0 , c1, c0
	bic r0, #(1<<13)
	MCR p15, 0, r0 , c1, c0

	ldr r0, = _vector
	mrc p15, 0, r2, c1, c0, 0
	ands r2, r2, #(1 << 13)
	ldreq r1, =0x00000000
	ldrne r1, =0xFFFF0000
	
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}
	
	/* Initial system clock, ddr add uart */
	/* bl sys_clock_init
	bl sys_dram_init
	bl sys_print_init
	bl sys_copyself */


	/* Boot speed up, leave slower sram */
	adr r0, _start
	ldr r1, =_start
	cmp r0, r1
	beq _speedup
	ldr r0, =0x81f80000
	adr r1, _start
	ldr r2, =__bootloader_size
	bl memcpy
	ldr r0, =_speedup
	ldr r1, =_start
	sub r0, r0, r1
	ldr r1, =0x81f80000
	add r0, r0, r1
	mov pc, r0
_speedup:
	nop 

	/* Clear bss section */
	ldr r0, _bss_start
	ldr r2, _bss_end
	sub r2, r2, r0
	mov r1, #0
	bl memset

	B     boot_main

	.global jump_to_app
jump_to_app:
  mov     lr, r0
	bx      lr
/*
 * Exception handlers
 */
	.align 5
undefined_instruction:
	b .

	.align 5
software_interrupt:
	b .

	.align 5
prefetch_abort:
	b .

	.align 5
data_abort:
	b .

	.align 5
not_used:
	b .

	.align 5
irq:
	ldr SP,=IRQ_Stack 
	sub sp, sp, #72
	stmia sp, {r0 - r12}
	add r8, sp, #60
	stmdb r8, {sp, lr}^
	nop
	str lr, [r8, #0]
	mrs r6, spsr
	str r6, [r8, #4]
	str r0, [r8, #8]
	mov r0, sp
	bl arm32_do_irq
	ldmia sp, {r0 - lr}^
	mov r0, r0
	ldr lr, [sp, #60]
	add sp, sp, #72
	subs pc, lr, #4

	.align 5
fiq:
	ldr SP,=FIQ_Stack 
	sub sp, sp, #72
	stmia sp, {r0 - r12}
	add r8, sp, #60
	stmdb r8, {sp, lr}^
	nop
	str lr, [r8, #0]
	mrs r6, spsr
	str r6, [r8, #4]
	str r0, [r8, #8]
	mov r0, sp
	bl arm32_do_fiq
	ldmia sp, {r0 - lr}^
	mov r0, r0
	ldr lr, [sp, #60]
	add sp, sp, #72
	subs pc, lr, #4


_bss_start:
	.long __bss_start
_bss_end:
	.long __bss_end

	.end