#ifndef X86_PIC_H
#define X86_PIC_H

#include <stdint.h>

#include <arch/interrupt.h>

void pic_remap(interrupt_vector_t irq0, interrupt_vector_t irq8);
void pic_irq_mask(uint16_t irq_mask);
void pic_eoi_master();  // irq 0-7
void pic_eoi_slave();   // irq 8-15

#endif
