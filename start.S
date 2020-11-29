.equ ARM_MODE_USR, 0x10
.equ ARM_MODE_FIQ, 0x11
.equ ARM_MODE_IRQ, 0x12
.equ ARM_MODE_SVC, 0x13
.equ ARM_MODE_ABT, 0x17
.equ ARM_MODE_UND, 0x1B
.equ ARM_MODE_SYS, 0x1F

.equ STACK_LEN_KERNEL, 0x4000
.equ STACK_LEN_EXCEPTION, 0x400

.section .init
.global _start
_start:
    ldr r0, _stack_base
    mrs r1, CPSR
    bic r1, r1, #ARM_MODE_SYS

    orr r2, r1, #ARM_MODE_FIQ
    msr CPSR, r2
    mov sp, r0
    sub r0, r0, #STACK_LEN_EXCEPTION

    orr r2, r1, #ARM_MODE_IRQ
    msr CPSR, r2
    mov sp, r0
    sub r0, r0, #STACK_LEN_EXCEPTION

    orr r2, r1, #ARM_MODE_SVC
    msr CPSR, r2
    mov sp, r0
    sub r0, r0, #STACK_LEN_EXCEPTION

    orr r2, r1, #ARM_MODE_ABT
    msr CPSR, r2
    mov sp, r0
    sub r0, r0, #STACK_LEN_EXCEPTION

    orr r2, r1, #ARM_MODE_UND
    msr CPSR, r2
    mov sp, r0
    sub r0, r0, #STACK_LEN_EXCEPTION

    orr r2, r1, #ARM_MODE_SYS
    msr CPSR, r2
    mov sp, r0
    bl kernel

_stack_base: .word 0x23FFFFFF
