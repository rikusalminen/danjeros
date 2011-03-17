#include <stdint.h>

#include <arch/interrupt.h>

#include <arch/x86/pic.h>
#include <arch/x86/ioport.h>

void interrupt_handler(
    interrupt_vector_t vector,
    uint64_t error_code,
    registers_t *registers,
    interrupt_stack_frame_t *interrupt_stack_frame
    )
{
    (void)vector;
    (void)error_code;
    (void)registers;
    (void)interrupt_stack_frame;
    // this is an excellent place to put debugger breakpoints

    if(vector == INT_IRQ1) inb(0x60); // read data from keyboard controller, for it to fire more than once

    // send end-of-interrupt (EOI) to PIC
    if(vector >= INT_IRQ0 && vector <= INT_IRQ7) pic_eoi_master();
    else if(vector >= INT_IRQ8 && vector <= INT_IRQ15) pic_eoi_slave();

    volatile uint16_t *screen = (uint16_t*)0xb8000;
    *screen = 0x0739;
}

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    const uint64_t multiboot_magic_number = 0x2badb002;
    if(magic != multiboot_magic_number) return;

    interrupt_init();

    pic_remap(INT_IRQ0, INT_IRQ7);
    pic_irq_mask(0xfffd); // unmask keyboard interrupt (irq #1)

    interrupt_enable();

    while(true); // wait for external interrupts

    interrupt_disable();
}
