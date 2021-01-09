.global ivt
ivt:
    ldr pc,_reset_handler
    ldr pc,_undefined_handler
    ldr pc,_swi_handler
    ldr pc,_prefetch_abort_handler
    ldr pc,_data_abort_handler
    ldr pc,_reserved
    ldr pc,_irq_handler
    ldr pc,_fiq_handler

_reset_handler: .word _start
_undefined_handler: .word undef_handler
_swi_handler: .word swi_trampoline
_prefetch_abort_handler: .word prefetch_abort_handler
_data_abort_handler: .word data_abort_handler
_reserved: .word 0x0
_irq_handler: .word interrupt_trampoline
_fiq_handler: .word fiq_handler

interrupt_trampoline:
    stmfd sp!, {lr}
    stmfd sp, {r0-r14}^
    sub sp, sp, #(15*4)
    mov r0, sp
    bl irq_handler
    add sp, sp, #(15*4)
    ldr r14, [r0, #(15*4)]
    ldmfd r0, {r0-r14}^
    subs pc, r14, #4

swi_trampoline:
    stmfd sp!, {lr}
    stmfd sp, {r0-r14}^
    sub sp, sp, #(15*4)
    mov r0, sp
    bl swi_handler
    add sp, sp, #(15*4)
    ldr r14, [r0, #(15*4)]
    ldmfd r0, {r0-r14}^
    subs r14, r14, #4
    movs pc, r14
