.equ ARM_MODE_SYS, 0x1F
.equ ARM_MODE_IRQ, 0x12
.equ I_BIT, 0x80
.equ AIC_BASE, 0xFFFFF000

.macro IRQ_ENTRY
   # Adjust and save LR of current mode in current stack
   sub r14, r14, #4
   stmfd sp!, {r14}
   # Save SPSR and r0 in current stack
   mrs r14, SPSR
   stmfd sp!, {r0, r14}
   # Read Modify Write the CPSR to Enable the Core Interrupt
   # and Switch in SYS Mode ( same LR and stack than USR Mode )
   mrs r14, CPSR
   bic r14, r14, #I_BIT
   orr r14, r14, #ARM_MODE_SYS
   msr CPSR, r14
   # Save used registers and LR_usr in the System/User Stack
   stmfd sp!, {r0-r12,r14}
.endm

.macro IRQ_EXIT
   # Restore used registers and LR_usr from the System/User Stack
   ldmfd sp!, {r0-r12,r14}
   # Read Modify Write the CPSR to disable interrupts
   # and to go back in the mode corresponding to the exception
   mrs r0, CPSR
   bic r0, r0, #ARM_MODE_SYS
   orr r0, r0, #ARM_MODE_IRQ
   msr CPSR, r0
   # Mark the End of Interrupt on the interrupt controller
   ldr r0, #AIC_BASE
   str r0, [r0, #AIC_EOICR]
   # Restore SPSR_irq and r0 from the IRQ stack
   ldmfd sp!, {r0, r14}
   msr SPSR, r14
   # Restore ajusted LR_irq from IRQ stack directly in the PC
   ldmfd sp!, {pc}^
.endm
