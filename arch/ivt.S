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
_swi_handler: .word swi_handler
_prefetch_abort_handler: .word prefetch_abort_handler
_data_abort_handler: .word data_abort_handler
_reserved: .word 0x0
_irq_handler: .word irq_handler
_fiq_handler: .word fiq_handler
