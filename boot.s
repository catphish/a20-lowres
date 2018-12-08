// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.globl _start
 
_start:

ldr pc, _reset_h
ldr pc, _undefined_instruction_vector_h
ldr pc, _software_interrupt_vector_h
ldr pc, _prefetch_abort_vector_h
ldr pc, _data_abort_vector_h
ldr pc, _unused_handler_h
ldr pc, _interrupt_vector_h
ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   _reset_
_software_interrupt_vector_h:       .word   _reset_
_prefetch_abort_vector_h:           .word   _reset_
_data_abort_vector_h:               .word   _reset_
_unused_handler_h:                  .word   _reset_
_interrupt_vector_h:                .word   interrupt
_fast_interrupt_vector_h:           .word   _reset_

_reset_:

	mov     r0, #0x8000
	mov     r1, #0x0000
	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

	// Setup the stack.
	mov sp, #0x4000

	// Clear out bss.
	ldr r4, =__bss_start
	ldr r9, =__bss_end
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	b       2f
 
1:
	// store multiple at r4.
	stmia r4!, {r5-r8}
 
	// If we are still below bss_end, loop.
2:
	cmp r4, r9
	blo 1b
 
	// Call kernel_main
	ldr r3, =kernel_main
	blx r3
 
	// halt
halt:
	wfe
	b halt

