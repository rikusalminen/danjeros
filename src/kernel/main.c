#include <stdint.h>

#include <arch/interrupt.h>

void interrupt_handler(
    uint64_t vector,
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
    volatile uint16_t *screen = (uint16_t*)0xb8000;
    *screen = 0x0739;
}

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    const uint64_t multiboot_magic_number = 0x2badb002;
    if(magic != multiboot_magic_number) return;

    interrupt_init();
    // interrupt_enable();

    // push debugging markers into the stack and registers and cause an interrupt
    __asm__ __volatile__ (
        "push %[bottom] ; mov %[reg], %%rax ; int $0x30 ; pop %%rax"
        :
        : [bottom] "r" (0xdeadbeefcadebabe)
        , [reg] "r" (0x0123456789abcdef)
        : "%rax"
        );

    // interrupt_disable();
}

